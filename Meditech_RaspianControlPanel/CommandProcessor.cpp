/**
 \file CommandProcessor.cpp
 \brief CommandProcessor class manages the command parsing and commands creation 
 for the serial communication process with the control panel board. This class
 implies the LCDTemplatesMaster class for the commands related to the control panel
 LCD display templates.
 */

#include "CommandProcessor.h"

/**
 \brief Constructor method
 */
CommandProcessor::CommandProcessor() {
}

/**
 \brief Destructor method
 */
CommandProcessor::~CommandProcessor() {
}

/**
 \brief Generate a template creation command

 This method based on the requested templateID create the command string ready to
 be sent to the control panel board.
 
 \param templateID The id of the requested template
 \return The string with the full command.
 */
char* CommandProcessor::buildCommandDisplayTemplate(int templateID) {
	//! The full command string
	char command[MAX_CMD_LEN];
	int cPos = 0;	///< character position counter in the command string
	
	// Generates the desired template fields and parameters.
	mTemplates.createDisplay(templateID);
	
	// Now the tParams calls instance contains already defined the field settings
	// and parameters to build the command.
	command[cPos++] = CMD_SEPARATOR;		// start with command 
	command[cPos++] = CMD_LCDTEMPLATE;		// Add the command character
	command[cPos++] = FIELD_SEPARATOR;		// Add the field separator
	// Convert the field ID integer to the proper character sequence
	std::string temp = intToString(templateID, PARM_FIELDID_LEN);
	for(int k = 0; k < temp.size(); k++)
		command[cPos++] = temp.at(k);
	
	// Loop creating fields
	for(int j = 0; j < mTemplates.getNumFields(); j++) {
		command[cPos++] = FIELD_SEPARATOR; // Add the field separator
		command[cPos++] = STRING_DELIMITER; // Add the left string delimiter
		// Load the field characters in the array
		char* s = (char *)mTemplates.getField(j);
		for(int k = 0; k < strlen(s); k++)
			command[cPos++] = s[k];

		command[cPos++] = STRING_DELIMITER; // Add the right string delimiter
	} // End of command build
	
	command[cPos] = CMD_NULLCHAR;
	return command;
}

/**
 \brief Convert an Integer to string
 
 \param i The integer value to convert
 \return The converted string
 */
std::string CommandProcessor::intToString(int i) {
	std::ostringstream ss;
	ss << i;

	return ss.str();
}

/**
 \brief Convert an Integer to string of fixed length
 
 \param i The integer value to convert
 \param l The requested integer length
 \return The converted string or a null string if the resulting integer
 conversion is larger than the requested length. If the string is shorter
 than the requested length it is left-filled with zeroes.
 */
std::string CommandProcessor::intToString(int i, int l) {
	//! The bare converted number
	std::string temp = intToString(i);
	
	if(temp.size() > l)
		return "";	// Number too big to fit!
	
	if(temp.size() < l) {
		std::string temp2 = "";
		// Left fill the string
		for(int j = 0; j < (l - temp.size()); j++)
			temp2.append("0");
		// Add the converted number;
		temp2.append(temp);
		return temp2;
	} // when the number is smaller
	
	// The number is of the requested length
	return temp;
}

/**
 \breif Convert a string to integer
 
 \param s The string to convert
 \return The integer conversion result or 0 if the string is not numeric
 */
int CommandProcessor::stringToInt(std::string &s) {
	int result;
	std::istringstream ss(s);

	return ss >> result ? result : 0;
}


