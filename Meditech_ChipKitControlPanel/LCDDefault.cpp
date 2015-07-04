/**
  \file LCDDefault.cpp
  \brief LCD display template for the default view

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
LCDDefault::LCDDefault(AlphaLCD myLCD) {
  mLcd = myLCD;
  id = TID_DEFAULT;
  
  // Initialise the fields
  lcdFields[DEFAULT_TITLE].row = 0;
  lcdFields[DEFAULT_TITLE].col = 0;
  
  lcdFields[DEFAULT_VERSION].row = 1;
  lcdFields[DEFAULT_VERSION].col = 0;
  
  lcdFields[DEFAULT_STATUS].row = 1;
  lcdFields[DEFAULT_STATUS].col = 10;
}

/**
  \brief Create the full display view.
  
  This method should be called only when the template
  is shown on start then only the updateDisplay() method
  should be used to change the variable fields.
  */
void LCDDefault::createDisplay() {
  mLcd.clear();

  for(int j = 0; j < DEFAULT_FIELDS; j++) {
    mLcd.setCursor(lcdFields[j].col, lcdFields[j].row);
    mLcd << lcdFields[j].val;
  }
}


