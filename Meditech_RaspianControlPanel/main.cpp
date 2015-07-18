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

*/

#include <wiringPi.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lirc/lirc_client.h>
#include <time.h>

#include "Globals.h"
#include "ControllerKeys.h"
#include "LCDTemplatesMaster.h"
#include "CommandProcessor.h"
 
#define __DEBUG

/**
 \brief main The main entry point of the program
 
 This program is organized to manage the incoming IR commands, accordingly with
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
		// This is virtually our infinite loop. The only exit condition
		// is when the socket is closed.
		while(lirc_nextcode(&code) == 0) {
			//If code = NULL, meaning nothing was returned from LIRC socket,
			//then skip lines below and start while loop again.
			if(code == NULL) continue;

			// Loop on the IR keys array key names searching if a valid
			// key has been pressed.
			for(int i = 0; i < NUM_KEYS; i++) {
				// Search for a corresponding key
				if(strstr (code, IR_KEYS[i])){
#ifdef __DEBUG
					printf("detect>%s\n", IR_KEYS[i]);
#endif
					// Parse the key event
					parseIR(i);
					break;
				} // found the pressed key
			} // Loop searching the key press.
		} // IR code processing
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
 
 This function represent the first level of parsing interfacing the user choice
 when a recognized command button is pressed on the IR controller with the second
 parsing level to execute the command, depending on the actual condition of the 
 system.
 
 \param infraredID The IR command ID
 */
void parseIR(int infraredID) {
	//! CommandProcessor class instance.
	CommandProcessor cProc;
	//! The command string to be sent to the control panel
	char* command = '\0';
	//! The command ready to send flag
	bool toSend = false;
	
#ifdef __DEBUG
			printf("exec>%i\n", infraredID);
#endif
	
	// Process the ID
	switch(infraredID) {
		case CMD_MENU:
			command = cProc.buildCommandDisplayTemplate(TID_DEFAULT);
			toSend = true;
			break;
		case CMD_POWER:
			break;
		case CMD_NUMERIC_0:
			break;
		case CMD_NUMERIC_1:
			command = cProc.buildCommandDisplayTemplate(TID_STETHOSCOPE);
			toSend = true;
			break;
		case CMD_NUMERIC_2:
			command = cProc.buildCommandDisplayTemplate(TID_BLOODPRESS);
			toSend = true;
			break;
		case CMD_NUMERIC_3:
			command = cProc.buildCommandDisplayTemplate(TID_HEARTBEAT);
			toSend = true;
			break;
		case CMD_NUMERIC_4:
			command = cProc.buildCommandDisplayTemplate(TID_TEMPERATURE);
			toSend = true;
			break;
		case CMD_NUMERIC_5:
			command = cProc.buildCommandDisplayTemplate(TID_ECG);
			toSend = true;
			break;
		case CMD_NUMERIC_6:
			break;
		case CMD_NUMERIC_7:
			break;
		case CMD_NUMERIC_8:
			break;
		case CMD_NUMERIC_9:
			break;
		case CMD_UP:
			break;
		case CMD_DOWN:
			break;
		case CMD_LEFT:
			break;
		case CMD_RIGHT:
			break;
		case CMD_RED:
			command = cProc.buildCommandDisplayTemplate(TID_TEST);
			toSend = true;
			break;
		case CMD_GREEN:
			command = cProc.buildCommandDisplayTemplate(TID_INFO);
			toSend = true;
			break;
		case CMD_YELLOW:
			break;
		case CMD_BLUE:
			break;
		case CMD_OK:
			break;
		case CMD_MUTE:
			break;
		case CMD_VOLUMEUP:
			break;
		case CMD_VOLUMEDOWN:
			break;
		case CMD_CHANNELUP:
			break;
		case CMD_CHANNELDOWN:
			break;
			
		default:
			break;
	}
	
	// Send the command if needed
	if(toSend) {
#ifdef __DEBUG
		printf("CMD>%s\n", command);
#endif
	}
}
 
