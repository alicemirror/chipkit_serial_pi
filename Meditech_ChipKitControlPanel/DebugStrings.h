/**
  \file DebugStrings.h
  \brief Definition of the debug strings
  
  To disable the strings usage undef the __DEBUG definition
*/

#undef __DEBUG

#ifdef __DEBUG

#define DEBG_PREFIX "[DEBUG]"
#define DEBG_LIDOPEN  "LID OPEN"
#define DEBG_LIDCLOSED  "LID CLOSED"

// =====================
// Parser debug strings
// =====================

#else

#define DEBG_PREFIX ""
#define DEBG_LIDOPEN ""
#define DEBG_LIDCLOSED ""

#endif
