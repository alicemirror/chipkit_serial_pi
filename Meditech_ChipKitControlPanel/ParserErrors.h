/** 
  \file ParserErrors.h
  \brief Defines the parser error codes.
  */

#ifndef __PARSERERRORS_H__
#define __PARSERERRORS_H__

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
//! The requested parameter separator was missing
#define COMMAND_OUT_OF_RANGE 10
//! The received subcommand is unknown
#define PARSER_SUBCOMMAND_UNKNOWN 4

#define COMMAND_BODYTEMP_PARAMERROR 8
#define COMMAND_HEARTBEAT_PARAMERROR 9
#define COMMAND_PRESSURE_PARAMERROR 7
#define COMMAND_ECG_PARAMERROR 6
#define COMMAND_STETHOSCOPE_PARAMERROR 5
#define COMMAND_ECG_PARAMERROR 6

#endif

