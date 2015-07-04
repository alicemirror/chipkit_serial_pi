/**
  \file Meditech_ChipKitControlPanel.pde
  \brief meditech ChipKit control panel main application

  \mainpage
  This application will run on the ChipKit PI module of the Meditech device. The module is
  connected to the Raspberry PI Mod B2. The software includes two parts: one independent series
  of features, related to the Meditech general health status overriding the user controlled
  parameters. The board also control che alphanumeric LCD display for status messages, warnings and alarms. \n
  The user-controlled part instead is managed by the Raspberry PI "PI master" controlling the active probes
  and other functions. The counterpart of this application is a C++ command set running on the PI linux
  machine communicating with the board through a custom serial protocol.
  To reduce the weight of the microcontroller application most of the strings shown by
  the control panel display are sent by the linux machine.
  
  \note Most of the control panel functions should follow specific priorities, e.g. the lid open status while
  other functions should be exectued periodically byt the board. The high frequency check methods are managed
  by timer interrupts while the other functions will run in background via the board Task Manager. The use of
  these two strategies makes the loop() method more simple and the entire system is more efficient.
  
  \license This code is distributed under the Apache license
  These sources has been developed under the mpide application, adapted for the
  ChipKit PI board.\n
  This application is part of the Meditech project, created by Enrico Miglino for
  Balearic Dynamics - Balearic Islands - Spain\n
  For the last updated application version and subversion, see the version.h include file.
  
  \author Enrico Miglino <enrico.miglino@gmail.com>
  \copyright Balearic Dynamics - Spain <balearicdynamics@gmail.com>
  \version 1.0b
  \date First version on July 2015
*/

#include <AlphaLCD.h>
#include <Streaming.h>
#include "LCD.h"
#include "Analog.h"
#include "StringsGlobals.h"
#include "Version.h"
#include "Globals.h"
#include "Temperature.h"
#include <SoftPWMServo.h>
#include "LCDTemplates.h"
#include "DebugStrings.h"
#include "CommandProcessor.h"
#include "ParserErrors.h"

//! Display class instance
AlphaLCD lcd(LCDdataPin, LCDclockPin, LCDlatchPin);

//! Internal temperature sensor class instance
Temperature internalTemp;

//! Initial potentiometer value
int pValue = 0;

//! Current lid status.
//! The variable is declared volatile as it is used by the
//! IRQ callback function too.
volatile boolean lidStatus;

//! The update display task id (assigned on setup)
int updateDispalyTaskID;

//! Stethoscope display template
LCDStethoscope steth(lcd);

//! Parser command structure
command cmd;
  
/** 
  \brief Initialisation method
  
  Loaded once at power-on the setup() method presets the parameters of the application and initializes
  the ChipKit board to its initial conditions. The startup conditions assumes that the Meditech lid
  is closed. If not, after a couple of seconds the alarm starts immediately.
  */
void setup() {
  Serial1.begin(SERIAL_SPEED);

  // Initializes the Lid status switch pin
  pinMode(LIDSTATUS, INPUT);
  lidStatus = LIDCLOSED;
  
  pinMode(ECG_STATUS, OUTPUT);
  pinMode(STETHOSCOPE_STATUS, OUTPUT);
  pinMode(CAMERA_STATUS, OUTPUT);

  digitalWrite(ECG_STATUS, HIGH);
  digitalWrite(STETHOSCOPE_STATUS, HIGH);
  digitalWrite(CAMERA_STATUS, HIGH);

  // Initializes the LCD library
  lcd.begin(LCDCHARS, LCDROWS);
  // Turn LCD On
  lcd.display();

  // Initialisation string
  welcome();
  // Test the LEDs
  testStatusLED();
  // Read the actual temperature
  internalTemp.CalcTemp(analogRead(TEMP_SENSOR));
  // Shows the actual internal temperature
  showTemp();
  
  // Start the fan to stopped state
  setFanSpeed(STOP_FAN);
  // Test LEDs on startup
  testStatusLED();
  // Test the fan variable speed
  testFanSpeed();
  
  // Set and start the timer for lid status
  attachCoreTimerService(isLidStatusChanged);
  // Set and start the timer for fan cooler speed regulation
  attachCoreTimerService(fanSpeedRegulation);
  
  // Create the display update task.
  // This task updates automatically only the reserved display
  // areas, i.e. the temperature monitor and other information.
  updateDispalyTaskID = createTask(updateDisplay, TASK_UPDATEDISPLAY, TASK_ENABLE, NULL);
  // Test only !!!
//  steth.createDisplay();
}

/** 
  \brief Main loop method
  
  The main application loop is interrupted by the lid open status generating
  a high priority alarm. Also the internal temperature is checked periodically
  to set the fan speed to the correct value.\n
  When serial data are present (a command waiting from the PI main) the data are
  parsed as needed.
  */
void loop(void) {

  // Check if the lid is open
  if(lidStatus == LIDCLOSED) {
    // Test only !!!
//    int pValue = analogRead(CALIBRATION_POT);  // Read the pot value
//    steth.updateDisplay(map(pValue, 0, ANALOGDIVIDER, MINGAIN, MAXGAIN));
    checkSerial();
  }
  else {
    // Show the error message
    lcd.clear();
    message(_LID_OPEN, 5, LCDTOPROW);
  }
}

// -------- Control functions

/**
  \brief Periodically update the display. This is a task callback function
  */
void updateDisplay(int id, void * tptr) {
 tempMonitor(); 
}

/**
  \brief Control the presence of data from the serial interface. 
  
  When data are detected on the serial buffer the parser function is 
  launched to process the command.
  
  */
void checkSerial() {

  if(Serial1.available()) {
    parser();
  }

}

/**
 * \brief Update the internal temperature from the sensor and update the display.
 */
void tempMonitor() {

  // Read the actual temperature
  internalTemp.CalcTemp(analogRead(TEMP_SENSOR));
  showTemp();

}

/**
 \brief shows the actual internal temperature on the top right corner. 
 This is a display-only method.
 */
void showTemp() {

  lcd.setCursor(14, LCDTOPROW);
  lcd << internalTemp.Celsius() << _CELSIUS;
}

/**
  \brief Callback function from the list status change hardware interrupt
  
  When the lid status switch changes (pinned to the LIDSTATUS pin) the attached
  interrupt calls this callback that simply invert the lidStatus state of the
  boolean variable. After the function has been exectued the timer is restarted.
  */
uint32_t isLidStatusChanged(uint32_t currentTime) {
  
    lidStatus = digitalRead(LIDSTATUS);
    
    // Restart the timer
    return (currentTime + CORE_TICK_RATE * LID_OPEN_TIMEOUT);
}

/**
  \brief Callback function from the fan cooler speed regulation interrupt
  
  As the interrupt triggers periodically a new temperature reading is done and the
  PWM fan cooler speed is set accordingly.
  */
uint32_t fanSpeedRegulation(uint32_t currentTime) {
  
  float temp;
  int pwmSpeed;

  temp = (((analogRead(TEMP_SENSOR) / 1024) * 5) * 100) - TEMP_OFFSET;
  pwmSpeed = map(temp, MIN_TEMP, MAX_TEMP, MIN_FANSPEED, MAX_FANSPEED);
  SoftPWMServoPWMWrite(FAN_SPEED, pwmSpeed);

  // Restart the timer
  return (currentTime + CORE_TICK_RATE * UPDATE_FAN_SPEED_TIMEOUT);
}

/**
 * \brief Welcome message shown at device power-on
 */
void welcome() {

  lcd.clear();
  lcd.setCursor(0, LCDTOPROW);
  lcd << project();
  lcd.setCursor(0, LCDBOTTOMROW);
  lcd << _VERSION << _SPACING << version() << _SPACING << _BUILD << _SPACING << build(); 
  delay(LCDMESSAGE_DELAY);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(_BD);
  lcd.setCursor(0, 1);
  lcd.print(_MEDITECH);
  delay(2500);
  lcd.clear();
}

/**
 * \brief Display a string on the LCD at the cursor position
 *
 * \param m the message string
 */
void message(String m) {
  lcd.print(m);
}

/**
 * \brief Display an error message at the specified cursor coordinates
 *
 * The error message is shown for a LCDERROR_DELAY milliseconds.
 * After the timeout expires the screen is not cleared so the next steps should be managed
 * by the program flow. It is expected that error messages are shown in a calling code that
 * manages the error conditions.
 *
 * \param m the message string
 * \param x the cursor column zero based
 * \param y the row number zero based
 */
void error(String m, int x, int y) {
  message(m, x, y);
  delay(LCDERROR_DELAY);
}

/**
 * \brief Display an error message at the cursor position
 *
 * The error message is shown for a LCDERROR_DELAY milliseconds.
 * After the timeout expires the screen is not cleared so the next steps should be managed
 * by the program flow. It is expected that error messages are shown in a calling code that
 * manages the error conditions.
 *
 * \param m the string message
 */
void error(String m) {
  message(m);
  delay(LCDERROR_DELAY);
}

/**
 * \brief Display a string on the LCD at the specified cursor coordinates
 *
 * \param m the string message
 * \param x the cursor column zero based
 * \param y the row number zero based
 */
void message(String m, int x, int y) {
  lcd.setCursor(x, y);
  message(m);
}

/**
  \brief Show the status led blinking for a cycle to test that all are working
  */
void testStatusLED() {

  digitalWrite(ECG_STATUS, LOW);
  delay(200);  
  digitalWrite(STETHOSCOPE_STATUS, LOW);
  delay(200);  
  digitalWrite(CAMERA_STATUS, LOW);
  delay(200);  

  digitalWrite(ECG_STATUS, HIGH);
  delay(200);  
  digitalWrite(STETHOSCOPE_STATUS, HIGH);
  delay(200);  
  digitalWrite(CAMERA_STATUS, HIGH);
  delay(200);  

}

/**
  \brief Variable fan speed test cycle
  */
void testFanSpeed() {
 
 for(int j = MIN_FANSPEED; j <= MAX_FANSPEED; j += 10) {
   setFanSpeed(j);
   delay(500);
 } 

 for(int j = MAX_FANSPEED; j <= MIN_FANSPEED; j -= 10) {
   setFanSpeed(j);
   delay(500);
 } 
}

/**
  \brief Set the fan speed based on the actual temperature
  
  The fan speed is mapped on the current temperature scaled to the
  PWM range values. Note that any PWM frequency under MIN_FAN_SPEED should
  not used at is has no effect (the motor don't start)
  
  \note This function sets manually the fan speed based on the internal temperature
  sensor. To use this function the interrupt timer fanSpeedRegulationTaskID should be
  disabled before.
  */
void setFanSpeed(float temp) {
  
  //! The filtered temperature to control the out-of-range values
  float filteredTemp;
  
  // Check the temperature range
  if (temp < MIN_TEMP)
    filteredTemp = MIN_TEMP;
  else
    if (temp > MAX_TEMP)
      filteredTemp = MAX_TEMP;
      else
        filteredTemp = temp;
  
  int pwmSpeed = map(temp, MIN_TEMP, MAX_TEMP, MIN_FANSPEED, MAX_FANSPEED);
  
  SoftPWMServoPWMWrite(FAN_SPEED, pwmSpeed);
}

/**
  \brief Set the fan speed at the desired PWM frequency
  
  The PWM frequency should be included between the min and max fan speed
  values. If these values are outside the limits the fan is set to off
  */
int setFanSpeed(int freq) {
  //! Fan speed PWM frequency, initialised to the stop status 
  int pwmFreq = STOP_FAN;
  
  // Check if the freq is a correct value
  if( (freq >= STOP_FAN) && (freq <= MAX_FANSPEED) ) {
    pwmFreq = freq;
  }
  // Set the current fan speed
  SoftPWMServoPWMWrite(FAN_SPEED, pwmFreq);
  
  return pwmFreq;
  
}

/**
  \brief Serial debugging function
*/
void debug(String msg) {
#ifdef __DEBUG
  Serial1 << DEBG_PREFIX << "> " << msg << endl;
#endif
}
  
/** 
  \brief Parses the serial input for control command
  
  If the syntax checker doesn't recognize any valid command
  strings are ignored and discharged. Every command is a single-character,
  case sensitive. All the characters recognised as a command are
  listed in the CMD_CHARACTERS constant; when the parser starts only those
  commands included in this string and stored in the cBuff character buffer
  can be accepted as valid commands, else the string portion is not processed
  until the next command separator (or the end of the string if there is only
  one command or it is the last).
  */
void parser() {
  //! Counters and indexes. The initaliasation
  //! matters are these are the correct points for the
  //! parser recursive process.
  int i = 0, k = 0, j = 0, value;
  //! Single-character commands array
  char c[] = { 'E', 'D', 'G', 'I', 'T', 'R', 'P', 'r', '\0' };
  
  // Initialises the command structure
  cmd.cmdData[0] = '\0';
  cmd.message = "";

  // Load the command string coming from serial in the character array
  while (Serial1.available() > 0)
      cmd.cmdData[i++] = (char)Serial1.read();
  
  // Add the closing command character
  cmd.cmdData[i++] = CMD_SEPARATOR;
  cmd.cmdData[i] = '\0';
  
  debug(cmd.cmdData);
  
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
      // Process the current command character
      if (c[j] == cmd.cmdData[k]) {
        // Found a matching command
        switch(cmd.cmdData[k]) {
          // Show a string on the display.
          case CMD_DISPLAY:
            appendResponse(CMD_DISPLAY);
            // Syntax checking
            if (!parseFieldSeparator(cmd.cmdData[++k])) {
              syntaxCheck(COMMAND_MISSINGSEPARATOR);
              ackMaster();
              k = nextCommandSeparator(k);
              break;
            }
            // We expect the next paramter is row expressed in integer format
            value = charsToInt(++k, PARM_INTEGER_LEN);
            if (value < LCDROWS) {
              syntaxCheck(COMMAND_OK);
              cmd.intValue[0] = value;
            } // No errors
            else {
              syntaxCheck(COMMAND_OUT_OF_RANGE);
              ackMaster();
              k = nextCommandSeparator(k);
              break;
            } // Error out of range
            
            k += PARM_INTEGER_LEN; // Update the command string pointer

            // Syntax checking
            if (!parseFieldSeparator(cmd.cmdData[k])) {
              syntaxCheck(COMMAND_MISSINGSEPARATOR);
              ackMaster();
              k = nextCommandSeparator(k);
              break;
            }
            // We expect the next paramter is column expressed in integer format
            value = charsToInt(++k, PARM_INTEGER_LEN);
            if (value < LCDCHARS) {
              syntaxCheck(COMMAND_OK);
              cmd.intValue[1] = value;
            } // No errors
            else {
              syntaxCheck(COMMAND_OUT_OF_RANGE);
              ackMaster();
              k = nextCommandSeparator(k);
              break;
            } // Error out of range
            
            k += PARM_INTEGER_LEN; // Update the command string pointer

            // Syntax checking
            if (!parseFieldSeparator(cmd.cmdData[k])) {
              syntaxCheck(COMMAND_MISSINGSEPARATOR);
              ackMaster();
              k = nextCommandSeparator(k);
              break;
            }
            // We expect the next paramter is the data string
            cmd.stringValue = charsToString(k);
            syntaxCheck(COMMAND_OK);
            // Display the message string on LCD
            message(cmd.stringValue, cmd.intValue[1], cmd.intValue[0]);
            ackMaster();
            break;

            // Enable a status on the control panel (mainly a probe setting).
            // The details of the object to enable are specified in the subcommand
            case CMD_ENABLE:
              appendResponse(CMD_ENABLE);
              // Search for subcommands
              if (!parseFieldSeparator(cmd.cmdData[++k])) {
                syntaxCheck(COMMAND_MISSINGSEPARATOR);
                ackMaster();
                k = nextCommandSeparator(k);
                break;
              } // Search subcommands
              cmd.subcommand[0] = cmd.cmdData[++k];
              // Manage the subcommands parameter
              switch(cmd.subcommand[0]) {
                // Enable / disable the stethoscope probe control panel settings
                case S_STETHOSCOPE:
                  appendResponse(S_STETHOSCOPE);
                  // Check for subcommand separator
                  if (!parseFieldSeparator(cmd.cmdData[++k])) {
                      syntaxCheck(COMMAND_MISSINGSEPARATOR);
                      ackMaster();
                      k = nextCommandSeparator(k);
                      break;
                  } // check for separator
                  if (!setStethoscopeStatus(++k))
                      syntaxCheck(COMMAND_STETHOSCOPE_PARAMERROR);
                  else
                      syntaxCheck(COMMAND_OK);
                  ackMaster();
                  k = nextCommandSeparator(k);
                  break;
                // Enable / disable the ECG probe control panel settings
                case S_ECG:
                  appendResponse(S_ECG);
                  // Check for subcommand separator
                  if (!parseFieldSeparator(cmd.cmdData[++k])) {
                      syntaxCheck(COMMAND_MISSINGSEPARATOR);
                      ackMaster();
                      k = nextCommandSeparator(k);
                      break;
                  } // check for separator
                  if (!setECGStatus(++k))
                      syntaxCheck(COMMAND_ECG_PARAMERROR);
                  else
                      syntaxCheck(COMMAND_OK);
                  ackMaster();
                  k = nextCommandSeparator(k);
                  break;
                // Enable / disable the Blood Pressure probe control panel settings
                case S_PRESSURE:
                  appendResponse(S_PRESSURE);
                  // Check for subcommand separator
                  if (!parseFieldSeparator(cmd.cmdData[++k])) {
                      syntaxCheck(COMMAND_MISSINGSEPARATOR);
                      ackMaster();
                      k = nextCommandSeparator(k);
                      break;
                  } // check for separator
                  if (!setPressureStatus(++k))
                      syntaxCheck(COMMAND_PRESSURE_PARAMERROR);
                  else
                      syntaxCheck(COMMAND_OK);
                  ackMaster();
                  k = nextCommandSeparator(k);
                  break;
                // Enable / disable the Body Temperature probe control panel settings
                case S_BODYTEMP:
                  appendResponse(S_BODYTEMP);
                  // Check for subcommand separator
                  if (!parseFieldSeparator(cmd.cmdData[++k])) {
                      syntaxCheck(COMMAND_MISSINGSEPARATOR);
                      ackMaster();
                      k = nextCommandSeparator(k);
                      break;
                  } // check for separator
                  if (!setBodyTempStatus(++k))
                      syntaxCheck(COMMAND_BODYTEMP_PARAMERROR);
                  else
                      syntaxCheck(COMMAND_OK);
                  ackMaster();
                  k = nextCommandSeparator(k);
                  break;
                // Enable / disable the Heart Beat probe control panel settings
                case S_HEARTBEAT:
                  appendResponse(S_HEARTBEAT);
                  // Check for subcommand separator
                  if (!parseFieldSeparator(cmd.cmdData[++k])) {
                      syntaxCheck(COMMAND_MISSINGSEPARATOR);
                      ackMaster();
                      k = nextCommandSeparator(k);
                      break;
                  } // check for separator
                  if (!setHeartBeatStatus(++k))
                      syntaxCheck(COMMAND_HEARTBEAT_PARAMERROR);
                  else
                      syntaxCheck(COMMAND_OK);
                  ackMaster();
                  k = nextCommandSeparator(k);
                  break;
                // Subcommand unknown
                default:
                    syntaxCheck(PARSER_SUBCOMMAND_UNKNOWN);
                    ackMaster();
                    break;
            } // Subcommands for the Enable command
            break;

            // Show the global info on the control panel display
            case CMD_INFO:
              appendResponse(CMD_INFO);
              ackMaster();
            break;

            // Executes a test cycle of the control panel
            case CMD_TEST:
              appendResponse(CMD_TEST);
              ackMaster();
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
int nextCommandSeparator(int startChar) {
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
  int res = 0;

  res = (int)charsToLong(startChar, numChars);
  
  return res;
}

/**
  \brief Convert a portion of the char array command string to string
  
  This function does not include the number of characters as it is a variable
  lenght. The command strings are identified by the initial and final '"'
  string delimiter character, so the function search for the first delimiter
  and queue charaters in the destination string until the second string delimiter
  is not found.\n
  If no string delimiters are found the cycle stops when reach the CMD_MSGLEN character
  count limit; in this case an empty string is returned. If only the first string delimiter
  is found the function returns the entire right string command character array.
  
  \warning As the character '"' is used as string delimiter it should not be used as
  part of the string. Use the ''' instead.
  
  \param startChar initial character in the command string
  \return The converted interger value
  */
String charsToString(int startChar) {
  String temp = "";
  int i = 0;
  bool inString = false;
  
  // ========================================== FIRST STRING DELIMITER
  // Search for the first string delimiter
  while(!inString) {
    // Check if the current char points to the first string delimiter
    if(cmd.cmdData[i + startChar] == STRING_DELIMITER) {
      i++;
      inString = true;
    } // String delimieter found ?
    else {
      // Check for the end of string
      if(cmd.cmdData[i + startChar] == MAX_CMD_LEN) {
        // Exit with an empty string
        return "";
      } // Reached the end of the command string ?
      else {
        i++;
      } // Search to next character
    } // Delimiter not found
  } // While out of string

  // ========================================== UNTIL SECOND STRING DELIMITER
  // Collect the string characters
  while(inString) {
    // Check if the current char points to the first string delimiter
    if(cmd.cmdData[i + startChar] == STRING_DELIMITER) {
      inString = false;
    } // String delimieter found ?
    else {
      // Check for the end of string
      if(cmd.cmdData[i + startChar] == MAX_CMD_LEN) {
        // Exit with the collected string
        return temp;
      } // Reached the end of the command string ?
      else {
        // Collect the character in the string and move to next
        temp.concat(cmd.cmdData[i + startChar]);
        i++;
      } // Search to next character
    } // Delimiter not found
  } // While inside the string

  // Found both delimiters and collected the string.
  return temp;
}

/**
  \brief Convert a string to long integer
  
  \param startChar initial character in the command string
  \param numChars number of characters composing the number
  \return The converted long interger value
  */
long charsToLong(int startChar, int numChars) {
  int i;
  long res = 0;
  String extract = "";

  for (i = 0; i < numChars; i++)
    extract.concat(cmd.cmdData[i + startChar]);
    
  res = extract.toInt();

  return res;
}

/**
  \brief Convert a string to floating point
  
  \param startChar initial character in the command string
  \param numChars number of characters composing the number
  \return The converted floating value
  */
float charsToFloat(int startChar, int numChars) {
  int i;
  float res = 0;
  String extract = "";

  for (i = 0; i < numChars; i++)
    extract.concat(cmd.cmdData[i + startChar]);
    
  res = extract.toFloat();

  return res;
}

/**
  \brief check for separator character
  
  \param test the character to test
  \return true if test is equal else returns false.
  */
boolean parseFieldSeparator(char test) {
  if (test == FIELD_SEPARATOR)
      return true;
  else
      return false;
}

/**
  \brief Append the error code to the master response message string.
  
  \param errCode the error code from the command
  */
void syntaxCheck(int errCode) {
  appendResponse(errCode);
}

/**
  \brief Append the string to the response message string
  
  Every new step of the parser that needs to be mentioned to the connected master
  updates the response string. The string should be sent when the parser exits
  for any condition.
  
  \note The reponse string is NOT verbose as it should be managed faster and efficiently
  by the master. Every new message parameter added to the response string is separated
  by a RESPONSE_SEPARATOR character.
  
  \param message The response message to be added, usually a single-character elment or an error code
  */
void appendResponse(String message) {
  cmd.message.concat(RESPONSE_SEPARATOR);
  cmd.message.concat(message);
}

/**
  \brief Append the character to the response message string
  
  Every new step of the parser that needs to be mentioned to the connected master
  updates the response string. The string should be sent when the parser exits
  for any condition.
  
  \note The reponse string is NOT verbose as it should be managed faster and efficiently
  by the master. Every new message parameter added to the response string is separated
  by a RESPONSE_SEPARATOR character.
  
  \param message The response message to be added, usually a single-character elment or an error code
  */
void appendResponse(char message) {
  cmd.message.concat(RESPONSE_SEPARATOR);
  cmd.message.concat(message);
}

/**
  \brief Append the integer to the response message string
  
  Every new step of the parser that needs to be mentioned to the connected master
  updates the response string. The string should be sent when the parser exits
  for any condition.
  
  \note The reponse string is NOT verbose as it should be managed faster and efficiently
  by the master. Every new message parameter added to the response string is separated
  by a RESPONSE_SEPARATOR character.
  
  \param message The response message to be added, usually a single-character elment or an error code
  */
void appendResponse(int message) {
  cmd.message.concat(RESPONSE_SEPARATOR);
  cmd.message.concat(message);
}

/**
  \brief Append the long integer to the response message string
  
  Every new step of the parser that needs to be mentioned to the connected master
  updates the response string. The string should be sent when the parser exits
  for any condition.
  
  \note The reponse string is NOT verbose as it should be managed faster and efficiently
  by the master. Every new message parameter added to the response string is separated
  by a RESPONSE_SEPARATOR character.
  
  \param message The response message to be added, usually a single-character elment or an error code
  */
void appendResponse(long int message) {
  cmd.message.concat(RESPONSE_SEPARATOR);
  cmd.message.concat(message);
}

/**
  \brief Append the float to the response message string
  
  Every new step of the parser that needs to be mentioned to the connected master
  updates the response string. The string should be sent when the parser exits
  for any condition.
  
  \note The reponse string is NOT verbose as it should be managed faster and efficiently
  by the master. Every new message parameter added to the response string is separated
  by a RESPONSE_SEPARATOR character.
  
  \param message The response message to be added, usually a single-character elment or an error code
  */
void appendResponse(float message) {
  cmd.message.concat(RESPONSE_SEPARATOR);
  cmd.message.concat(message);
}

/**
  \brief Acknowledge the master caller when a command parsing has been completed
  
  */
void ackMaster() {
  Serial1 << cmd.message << endl;
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

    Serial1 << int(val) << ".";

    for (i = 1; i <= precision; i += i * 10)
        if (frac < i)
            Serial1 << "0";

    Serial1 << frac;
}

/**
  \biref Set the stethoscope status flag and initialises the
  display parameters if needed.
  
  \param startChar initial character in the command string
  \todo Implement this function
  */
bool setStethoscopeStatus(int startChar) {
  int res = 0;
  String extract = "";

  extract.concat(cmd.cmdData[startChar]);
  res = extract.toInt();

  if(res == 0)
    return false;
  else
    return true;
}

/**
  \biref Set the ECG status flag and initialises the
  display parameters if needed.
  
  \todo Implement this function
  */
bool setECGStatus(int startChar) {
  int res = 0;
  String extract = "";
  
  extract.concat(cmd.cmdData[startChar]);
  res = extract.toInt();

  if(res == 0)
    return false;
  else
    return true;
}

/**
  \biref Set the bood pressure status flag and initialises the
  display parameters if needed.
  
  \param startChar initial character in the command string
  \todo Implement this function
  */
bool setPressureStatus(int startChar) {
  int res = 0;
  String extract = "";

  extract.concat(cmd.cmdData[startChar]);
  res = extract.toInt();

  if(res == 0)
    return false;
  else
    return true;
}

/**
  \biref Set the body temperature status flag and initialises the
  display parameters if needed.
  
  \param startChar initial character in the command string
  \todo Implement this function
  */
bool setBodyTempStatus(int startChar) {
  int res = 0;
  String extract = "";

  extract.concat(cmd.cmdData[startChar]);
  res = extract.toInt();

  if(res == 0)
    return false;
  else
    return true;
}

/**
  \biref Set the heart beat status flag and initialises the
  display parameters if needed.
  
  The function does not need the number of characters parameters as the
  status ia boolean single character for the flag
  
  \param startChar initial character in the command string
  \todo Implement this function
  */
bool setHeartBeatStatus(int startChar) {
  int res = 0;
  String extract = "";
  
  extract.concat(cmd.cmdData[startChar]);
  res = extract.toInt();

  if(res == 0)
    return false;
  else
    return true;
}

