#ifndef ESP32SIM_ABI_H
#define ESP32SIM_ABI_H

/*
 * Stable C ABI between the C++23 core and any host: WASM loader, native test
 * harness or tooling. Rules:
 *
 * - This header must stay valid C11 and self-contained.
 * - Only fixed-width integers, pointers and plain structs cross the boundary.
 * - No exceptions, no RTTI, no ownership transfer beyond create/destroy.
 * - Functions are deterministic: same inputs, same outputs, no wall clock.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Major in the high 16 bits, minor in the low 16 bits. 0.1 packs to 0x0001. */
#define ESP32SIM_ABI_VERSION_MAJOR 0
#define ESP32SIM_ABI_VERSION_MINOR 1
/* Written as a typed literal because this header must stay valid C11 and cannot
 * use C++ casts. The C++ side asserts that the parts and the packed value agree. */
#define ESP32SIM_ABI_VERSION UINT32_C(0x00000001)

typedef int32_t esp32sim_status;

#define ESP32SIM_OK 0
#define ESP32SIM_ERROR_INVALID_ARGUMENT 1
#define ESP32SIM_ERROR_OUT_OF_MEMORY 2
#define ESP32SIM_ERROR_STATE 3

/*
 * Build identity. FROZEN: never append fields to this struct. It has no
 * `struct_size`, so an extra field would make a newer core write past the
 * buffer of an older host. New identity data goes in a new function. The
 * 16-byte layout is asserted in core/src/abi.cpp.
 */
typedef struct esp32sim_version {
  uint32_t major;
  uint32_t minor;
  uint32_t patch;
  uint32_t abi_version;
} esp32sim_version;

/* ABI and build identity. */
uint32_t esp32sim_abi_version(void);
esp32sim_status esp32sim_get_version(esp32sim_version* out);
const char* esp32sim_version_string(void);

/*
 * Creation config. `NULL` means defaults.
 *
 * Evolution rule: fields may only be appended. The caller writes its own
 * `struct_size`; the core reads at most that many bytes and rejects a config
 * smaller than the fields it knows. `flags` is reserved and must be zero.
 */
typedef struct esp32sim_config {
  uint32_t struct_size;
  uint32_t flags;
} esp32sim_config;

/* Opaque handle: one per simulated instance. */
typedef struct esp32sim_handle esp32sim_handle;

esp32sim_status esp32sim_create(const esp32sim_config* config,
                                esp32sim_handle** out_handle);
void esp32sim_destroy(esp32sim_handle* handle);

/* Advances the instance by `cycles`. Rejects null and overflow. */
esp32sim_status esp32sim_run(esp32sim_handle* handle, uint64_t cycles);

/* Returns 0 for a null handle, so the call is safe before create(). */
uint64_t esp32sim_cycle_count(const esp32sim_handle* handle);

/*
 * Event ring buffer. Single producer (core) / single consumer (host).
 *
 * - `head` and `tail` are free-running counters; the slot is
 *   `counter & (capacity - 1)`.
 * - The core writes `events`, `head` and `dropped`; the host reads `head` and
 *   writes only `tail`.
 * - Every access to `head`, `tail` and `dropped` must be atomic: `std::atomic_ref`
 *   in C++, `Atomics` in JS. Plain reads can tear.
 * - The ring is best effort: when full, the newest event is dropped and
 *   `dropped` is incremented.
 */
typedef struct esp32sim_event {
  uint32_t type;
  uint32_t code;
  uint64_t cycle;
  uint64_t payload;
} esp32sim_event;

typedef struct esp32sim_event_ring {
  uint32_t capacity;   /* number of events; power of two, >= 2 */
  uint32_t head;       /* producer-owned */
  uint32_t tail;       /* consumer-owned */
  uint32_t dropped;    /* producer-owned */
} esp32sim_event_ring;

/* Types from docs/03-arquitetura.md §5.3. */
#define ESP32SIM_EVENT_SERIAL_TX UINT32_C(1)
#define ESP32SIM_EVENT_PIN_CHANGE UINT32_C(2)
#define ESP32SIM_EVENT_IRQ UINT32_C(3)
#define ESP32SIM_EVENT_DEVICE_STATE UINT32_C(4)
#define ESP32SIM_EVENT_TRACE_MARK UINT32_C(5)
#define ESP32SIM_EVENT_ERROR UINT32_C(6)
#define ESP32SIM_EVENT_STOPPED UINT32_C(7)
#define ESP32SIM_EVENT_BREAKPOINT UINT32_C(8)

/* Allocates the ring inside the core's memory. Call once per handle.
 * `capacity` is the number of events, a power of two >= 2. */
esp32sim_status esp32sim_event_ring_init(esp32sim_handle* handle, uint32_t capacity);

/* Pointers into the core's memory. The host may write only `ring->tail`. */
esp32sim_event_ring* esp32sim_event_ring_header(esp32sim_handle* handle);
const esp32sim_event* esp32sim_event_ring_events(const esp32sim_handle* handle);

/* Test-only hook: pushes one event with the current cycle. Returns OK even
 * when the event is dropped; check `dropped`. Removed when GPIO and UART
 * produce events (E1-02). */
esp32sim_status esp32sim_test_emit(esp32sim_handle* handle, uint32_t type,
                                   uint32_t code, uint64_t payload);

#ifdef __cplusplus
}
#endif

#endif /* ESP32SIM_ABI_H */
