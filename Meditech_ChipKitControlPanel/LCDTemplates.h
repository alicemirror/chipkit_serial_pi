/**
  \file LCDTemplates.h
  \brief LCD display templates


  \page lcd_template How LCD templates works
  To simplify the display of the different conditions a set of predefined templates are defined
  so it is sufficient to send the IDs and relative strings to create the needed visualisation.
  This mechanism will also reduce the code size of the program.\n
  
  Every template has a symbolic ID and is built by a simple structure where every string to be shown
  has its own row and columns. Then a single recursive method can be used to generate the visualisation
  The templates are though as static objects leaving the space, where needed, for the variable data; in this
  case the template defines only the position on the display but does not define the content.\n
  
  Tas the template is an abstraction of the visualisation the strings are not defined statically:\n
  <b> the template is filled at runtime</b>\n
  
  Every template is an array of a set of basic <b>fields</b> defining the parameters where a certain value should
  be shown. 
  */

#ifndef __LCDTEMPLATES_H__
#define __LCDTEMPLATES_H__

#include "LCD.h"

/**
  \brief Defines the active probe bit
  
  The two-bytes word has the corresponding probe bit
  when the probe is shown on the LCD display.\n
  
  The bit order is the following:\n
  <ul>
  <li>0x0001 - STETHOSCOPE_ON</li>
  <li>0x0002 - BLOODPRESS_ON</li>
  <li>0x0004 - HEARTBEAT_ON</li>
  <li>0x0008 - TEMPERATURE_ON</li>
  <li>0x0010 - ECG_ON</li>
  <li>0x0020 - Unused</li>
  <li>0x0040 - Unused</li>
  <li>0x0080 - Unused</li>
  <li>0x0100 - Unused</li>
  <li>0x0200 - Unused</li>
  <li>0x0400 - Unused</li>
  <li>0x0800 - Unused</li>
  </ul>
  */
typedef word probeActive;

//! Stethoscope enabled bit
#define STETHOSCOPE_ON 0x0001
//! Blood press enabled bit
#define BLOODPRESS_ON 0x0002
//! Heart beat enabled bit
#define HEARTBEAT_ON 0x0004
//! Temperature enabled bit
#define TEMPERATURE_ON 0x0008
//! E.C.G. enabled bit
#define ECG_ON 0x0010

//! Max number of templates
#define MAX_TEMPLATES 7

//! Largest field array. Corresonds to the largest
//! possible template
#define MAX_FIELDS 6;

/**
  \brief LCD template field type definition
  
  Defines a generic LCD display field
  */
typedef struct LCDTemplateField {
  int row[6];       ///< Field row
  int col[6];       ///< Field column
} field;

//! Microphonic stethoscope template
#define TID_STETHOSCOPE 0
#define STETHOSCOPE_FIELDS 3
#define STET_TITLE 0
#define STET_GAIN 1
#define STET_GAINVAL 2

//! Blood pressure template parameters
#define TID_BLOODPRESS 1
#define BLOODPRESS_FIELDS 6
#define BLOOD_TITLE 0
#define BLOOD_WAIT 1
#define BLOOD_MIN 2
#define BLOOD_MINVAL 3
#define BLOOD_MAX 4
#define BLOOD_MAXVAL 5

//! Heartbeat frequency template
#define TID_HEARTBEAT 2
#define HEARTBEAT_FIELDS 5
#define HEARTBEAT_TITLE 0
#define HEARTBEAT_SPOT 1
#define HEARTBEAT_SPOTVAL 2
#define HEARTBEAT_AVERAGE 3
#define HEARTBEAT_AVERAGEVAL 4

//! Temperature frequency template
#define TID_TEMPERATURE 3
#define TEMPERATURE_FIELDS 5
#define TEMPERATURE_TITLE 0
#define TEMPERATURE_SPOT 1
#define TEMPERATURE_SPOTVAL 2
#define TEMPERATURE_AVERAGE 3
#define TEMPERATURE_AVERAGEVAL 4

//! Control panel E.C.G. template
#define TID_ECG 4
#define ECG_FIELDS 3
#define ECG_TITLE 0
#define ECG_STATUS 1
#define ECG_STATUSFLAG 2

//! Control panel test cycle template
#define TID_TEST 5
#define TEST_FIELDS 2
#define TEST_TITLE 0
#define TEST_STATUS 1

//! Control panel info template
#define TID_INFO 6
#define INFO_FIELDS 5
#define INFO_TITLE 0
#define INFO_RPM 1
#define INFO_DATE 2
#define INFO_TIME 3
#define INFO_GPS 4

//! Control panel default template
#define TID_DEFAULT 7
#define DEFAULT_FIELDS 3
#define DEFAULT_TITLE 0
#define DEFAULT_VERSION 1
#define DEFAULT_STATUS 2

class LCDTemplates {
  public:
    LCDTemplates(AlphaLCD myLCD);
    void createDisplay();
    void updateDisplay(String val, int fieldID);
    int id;
    LCDTemplateField fields;
  private:
    AlphaLCD mLcd;
};

#endif
