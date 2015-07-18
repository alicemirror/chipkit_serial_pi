/**
\file LCDTemplatesMater.cpp
\brief LCD display template class.

 LCDTemplatesMater class defines the templates objects for the LCD display on the ChipKit
 control panel board. This class is similar to the LCDTemplates used in the control
 panel board firmware but defines the templates content string for every field instead
 of the templates position elements.
*/

#include "LCDTemplatesMaster.h"

//! Defines the char array of the max number of fields in the
//! template
char *fields[] = { '\0', '\0', '\0', '\0', '\0', '\0' };

/**
\brief Class constructor

 Initializes the field ID for the display creation, then load
 the correct display template string set. This is the default set
 that can be changes reloading the createDisplay method at any time.

*/
LCDTemplatesMaster::LCDTemplatesMaster(int templateID) {
	
  // Check for the ID valid
	if( (templateID >= 0 ) && (templateID < MAX_TEMPLATES) ) {
		id = templateID;
		nFields = createDisplay();
	} // Assign the template fields
	else {
		id = -1;
		nFields = -1;
	} // Invalid template ID
}

/**
\brief Class constructor

 */
LCDTemplatesMaster::LCDTemplatesMaster() {
	id = -1;
	nFields = -1;
}

/**
  \brief Create the full display view parameters.
  
  This method should be called only when the template ID
  has already been set.
  
 \param templateID The requested template to set
 \return The number of fields of the selected template or -1 if
 the current template ID is invalid
  */
int LCDTemplatesMaster::createDisplay(int templateID) {

  // Check for the ID valid
	if( (templateID >= 0 ) && (templateID < MAX_TEMPLATES) ) {
		id = templateID;
		nFields = createDisplay();
	} // Assign the template fields
	else
		return -1; // Invalid template ID
}

/**
  \brief Create the full display view parameters.
  
  This method should be called only when the template ID
  has already been set.
  
  \return The number of fields of the selected template or -1 if
 the current template ID is invalid
  */
int LCDTemplatesMaster::createDisplay() {
  int numFields;
  
  // Check for the ID valid
  if( (id >= 0 ) && (id < MAX_TEMPLATES) ) {
	  // Select the ID-based template
	switch(id) {
	  case TID_STETHOSCOPE:
		fields[0] = (char*)STET_TITLE;
		fields[1] = (char*)STET_GAIN;
		fields[2] = (char*)STET_GAINVAL;
		numFields = STETHOSCOPE_FIELDS;
	  break;
	  case TID_BLOODPRESS:
		fields[0] = (char*)BLOOD_TITLE;
		fields[1] = (char*)BLOOD_WAIT;
		fields[2] = (char*)BLOOD_MIN;
		fields[3] = (char*)BLOOD_MINVAL;
		fields[4] = (char*)BLOOD_MAX;
		fields[5] = (char*)BLOOD_MAXVAL;
		numFields = BLOODPRESS_FIELDS;
	  break;
	  case TID_HEARTBEAT:
		fields[0] = (char*)HEARTBEAT_TITLE;
		fields[1] = (char*)HEARTBEAT_SPOT;
		fields[2] = (char*)HEARTBEAT_SPOTVAL;
		fields[3] = (char*)HEARTBEAT_AVERAGE;
		fields[4] = (char*)HEARTBEAT_AVERAGEVAL;
		numFields = HEARTBEAT_FIELDS;
	  break;
	  case TID_TEMPERATURE:
		fields[0] = (char*)TEMPERATURE_TITLE;
		fields[1] = (char*)TEMPERATURE_SPOT;
		fields[2] = (char*)TEMPERATURE_SPOTVAL;
		fields[3] = (char*)TEMPERATURE_AVERAGE;
		fields[4] = (char*)TEMPERATURE_AVERAGEVAL;
		numFields = TEMPERATURE_FIELDS;
	  break;
	  case TID_ECG:
		fields[0] = (char*)ECG_TITLE;
		fields[1] = (char*)ECG_STATUS;
		fields[2] = (char*)ECG_STATUSFLAG;
		numFields = ECG_FIELDS;
	  break;
	  case TID_TEST:
		fields[0] = (char*)TEST_TITLE;
		fields[1] = (char*)TEST_STATUS;
		numFields = TEST_FIELDS;
	  break;
	  case TID_INFO:
		fields[0] = (char*)INFO_TITLE;
		fields[1] = (char*)INFO_RPM;
		fields[2] = (char*)INFO_DATE;
		fields[3] = (char*)INFO_TIME;
		fields[4] = (char*)INFO_GPS;
		numFields = INFO_FIELDS;
	  break;
	  case TID_DEFAULT:
		fields[0] = (char*)DEFAULT_TITLE;
		fields[1] = (char*)DEFAULT_VERSION;
		fields[2] = (char*)DEFAULT_STATUS;
		numFields = DEFAULT_FIELDS;
	  break;
	} // Assign the template fields
	return numFields;
  }
  else
	  return -1; // Template ID is invalid
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
void LCDTemplatesMaster::updateDisplay(char *val, int fieldID) {
  fields[fieldID] = val;
}

/**
 \brief Return the current template ID
 */
int LCDTemplatesMaster::getID() {
	return id;
}

/**
 \brief Return the number of fields of the current template ID
 */
int LCDTemplatesMaster::getNumFields() {
	return nFields;
}

/**
 \brief Return the requested field string
 
 \param f The desired field ID
 \return The corresponding string field or a null character if the requested field
 ID is out of range.
 */
char* LCDTemplatesMaster::getField(int f) {
	// Check for the field validity
	if( (f >= 0) && (f < nFields) )
		return fields[f];
	else
		return CMD_NULLFIELD;
}
