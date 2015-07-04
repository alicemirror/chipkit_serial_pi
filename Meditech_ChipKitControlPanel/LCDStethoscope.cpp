/**
  \file LCDStethoscope.cpp
  \brief LCD display template for stethoscope probe
  */

#include "LCD.h"
#include "LCDTemplates.h"

/**
  \brief Class constructor
  
  Initialises the fields with the static values
  and assign the local LCD class pointer with the calling
  program LCD class global instance.
  The field name strings are assigned by the parser as far as what is
  received from the master.
  */
LCDStethoscope::LCDStethoscope(AlphaLCD myLCD) {
  mLcd = myLCD;
  id = TID_STETHOSCOPE;
  
  // Initialise the fields
  lcdFields[STET_TITLE].row = 0;
  lcdFields[STET_TITLE].col = 0;
  
  lcdFields[STET_GAIN].row = 1;
  lcdFields[STET_GAIN].col = 0;
  
  lcdFields[STET_GAINVAL].row = 1;
  lcdFields[STET_GAINVAL].col = 12;
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
  of a variable is updated. It affects only the data content fields. To 
  recreate the entire display the createDisplay() method
  should be called.
  When the method is called, the class field value is updated after
  the value conversion.
  
  \param val The value to update
  \param field The field ID
  */
void LCDStethoscope::updateDisplay(int val, int field) {
  lcdFields[field].val = String(val);

  mLcd.setCursor(lcdFields[field].col, lcdFields[field].row);
  mLcd << lcdFields[field].val;
}

