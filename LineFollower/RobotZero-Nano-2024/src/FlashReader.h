#ifndef FLASHREADER_H
#define FLASHREADER_H

#include "FlashManager.h"
#include "DataStructures.h"

#if DEBUG_LEVEL > 0

class FlashReader {
public:
    // Initialize reader
    static void initialize();

    // Process serial commands
    static void processCommands();

private:
    static const char CMD_READ_FLASH = 'R';  // Command to start reading
    static const char CMD_ERASE_FLASH = 'E'; // Command to erase flash
    static const char START_MARKER = '$';
    static const char END_MARKER = '#';

    // Protocol markers for different sections
    static const uint8_t MARKER_HEADER = 0x01;
    static const uint8_t MARKER_PERFORMANCE = 0x02;
    static const uint8_t MARKER_EVENT = 0x03;
    static const uint8_t MARKER_LAP = 0x04;

    // Internal methods
    static void sendFlashContents();
    static void sendSessionHeader();
    static void sendPerformanceRecords();
    static void sendEventRecords();
    static void sendLapStats();
    static void sendMarker(uint8_t marker);
    static void sendChecksum();

    // Utility methods
    static void writeHex(uint8_t value);
    static uint16_t calculateChecksum(const void* data, uint16_t size);
};

#endif // DEBUG_LEVEL > 0
#endif // FLASHREADER_H