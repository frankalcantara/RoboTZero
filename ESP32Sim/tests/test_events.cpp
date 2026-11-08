#include "esp32sim/abi.h"

#include <cstdint>

#include "expect.hpp"

using esp32sim_test::expect;

namespace {

// Single-threaded test: plain reads are fine here, Atomics only matter when the
// consumer is another thread.
esp32sim_event read_event(const esp32sim_handle* handle, std::uint32_t slot) {
  return esp32sim_event_ring_events(handle)[slot];
}

}  // namespace

int main() {
  // Sem ring: as funções de evento são seguras e recusam o handle nulo.
  expect(esp32sim_event_ring_init(nullptr, 4U) == ESP32SIM_ERROR_INVALID_ARGUMENT,
         "init null handle");
  expect(esp32sim_event_ring_header(nullptr) == nullptr, "header null handle");
  expect(esp32sim_event_ring_events(nullptr) == nullptr, "events null handle");
  expect(esp32sim_test_emit(nullptr, ESP32SIM_EVENT_TRACE_MARK, 0U, 0U) ==
             ESP32SIM_ERROR_INVALID_ARGUMENT,
         "emit null handle");

  esp32sim_handle* handle = nullptr;
  expect(esp32sim_create(nullptr, &handle) == ESP32SIM_OK, "create");
  expect(esp32sim_event_ring_header(handle) == nullptr, "no ring before init");
  expect(esp32sim_event_ring_events(handle) == nullptr, "no events before init");
  expect(esp32sim_test_emit(handle, ESP32SIM_EVENT_TRACE_MARK, 0U, 0U) ==
             ESP32SIM_ERROR_INVALID_ARGUMENT,
         "emit before init");

  // Capacidade: potência de 2 e >= 2.
  expect(esp32sim_event_ring_init(handle, 0U) == ESP32SIM_ERROR_INVALID_ARGUMENT,
         "capacity 0 rejected");
  expect(esp32sim_event_ring_init(handle, 1U) == ESP32SIM_ERROR_INVALID_ARGUMENT,
         "capacity 1 rejected");
  expect(esp32sim_event_ring_init(handle, 3U) == ESP32SIM_ERROR_INVALID_ARGUMENT,
         "capacity 3 rejected");
  expect(esp32sim_event_ring_header(handle) == nullptr, "rejected init leaves no ring");

  expect(esp32sim_event_ring_init(handle, 4U) == ESP32SIM_OK, "init 4");
  expect(esp32sim_event_ring_init(handle, 8U) == ESP32SIM_ERROR_STATE,
         "second init rejected");

  esp32sim_event_ring* ring = esp32sim_event_ring_header(handle);
  expect(ring != nullptr, "header after init");
  expect(esp32sim_event_ring_events(handle) != nullptr, "events after init");
  expect(ring->capacity == 4U, "capacity stored");
  expect(ring->head == 0U, "head starts at 0");
  expect(ring->tail == 0U, "tail starts at 0");
  expect(ring->dropped == 0U, "dropped starts at 0");

  // Emissão: o evento carrega o ciclo corrente.
  expect(esp32sim_run(handle, 100U) == ESP32SIM_OK, "run 100");
  expect(esp32sim_test_emit(handle, ESP32SIM_EVENT_PIN_CHANGE, 7U, 0xAABBCCDDEEFF0011ULL) ==
             ESP32SIM_OK,
         "emit pin change");
  expect(ring->head == 1U, "head after one emit");
  expect(ring->dropped == 0U, "nothing dropped");

  const esp32sim_event first = read_event(handle, 0U);
  expect(first.type == ESP32SIM_EVENT_PIN_CHANGE, "event type");
  expect(first.code == 7U, "event code");
  expect(first.cycle == 100U, "event cycle");
  expect(first.payload == 0xAABBCCDDEEFF0011ULL, "event payload");

  // O consumidor avança `tail`; o produtor respeita o espaço livre.
  ring->tail = 1U;
  expect(esp32sim_test_emit(handle, ESP32SIM_EVENT_SERIAL_TX, 1U, 0U) == ESP32SIM_OK, "emit 2");
  expect(esp32sim_test_emit(handle, ESP32SIM_EVENT_SERIAL_TX, 2U, 0U) == ESP32SIM_OK, "emit 3");
  expect(esp32sim_test_emit(handle, ESP32SIM_EVENT_SERIAL_TX, 3U, 0U) == ESP32SIM_OK, "emit 4");
  expect(ring->head == 4U, "head after four emits");
  expect(read_event(handle, 3U).code == 3U, "slot 3 is the fourth event");

  // Último slot livre: ainda aceito.
  expect(esp32sim_test_emit(handle, ESP32SIM_EVENT_SERIAL_TX, 4U, 0U) == ESP32SIM_OK, "emit 5");
  expect(ring->head == 5U, "last free slot accepted");

  // Cheio: o mais novo é descartado e `dropped` conta.
  expect(esp32sim_test_emit(handle, ESP32SIM_EVENT_ERROR, 99U, 0U) == ESP32SIM_OK,
         "emit on full ring is not an error");
  expect(ring->head == 5U, "full ring does not advance head");
  expect(ring->dropped == 1U, "full ring counts the drop");

  // Wrap-around dos contadores: `head - tail` é aritmética de 32 bits.
  ring->head = 0xFFFFFFFEU;
  ring->tail = 0xFFFFFFFEU;
  expect(esp32sim_test_emit(handle, ESP32SIM_EVENT_TRACE_MARK, 1U, 0U) == ESP32SIM_OK, "wrap 1");
  expect(ring->head == 0xFFFFFFFFU, "head before wrap");
  expect(read_event(handle, 0xFFFFFFFEU & 3U).code == 1U, "slot before wrap");

  expect(esp32sim_test_emit(handle, ESP32SIM_EVENT_TRACE_MARK, 2U, 0U) == ESP32SIM_OK, "wrap 2");
  expect(ring->head == 0U, "head wrapped to 0");
  expect(read_event(handle, 3U).code == 2U, "slot 3 after wrap");

  expect(esp32sim_test_emit(handle, ESP32SIM_EVENT_TRACE_MARK, 3U, 0U) == ESP32SIM_OK, "wrap 3");
  expect(read_event(handle, 0U).code == 3U, "slot 0 after wrap");

  expect(esp32sim_test_emit(handle, ESP32SIM_EVENT_TRACE_MARK, 4U, 0U) == ESP32SIM_OK, "wrap 4");
  expect(ring->head == 2U, "head after wrap");

  // Cheio com contadores perto do limite: `head - tail` fecha em 4.
  expect(esp32sim_test_emit(handle, ESP32SIM_EVENT_ERROR, 0U, 0U) == ESP32SIM_OK, "wrap full");
  expect(ring->head == 2U, "wrap full does not advance head");
  expect(ring->dropped == 2U, "wrap full counts the drop");

  esp32sim_destroy(handle);
  esp32sim_destroy(nullptr);

  return esp32sim_test::finish("events test");
}
