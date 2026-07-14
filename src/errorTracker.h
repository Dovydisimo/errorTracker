/*
                                         errorTracker.h - Error flag handling library
                                         Created by Dovydas Bružas, 2025 September 22
                                               Released into the public domain


					--- IMPLEMENTATION INSTRUCTIONS ---
    1. Define all errors in a convinent way using `enum errorCodes : uint16_t {ERROR_ONE = 0, ...}.
    2. OPTIONAL: in your project, define `errorTracker_t errorTracker_errorTable[ERROR_TRACKER_MAX_ERRORS] = {ERROR_ONE, "ERROR ONE"}, ...` with all error codes and names.
    3. OPTIONAL: Declare `void errorTracker_onErrorChange(uint16_t newFlags)` callback function.
    4. Set, clear and check errors using `setError()`, `clearError()`, `hasError()`, `isAnyErrorPresent()`, `isInPanicMode()`.
    5. Print all active errors using `printErrorFlags(errorFlags)`.
    6. 

							--- LIMITATIONS ---
    1. Maximum 16 error codes (because errorFlags is uint16_t).
    2. OPTIONAL: User must define `errorTracker_errorTable[]` and keep it in sync with `enum errorCodes`.
    3. 

								--- TODO ---
    1. Add mux for `errorTracker_printErrorFlags` function.
    2. 
*/

#ifndef INC_ERRORTRACKER_H_
#define INC_ERRORTRACKER_H_


//========================================= DEFINES ==============================================//
#define ERROR_TRACKER_MAX_ERRORS 	16
#define ERROR_NONE               	0


//====================================== STRUCTURE VARIABLES =============================================//
struct errorTracker_t
{
  uint16_t errorBit;       //- single bit (bitmask).
  const char *errorName;   //- human-readable name.
};

/*__attribute__((weak)) errorTracker_t errorTracker_errorTable[ERROR_TRACKER_MAX_ERRORS] =
{
  {ERROR_UNKNOWN_1,   "UNKNOWN1"          },
  {ERROR_UNKNOWN_2,   "UNKNOWN2"          },
  {ERROR_UNKNOWN_3,   "UNKNOWN3"          },
  {ERROR_UNKNOWN_4,   "UNKNOWN4"          },
  {ERROR_UNKNOWN_5,   "UNKNOWN5"          },
  {ERROR_UNKNOWN_6,   "UNKNOWN6"          },
  {ERROR_UNKNOWN_7,   "UNKNOWN7"          },
  {ERROR_UNKNOWN_8,   "UNKNOWN8"          },
  {ERROR_UNKNOWN_9,   "UNKNOWN9"          },
  {ERROR_UNKNOWN_10,  "UNKNOWN10"         }, 
  {ERROR_UNKNOWN_11,  "UNKNOWN11"         },
  {ERROR_UNKNOWN_12,  "UNKNOWN12"         },
  {ERROR_UNKNOWN_13,  "UNKNOWN13"         },
  {ERROR_UNKNOWN_14,  "UNKNOWN14"         },
  {ERROR_UNKNOWN_15,  "UNKNOWN15"         },
  {ERROR_PANIC_MODE,  "ERROR_PANIC_MODE"  }
};*/


//========================================= OTHER ==============================================//
//static_assert(sizeof(errorTracker_errorTable)/sizeof(errorTracker_errorTable[0]) == ERROR_COUNT, "Error: errorTable size does not match number of errorCodes!");


//========================================= FUNCTION PROTOTYPES ==============================================//
bool errorTracker_hasError(uint16_t bitNumber);
void errorTracker_setError(uint16_t bitNumber);
void errorTracker_clearError(uint16_t bitNumber);
void errorTracker_clearAllErrors();
const char* errorTracker_getErrorString(const uint16_t activeFlags);
void errorTracker_printErrorFlags(const uint16_t activeFlags);
bool errorTracker_isAnyErrorPresent(void);

extern void errorTracker_onErrorChange(uint16_t newFlags);  //- Delete for library (.h) use.


#endif //- INC_ERRORTRACKER_H_