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
  ChipKit PI board.
  This application is part of the Meditech project, created by Enrico Miglino for
  Balearic Dynamics - Balearic Islands - Spain\n
  For the last updated application version and subversion, see the version.h include file.
  
  \author Enrico Miglino <enrico.miglino@gmail.com>
  \author Balearic Dynamics - Spain <balearicdynamics@gmail.com>
  \version 1.0b
  \date First version on July 2015
*/

#include <AlphaLCD.h>
#include <Streaming.h>

#include "LCD.h"
#include "Analog.h"
#include "Strings.h"
#include "Version.h"
#include "Globals.h"
#include "Temperature.h"
#include <SoftPWMServo.h>

#include "DebugStrings.h"

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

/** 
  \brief Initialisation method
  
  Loaded once at power-on the setup() method presets the parameters of the application and initializes
  the ChipKit board to its initial conditions. The startup conditions assumes that the Meditech lid
  is closed. If not, after a couple of seconds the alarm starts immediately.
  */
void setup() {
  Serial1.begin(9600);

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
  temperature();
  
  // Start the fan to stopped state
  setFanSpeed(STOP_FAN);
  // Test LEDs on startup
  testStatusLED();
  
  // Set and start the timer
  attachCoreTimerService(isLidStatusChanged);
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
    pValue = analogRead(CALIBRATION_POT);  // Read the pot value
    int gainValue = map(pValue, 0, ANALOGDIVIDER, MINGAIN, MAXGAIN);
    // Update the display
    stethoscopeGainLevel(gainValue);
  }
  else {
      // Show the error message
      lcd.clear();
      message(_LID_OPEN, 5, LCDTOPROW);
      // Temperature monitoring
      tempMonitor();
  }
}

// -------- Control functions

/**
 * \brief Continuously monitor the internal temperature
 */
void tempMonitor() {

  // Read the actual temperature
  internalTemp.CalcTemp(analogRead(TEMP_SENSOR));

  lcd.setCursor(0, 1);
  lcd << _INTERNAL_TEMP << " " << internalTemp.Celsius() << _CELSIUS;
  delay(50);

}

/**
 * \brief shows the actual internal temperature
 */
void temperature() {

  lcd.setCursor(0, 0);
  lcd.print(_INTERNAL_TEMP);
  lcd.setCursor(0, 1);
  lcd  << _EMPTY_NUMBER4 << internalTemp.Celsius() << _CELSIUS;

}

/**
  \brief Callback function from the list status change hardware interrupt
  
  Then the lid status switch changes (pinned to the LIDSTATUS pin) the attached
  interrupt calls this callback that simply invert the lidStatus state of the
  boolean variable. After the function has been exectued the timer is restarted.
  */
uint32_t isLidStatusChanged(uint32_t currentTime) {
  
    lidStatus = digitalRead(LIDSTATUS);
    
    // Restart the timer
    return (currentTime + CORE_TICK_RATE/LID_OPEN_TIMEOUT);
}

/**
 * \brief Stethoscope title
 */
void stethoscopeMsg() {

  lcd.clear();
  lcd.setCursor(0, LCDTOPROW);
//  lcd << _STET;
  lcd.setCursor(0, LCDBOTTOMROW);
//  lcd << _STET_CAL;

}

/**
 * \brief Update the scaled analog value corresponding to the gain setting
 */
void stethoscopeGainLevel(double value) {
  lcd.setCursor(12, LCDBOTTOMROW);
  lcd << value;
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
 * \brief Display an integer value in decimal format at the cursor position
 *
 * \param n the integer to show in decimal format
 */
void dec(int n) {
  lcd.print(n, DEC);
}

/**
 * \brief Display an integer value in hex format at the cursor position
 *
 * \param n the integer to show in hexadecimal format
 */
void hex(int n) {
  lcd.print("0x");
  lcd.print(n, HEX);
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
 * \brief Clean the display
 *
 * A delay of 100 ms is added after the hardware clear() call to give the display the time
 * to complete the operation.
 */
void clean() {
  lcd.clear();
  delay(LCDCLEAR_DELAY);
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
  \brief Set the fan speed based on the actual temperature
  
  The fan speed is mapped on the current temperature scaled to the
  PWM range values. Note that any PWM frequency under MIN_FAN_SPEED should
  not used at is has no effect (the motor don't start)
  */
void setFanSpeed(float temp) {
    
    SoftPWMServoPWMWrite(FAN_SPEED, temp);
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
  


