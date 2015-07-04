/**
  \file LCDTest.cpp
  \brief LCD display template for the test screen
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
LCDTest::LCDTest(AlphaLCD myLCD) {
  mLcd = myLCD;
  id = TID_TEST;
  
  // Initialise the fields
  lcdFields[TEST_TITLE].row = 0;
  lcdFields[TEST_TITLE].col = 0;
  
  lcdFields[TEST_STATUS].row = 1;
  lcdFields[TEST_STATUS].col = 0;
}

/**
  \brief Create the full display view.
  
  This method should be called only when the template
  is shown on start then only the updateDisplay() method
  should be used to change the variable fields.
  */
void LCDTest::createDisplay() {
  mLcd.clear();

  for(int j = 0; j < TEST_FIELDS; j++) {
    mLcd.setCursor(lcdFields[j].col, lcdFields[j].row);
    mLcd << lcdFields[j].val;
  }
}


