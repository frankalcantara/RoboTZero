#include "esp32sim/abi.h"

/* Compile-only: este arquivo é compilado como C11, então um construto
 * exclusivo de C++ em abi.h falha aqui em vez de falhar num host em C. */
esp32sim_status esp32sim_abi_c11_check(esp32sim_version* out) {
  return esp32sim_get_version(out);
}
