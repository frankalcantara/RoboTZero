#ifndef FLASHMANAGER_H
#define FLASHMANAGER_H

#include <Arduino.h>
#include "config.h"

#if DEBUG_LEVEL > 0

class FlashManager {
public:
    // Initialize flash manager
    static bool initialize();

    // Write a block of data to flash
    static bool writeBlock(const void* data, uint16_t size);

    // Read a block of data from flash
    static bool readBlock(void* data, uint16_t size, uint32_t address);

    // Check if we can write safely
    static bool canWrite(int16_t linePosition);

    // Get current write position
    static uint32_t getCurrentAddress();

    // Check if there's enough space for bytes
    static bool hasSpace(uint16_t bytes);

    // Reset flash position to start
    static void reset();

    // Log ready flag management
    static bool setLogReady();
    static bool clearLogReady();
    static bool isLogReady();

private:
    static uint32_t currentAddress;    // Current write position
    static bool isInitialized;         // Initialization flag

    // Internal methods
    static bool erasePage(uint32_t address);
    static bool writePage(const void* data, uint16_t size, uint32_t address);
    static uint16_t calculateChecksum(const void* data, uint16_t size);
    static bool verifyWrite(const void* data, uint16_t size, uint32_t address);
};

#endif // DEBUG_LEVEL > 0
#endif // FLASHMANAGER_H