/**
\file ControllerKeys.h
\brief Definition of the IR controller assigned control keys. This include file
 is derived from the /etc/lirc/lircd.con file.
 
 This header file defines the key string name as it is included in the lirc code
 returned when the corresponding button is pressed on the IR controller.
 To make shorter and faster the code - including the IR controller code identification
 in a single loop - the key names are assigned to an array whose index correspond
 to the symbolic numeric ID of every command.
 The next group of defines contains the command symbolic ID so it is possible to
 parse the user commands with a simple integer control reducing the string comparison
 to a single case in the program.
 
 \note This header file also includes the function prototypes of the main program.
 
 */

#ifndef CONTROLLERKEYS_H
#define	CONTROLLERKEYS_H

#define	KEY_MENU			"KEY_MENU"
#define	KEY_POWER			"KEY_POWER"
#define	KEY_NUMERIC_0		"KEY_NUMERIC_0"
#define	KEY_NUMERIC_1		"KEY_NUMERIC_1"
#define	KEY_NUMERIC_2		"KEY_NUMERIC_2"
#define	KEY_NUMERIC_3		"KEY_NUMERIC_3"
#define	KEY_NUMERIC_4		"KEY_NUMERIC_4"
#define	KEY_NUMERIC_5		"KEY_NUMERIC_5"
#define	KEY_NUMERIC_6		"KEY_NUMERIC_6"
#define	KEY_NUMERIC_7		"KEY_NUMERIC_7"
#define	KEY_NUMERIC_8		"KEY_NUMERIC_8"
#define	KEY_NUMERIC_9		"KEY_NUMERIC_9"
#define	KEY_UP				"KEY_UP"
#define	KEY_DOWN			"KEY_DOWN"
#define	KEY_LEFT			"KEY_LEFT"
#define	KEY_RIGHT			"KEY_RIGHT"
#define	KEY_RED				"KEY_RED"
#define	KEY_GREEN			"KEY_GREEN"
#define	KEY_YELLOW			"KEY_YELLOW"
#define	KEY_BLUE			"KEY_BLUE"
#define	KEY_OK				"KEY_OK"
#define	KEY_MUTE			"KEY_MUTE"
#define	KEY_VOLUMEUP		"KEY_VOLUMEUP"
#define	KEY_VOLUMEDOWN		"KEY_VOLUMEDOWN"
#define	KEY_CHANNELUP		"KEY_CHANNELUP"
#define	KEY_CHANNELDOWN		"KEY_CHANNELDOWN"

#define	CMD_MENU			0
#define	CMD_POWER			1
#define	CMD_NUMERIC_0		2
#define	CMD_NUMERIC_1		3
#define	CMD_NUMERIC_2		4
#define	CMD_NUMERIC_3		5
#define	CMD_NUMERIC_4		6
#define	CMD_NUMERIC_5		7
#define	CMD_NUMERIC_6		8
#define	CMD_NUMERIC_7		9
#define	CMD_NUMERIC_8		10
#define	CMD_NUMERIC_9		11
#define	CMD_UP				12
#define	CMD_DOWN			13
#define	CMD_LEFT			14
#define	CMD_RIGHT			15
#define	CMD_RED				16
#define	CMD_GREEN			17
#define	CMD_YELLOW			18
#define	CMD_BLUE			19
#define	CMD_OK				20
#define	CMD_MUTE			21
#define	CMD_VOLUMEUP		22
#define	CMD_VOLUMEDOWN		23
#define	CMD_CHANNELUP		24
#define	CMD_CHANNELDOWN		25

//! Number of valid IR keys on the controller. It is also the IR_KEYS array elements.
#define NUM_KEYS	26

// Function prototypes
void parseIR(int);
void initFlags(void);
void setPowerOffStatus(int);
void manageSerial(void);

#endif	/* CONTROLLERKEYS_H */

