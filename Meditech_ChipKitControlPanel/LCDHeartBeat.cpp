/**
  \file LCDHeartBeat.cpp
  \brief LCD display template for the heart beat probe status

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
LCDHeartBeat::LCDHeartBeat(AlphaLCD myLCD) {
  mLcd = myLCD;
  id = TID_HEARTBEAT;
  
  // Initialise the fields
  lcdFields[HEARTBEAT_TITLE].row = 0;
  lcdFields[HEARTBEAT_TITLE].col = 0;
  
  lcdFields[HEARTBEAT_SPOT].row = 1;
  lcdFields[HEARTBEAT_SPOT].col = 0;
  
  lcdFields[HEARTBEAT_SPOTVAL].row = 1;
  lcdFields[HEARTBEAT_SPOTVAL].col = 5;

  lcdFields[HEARTBEAT_AVERAGE].row = 1;
  lcdFields[HEARTBEAT_AVERAGE].col = 10;

  lcdFields[HEARTBEAT_AVERAGEVAL].row = 1;
  lcdFields[HEARTBEAT_AVERAGEVAL].col = 15;
}

/**
  \brief Create the full display view.
  
  This method should be called only when the template
  is shown on start then only the updateDisplay() method
  should be used to change the variable fields.
  */
void LCDHeartBeat::createDisplay() {
  mLcd.clear();

  for(int j = 0; j < HEARTBEAT_FIELDS; j++) {
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
void LCDHeartBeat::updateDisplay(int val, int field) {
  lcdFields[field].val = String(val);

  mLcd.setCursor(lcdFields[field].col, lcdFields[field].row);
  mLcd << lcdFields[field].val;
}


