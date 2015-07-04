/**
  \file DebugStrings.h
  \brief Definition of the debug strings
  
  To disable the strings usage undef the __DEBUG definition
*/

#define __DEBUG

#ifdef __DEBUG

#define DEBG_PREFIX "[DEBUG]"
#define DEBG_LIDOPEN  "LID OPEN"
#define DEBG_LIDCLOSED  "LID CLOSED"

// =====================
// Parser debug strings
// =====================
#define DBG_HEARTBEAT "setHeartBeatStatus()"
#define DBG_BODYTEMP "setBodyTempStatus()"
#define DBG_PRESSURE "setPressureStatus()"
#define DBG_ECG "setECGStatus()"
#define DBG_STETHOSCOPE "setStethoscopeStatus()"
#define DBG_PARSER "parser()"
#define DBG_NEXTSEP "nextSeparator()"
#define DBG_TOINT "charsToInt()"
#define DBG_TOSTRING "charsToString()"
#define DBG_TOLONG "charsToLong()"
#define DBG_TOFLOAT "charsToFloat()"
#define DBG_SYNTACHECK "syntaxCheck()"
#define DBG_COMMANDRETURN "commandReturn()"

#else

#define DEBG_PREFIX ""
#define DEBG_LIDOPEN ""
#define DEBG_LIDCLOSED ""
#define DBG_HEARTBEAT ""
#define DBG_BODYTEMP ""
#define DBG_PRESSURE ""
#define DBG_ECG ""
#define DBG_STETHOSCOPE ""
#define DBG_PARSER ""
#define DBG_NEXTSEP ""
#define DBG_TOINT ""
#define DBG_TOSTRING ""
#define DBG_TOLONG ""
#define DBG_TOFLOAT ""
#define DBG_SYNTACHECK ""
#define DBG_COMMANDRETURN ""

#endif
