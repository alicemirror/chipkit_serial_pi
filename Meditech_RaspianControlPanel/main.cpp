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
	char *c;
 
	//Initiate LIRC. Exit on failure
	if(lirc_init(LIRC_CLIENT, 1) == -1)
			exit(EXIT_FAILURE);
 
	//Read the default LIRC config at /etc/lirc/lircd.conf
	if(lirc_readconfig(NULL, &config, NULL) == 0) {
		// This is virtually our infinite loop. The only exit condition
		// is when the socket is closed.
		while(lirc_nextcode(&code)==0) {
				//If code = NULL, meaning nothing was returned from LIRC socket,
				//then skip lines below and start while loop again.
				if(code == NULL) continue;{
					// Process the IR code read from the controller
					
				} // IR code processing
				// Need to free up code before the next loop
				free(code);
		} // infinite reading loop
		//Frees the data structures associated with config.
		lirc_freeconfig(config);
	} // Read lirc config
	// Closes the connection to lircd and does some internal clean-up stuff.
	lirc_deinit();
	exit(EXIT_FAILURE); // The /etc/lirc/lircd,conf file does not exist.
}
 
