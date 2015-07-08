/** 
  \file CommandProcessor.h
  \brief Constants and types used by the remote control to manage the behaviour of the
  board.
  
  The Remote control include file defines the constants and structures used by the 
  RemoteControl class to process the RPI master requests and to exchange data through
  the serial connection.
  */

#ifndef __COMMANDPROCESSOR_H__
#define __COMMANDPROCESSOR_H__

//! The max len of an unparsed command string
#define MAX_CMD_LEN 1024
//! The max len of the message command string
#define CMD_MSGLEN 20
//! The max len of a header/menu string
#define CMD_HEADERLEN 14

/** 
  \brief Command separator for multi-command strings

  A single string can include multiple commands with a variable
  number of parameters. Every command should be separated on the string
  by the '@' character that is recognised by the parser as the command separator\n
  
  Example: <cmd 1>@<cmd 2>@<cmd 3> \n
*/
#define CMD_SEPARATOR '@'

/**
  \brief Separator for subcommands and command parameters.
  
  The single-character commands can be followed by a sub-command or a
  series of parameters. Every one of them should be separed by a 
  field separator character.
  */
#define FIELD_SEPARATOR ';'

//! String delimiter
#define STRING_DELIMITER '"'

//! Max number of integer parameters in a command
#define MAX_INT 2
//! Max number of long integer parameters in a command
#define MAX_LONG 2
//! Max number of float parameters in a command
#define MAX_FLOAT 2

/** 
  \typedef command
  \brief Command Structure

  This structure defines the parameters associated to the commands.
  When the syntax checker of the parser decode a command the structure is filled
  for the further process.
*/
typedef struct parseCommand {
    char *toParse;
    //! char subcommand[1]
    //! Contains the last parsed subcommand
    char subcommand[1];
    //! Command associated message string, used by S_SUBSTRING, CMD_WRITE, CMD_EXEC 
    char message[CMD_MSGLEN];
   
    char stringValue;
    long longValue[MAX_LONG];
    int intValue[MAX_INT];
    float floatValue[MAX_FLOAT];
    int separator; 
    /**
      \brief Unparsed Command String
  
      cmdData is the string after syntax checking.
      The string can contain one or more commands.
      When the command string is sent to the parser this value contains
      the command sequence. When the this string is empy
      all the commands has been processed and the system is ready to receive
      a new command.
    */
    char cmdData[MAX_CMD_LEN];
} command;

/**
  \brief All the commands in one string
  
  This string is used by the parser to check if a received character
  is a command or not.
  
  \note If a command has been defined but is not listed in this string,
  the parser syntax checker can't process it. Otherwise, never set a character
  in this string if there is not a corresponding command definition.
  */
#define CMD_CHARACTERS "EDGITRPr"

//! The lenght of CMD_CHARACTERS + 1
#define CMD_CHARLEN 9

/**
  \brief command: Enable/disable a probe
  
  description: Enable a probe and accept setting parameters or disable the probe
  if already enabled. The subcommand contains the probe and eventually parameters\n
  name: E \n
  usage: E;<subcommand> \n
  direction: receive
  example: E;S \n
  Enable the microphonic stethoscope
  */
#define CMD_ENABLE 'E'

/**
  \brief command: send [[parameter] value]
  
  description: send a couple parameter-value to the master\n
  name: P \n
  usage: P;<parameter>;<value> \n
  direction: send
  example: P;G;23 \n
  Send the stethoscope gain level to the master
  \todo Not yet implemented
  */
#define CMD_PARAMETER 'P'

/**
  \brief command: Shows a message on the display
  
  description: this is a descriptive command only, reflecting the state of a
  user request from the remote IR controller. The row and column values should be
  integers, zero-based, inside the physical limits of the LCD display.
  
  \note This command needs less controls than expected because errors like
  out of bound string lenght, wrong cursor position etc. never occurs using the
  display templates. Templates include file is the same on both the master and the microcontroller
  with the difference that the master also includes the strings definition that are
  sent when a display should be drawn with a template.
  
  name: D \n
  usage: D;<row(int)>;<column(int)>;<string> \n
  direction: receive\n
  example: D;1;3;Test \n
  */
#define CMD_DISPLAY 'D'

/**
  \brief command: start running the actual pending command
  
  description: confirm a pending action / parameters setting request \n
  name: G \n
  usage: G \n
  direction: receive\n
  \todo Not yet implemented
  */
#define CMD_GO 'G'

/**
  \brief command: info
  
  description: shows the current health status parameters of the
  control panel (temperature, fan speed, active flags etc.)
  name: I \n
  usage: I \n
  direction: receive\n
  */
#define CMD_INFO 'I'

/**
  \brief command: executes a control panel test cycle
  
  description: suspend all the active interrupts and tasks and execute
  a full test of the control panel.\n
  name: T \n
  usage: T \n\n
  direction: receive\n
  */
#define CMD_TEST 'T'

/**
  \brief command: request a parameter definition
  
  description: request to the master the parameter definition for
  one of the template fields to show on the display. In most of the cases these are
  strings managed by the master to save memory space on the board and support
  the multi-language localisation.\n
  name: r \n
  usage: r;<parameter ID> \n
  direction: send\n
  \todo Should be implemented in the RPI master before
  */
#define CMD_REQUEST 'r'

/**
  \brief command: receive a parameter value from master
  
  description: receive from the master the requested value for
  one of the template fields to show on the display. In most of the cases these are
  strings managed by the master to save memory space on the board and support
  the multi-language localisation.\n
  name: R \n
  usage: R;<parameter ID>;<value>\n
  direction: receive\n
  \todo Not yet implemented
  */
#define CMD_REQUEST 'R'

/**
  \brief subcommand: Enable Stethoscope probe status
  
  description: enable or disable the stethoscope probe. \n
  usage: S (int)status \n
  parameters: the status accepted values are STETH_ENABLE and STETH_DISABLE
  example: S;001 \n
  Enable the stethoscope probe.
*/
#define S_STETHOSCOPE 'S'

/**
  \brief subcommand: Enable ECG probe status
  
  description: enable or disable the E.C.G. probe. \n
  usage: G (int)status \n
  parameters: the status accepted values are FLAG_ENABLE and FLAG_DISABLE
  example: G;001 \n
*/
#define S_ECG 'G'

/**
  \brief subcommand: Enable Blood pressure probe status
  
  description: enable or disable the Sphygmomanometer probe. \n
  usage: P (int)status \n
  parameters: the status accepted values are FLAG_ENABLE and FLAG_DISABLE
  example: P;001 \n
*/
#define S_PRESSURE 'P'

/**
  \brief subcommand: Enable Body temperature probe status
  
  description: enable or disable the body temperature probe. \n
  usage: T (int)status \n
  parameters: the status accepted values are FLAG_ENABLE and FLAG_DISABLE
  example: T;001 \n
*/
#define S_BODYTEMP 'T'

/**
  \brief subcommand: Enable Heartbeat probe status
  
  description: enable or disable the Heart Beat probe. \n
  usage: H (int)status \n
  parameters: the status accepted values are FLAG_ENABLE and FLAG_DISABLE
  example: H;001 \n
*/
#define S_HEARTBEAT 'H'

//! Enable flag
#define FLAG_ENABLE 1
//! Disable flag
#define FLAG_DISABLE 0

//! Command string separator
#define COMMAND_SEPARATOR "::"

#endif

