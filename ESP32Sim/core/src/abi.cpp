#include "esp32sim/abi.h"

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <new>

#include "esp32sim/version.hpp"

// ABI contract checks live here, not in wasm/module.cpp: this file compiles on
// both the native and the WASM build, so drift fails everywhere.
static_assert(ESP32SIM_ABI_VERSION ==
                  ((std::uint32_t{ESP32SIM_ABI_VERSION_MAJOR} << 16) |
                   std::uint32_t{ESP32SIM_ABI_VERSION_MINOR}),
              "ABI version macro is inconsistent");
static_assert(ESP32SIM_OK == 0, "status codes are part of the ABI");
static_assert(sizeof(esp32sim_version) == 16,
              "esp32sim_version is frozen at 16 bytes; add a new function instead");
static_assert(sizeof(esp32sim_config) == 8, "esp32sim_config layout is part of the ABI");
static_assert(sizeof(std::uint32_t) == 4, "the ABI assumes 32-bit unsigned");
static_assert(sizeof(std::uint64_t) == 8, "the ABI assumes 64-bit unsigned");

// The host reads the ring from JS with Atomics, so the layout must not depend on
// the compiler's padding and the counters must be lock-free.
static_assert(sizeof(esp32sim_event) == 24, "event layout is part of the ABI");
static_assert(offsetof(esp32sim_event, type) == 0, "event.type offset");
static_assert(offsetof(esp32sim_event, code) == 4, "event.code offset");
static_assert(offsetof(esp32sim_event, cycle) == 8, "event.cycle offset");
static_assert(offsetof(esp32sim_event, payload) == 16, "event.payload offset");
static_assert(sizeof(esp32sim_event_ring) == 16, "ring header layout is part of the ABI");
static_assert(std::atomic_ref<std::uint32_t>::is_always_lock_free,
              "the ring needs lock-free 32-bit atomics");

struct esp32sim_handle {
  std::uint64_t cycles = 0;
  esp32sim_event_ring* ring = nullptr;
  esp32sim_event* events = nullptr;
};

namespace {

// Single producer: only the core calls this. The consumer owns `tail`.
void push_event(esp32sim_handle& handle, const esp32sim_event& event) {
  esp32sim_event_ring& ring = *handle.ring;
  const std::uint32_t head = std::atomic_ref{ring.head}.load(std::memory_order_relaxed);
  const std::uint32_t tail = std::atomic_ref{ring.tail}.load(std::memory_order_acquire);
  if (head - tail >= ring.capacity) {
    std::atomic_ref{ring.dropped}.fetch_add(1, std::memory_order_relaxed);
    return;
  }
  handle.events[head & (ring.capacity - 1)] = event;
  std::atomic_ref{ring.head}.store(head + 1, std::memory_order_release);
}

}  // namespace

extern "C" {

std::uint32_t esp32sim_abi_version(void) { return ESP32SIM_ABI_VERSION; }

esp32sim_status esp32sim_get_version(esp32sim_version* out) {
  if (out == nullptr) {
    return ESP32SIM_ERROR_INVALID_ARGUMENT;
  }
  const esp32sim::Version v = esp32sim::version();
  out->major = v.major;
  out->minor = v.minor;
  out->patch = v.patch;
  out->abi_version = ESP32SIM_ABI_VERSION;
  return ESP32SIM_OK;
}

const char* esp32sim_version_string(void) { return esp32sim::version_string().data(); }

esp32sim_status esp32sim_create(const esp32sim_config* config,
                                esp32sim_handle** out_handle) {
  if (out_handle == nullptr) {
    return ESP32SIM_ERROR_INVALID_ARGUMENT;
  }
  *out_handle = nullptr;

  if (config != nullptr) {
    if (config->struct_size < sizeof(esp32sim_config) || config->flags != 0U) {
      return ESP32SIM_ERROR_INVALID_ARGUMENT;
    }
  }

  *out_handle = new (std::nothrow) esp32sim_handle{};
  return *out_handle != nullptr ? ESP32SIM_OK : ESP32SIM_ERROR_OUT_OF_MEMORY;
}

void esp32sim_destroy(esp32sim_handle* handle) {
  if (handle == nullptr) {
    return;
  }
  delete[] handle->events;
  delete handle->ring;
  delete handle;
}

esp32sim_status esp32sim_run(esp32sim_handle* handle, std::uint64_t cycles) {
  if (handle == nullptr) {
    return ESP32SIM_ERROR_INVALID_ARGUMENT;
  }
  if (cycles > std::numeric_limits<std::uint64_t>::max() - handle->cycles) {
    return ESP32SIM_ERROR_STATE;
  }
  handle->cycles += cycles;
  return ESP32SIM_OK;
}

std::uint64_t esp32sim_cycle_count(const esp32sim_handle* handle) {
  return handle != nullptr ? handle->cycles : 0;
}

esp32sim_status esp32sim_event_ring_init(esp32sim_handle* handle,
                                         std::uint32_t capacity) {
  if (handle == nullptr) {
    return ESP32SIM_ERROR_INVALID_ARGUMENT;
  }
  // Power of two, so the slot index is a mask and not a modulo.
  if (capacity < 2U || (capacity & (capacity - 1U)) != 0U) {
    return ESP32SIM_ERROR_INVALID_ARGUMENT;
  }
  if (handle->ring != nullptr) {
    return ESP32SIM_ERROR_STATE;
  }

  esp32sim_event_ring* ring = new (std::nothrow) esp32sim_event_ring{};
  esp32sim_event* events = new (std::nothrow) esp32sim_event[capacity]{};
  if (ring == nullptr || events == nullptr) {
    delete[] events;
    delete ring;
    return ESP32SIM_ERROR_OUT_OF_MEMORY;
  }

  ring->capacity = capacity;
  handle->ring = ring;
  handle->events = events;
  return ESP32SIM_OK;
}

esp32sim_event_ring* esp32sim_event_ring_header(esp32sim_handle* handle) {
  return handle != nullptr ? handle->ring : nullptr;
}

const esp32sim_event* esp32sim_event_ring_events(const esp32sim_handle* handle) {
  return handle != nullptr ? handle->events : nullptr;
}

esp32sim_status esp32sim_test_emit(esp32sim_handle* handle, std::uint32_t type,
                                   std::uint32_t code, std::uint64_t payload) {
  if (handle == nullptr || handle->ring == nullptr) {
    return ESP32SIM_ERROR_INVALID_ARGUMENT;
  }
  push_event(*handle, esp32sim_event{type, code, handle->cycles, payload});
  return ESP32SIM_OK;
}

}  // extern "C"
