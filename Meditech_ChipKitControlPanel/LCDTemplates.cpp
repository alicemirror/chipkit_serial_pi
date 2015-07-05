/**
  \file LCDTemplates.cpp
  \brief LCD display template class.

  */

#include "LCD.h"
#include "LCDTemplates.h"

/**
  \brief Class constructor
  
  */
LCDTemplates::LCDTemplates(AlphaLCD myLCD) {
  mLcd = myLCD;
}

/**
  \brief Create the full display view parameters.
  
  This method should be called only when the template ID
  has already been set.
  */
void LCDTemplates::createDisplay() {

  switch(id) {
    case TID_STETHOSCOPE:
      fields.row[STET_TITLE] = 0;
      fields.col[STET_TITLE] = 0;
      fields.row[STET_GAIN] = 1;
      fields.col[STET_GAIN] = 0;
      fields.row[STET_GAINVAL] = 1;
      fields.col[STET_GAINVAL] = 12;
    break;
    case TID_BLOODPRESS:
      fields.row[BLOOD_TITLE] = 0;
      fields.col[BLOOD_TITLE] = 0;
      fields.row[BLOOD_WAIT] = 1;
      fields.col[BLOOD_WAIT] = 16;
      fields.row[BLOOD_MIN] = 1;
      fields.col[BLOOD_MIN] = 0;
      fields.row[BLOOD_MINVAL] = 1;
      fields.col[BLOOD_MINVAL] = 3;
      fields.row[BLOOD_MAX] = 1;
      fields.col[BLOOD_MAX] = 7;
      fields.row[BLOOD_MAXVAL] = 1;
      fields.col[BLOOD_MAXVAL] = 9;
    break;
    case TID_HEARTBEAT:
      fields.row[HEARTBEAT_TITLE] = 0;
      fields.col[HEARTBEAT_TITLE] = 0;
      fields.row[HEARTBEAT_SPOT] = 1;
      fields.col[HEARTBEAT_SPOT] = 0;
      fields.row[HEARTBEAT_SPOTVAL] = 1;
      fields.col[HEARTBEAT_SPOTVAL] = 5;
      fields.row[HEARTBEAT_AVERAGE] = 1;
      fields.col[HEARTBEAT_AVERAGE] = 10;
      fields.row[HEARTBEAT_AVERAGEVAL] = 1;
      fields.col[HEARTBEAT_AVERAGEVAL] = 15;
    break;
    case TID_TEMPERATURE:
      fields.row[TEMPERATURE_TITLE] = 0;
      fields.col[TEMPERATURE_TITLE] = 0;
      fields.row[TEMPERATURE_SPOT] = 1;
      fields.col[TEMPERATURE_SPOT] = 0;
      fields.row[TEMPERATURE_SPOTVAL] = 1;
      fields.col[TEMPERATURE_SPOTVAL] = 5;
      fields.row[TEMPERATURE_AVERAGE] = 1;
      fields.col[TEMPERATURE_AVERAGE] = 10;
      fields.row[TEMPERATURE_AVERAGEVAL] = 1;
      fields.col[TEMPERATURE_AVERAGEVAL] = 15;
    break;
    case TID_ECG:
      fields.row[TEMPERATURE_TITLE] = 0;
      fields.col[TEMPERATURE_TITLE] = 0;
      fields.row[TEMPERATURE_SPOT] = 1;
      fields.col[TEMPERATURE_SPOT] = 0;
      fields.row[TEMPERATURE_SPOTVAL] = 1;
      fields.col[TEMPERATURE_SPOTVAL] = 5;
      fields.row[TEMPERATURE_AVERAGE] = 1;
      fields.col[TEMPERATURE_AVERAGE] = 10;
      fields.row[TEMPERATURE_AVERAGEVAL] = 1;
      fields.col[TEMPERATURE_AVERAGEVAL] = 15;
    break;
    case TID_TEST:
      fields.row[TEST_TITLE] = 0;
      fields.col[TEST_TITLE] = 0;
      fields.row[TEST_STATUS] = 1;
      fields.col[TEST_STATUS] = 0;
    break;
    case TID_INFO:
      fields.row[INFO_TITLE] = 0;
      fields.col[INFO_TITLE] = 0;
      fields.row[INFO_RPM] = 1;
      fields.col[INFO_RPM] = 16;
      fields.row[INFO_DATE] = 0;
      fields.col[INFO_DATE] = 4;
      fields.row[INFO_TIME] = 0;
      fields.col[INFO_TIME] = 9;
      fields.row[INFO_GPS] = 1;
      fields.col[INFO_GPS] = 0;
    break;
    case TID_DEFAULT:
      fields.row[DEFAULT_TITLE] = 0;
      fields.col[DEFAULT_TITLE] = 0;
      fields.row[DEFAULT_VERSION] = 1;
      fields.col[DEFAULT_VERSION] = 0;
      fields.row[DEFAULT_STATUS] = 1;
      fields.col[DEFAULT_STATUS] = 10;
    break;
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
  
  \param val The string to update
  \param field The field ID
  */
void LCDTemplates::updateDisplay(String val, int fieldID) {
  mLcd.setCursor(fields.col[fieldID], fields.row[fieldID]);
  mLcd << val;
}

