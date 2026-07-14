



//========================================= INCLUDES ==============================================//
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "esp_system.h"

#include "errorTracker.h"


//====================================== GLOBAL VARIABLES =============================================//
extern errorTracker_t errorTracker_errorTable[ERROR_TRACKER_MAX_ERRORS]; //- Must be defined in main.cpp.

volatile uint16_t errorTracker_flags = 0;
portMUX_TYPE errorTracker_flagMux = portMUX_INITIALIZER_UNLOCKED;


//========================================= FUNCTIONS ==============================================//
__attribute__((weak)) void errorTracker_onErrorChange(uint16_t newFlags)
{
  //- Optional user function.
}

bool errorTracker_hasError(uint16_t bitNumber)
{
  portENTER_CRITICAL(&errorTracker_flagMux);
  bool hasError = (errorTracker_flags & (1 << bitNumber));
  portEXIT_CRITICAL(&errorTracker_flagMux);
  return hasError;
}

void errorTracker_setError(uint16_t bitNumber)
{
  if(!errorTracker_hasError(bitNumber))
  { 
    portENTER_CRITICAL(&errorTracker_flagMux);
    errorTracker_flags |= (1 << bitNumber);
    portEXIT_CRITICAL(&errorTracker_flagMux);
    errorTracker_onErrorChange(errorTracker_flags);
  }
}

void errorTracker_clearError(uint16_t bitNumber)
{
  if(errorTracker_hasError(bitNumber))
  {
    portENTER_CRITICAL(&errorTracker_flagMux);
    errorTracker_flags &= ~(1 << bitNumber);
    portEXIT_CRITICAL(&errorTracker_flagMux);
    errorTracker_onErrorChange(errorTracker_flags);
  }
}

void errorTracker_clearAllErrors()
{
  portENTER_CRITICAL(&errorTracker_flagMux);
  errorTracker_flags = ERROR_NONE;
  portEXIT_CRITICAL(&errorTracker_flagMux);
  errorTracker_onErrorChange(errorTracker_flags);
}

const char* errorTracker_getErrorString(const uint16_t activeFlags)
{
  static char buffer[128]; //- Sized for worst-case: all 16 error names concatenated with " | " separators.
  buffer[0] = '\0';

  if(activeFlags == ERROR_NONE)
  {
    strcpy(buffer, "NONE");
    return buffer;
  }

  bool first = true;
  for(uint8_t i = 0; i < ERROR_TRACKER_MAX_ERRORS; i++)
  {
    if(activeFlags & (1 << i))
    {
      if(!first) strcat(buffer, " | ");
      strcat(buffer, errorTracker_errorTable[i].errorName);
      first = false;
    }
  }

  return buffer;
}

void errorTracker_printErrorFlags(const uint16_t activeFlags)
{
  Serial.printf("%s", errorTracker_getErrorString(activeFlags));
  //Serial.printf("\n");
}

bool errorTracker_isAnyErrorPresent()
{
  portENTER_CRITICAL(&errorTracker_flagMux);
  bool anyErrorPresent = (errorTracker_flags != 0);
  portEXIT_CRITICAL(&errorTracker_flagMux);
  return anyErrorPresent;
}

/*bool errorTracker_isInPanicMode()
{
  portENTER_CRITICAL(&errorTracker_flagMux);
  bool isPanicMode = (errorTracker_flags & (1 << ERROR_PANIC_MODE));
  portEXIT_CRITICAL(&errorTracker_flagMux);
  return isPanicMode;
}*/
