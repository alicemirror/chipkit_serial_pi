/**
\file CommandProcessor.h
\brief Class to process the command dialog with the control panel board.
 */

#include "LCDTemplatesMaster.h"
#include "CommandParameters.h"
#include <iomanip>
#include <locale>
#include <sstream>
#include <string.h>

#ifndef COMMANDPROCESSOR_H
#define	COMMANDPROCESSOR_H

class CommandProcessor: public LCDTemplatesMaster {
public:
	CommandProcessor();
	virtual ~CommandProcessor();
	char* buildCommandDisplayTemplate(int templateID);
private:
	LCDTemplatesMaster mTemplates;
	
	std::string intToString(int i);
	std::string intToString(int i, int l);
	int stringToInt(std::string &s);

};

#endif	/* COMMANDPROCESSOR_H */

