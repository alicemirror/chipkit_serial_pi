/**
  \file LCDInfo.cpp
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
LCDInfo::LCDInfo(AlphaLCD myLCD) {
  mLcd = myLCD;
  id = TID_INFO;
  
  // Initialise the fields
  lcdFields[INFO_TITLE].row = 0;
  lcdFields[INFO_TITLE].col = 0;
  
  lcdFields[INFO_RPM].row = 1;
  lcdFields[INFO_RPM].col = 16;
  
  lcdFields[INFO_DATE].row = 0;
  lcdFields[INFO_DATE].col = 4;

  lcdFields[INFO_TIME].row = 0;
  lcdFields[INFO_TIME].col = 9;

  lcdFields[INFO_GPS].row = 1;
  lcdFields[INFO_GPS].col = 0;
}

/**
  \brief Create the full display view.
  
  This method should be called only when the template
  is shown on start then only the updateDisplay() method
  should be used to change the variable fields.
  */
void LCDInfo::createDisplay() {
  mLcd.clear();

  for(int j = 0; j < INFO_FIELDS; j++) {
    mLcd.setCursor(lcdFields[j].col, lcdFields[j].row);
    mLcd << lcdFields[j].val;
  }
}


