/**
\file Globals.h
\brief Application global definitions
 
*/

#ifndef GLOBALS_H
#define	GLOBALS_H

//! Lirc library name
#define LIRC_CLIENT	"lirc"

//! Serial device
#define UART_DEVICE "/dev/ttyAMA0"

//! To-send status: remote communication idle. No action in progress.
#define SERIAL_IDLE_STATUS			0
//! To-send status: command ready for sending
#define SERIAL_READY_TO_SEND		1
//! To-send status: command sent, next event should be a response from remote
#define SERIAL_JUST_SENT			2
//! To-send status: a break action has been requested
#define SERIAL_ACTION_BREAK			3
//! To-send status: response received from remote. Start parsing
#define SERIAL_RESPONSE_RECEIVED	4

//! Active probe code: no probes acitve
#define PROBE_ACTIVE_NONE			0
//! Active probe code: last request sent to stethoscope
#define PROBE_ACTIVE_STETHOSCOPE	1
//! Active probe code: last request sent to ECG
#define PROBE_ACTIVE_ECG			2
//! Active probe code: last request sent to heatbeat
#define PROBE_ACTIVE_HEARTBEAT		3
//! Active probe code: last request sent to temperature
#define PROBE_ACTIVE_TEMPERATURE	4
//! Active probe code: last request sent to boold pressure
#define PROBE_ACTIVE_PRESSURE		5

//! Power-off button : button is inactive
#define POWEROFF_NONE		0
//! Power-off button : Power-off button has been pressed. Request sent
#define POWEROFF_REQUEST	1
//! Power-off button : Power-off button has been confirmex. Ending.
#define POWEROFF_CONFIRMED	2

//! Command code to force the program to generate the TTS voice
//! strings instead the normal execution.
#define VOICE_STRINGS "-v"

/**
 \brief Boolean states and flags to take track of the application status.
 Note that some of these status parameters are updated on the database for
 sharing with the Meditech architecture.
 */
typedef struct ControllerStatusFlags {
	/**
	 when active a controller key press has created a command to be sent to the 
	 control panel board. This is a multi-state flag that can assume the following
	 values: SERIAL_IDLE_STATUS, SERIAL_READY_TO_SEND, SERIAL_JUST_SENT, SERIAL_ACTION_BREAK,
	 SERIAL_RESPONSE_RECEIVED
	 */
	int serialState;
	
	/**
	 Active probe is the last probe request sent to the control panel that returned 
	 a confirmation answer. This status variable can assume the following values: 
	 */
	int activeProbe;
	
	//! Lirc IR status
	bool isLircRunning;
	
	//! UART Serial status
	bool isUARTRunning;
	
	/**
	 Meditech global running status. This flag is set when all the other devices
	 have completed the boot and has acknowledged the master on the network. Until
	 the boot status is not complete the system can't operate with all the features
	 running.
	 */
	bool isSystemRunning;
	
	/**
	 Power-off sequence needs two steps: the power-off button require the power-off
	 sequence. If the immediately next button is the OK button the power-off (shutdown)
	 sequence is initiated, else if any other button is pressed the power-off status
	 is reset. The powerOff status can assume the following states:
	 POWEROFF_NONE, POWEROFF_REQUEST, POWEROFF_CONFIRMED
	 */
	int powerOff;
	
	//! Last key ID received from LIRC. Used to avoid command key bouncing.
	int lastKey;
	
	//! Voice messages status
	bool isMuted;
	
} states;

#endif	/* GLOBALS_H */

