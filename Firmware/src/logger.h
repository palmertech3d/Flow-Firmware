/**
 * @Author: Nick Steele
 * @Date:   13:35 Sep 05 2021
 * @Last modified by:   Nick Steele
 * @Last modified time: 24:35 Sep 09 2021
 */

#include "config_defaults.h"

#ifndef LOGGER_H
#define LOGGER_H

#define MACRO_STRIZE(x) #x

#ifdef ENABLE_VERBOSE_LOGGING
  #define VERBOSE_LOG(str) Serial.print(str)
#else /* ifdef ENABLE_VERBOSE_LOGGING */
  #define VERBOSE_LOG(str)
#endif /* ifdef ENABLE_VERBOSE_LOGGING */

#ifdef ENABLE_DEBUG_LOGGING
  #define DEBUG_LOG(str) Serial.print(str)
#else /* ifdef ENABLE_DEBUG_LOGGING */
  #define DEBUG_LOG(str)
#endif /* ifdef ENABLE_DEBUG_LOGGING */

#ifdef ENABLE_INFO_LOGGING
  #define INFO_LOG(str) Serial.print(str)
#else /* ifdef ENABLE_INFO_LOGGING */
  #define INFO_LOG(str)
#endif /* ifdef ENABLE_INFO_LOGGING */

#ifdef ENABLE_WARN_LOGGING
  // This wil print the given string, after "WARN(<file name>:<linenumber>)"
  #define WARN_LOG_FL(str, file, line) do{ Serial.print(F("WARN(" file ":" MACRO_STRIZE(line) "): " str)); }while(0)
  #define WARN_LOG(str) WARN_LOG_FL(str, __FILE__, __LINE__)
#else /* ifdef ENABLE_WARN_LOGGING */
  #define WARN_LOG(str)
#endif /* ifdef ENABLE_WARN_LOGGING */

// Error logging cannot be disabled
// This wil print the given string, after "WARN(<file name>:<linenumber>)"
#define ERROR_LOG_FL(str, file, line) do{ Serial.print(F("ERR(" file ":" MACRO_STRIZE(line) "): " str)); }while(0)
#define ERROR_LOG(str) ERROR_LOG_FL(str, __FILE__, __LINE__)

#endif /* ifndef LOGGER_H */
