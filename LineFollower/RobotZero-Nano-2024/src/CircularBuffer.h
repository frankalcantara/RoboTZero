#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <Arduino.h>

#if DEBUG_LEVEL > 0

template<typename T, uint8_t SIZE>
class CircularBuffer {
public:
    CircularBuffer() : head(0), tail(0), count(0) {}

    // Add item to buffer
    bool push(const T& item) {
        if (isFull()) {
            return false;
        }

        buffer[head] = item;
        head = (head + 1) % SIZE;
        count++;
        return true;
    }

    // Get item from buffer
    bool pop(T& item) {
        if (isEmpty()) {
            return false;
        }

        item = buffer[tail];
        tail = (tail + 1) % SIZE;
        count--;
        return true;
    }

    // Peek at next item without removing
    bool peek(T& item) const {
        if (isEmpty()) {
            return false;
        }

        item = buffer[tail];
        return true;
    }

    // Get item at specific index
    bool at(uint8_t index, T& item) const {
        if (index >= count) {
            return false;
        }

        uint8_t actual_index = (tail + index) % SIZE;
        item = buffer[actual_index];
        return true;
    }

    // Clear the buffer
    void clear() {
        head = tail = count = 0;
    }

    // Get number of items in buffer
    uint8_t getCount() const {
        return count;
    }

    // Get buffer capacity
    static constexpr uint8_t getCapacity() {
        return SIZE;
    }

    // Check if buffer is empty
    bool isEmpty() const {
        return count == 0;
    }

    // Check if buffer is full
    bool isFull() const {
        return count >= SIZE;
    }

    // Get free space
    uint8_t getFreeSpace() const {
        return SIZE - count;
    }

    // Batch operations
    uint8_t pushMultiple(const T* items, uint8_t numItems) {
        uint8_t added = 0;
        for (uint8_t i = 0; i < numItems && !isFull(); i++) {
            if (push(items[i])) {
                added++;
            }
        }
        return added;
    }

    uint8_t popMultiple(T* items, uint8_t maxItems) {
        uint8_t removed = 0;
        for (uint8_t i = 0; i < maxItems && !isEmpty(); i++) {
            if (pop(items[i])) {
                removed++;
            }
        }
        return removed;
    }

    // Get continuous read/write block sizes
    uint8_t getContinuousReadSize() const {
        if (isEmpty()) return 0;
        if (tail < head) return head - tail;
        return SIZE - tail;
    }

    uint8_t getContinuousWriteSize() const {
        if (isFull()) return 0;
        if (head < tail || count == 0) return SIZE - head;
        return tail - head;
    }

    // Direct access to buffer areas (use with caution)
    T* getWritePtr() {
        return &buffer[head];
    }

    const T* getReadPtr() const {
        return &buffer[tail];
    }

private:
    T buffer[SIZE];       // Storage array
    uint8_t head;         // Write index
    uint8_t tail;         // Read index
    uint8_t count;        // Number of items
};

#endif // DEBUG_LEVEL > 0
#endif // CIRCULARBUFFER_H