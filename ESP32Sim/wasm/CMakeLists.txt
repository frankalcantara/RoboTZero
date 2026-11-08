# Módulo WASM com ABI C (E0-04).
#
# O alvo é um módulo WASM autônomo (`-sSTANDALONE_WASM`): nenhum import, nenhum
# glue JS, nomes de export estáveis iguais aos da ABI C. O Worker do E0-05
# instancia o `.wasm` direto e lê a memória linear sem passar por JS por ciclo.

if(NOT EMSCRIPTEN)
  message(FATAL_ERROR "wasm/ só é configurado com o toolchain Emscripten")
endif()

add_executable(esp32sim_module module.cpp)

target_link_libraries(esp32sim_module PRIVATE esp32sim_core)
esp32sim_set_warnings(esp32sim_module)

target_link_options(esp32sim_module PRIVATE
  --no-entry
  "-sEXPORTED_FUNCTIONS=['_malloc','_free','_esp32sim_abi_version','_esp32sim_get_version','_esp32sim_version_string','_esp32sim_create','_esp32sim_destroy','_esp32sim_run','_esp32sim_cycle_count','_esp32sim_event_ring_init','_esp32sim_event_ring_header','_esp32sim_event_ring_events','_esp32sim_test_emit']"
  -sSTANDALONE_WASM=1
  # SharedArrayBuffer na memória linear: o Worker publica o mesmo buffer para a
  # main thread, que lê o ring com Atomics. Sem pthreads, sem imports.
  -sSHARED_MEMORY=1
  # O dlmalloc, com atomics, arrasta `clock_time_get` e `__do_set_thread_state`
  # (estado de thread do pthread) e o módulo deixa de ter zero imports.
  # shortcut: emmalloc é mais simples e não puxa pthread; revisitar se o perfil
  # de alocação do core justificar dlmalloc.
  -sMALLOC=emmalloc
  -sALLOW_MEMORY_GROWTH=0
  -sFILESYSTEM=0
  -sERROR_ON_UNDEFINED_SYMBOLS=1
)

set_target_properties(esp32sim_module PROPERTIES
  OUTPUT_NAME "esp32sim"
  SUFFIX ".wasm"
)

# Smoke test: instancia o módulo no Node e chama a ABI como o Worker fará.
# Registrado no CTest para rodar com `ctest --preset wasm`.
find_program(ESP32SIM_NODE_EXECUTABLE node REQUIRED)
add_test(NAME wasm_smoke
  COMMAND "${ESP32SIM_NODE_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/smoke.cjs"
          "$<TARGET_FILE:esp32sim_module>"
)

# Worker real (node:worker_threads) publicando o SharedArrayBuffer e o ring.
add_test(NAME worker_smoke
  COMMAND "${ESP32SIM_NODE_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/worker/worker.test.mjs"
          "$<TARGET_FILE:esp32sim_module>"
)
