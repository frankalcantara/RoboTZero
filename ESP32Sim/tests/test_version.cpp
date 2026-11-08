#include "esp32sim/version.hpp"

#include "expect.hpp"

int main() {
  const auto v = esp32sim::version();
  esp32sim_test::expect(v.major == 0U, "major");
  esp32sim_test::expect(v.minor == 1U, "minor");
  esp32sim_test::expect(v.patch == 0U, "patch");
  esp32sim_test::expect(esp32sim::version_string() == "0.1.0", "version_string");

  return esp32sim_test::finish("version test");
}
