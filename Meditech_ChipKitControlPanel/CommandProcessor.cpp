/** 
  \file CommandProcessor.cpp
  \brief Command parser class for the serial communication with the RPI master controller.
  
  */

#include "CommandProcessor.h"
#include "LCD.h"

/**
  \brief Class constructor
  */
CommandProcessor::CommandProcessor() {
  
}

/** 
  \brief Check for new commands and process the input string
  
  If the syntax checker doesn't recognize any valid command
  strings are ignored and discharged. Every command is a single-character,
  case sensitive. All the characters recognised as a command are
  listed in the CMD_CHARACTERS constant; when the parser starts only those
  commands included in this string and stored in the cBuff character buffer
  can be accepted as valid commands, else the string portion is not processed
  until the next command separator (or the end of the string if there is only
  one command or it is the last).
  */
void CommandProcessor::parser() {
  //! Counters and indexes. The initaliasation
  //! matters are these are the correct points for the
  //! parser recursive process.
  int i = 0, k = 0, j = 0, value;
  //! Single-character commands array
  char c[CMD_CHARLEN];
  //! Char buffer pointer with all the valid single-character commands
  PString cBuff(c, sizeof(c));

  // Load the sequence of valid characters in the buffer  
  cBuff = CMD_CHARACTERS;
  cmd.cmdData[0] = '\0';

  // Load the command string coming from serial in the character array
  while (Serial.available() > 0)
      cmd.cmdData[i++] = (char)Serial.read();
  
  // Add the closing command character
  cmd.cmdData[i++] = CMD_SEPARATOR;
  cmd.cmdData[i] = '\0';
  
  // Here we start processing recursively the array until the end.
  while (k < i) {
    // If a command separator is found, anyway we have already finished processing
    // the command (including eventually an error condition). So the character is
    // skipped until a new command start is not found.
    // Using this strategy it is possible to include between two commands or anywhere
    // outside a valid sequence any kind of comment.
    if (cmd.cmdData[k] == CMD_SEPARATOR)
      k++;

    // Search for a valid command character and when found set the procssing flags.
    // Else generates an error condition. 
    for (j = 0; j < CMD_CHARLEN; j++) {
      // Process the current commant character
      if (c[j] == cmd.cmdData[k]) {
        // Found a matching command
        switch(cmd.cmdData[k]) {
          // Show a string on the display.
          case CMD_DISPLAY:
            commandReturn(CMD_DISPLAY);
            // Syntax checking
            if (!parseSeparator(cmd.cmdData[++k])) {
              syntaxError(COMMAND_MISSINGSEPARATOR);
              k = nextSeparator(k);
              break;
            }
            // We expect the next paramter is row expressed in 00-nn
            value = charsToInt(++k, 2);
            if (value < LCDROWS) {
              syntaxError(COMMAND_OK);
              cmd.intValue[0] = value;
            } // No errors
            else {
              syntaxError(COMMAND_OUT_OF_RANGE);
              k = nextSeparator(k);
              break;
            } // Error out of range
            // We expect the next paramter is column expressed in 00-nn
            value = charsToInt(++k, 2);
            if (value < LCDCHARS) {
              syntaxError(COMMAND_OK);
              cmd.intValue[1] = value;
            } // No errors
            else {
              syntaxError(COMMAND_OUT_OF_RANGE);
              k = nextSeparator(k);
              break;
            } // Error out of range
            // We expect the next paramter is the data string
            cmd.stringValue = charsToString(++k);
            syntaxError(COMMAND_OK);
            break;

            // Enable a status on the control panel (mainly a probe setting).
            // The details of the object to enable are specified in the subcommand
            case CMD_ENABLE:
              commandReturn(CMD_ENABLE);
              // Search for subcommands
              if (!parseSeparator(cmd.cmdData[++k])) {
                k = nextSeparator(k);
                syntaxError(COMMAND_MISSINGSEPARATOR);
                break;
              } // Search subcommands
              cmd.subcommand[0] = cmd.cmdData[++k];
              // Manage the subcommands parameter
              switch(cmd.subcommand[0]) {
                // Enable / disable the stethoscope probe control panel settings
                case S_STETHOSCOPE:
                  commandReturn(S_STETHOSCOPE);
                  // Check for subcommand separator
                  if (!parseSeparator(cmd.cmdData[++k])) {
                      syntaxError(COMMAND_MISSINGSEPARATOR);
                      k = nextSeparator(k);
                      break;
                  } // check for separator
                  if (!setStethoscopeStatus(++k))
                      syntaxError(COMMAND_STETHOSCOPE_PARAMERROR);
                  else
                      syntaxError(COMMAND_OK);
                  k = nextSeparator(k);
                  break;
                // Enable / disable the ECG probe control panel settings
                case S_ECG:
                  commandReturn(S_ECG);
                  // Check for subcommand separator
                  if (!parseSeparator(cmd.cmdData[++k])) {
                      syntaxError(COMMAND_MISSINGSEPARATOR);
                      k = nextSeparator(k);
                      break;
                  } // check for separator
                  if (!setECGStatus(++k))
                      syntaxError(COMMAND_ECG_PARAMERROR);
                  else
                      syntaxError(COMMAND_OK);
                  k = nextSeparator(k);
                  break;
                // Enable / disable the Blood Pressure probe control panel settings
                case S_PRESSURE:
                  commandReturn(S_PRESSURE);
                  // Check for subcommand separator
                  if (!parseSeparator(cmd.cmdData[++k])) {
                      syntaxError(COMMAND_MISSINGSEPARATOR);
                      k = nextSeparator(k);
                      break;
                  } // check for separator
                  if (!setPressureStatus(++k))
                      syntaxError(COMMAND_PRESSURE_PARAMERROR);
                  else
                      syntaxError(COMMAND_OK);
                  k = nextSeparator(k);
                  break;
                // Enable / disable the Body Temperature probe control panel settings
                case S_BODYTEMP:
                  commandReturn(S_BODYTEMP);
                  // Check for subcommand separator
                  if (!parseSeparator(cmd.cmdData[++k])) {
                      syntaxError(COMMAND_MISSINGSEPARATOR);
                      k = nextSeparator(k);
                      break;
                  } // check for separator
                  if (!setBodyTempStatus(++k))
                      syntaxError(COMMAND_BODYTEMP_PARAMERROR);
                  else
                      syntaxError(COMMAND_OK);
                  k = nextSeparator(k);
                  break;
                // Enable / disable the Heart Beat probe control panel settings
                case S_HEARTBEAT:
                  commandReturn(S_HEARTBEAT);
                  // Check for subcommand separator
                  if (!parseSeparator(cmd.cmdData[++k])) {
                      syntaxError(COMMAND_MISSINGSEPARATOR);
                      k = nextSeparator(k);
                      break;
                  } // check for separator
                  if (!setHeartBeatStatus(++k))
                      syntaxError(COMMAND_HEARTBEAT_PARAMERROR);
                  else
                      syntaxError(COMMAND_OK);
                  k = nextSeparator(k);
                  break;
                // Subcommand unknown
                default:
                    syntaxError(PARSER_SUBCOMMAND_UNKNOWN);
                    break;
            } // Subcommands for the Enable command
            break;

            // Show the global info on the control panel display
            case CMD_INFO:
              commandReturn(CMD_INFO);
              if (!parseSeparator(cmd.cmdData[++k])) {
                syntaxError(COMMAND_MISSINGSEPARATOR);
                k = nextSeparator(k);
                break;
              }
            break;

            // Executes a test cycle of the control panel
            case CMD_TEST:
              commandReturn(CMD_TEST);
              if (!parseSeparator(cmd.cmdData[++k])) {
                syntaxError(COMMAND_MISSINGSEPARATOR);
                k = nextSeparator(k);
                break;
              }
            break;
            
            // If no command is recognised, do nothing
            default:
              break;
        } // Command processing switch case
        j = CMD_CHARLEN;
      } // Check with the current command character
    } // Loop on command characters (syntax checking)
    k++;
  } // main while loop
  i = 0;
}

/**
  \brief Search the position for next command separator
  
  This function is used for complex commands to find the first 
  starting point of a new command (if exist)
  The function starts from the startChar and ends when a command
  separator is found. If none exists, the function ends when a
  null character is encountered.
  
  \param startChar initial character in the command string
  \return the position of the first command separator (if any) 
  or the last position before the end of the command string.
  */
int nextSeparator(int startChar) {
  int i = 0;

  while (cmd.cmdData[startChar + i] != '\0') {
    if (cmd.cmdData[startChar + i] == CMD_SEPARATOR)
      return startChar + i;
    i++;
  }
  return i;
}

/**
  \brief Convert a string to integer
  
  \param startChar initial character in the command string
  \param numChars number of characters composing the number
  \return The converted interger value
  */
int charsToInt(int startChar, int numChars) {
  char * value_to_convert;
  String temp = String(6);
  int i;

  for (i = 0; i < numChars; i++)
    temp += cmd.cmdData[i + startChar];

  value_to_convert = temp;
  i = atoi(temp);

  return i;
}

/**
  \brief Convert a string to long integer
  
  \param startChar initial character in the command string
  \param numChars number of characters composing the number
  \return The converted long interger value
  */
long charsToLong(int startChar, int numChars) {
  char * value_to_convert;
  String temp = String(8);
  long i;

  for (i = 0; i < numChars; i++)
    temp += cmd.cmdData[i + startChar];

  value_to_convert = temp;
  i = atol(temp);
  return i;
}

/**
  \brief Convert a string to floating point
  
  \param startChar initial character in the command string
  \param numChars number of characters composing the number
  \return The converted floating value
  */
float charsToFloat(int startChar, int numChars) {
  char * value_to_convert;
  String temp = String(8);
  int i;
  float result;
  
  for (i = 0; i < numChars; i++)
    temp += cmd.cmdData[i + startChar];
  
  value_to_convert = temp;
  result = atof(temp);
  return result;
}

/**
  \brief check for separator character
  
  \param test the character to test
  \return true if test is equal else returns false.
  */
boolean parseSeparator(char test) {
    if (test == FIELD_SEPARATOR)
        return true;
    else
        return false;
}

/**
  \brief Send command return code to the caller master
  
  Send the command and the error code to the caller master: is the return
  cide us OK the command has been exectued.
  This message is sent back when the first part of the parsing has been completed.
  
  \param errCode the error code from the command
  */
void syntaxError(int errCode) {
    Serial << COMMAND_SEPARATOR << errCode << endl;
}

/**
  \brief Send command or subcommand character to the calling master
  
  Send back the command or subcommand character to the master in the 
  format "::command/subcommand"
  
  This is the first part of the parser generate return string to the master. 
  When the parser complete the command identification or a syntax error occours, 
  it is called the syntaxError() function with the proper return code and the 
  endline character.
  
  \param commandCode The command/subcommand character code.
  */
void commandReturn(char commandCode) {
    Serial << COMMAND_SEPARATOR << commandCode;
}

/**
  \brief Convert a floating value to string with the specified precision
  
  As a matter of fact the ouput functions (string, serial, stream library etc.)
  does not represent in a flexible way the  floating point values. 
  This function calculates the right string with the desidred decimal
  precision sending to serial with the streaming library.
  
  Calculating the decimal value the function add the number of leading zeroes
  searching for all the decimal places before the first non-zero number.

  The loop for the leading zeroes grows by a factor 10
  between 0 (no leading zeroes needed) and precision (all zeroes, no decimal
  numbers).

  \note Precision is represented by 1 followed by the number of zeroes for the 
  desired number of decimial places. \n
  example: strLong(3.1415, 100) prints 3.14 (two decimal places)
  
  \param val the floating value to represent
  \param precision the decimal precision
  */
void strFloat(float val, unsigned int precision) {
    int i;
    
    unsigned int frac;
    if(val >= 0)
      frac = (val - int(val)) * precision;
    else
      frac = (int(val)- val ) * precision;

    Serial << int(val) << ".";

    for (i = 1; i <= precision; i += i * 10)
        if (frac < i)
            Serial << "0";

    Serial << frac;
}


