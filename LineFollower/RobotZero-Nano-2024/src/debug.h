#ifndef DEBUG_H
#define DEBUG_H

#include "config.h"
#include <avr/pgmspace.h>

// Store debug messages in Flash memory instead of RAM
const char DEBUG_BASE[] PROGMEM = "Base: ";
const char DEBUG_ERROR[] PROGMEM = " Error: ";
const char DEBUG_CORRECTION[] PROGMEM = " Correction: ";
const char DEBUG_GEOMETRY[] PROGMEM = "Geometry: ";
const char DEBUG_RIGHT_MARKER[] PROGMEM = "rightMarkerDetected: ";
const char DEBUG_LEFT_MARKER[] PROGMEM = " leftMarkerDetected: ";
const char DEBUG_A0[] PROGMEM = " A0: ";
const char DEBUG_A7[] PROGMEM = " A7: ";
const char DEBUG_SLOW_MODE[] PROGMEM = "Slow mode activated";
const char DEBUG_FAST_MODE[] PROGMEM = "Fast mode activated";
const char DEBUG_INTERSECTION[] PROGMEM = "Intersection detected";
const char DEBUG_SETUP_START[] PROGMEM = "Starting setup";
const char DEBUG_SETUP_COMPLETE[] PROGMEM = "Setup completed";

// Helper function to print strings from Flash
inline void debugPrintFlash(const char* str) {
    char c;
    while ((c = pgm_read_byte(str++))) {
        Serial.write(c);
    }
}

// Debug macros - only active when DEBUG_LEVEL > 0
#if DEBUG_LEVEL > 0
#define DEBUG_BEGIN(x) Serial.begin(x)
#define DEBUG_PRINT(x) debugPrintFlash(x)
#define DEBUG_PRINTLN(x) do { debugPrintFlash(x); Serial.println(); } while(0)
#define DEBUG_PRINT_VAL(x) Serial.print(x)
#define DEBUG_PRINTLN_VAL(x) Serial.println(x)
#else
#define DEBUG_BEGIN(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT_VAL(x)
#define DEBUG_PRINTLN_VAL(x)
#endif

#endif // DEBUG_H