/**
  \file LCDStethoscope.cpp
  \brief LCD display template for stethoscope probe

  Defines the visualisation templates to simplify the LCD screen output in the various
  conditions for the Stethoscope probe.
    
  */

#include <AlphaLCD.h>
#include <Streaming.h>
#include "LCD.h"
#include "LCDTemplates.h"

// Contructor
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

void LCDStethoscope::createDisplay() {
  mLcd.clear();

 for(int j = 0; j < STETHOSCOPE_FIELDS; j++) {
    mLcd.setCursor(lcdFields[j].col, lcdFields[j].row);
    mLcd << lcdFields[j].val;
 }
}


