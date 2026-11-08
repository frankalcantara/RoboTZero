#include "esp32sim/abi.h"

#include <cstdint>
#include <cstring>
#include <limits>

#include "expect.hpp"

using esp32sim_test::expect;

int main() {
  // Identidade.
  expect(esp32sim_abi_version() == ESP32SIM_ABI_VERSION, "abi_version");
  expect(ESP32SIM_ABI_VERSION == 0x0001U, "packed abi version");
  expect(std::strcmp(esp32sim_version_string(), "0.1.0") == 0, "version_string");

  esp32sim_version v{};
  expect(esp32sim_get_version(&v) == ESP32SIM_OK, "get_version status");
  expect(v.major == 0U, "version major");
  expect(v.minor == 1U, "version minor");
  expect(v.patch == 0U, "version patch");
  expect(v.abi_version == ESP32SIM_ABI_VERSION, "version abi");
  expect(esp32sim_get_version(nullptr) == ESP32SIM_ERROR_INVALID_ARGUMENT,
         "get_version null out");

  // Config: nula vale defaults; struct curto e flags reservadas são rejeitados.
  esp32sim_config config{};
  config.struct_size = sizeof(esp32sim_config);
  config.flags = 0U;

  esp32sim_handle* handle = nullptr;
  expect(esp32sim_create(nullptr, nullptr) == ESP32SIM_ERROR_INVALID_ARGUMENT,
         "create null out");
  expect(esp32sim_create(&config, &handle) == ESP32SIM_OK, "create with config");
  expect(handle != nullptr, "handle not null");

  esp32sim_config short_config{};
  short_config.struct_size = 4U;
  esp32sim_handle* rejected = nullptr;
  expect(esp32sim_create(&short_config, &rejected) == ESP32SIM_ERROR_INVALID_ARGUMENT,
         "short config rejected");
  expect(rejected == nullptr, "rejected create leaves handle null");

  esp32sim_config bad_flags{};
  bad_flags.struct_size = sizeof(esp32sim_config);
  bad_flags.flags = 1U;
  expect(esp32sim_create(&bad_flags, &rejected) == ESP32SIM_ERROR_INVALID_ARGUMENT,
         "reserved flags rejected");

  esp32sim_handle* defaults = nullptr;
  expect(esp32sim_create(nullptr, &defaults) == ESP32SIM_OK, "create with null config");
  expect(defaults != nullptr, "default handle not null");
  esp32sim_destroy(defaults);

  // Contagem de ciclos.
  expect(esp32sim_cycle_count(handle) == 0U, "initial count");
  expect(esp32sim_run(handle, 1000U) == ESP32SIM_OK, "run 1000");
  expect(esp32sim_cycle_count(handle) == 1000U, "count after 1000");
  expect(esp32sim_run(handle, 500U) == ESP32SIM_OK, "run 500");
  expect(esp32sim_cycle_count(handle) == 1500U, "count after 1500");
  expect(esp32sim_run(handle, 0U) == ESP32SIM_OK, "run 0 is valid");
  expect(esp32sim_cycle_count(handle) == 1500U, "count unchanged after 0");

  // Guardas.
  constexpr std::uint64_t kMax = std::numeric_limits<std::uint64_t>::max();
  expect(esp32sim_run(handle, kMax) == ESP32SIM_ERROR_STATE, "overflow rejected");
  expect(esp32sim_cycle_count(handle) == 1500U, "count unchanged after overflow");
  expect(esp32sim_run(nullptr, 1U) == ESP32SIM_ERROR_INVALID_ARGUMENT, "run null");
  expect(esp32sim_cycle_count(nullptr) == 0U, "count null is 0");

  esp32sim_destroy(handle);
  esp32sim_destroy(nullptr);

  // Determinismo: instâncias independentes com a mesma sequência coincidem.
  esp32sim_handle* a = nullptr;
  esp32sim_handle* b = nullptr;
  expect(esp32sim_create(nullptr, &a) == ESP32SIM_OK, "create a");
  expect(esp32sim_create(nullptr, &b) == ESP32SIM_OK, "create b");
  for (int i = 0; i < 100; ++i) {
    expect(esp32sim_run(a, 7U) == ESP32SIM_OK, "run a");
    expect(esp32sim_run(b, 7U) == ESP32SIM_OK, "run b");
  }
  expect(esp32sim_cycle_count(a) == 700U, "count a");
  expect(esp32sim_cycle_count(a) == esp32sim_cycle_count(b), "deterministic");
  esp32sim_destroy(a);
  esp32sim_destroy(b);

  return esp32sim_test::finish("abi test");
}
