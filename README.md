# errorTracker

A lightweight, thread-safe error flag tracking library for ESP32 / FreeRTOS projects (Arduino framework). Errors are represented as bits in a single `uint16_t`, making it cheap to set, clear, check, and print.

---

## Features

- Up to 16 independent error codes packed into one `uint16_t` bitmask.
- Thread-safe set/clear/check operations using a FreeRTOS `portMUX_TYPE` critical section.
- Optional user-defined callback (`errorTracker_onErrorChange`) fired whenever the flags change.
- Optional user-defined name table (`errorTracker_errorTable`) for human-readable error printing.
- Helper functions to get active errors as a string or print them directly over `Serial`.

## Limitations

- Maximum of 16 error codes (limited by the `uint16_t` bitmask).
- If you want named/printable errors, you must define and maintain `errorTracker_errorTable[]` yourself, kept in sync with your `errorCodes` enum.

## Installation

## Installation
1. Download or clone this repository
2. Place it in your Arduino libraries directory:

```
Documents/Arduino/libraries/errorTracker/
```

3. Restart Arduino IDE

4. In your project's source directory and include it:

```cpp
#include "errorTracker.h"
```
---

## Getting Started

### 1. Define your error codes

```cpp
enum errorCodes : uint16_t
{
  ERROR_SENSOR_TIMEOUT = 0,
  ERROR_LOW_BATTERY,
  ERROR_WIFI_DISCONNECTED,
  // ...
};
```

### 2. (Optional) Define the error name table

To use `errorTracker_getErrorString()` / `errorTracker_printErrorFlags()`, define a table matching your error codes, kept in sync with the enum above:

```cpp
errorTracker_t errorTracker_errorTable[ERROR_TRACKER_MAX_ERRORS] =
{
  { ERROR_SENSOR_TIMEOUT,     "SENSOR_TIMEOUT"     },
  { ERROR_LOW_BATTERY,        "LOW_BATTERY"        },
  { ERROR_WIFI_DISCONNECTED,  "WIFI_DISCONNECTED"  },
  // ...
};
```

### 3. (Optional) Define an on-change callback
Override a weakly defined function `void errorTracker_onErrorChange(uint16_t newFlags)` with your implementation.
```cpp
void errorTracker_onErrorChange(uint16_t newFlags)
{
  // e.g. update an LED, log to storage, notify a task, etc.
}
```

### 4. Set, clear, and check errors

```cpp
errorTracker_setError(ERROR_LOW_BATTERY);
errorTracker_clearError(ERROR_LOW_BATTERY);

if(errorTracker_hasError(ERROR_WIFI_DISCONNECTED))
{
  // handle reconnect logic
}

if(errorTracker_isAnyErrorPresent())
{
  // at least one error flag is set
}

errorTracker_clearAllErrors();
```

### 5. Print active errors
If `errorTracker_t errorTracker_errorTable[ERROR_TRACKER_MAX_ERRORS]` table is defined the following functions can be used. 
```cpp
errorTracker_printErrorFlags(errorTracker_flags);
```

Or get the formatted string yourself (e.g. for logging to something other than `Serial`):

```cpp
const char* errors = errorTracker_getErrorString(errorTracker_flags);
```

## Notes

- `errorTracker_getErrorString()` uses a static internal 128-byte buffer sized for the worst case of all 16 error names concatenated with `" | "` separators. Keep error names reasonably short if you use all 16 codes.
- Access to `errorTracker_flags` is guarded by a FreeRTOS critical section (`errorTracker_flagMux`), making the set/clear/check functions safe to call from multiple tasks.

## TODO

- Add a mutex around `errorTracker_printErrorFlags`.

## License & Author
MIT / Beerware.

If you find this useful, buy the author a beer 🍺 :) \
**Dovydas Bružas, Lithuania Dovydisimo@gmail.com

## Changelog

### v1.2 — 2026-07-12
1. **FEATURE:** New `errorTracker_getErrorString()` function.
2. **CHORE:** `errorTracker_printErrorFlags()` refactored to use `errorTracker_getErrorString()`, reducing logic duplication.

### v1.1 — 2025-11-10
1. `errorTracker_clearAllErrors()` introduced.

### v1.0 — 2025-09-22
1. Initial release.
