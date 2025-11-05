#include "FlashReader.h"
#include <Arduino.h>

#if DEBUG_LEVEL > 0

void FlashReader::initialize() {
    // Nothing to initialize
}

void FlashReader::processCommands() {
    // Start marker
    Serial.write(START_MARKER);

    // Send session header
    sendMarker(MARKER_HEADER);
    sendSessionHeader();

    // Send performance records
    sendMarker(MARKER_PERFORMANCE);
    sendPerformanceRecords();

    // Send event records
    sendMarker(MARKER_EVENT);
    sendEventRecords();

    // Send lap statistics
    sendMarker(MARKER_LAP);
    sendLapStats();

    // End marker and checksum
    Serial.write(END_MARKER);
    sendChecksum();

    // Clear the log ready flag after successful transmission
    FlashManager::clearLogReady();
}

void FlashReader::sendSessionHeader() {
    SessionHeader header;
    FlashManager::readBlock(&header, sizeof(SessionHeader), FLASH_LOG_START);
    Serial.write((uint8_t*)&header, sizeof(SessionHeader));
}

void FlashReader::sendPerformanceRecords() {
    uint32_t address = FLASH_LOG_START + sizeof(SessionHeader);
    PerformanceRecord record;

    while (address < FlashManager::getCurrentAddress()) {
        if (FlashManager::readBlock(&record, sizeof(PerformanceRecord), address)) {
            Serial.write((uint8_t*)&record, sizeof(PerformanceRecord));
        }
        address += sizeof(PerformanceRecord);
    }
}

void FlashReader::sendEventRecords() {
    uint32_t address = FLASH_LOG_START + sizeof(SessionHeader);
    EventRecord record;

    while (address < FlashManager::getCurrentAddress()) {
        if (FlashManager::readBlock(&record, sizeof(EventRecord), address)) {
            Serial.write((uint8_t*)&record, sizeof(EventRecord));
        }
        address += sizeof(EventRecord);
    }
}

void FlashReader::sendLapStats() {
    uint32_t address = FLASH_LOG_START + sizeof(SessionHeader);
    LapStats stats;

    while (address < FlashManager::getCurrentAddress()) {
        if (FlashManager::readBlock(&stats, sizeof(LapStats), address)) {
            Serial.write((uint8_t*)&stats, sizeof(LapStats));
        }
        address += sizeof(LapStats);
    }
}

void FlashReader::sendMarker(uint8_t marker) {
    Serial.write(marker);
}

void FlashReader::sendChecksum() {
    uint16_t checksum = 0;
    uint32_t address = FLASH_LOG_START;
    uint8_t byte;

    while (address < FlashManager::getCurrentAddress()) {
        if (FlashManager::readBlock(&byte, sizeof(byte), address)) {
            checksum += byte;
        }
        address++;
    }

    writeHex((checksum >> 8) & 0xFF);  // High byte
    writeHex(checksum & 0xFF);         // Low byte
}

void FlashReader::writeHex(uint8_t value) {
    static const char hexChars[] = "0123456789ABCDEF";
    Serial.write(hexChars[value >> 4]);
    Serial.write(hexChars[value & 0x0F]);
}

#endif // DEBUG_LEVEL > 0