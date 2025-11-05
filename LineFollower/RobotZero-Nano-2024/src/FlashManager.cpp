#include "FlashManager.h"
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#if DEBUG_LEVEL > 0

// Static member initialization
uint32_t FlashManager::currentAddress = FLASH_LOG_START;
bool FlashManager::isInitialized = false;

bool FlashManager::initialize() {
    if (isInitialized) return true;

    // Reset write position
    currentAddress = FLASH_LOG_START;
    isInitialized = true;
    return true;
}

bool FlashManager::writeBlock(const void* data, uint16_t size) {
    if (!isInitialized) return false;
    if (!hasSpace(size)) return false;

    // Calculate number of pages needed
    uint16_t pages = (size + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE;
    const uint8_t* dataPtr = (const uint8_t*)data;
    uint32_t address = currentAddress;

    // Write data page by page
    for (uint16_t i = 0; i < pages; i++) {
        uint16_t pageSize = min(size, FLASH_PAGE_SIZE);

        // Erase page
        if (!erasePage(address)) return false;

        // Write page
        if (!writePage(dataPtr, pageSize, address)) return false;

        // Verify written data
        if (!verifyWrite(dataPtr, pageSize, address)) return false;

        address += FLASH_PAGE_SIZE;
        dataPtr += pageSize;
        size -= pageSize;
    }

    currentAddress = address;
    return true;
}

bool FlashManager::readBlock(void* data, uint16_t size, uint32_t address) {
    if (!isInitialized) return false;
    if (address < FLASH_LOG_START && address != FLASH_CONTROL_BYTE) return false;
    if (address + size > currentAddress && address != FLASH_CONTROL_BYTE) return false;

    // Read data byte by byte
    uint8_t* dataPtr = (uint8_t*)data;
    for (uint16_t i = 0; i < size; i++) {
        *dataPtr++ = pgm_read_byte(address + i);
    }

    return true;
}

bool FlashManager::canWrite(int16_t linePosition) {
    return (abs(linePosition) < STRAIGHT_THRESHOLD);
}

uint32_t FlashManager::getCurrentAddress() {
    return currentAddress;
}

bool FlashManager::hasSpace(uint16_t bytes) {
    // Calculate required pages
    uint16_t requiredPages = (bytes + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE;
    uint32_t requiredSpace = requiredPages * FLASH_PAGE_SIZE;

    // Check if we have enough space
    return (currentAddress + requiredSpace <= FLASHEND);
}

void FlashManager::reset() {
    currentAddress = FLASH_LOG_START;
}

bool FlashManager::setLogReady() {
    if (!isInitialized) return false;

    uint8_t value = FLASH_LOG_READY;
    erasePage(FLASH_CONTROL_BYTE);
    return writePage(&value, sizeof(value), FLASH_CONTROL_BYTE);
}

bool FlashManager::clearLogReady() {
    if (!isInitialized) return false;

    uint8_t value = 0x00;
    erasePage(FLASH_CONTROL_BYTE);
    return writePage(&value, sizeof(value), FLASH_CONTROL_BYTE);
}

bool FlashManager::isLogReady() {
    if (!isInitialized) return false;

    uint8_t value = 0;
    if (!readBlock(&value, sizeof(value), FLASH_CONTROL_BYTE)) {
        return false;
    }
    return value == FLASH_LOG_READY;
}

// Private methods
bool FlashManager::erasePage(uint32_t address) {
    // Disable interrupts during flash operations
    uint8_t sreg = SREG;
    cli();

    // Erase page
    boot_page_erase(address);
    boot_spm_busy_wait();

    // Re-enable interrupts
    SREG = sreg;

    return true;
}

bool FlashManager::writePage(const void* data, uint16_t size, uint32_t address) {
    const uint8_t* dataPtr = (const uint8_t*)data;
    uint16_t i;

    // Disable interrupts during flash operations
    uint8_t sreg = SREG;
    cli();

    // Fill temporary buffer
    for (i = 0; i < size; i += 2) {
        uint16_t word = dataPtr[i];
        if (i + 1 < size) {
            word |= (uint16_t)dataPtr[i + 1] << 8;
        }
        boot_page_fill(address + i, word);
    }

    // Write page
    boot_page_write(address);
    boot_spm_busy_wait();

    // Re-enable interrupts
    SREG = sreg;

    return true;
}

uint16_t FlashManager::calculateChecksum(const void* data, uint16_t size) {
    uint16_t checksum = 0;
    const uint8_t* bytes = (const uint8_t*)data;

    for (uint16_t i = 0; i < size; i++) {
        checksum += bytes[i];
    }

    return checksum;
}

bool FlashManager::verifyWrite(const void* data, uint16_t size, uint32_t address) {
    const uint8_t* written = (const uint8_t*)data;

    for (uint16_t i = 0; i < size; i++) {
        if (pgm_read_byte(address + i) != written[i]) {
            return false;
        }
    }

    return true;
}

#endif // DEBUG_LEVEL > 0