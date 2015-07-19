/**
\file main.cpp
\brief Main application control
 
 This application is launched in background on startup and is never disabled. The role
 of the controller is to surround the entire application depending on the kind of choices
 of the user and the actual program status.
 
 As it is possible to manage activities running on separate machines (connected via
 the internal LAN) and there are also local tasks that runs continuously the IR controller
 react in parallel with other functionalities of the RPI master, e.g. the LCD display 
 management and the continuous data retrieved from some probes.
 
 \note In a normal running condition the IR controller is the only interaction point
 of the user with the system, resulting in a semi-automated architecture and a high
 usability level.
 
 The application is essentially built over a main while() loop controlling the IR controller
 button press. When the lirc interface has been opened correctly without errors, also
 the serial interface (connecting the control panel board) is opened for the remote communication
 Both the communication lines (serial and IR) are set to run in non-blocking mode to avoid
 system hangs and too long delays.
 The serial interface remain opened granting the communication with the control panel board
 but the commands are originated from the master. In the meantime, the UART connection is checked
 every IR cycle to see is there are characters waiting to be received in the queue, further sent
 to the parser. This grant that the master device is able to answer to calls from
 the control panel board, i.e. alarm or specific parameters requests.
 
 The architecture can work without changes also when more conditions should be managed
 in one of the two directions, simply including more accepted command requests in the
 parser or adding display templates for sending to the control panel board.
 
 \note When a command is recognized as a valid button it is not possible to send more commands
 (no queueing is supported). As a matter of fact the entire multi-computer Meditech is a
 parallel state machine that should work in a completely asynchronous way.

*/

#include <wiringPi.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <lirc/lirc_client.h>
#include <time.h>
#include "Globals.h"
#include "ControllerKeys.h"
#include "LCDTemplatesMaster.h"
#include "CommandProcessor.h"
 
#define __DEBUG

//! UART file stream to manage the serial connection
int uart0_filestream = -1;

//! Status flags structure
states controllerStatus;

//! The command string to be sent to the control panel
char* cmdString = '\0';

/**
 \brief main The main entry point of the program
 
 This program is organized to manage the incoming IR cmdStrings, accordingly with
 the /etc/lirc/lircd.conf file. Every command is associated to a specific
 action that is launched, mostly through the control panel. For a complete command
 management mechanism ready the Infrared Control commands definition.
 
 \todo manage a more complete lirc initialization including an error log file.
 */
int main(int argc, char *argv[]) {
	//! The lirc client configuration
	struct lirc_config *config;
	//! The last read code from the IR controller
	char *code;
	//! A character pointer. What else?
	char *s;
	
	initFlags();

//! Array with all the controller IR keys string name
//! If a string in the array match with the lirc return code the corresponding array index
//! is used to execute the associated command.
	const char *IR_KEYS[NUM_KEYS] = { KEY_MENU, KEY_POWER, KEY_NUMERIC_0, KEY_NUMERIC_1, KEY_NUMERIC_2,
				KEY_NUMERIC_3, KEY_NUMERIC_4, KEY_NUMERIC_5, KEY_NUMERIC_6,
				KEY_NUMERIC_7, KEY_NUMERIC_8, KEY_NUMERIC_9, KEY_UP, KEY_DOWN,
				KEY_LEFT, KEY_RIGHT, KEY_RED, KEY_GREEN, KEY_YELLOW, KEY_BLUE,
				KEY_OK, KEY_MUTE, KEY_VOLUMEUP, KEY_VOLUMEDOWN, KEY_CHANNELUP, KEY_CHANNELDOWN };

	//Initiate LIRC. Exit on failure
	if(lirc_init((char *)LIRC_CLIENT, 1) == -1)
			exit(EXIT_FAILURE);
 
	//Read the default LIRC config at /etc/lirc/lircd.conf
	if(lirc_readconfig(NULL, &config, NULL) == 0) {
		// Set the lirc status flag
		controllerStatus.isLircRunning = true;
		// As lirc is working intialise the serial connection
		uart0_filestream = open(UART_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
		// Check the UART opening status. If a problem occur, the application exits.
		if(uart0_filestream == -1) {
			//Frees the data structures associated with config.
			lirc_freeconfig(config);
			// Closes the connection to lircd and does some internal clean-up stuff.
			lirc_deinit();
			// Set the lirc status flag
			controllerStatus.isLircRunning = false;
			exit(EXIT_FAILURE); // The /etc/lirc/lircd,conf file does not exist.
		} // Problem opening the UART. Exit with error
		
		// Configure the UART connection
		struct termios options;
		tcgetattr(uart0_filestream, &options);
		options.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
		options.c_iflag = IGNPAR;
		options.c_oflag = 0;
		options.c_lflag = 0;
		// Serial buffer is flushed before setting the parameters correctly
		tcflush(uart0_filestream, TCIFLUSH);
		tcsetattr(uart0_filestream, TCSANOW, &options);		
		// Set the UART flag status
		controllerStatus.isUARTRunning = true;
		// ====================================================================
		// This is virtually our infinite loop. The only exit condition
		// is when the socket is closed.
		// ====================================================================
		while(lirc_nextcode(&code) == 0) {
			// Check the serial status
			manageSerial();
			
			// If code = NULL, meaning nothing was returned from LIRC socket,
			// then skip lines below and start while loop again.
			if(code == NULL) continue;
			
			// Loop on the IR keys array key names searching if a valid
			// key has been pressed.
			for(int i = 0; i < NUM_KEYS; i++) {
				// Search for a corresponding key
				if(strstr(code, IR_KEYS[i])){
//#ifdef __DEBUG
//					printf("Lirc detected>%s\n", IR_KEYS[i]);
//#endif
					// Parse the key event
					parseIR(i);
					break; // Forces the loop exit.
				} // found the pressed key
			} // Loop searching the key press.
		} // IR code processing
		// ====================================================================
		// Lirc controller infinite loop / END
		// ====================================================================
		// Need to free up code before the next loop
		free(code);
	} // infinite reading loop
	//Frees the data structures associated with config.
	lirc_freeconfig(config);
	// Closes the connection to lircd and does some internal clean-up stuff.
	lirc_deinit();
	exit(EXIT_FAILURE); // The /etc/lirc/lircd,conf file does not exist.
}

/**
 \brief Parses the infrared key ID and executes the associated command.
 
 This function is the first level of parsing interfacing the user choice
 when a recognized command button is pressed on the IR controller with the second
 parsing level to execute the command, depending on the actual condition of the 
 system.
 
 \note Some commands should not pressed multiple times so the case is ignored
 if the same comman has been already pressed. E.g. this is the case of the
 display templates that should not be required more than one time or the power off
 button that after pressed further press has no effect.
 
 \param infraredID The IR command ID
 */
void parseIR(int infraredID) {
	//! CommandProcessor class instance.
	CommandProcessor cProc;
	
	// Process the ID
	switch(infraredID) {
		case CMD_MENU:
			if(infraredID != controllerStatus.lastKey) {
				cmdString = cProc.buildCommandDisplayTemplate(TID_DEFAULT);
				setPowerOffStatus(POWEROFF_NONE);
			}
			break;
		case CMD_POWER:
			if(infraredID != controllerStatus.lastKey) {
				setPowerOffStatus(POWEROFF_REQUEST);
			}
			break;
		case CMD_NUMERIC_0:
			setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_NUMERIC_1:
			if(infraredID != controllerStatus.lastKey) {
				cmdString = cProc.buildCommandDisplayTemplate(TID_STETHOSCOPE);
				controllerStatus.serialState = SERIAL_READY_TO_SEND;
				setPowerOffStatus(POWEROFF_NONE);
				// Check the serial status
				manageSerial();
			}
			break;
		case CMD_NUMERIC_2:
			if(infraredID != controllerStatus.lastKey) {
				cmdString = cProc.buildCommandDisplayTemplate(TID_BLOODPRESS);
				controllerStatus.serialState = SERIAL_READY_TO_SEND;
				setPowerOffStatus(POWEROFF_NONE);
				// Check the serial status
				manageSerial();
			}
			break;
		case CMD_NUMERIC_3:
			if(infraredID != controllerStatus.lastKey) {
				cmdString = cProc.buildCommandDisplayTemplate(TID_HEARTBEAT);
				controllerStatus.serialState = SERIAL_READY_TO_SEND;
				setPowerOffStatus(POWEROFF_NONE);
				// Check the serial status
				manageSerial();
			}
			break;
		case CMD_NUMERIC_4:
			if(infraredID != controllerStatus.lastKey) {
				cmdString = cProc.buildCommandDisplayTemplate(TID_TEMPERATURE);
				controllerStatus.serialState = SERIAL_READY_TO_SEND;
				setPowerOffStatus(POWEROFF_NONE);
				// Check the serial status
				manageSerial();
			}
			break;
		case CMD_NUMERIC_5:
			if(infraredID != controllerStatus.lastKey) {
				cmdString = cProc.buildCommandDisplayTemplate(TID_ECG);
				controllerStatus.serialState = SERIAL_READY_TO_SEND;
				setPowerOffStatus(POWEROFF_NONE);
				// Check the serial status
				manageSerial();
			}
			break;
		case CMD_NUMERIC_6:
			setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_NUMERIC_7:
			setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_NUMERIC_8:
			setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_NUMERIC_9:
			setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_UP:
			setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_DOWN:
			setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_LEFT:
			setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_RIGHT:
			setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_RED:
			if(infraredID != controllerStatus.lastKey) {
				cmdString = cProc.buildCommandDisplayTemplate(TID_TEST);
				controllerStatus.serialState = SERIAL_READY_TO_SEND;
				setPowerOffStatus(POWEROFF_NONE);
				// Check the serial status
				manageSerial();
			}
			break;
		case CMD_GREEN:
			if(infraredID != controllerStatus.lastKey) {
				cmdString = cProc.buildCommandDisplayTemplate(TID_INFO);
				controllerStatus.serialState = SERIAL_READY_TO_SEND;
				setPowerOffStatus(POWEROFF_NONE);
				// Check the serial status
				manageSerial();
			}
			break;
		case CMD_YELLOW:
			setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_BLUE:
			setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_OK:
			if(infraredID != controllerStatus.lastKey) {
				if(controllerStatus.powerOff == POWEROFF_REQUEST)
					setPowerOffStatus(POWEROFF_CONFIRMED);
				else
					setPowerOffStatus(POWEROFF_NONE);
			}
			break;
		case CMD_MUTE:
				setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_VOLUMEUP:
				setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_VOLUMEDOWN:
				setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_CHANNELUP:
				setPowerOffStatus(POWEROFF_NONE);
			break;
		case CMD_CHANNELDOWN:
				setPowerOffStatus(POWEROFF_NONE);
			break;
			
		default:
			break;
	} // Button ID case
	
	// Update the last key ID
	controllerStatus.lastKey = infraredID;
}

/**
 \brief Manage the serial communication between the master and the control panel
 board.
 
 Depending on the serial flag status this function send the waiting command
 or check for the presence of an expected response from the remote system.
 
 \warning Use this function only when sure that the serial is connected and
 running as there are no controls on the serial status.
 */
void manageSerial(void) {
	int bytesWritten = 0;
	char rx_buffer[MAX_CMD_LEN];
	int rx_length;

	switch(controllerStatus.serialState) {
		case SERIAL_IDLE_STATUS:
			// No action is required
			break;
			
		case SERIAL_READY_TO_SEND:
			// There is a command ready to send in the command string queue
			bytesWritten = write(uart0_filestream, cmdString, MAX_CMD_LEN);
			// Change the serial status accordingly to the action
			controllerStatus.serialState = SERIAL_JUST_SENT;
			break;
			
		case SERIAL_JUST_SENT:
			// The previous action a command string was sent to the remote board
			// so check if an answer is arrived.
			if (uart0_filestream != -1) {
				rx_length = read(uart0_filestream, (void*)rx_buffer, MAX_CMD_LEN);
				// Check if there are bytes waiting
				if (rx_length > 0) {
					//Bytes received
					rx_buffer[rx_length] = '\0';
#ifdef __DEBUG
					printf("UART>%i bytes : %s\n", rx_length, rx_buffer);
#endif
					cmdString = rx_buffer;
				} // Load the data read
			} // Check for data on the uart buffer queue			
			controllerStatus.serialState = SERIAL_IDLE_STATUS;
			break;
			
		default:
			break;
	} // Serial status cases
	
}

/**
 \brief Initializes the status flags to the first run condition.
 
 \todo Manage the isSystemRunning flag status accordingly with the Meditech
 global status.
 */
void initFlags(void) {
	controllerStatus.activeProbe = PROBE_ACTIVE_NONE;
	controllerStatus.isLircRunning = false;
	controllerStatus.isUARTRunning = false;
	controllerStatus.isSystemRunning = true; // Not yet managed
	controllerStatus.serialState = SERIAL_IDLE_STATUS;
	controllerStatus.powerOff = POWEROFF_NONE;
	controllerStatus.lastKey = '\0';
}

/**
 \brief Manage the power status of the system
 
 The power off status flag is managed by this separate function as the status change
 is associated to different events involving the entire Meditech architecture. 
 If the power off sequence is confirmed then the master system should shutdown the other
 devices then shutdown itself.
 
 \todo Implement the full system shutdown sequence on power off request confirmation.
 
 */
void setPowerOffStatus(int status) {
	switch(status) {
		case POWEROFF_NONE:
			// The power off status is simply reset and no particular action
			// should be done
			controllerStatus.powerOff = POWEROFF_NONE;
			break;
			
		case POWEROFF_REQUEST:
			// The power off request has been armed and should be confirmed.
			// If no confirmation arrives, the sequence ends with no effect
			// and the status is reset.
			controllerStatus.powerOff = POWEROFF_REQUEST;
			break;
			
		case POWEROFF_CONFIRMED:
			// The power off request has been confirmed and the non-reversible
			// shutdown process has been started
			controllerStatus.powerOff = POWEROFF_CONFIRMED;
#ifdef __DEBUG
			exit(0); // Application is terminated
#else
			// Initiae a shutdown sequence
#endif
			break;
	} // Power off status cases
}
