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
  \brief LCD template field type definition
  
  Defines a generic LCD display field
  */
typedef struct LCDTemplateField {
  int row;       ///< Field row
  int col;       ///< Field column
  String val;    ///< Field content
} field;

//! Heartbeat frequency template
#define TID_HEARTBEAT 2
//! Test cycle template
#define TID_TEST 3
//! E.C.G. Template
#define TID_ECG 4
//! Control panel status template
#define TID_INFO 5

//! Microphonic stethoscope template
#define TID_STETHOSCOPE 0
#define STETHOSCOPE_FIELDS 3
#define STET_TITLE 0
#define STET_GAIN 1
#define STET_GAINVAL 2

class LCDStethoscope {
  public:
    LCDStethoscope(AlphaLCD myLCD);
    void createDisplay();
    void updateDisplay(int val);
    int id;
    field lcdFields[STETHOSCOPE_FIELDS];
  private:
    AlphaLCD mLcd;
};

/*
//! Blood pressure template
#define TID_BLOODPRESS 1
#define BLOODPRESS_FIELDS 6
#define BLOOD_TITLE 0
#define BLOOD_WAIT 1
#define BLOOD_MIN 2
#define BLOOD_MINVAL 3
#define BLOOD_MAX 4
#define BLOOD_MAXVAL 5

class LCDBloodPressure {
  public:
    LCDBloodPressure(AlphaLCD myLCD);
    void createDisplay();
    int id;
    field lcdFields[BLOODPRESS_FIELDS];
  private:
    AlphaLCD mLcd;
};
*/
#endif
