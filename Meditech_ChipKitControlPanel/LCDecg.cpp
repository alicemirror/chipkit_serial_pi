/**
  \file LCDecg.cpp
  \brief LCD display template for E.C.G. probe
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

#define TID_ECG 4
#define ECG_FIELDS 3
#define ECG_TITLE 0
#define ECG_STATUS 1
#define ECG_STATUSFLAG 2

LCDecg::LCDecg(AlphaLCD myLCD) {
  mLcd = myLCD;
  id = TID_ECG;
  
  // Initialise the fields
  lcdFields[ECG_TITLE].row = 0;
  lcdFields[ECG_TITLE].col = 0;
  
  lcdFields[ECG_STATUS].row = 1;
  lcdFields[ECG_STATUS].col = 0;
  
  lcdFields[ECG_STATUSFLAG].row = 1;
  lcdFields[ECG_STATUSFLAG].col = 7;
}

/**
  \brief Create the full display view.
  
  This method should be called only when the template
  is shown on start then only the updateDisplay() method
  should be used to change the variable fields.
  */
void LCDecg::createDisplay() {
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
void LCDecg::updateDisplay(int val, int field) {
  lcdFields[field].val = String(val);

  mLcd.setCursor(lcdFields[field].col, lcdFields[field].row);
  mLcd << lcdFields[field].val;
}

