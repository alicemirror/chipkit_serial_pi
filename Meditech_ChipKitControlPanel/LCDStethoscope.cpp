/**
  \file LCDStethoscope.cpp
  \brief LCD display template for stethoscope probe

  LCDStethoscope class defines the visualisation template of the LCD screen 
  output for the Stethoscope probe.

  */

#include "LCD.h"
#include "LCDTemplates.h"

/**
  \brief Class constructor
  
  Initialises the fields with the static values
  and assign the local LCD class pointer with the calling
  program LCD class global instance.
  */
LCDStethoscope::LCDStethoscope(AlphaLCD myLCD) {
  mLcd = myLCD;
  id = TID_STETHOSCOPE;
  
  // Initialise the fields
  lcdFields[STET_TITLE].row = 0;
  lcdFields[STET_TITLE].col = 0;
  lcdFields[STET_TITLE].val = "Stethoscope";
  
  lcdFields[STET_GAIN].row = 1;
  lcdFields[STET_GAIN].col = 0;
  lcdFields[STET_GAIN].val = "Gain value";
  
  lcdFields[STET_GAINVAL].row = 1;
  lcdFields[STET_GAINVAL].col = 12;
  lcdFields[STET_GAINVAL].val = "00";
}

/**
  \brief Create the full display view.
  
  This method should be called only when the template
  is shown on start then only the updateDisplay() method
  should be used to change the variable fields.
  */
void LCDStethoscope::createDisplay() {
  mLcd.clear();

 for(int j = 0; j < STETHOSCOPE_FIELDS; j++) {
    mLcd.setCursor(lcdFields[j].col, lcdFields[j].row);
    mLcd << lcdFields[j].val;
 }
}

/**
  \brief Update the variable content field(s) only
  
  This method should be called every time the field content
  is updated. It affects only the data content fields. To 
  recreate the entire display the createDisplay() method
  should be called.
  When the method is called, the class field value is updated after
  the proper conversion of the value,
  */
void LCDStethoscope::updateDisplay(int val) {
    lcdFields[STET_GAINVAL].val = String(val);
  
    mLcd.setCursor(lcdFields[STET_GAINVAL].col, lcdFields[STET_GAINVAL].row);
    mLcd << lcdFields[STET_GAINVAL].val;
  
}


