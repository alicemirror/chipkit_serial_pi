/**
  \file Meditech_ChipKitControlPanel.pde
  \brief meditech ChipKit control panel main application

  \mainpage
  This application will run on the ChipKit PI module of the Meditech device. The module is
  connected to the Raspberry PI Mod B2. The software includes two parts: one independent series
  of features, related to the Meditech general health status that can override the user controlled
  parameters. The board also control che alphanumeric LCD display for status messages, warnings and alarms. \n
  The user-controlled part is managed by the Raspberry PI "PI master" that control the active probes
  and other functions. The counterpart of this application is a C++ command set running on the PI linux
  machine and communicates with the board through a simple serial protocol.
  To reduce the weight of this microcontroller application most of the informational strings shown by
  the control panel display are send by the linux machine.
  
  \note This code is distributed under the Apache license
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

#undef _DEBUG

//! Display class instance
AlphaLCD lcd(LCDdataPin, LCDclockPin, LCDlatchPin);

//! Internal temperature sensor class instance
Temperature internalTemp;

//! Initial potentiometer value
int pValue = 0;

//! Current lid status
boolean lidStatus;

//! Initialisation method
void setup() 
{
  
  #ifdef _DEBUG
  Serial1.begin(9600);
  #endif

  // Initializes the Lid status swith pin
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
  
  // Read the actual temperature
  internalTemp.CalcTemp(analogRead(TEMP_SENSOR));
  
  // Shows the actual internal temperature
  temperature();

  // Digital stethoscope calibration 
  stethoscopeMsg();
}

//! Main loop application
void loop(void)
{
  #ifdef _DEBUG
  Serial1 << "=== DEBUG ON ==" << endl;
  testFan();
  // Read the actual temperature
  internalTemp.CalcTemp(analogRead(TEMP_SENSOR));
  Serial1 << "Temperature read: " << internalTemp.Celsius() << endl;
  Serial1 << "=== DEBUG OFF ==" << endl;
  #endif
  // Check if the lid is open
  if(isLidClose() == LIDCLOSED) {
    // If the lid was open before, reset the display status
    if(lidStatus == LIDOPEN) {
      // Reset the status
      lidStatus = LIDCLOSED;
      
      // Digital stethoscope calibration 
      stethoscopeMsg();
    }
      
    pValue = analogRead(CALIBRATION_POT);  // Read the pot value
  
    int gainValue = map(pValue, 0, ANALOGDIVIDER, MINGAIN, MAXGAIN);
    
    // Update the display
    stethoscopeGainLevel(gainValue);

  }
  else {
      // Set the status flag
      lidStatus = LIDOPEN;

      // Show the error message
      lcd.clear();
      message(_LID_OPEN, 5, LCDTOPROW);

      // Temperature monitoring
      tempMonitor();

      delay(50);

      testStatusLED();
            
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
  delay(2500);
  lcd.clear();

}

/**
 * \brief Check for lid status
 */
 boolean isLidClose() {
   return digitalRead(LIDSTATUS);
 }

/**
 * \brief Stethoscope title
 */
void stethoscopeMsg() {

  lcd.clear();
  lcd.setCursor(0, LCDTOPROW);
  lcd << _STET;
  lcd.setCursor(0, LCDBOTTOMROW);
  lcd << _STET_CAL;

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
 * \brief Creates a menu screen.
 *
 * The 2-lines LCD screen is divided in four sectors, that can be used or not. The LCD
 * sectors length and position are defined and based on the LCD size.
 * Every sector is filled with one of the four parameters string.
 * Sectors 1 & 2 are in the top row, sectors 3 & 4 in the bottom row
 *
 * \param sect1 The Upper Left display sector
 * \param sect2 The Upper Right display sector
 * \param sect3 The Lower Left display sector
 * \param sect4 The Lower Right display sector
 */
void menu(String sect1, String sect2, String sect3, String sect4) {
  clean();
  message(sect1, LCD_SECTOR1, LCDTOPROW);
  message(sect2, LCD_SECTOR2, LCDTOPROW);
  message(sect3, LCD_SECTOR3, LCDBOTTOMROW);
  message(sect4, LCD_SECTOR4, LCDBOTTOMROW);
}

/////////////
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

#ifdef _DEBUG
void testFan() {

    SoftPWMServoPWMWrite(FAN_SPEED, 0);
    Serial1 << "FanTest speed=0" << endl;
    delay(5000);
    SoftPWMServoPWMWrite(FAN_SPEED, 255);
    Serial1 << "FanTest speed=255" << endl;
    delay(5000);

}
#endif



