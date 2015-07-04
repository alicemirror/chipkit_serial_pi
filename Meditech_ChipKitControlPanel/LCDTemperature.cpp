/**
  \file LCDTemperature.cpp
  \brief LCD display template for the info screen

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
LCDTemperature::LCDTemperature(AlphaLCD myLCD) {
  mLcd = myLCD;
  id = TID_INFO;
  
  // Initialise the fields
  lcdFields[TEMPERATURE_TITLE].row = 0;
  lcdFields[TEMPERATURE_TITLE].col = 0;
  
  lcdFields[TEMPERATURE_SPOT].row = 1;
  lcdFields[TEMPERATURE_SPOT].col = 0;
  
  lcdFields[TEMPERATURE_SPOTVAL].row = 1;
  lcdFields[TEMPERATURE_SPOTVAL].col = 5;

  lcdFields[TEMPERATURE_AVERAGE].row = 1;
  lcdFields[TEMPERATURE_AVERAGE].col = 10;

  lcdFields[TEMPERATURE_AVERAGEVAL].row = 1;
  lcdFields[TEMPERATURE_AVERAGEVAL].col = 15;
}

/**
  \brief Create the full display view.
  
  This method should be called only when the template
  is shown on start then only the updateDisplay() method
  should be used to change the variable fields.
  */
void LCDTemperature::createDisplay() {
  mLcd.clear();

  for(int j = 0; j < TEMPERATURE_FIELDS; j++) {
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
void LCDTemperature::updateDisplay(int val, int field) {
  lcdFields[field].val = String(val);

  mLcd.setCursor(lcdFields[field].col, lcdFields[field].row);
  mLcd << lcdFields[field].val;
}

