/**
  \file LCDBloodPressure.cpp
  \brief LCD display template for the probe status

  */

#include "LCD.h"
#include "LCDTemplates.h"

/**
  \brief Class constructor
  
  Initialises the fields with the static values
  and assign the local LCD class pointer with the calling
  program LCD class global instance.
  The field name strings are assigned by the parser as far as are
  received from the master.
  */
LCDBloodPressure::LCDBloodPressure(AlphaLCD myLCD) {
  mLcd = myLCD;
  id = TID_BLOODPRESS;
  
  // Initialise the fields
  lcdFields[BLOOD_TITLE].row = 0;
  lcdFields[BLOOD_TITLE].col = 0;
  
  lcdFields[BLOOD_WAIT].row = 1;
  lcdFields[BLOOD_WAIT].col = 16;
  
  lcdFields[BLOOD_MIN].row = 1;
  lcdFields[BLOOD_MIN].col = 0;

  lcdFields[BLOOD_MINVAL].row = 1;
  lcdFields[BLOOD_MINVAL].col = 3;

  lcdFields[BLOOD_MAX].row = 1;
  lcdFields[BLOOD_MAX].col = 7;

  lcdFields[BLOOD_MAXVAL].row = 1;
  lcdFields[BLOOD_MAXVAL].col = 9;
}

/**
  \brief Create the full display view.
  
  This method should be called only when the template
  is shown on start then only the updateDisplay() method
  should be used to change the variable fields.
  */
void LCDBloodPressure::createDisplay() {
  mLcd.clear();

  for(int j = 0; j < BLOODPRESS_FIELDS; j++) {
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
void LCDBloodPressure::updateDisplay(int val, int field) {
  lcdFields[field].val = String(val);

  mLcd.setCursor(lcdFields[field].col, lcdFields[field].row);
  mLcd << lcdFields[field].val;
}

