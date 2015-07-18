/**
\file LCDTemplatesMaster.h
\brief LCD display templates contents
 
 This header file is similar to the corresponding LCDTemplates header of the same class
 used in the control panel board firmware (ChipKit board). The master defines the
 fields strings used to create the commands sent to the control panel board.
 
 \warning Change the templates and templates fields in this header accordingly with
 the changes applied to the control panel firmware corresponding class.
*/

#ifndef __LCDTEMPLATESMASTER_H__
#define __LCDTEMPLATESMASTER_H__

//! Max number of templates
#define MAX_TEMPLATES 7

//! Largest field array. Corresponds to the largest
//! possible template
#define MAX_FIELDS 6;

//! Null character if a field shoul be empty
#define CMD_NULLFIELD '\0'

//! Microphonic stethoscope template
#define TID_STETHOSCOPE 0
#define STETHOSCOPE_FIELDS 3
#define STET_TITLE			"Stethoscope"
#define STET_GAIN			"Gain"
#define STET_GAINVAL		"--"

//! Blood pressure template parameters
#define TID_BLOODPRESS 1
#define BLOODPRESS_FIELDS 6
#define BLOOD_TITLE			"B. Pressure"
#define BLOOD_WAIT			"Wait"
#define BLOOD_MIN			"Min"
#define BLOOD_MINVAL		"---"
#define BLOOD_MAX			"Max"
#define BLOOD_MAXVAL		"---"

//! Heartbeat frequency template
#define TID_HEARTBEAT 2
#define HEARTBEAT_FIELDS 5
#define HEARTBEAT_TITLE			"Heart Beat"
#define HEARTBEAT_SPOT			"Spot"
#define HEARTBEAT_SPOTVAL		"---"
#define HEARTBEAT_AVERAGE		"Avg"
#define HEARTBEAT_AVERAGEVAL	"---"

//! Temperature frequency template
#define TID_TEMPERATURE 3
#define TEMPERATURE_FIELDS 5
#define TEMPERATURE_TITLE		"Temperature"
#define TEMPERATURE_SPOT		"Spot"
#define TEMPERATURE_SPOTVAL		"--.-"
#define TEMPERATURE_AVERAGE		"Avg."
#define TEMPERATURE_AVERAGEVAL	"--.-"

//! Control panel E.C.G. template
#define TID_ECG 4
#define ECG_FIELDS 3
#define ECG_TITLE			"E.C.G."
#define ECG_STATUS			"Status"
#define ECG_STATUSFLAG		"???"

//! Control panel test cycle template
#define TID_TEST 5
#define TEST_FIELDS 2
#define TEST_TITLE		"Control Panel"
#define TEST_STATUS		"Test running"

//! Control panel info template
#define TID_INFO 6
#define INFO_FIELDS 5
#define INFO_TITLE		"Info"
#define INFO_RPM		"rpm"
#define INFO_DATE		"dd/mm/yy"
#define INFO_TIME		"hh:mm"
#define INFO_GPS		"GPS"

//! Control panel default template
#define TID_DEFAULT 7
#define DEFAULT_FIELDS 3
#define DEFAULT_TITLE		"Meditech"
#define DEFAULT_VERSION		"1.0"
#define DEFAULT_STATUS		"running"

class LCDTemplatesMaster {
  public:
	  LCDTemplatesMaster(int templateID);
	  LCDTemplatesMaster();
	  int createDisplay();
	  int createDisplay(int templateID);
	  void updateDisplay(char* val, int fieldID);
	  int getID();
	  int getNumFields();
	  char* getField(int f);
  private:
	  int id;
	  int nFields;
};

#endif
