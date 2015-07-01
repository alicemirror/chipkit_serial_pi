/** 
  \file CommandProcessor.h
  \brief Constants and types used by the remote control to manage the behaviour of the
  board.
  
  The Remote control include file defines the constants and structures used by the 
  RemoteControl class to process the RPI master requests and to exchange data through
  the serial connection.
  */

//! The max len of an unparsed command string
#define MAX_CMD_LEN 160

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

#ifndef COMMAND_STRUCT
#define COMMAND_STRUCT
/** 
  \typedef command
  \brief Command Structure

  This structure defines the parameters associated to the commands.
  When the syntax checker of the parser decode a command the structure is filled
  for the further process.
*/
typedef struct parseCommand
{
    //! char subcommand[1]
    //! Contains the last parsed subcommand
    char subcommand[1];
    //! Command associated message string, used by S_SUBSTRING, CMD_WRITE, CMD_EXEC 
    char message[CMD_MSGLEN]; 
    
    long longValue;     ///< Generic long parameter
    int intValue;       ///< Generic integer parameter
    float floatValue;   ///< Generic float parameter
    int separator;      ///< Separator character

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
}
command;
#endif

/**
  \brief All the commands in one string
  
  This string is used by the parser to check if a received character
  is a command or not.
  
  \note If a command has been defined but is not listed in this string,
  the parser syntax checker can't process it. Otherwise, never set a character
  in this string if there is not a corresponding command definition.
  */
#define CMD_CHARACTERS "EDGITRpr"

//! The lenght of CMD_CHARACTERS + 1
#define CMD_CHARLEN 12

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
  */
#define CMD_PARAMETER 'p'

/**
  \brief command: Shows a message on the display
  
  description: this is a descriptive command only, reflecting the state of a
  user request from the remote IR controller. The row and column values should be
  integers, zero-based, inside the physical limits of the LCD display.\n
  
  name: D \n
  usage: D;<row(int)>;<column(int)>;<string> \n
  direction: receive\n
  example: D;1;3;"Test" \n
  Display the string "Test" at row 1, col 3
  */
#define CMD_DISPLAY 'D'

/**
  \brief command: start running the actual pending command
  
  description: confirm a pending action / parameters setting request \n
  name: G \n
  usage: G \n
  direction: receive\n
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
  */
#define CMD_REQUEST 'R'

/** 
  \brief Error codes definition for parser answers
  
  */

//! Command string separator
#define COMMAND_SEPARATOR "::"
//! Command completion code
#define COMMAND_OK 0
//! Command unknown to the parser
#define COMMAND_UNKNOWN 1
//! Wrong or malformed command.
//! This message occours when the parameters following a command are wrong,
//! incomplete or missing.
#define COMMAND_WRONG 2
//! The requested parameter separator was missing
#define COMMAND_MISSINGSEPARATOR 3
//! The received subcommand is unknown
#define PARSER_SUBCOMMAND_UNKNOWN 6

