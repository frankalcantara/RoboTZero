#ifndef ESP32SIM_TESTS_EXPECT_HPP
#define ESP32SIM_TESTS_EXPECT_HPP

#include <cstdlib>
#include <iostream>

namespace esp32sim_test {

inline int failures = 0;

inline void expect(bool condition, const char* message) {
  if (!condition) {
    std::cerr << "FAIL: " << message << '\n';
    ++failures;
  }
}

inline int finish(const char* name) {
  if (failures != 0) {
    std::cerr << failures << " failure(s)\n";
    return EXIT_FAILURE;
  }
  std::cout << name << " OK\n";
  return EXIT_SUCCESS;
}

}  // namespace esp32sim_test

#endif /* ESP32SIM_TESTS_EXPECT_HPP */
