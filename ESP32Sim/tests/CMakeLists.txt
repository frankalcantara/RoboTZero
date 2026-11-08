add_executable(esp32sim_test_version test_version.cpp)
target_link_libraries(esp32sim_test_version PRIVATE esp32sim_core)
esp32sim_set_warnings(esp32sim_test_version)

add_test(NAME version COMMAND esp32sim_test_version)

add_executable(esp32sim_test_abi test_abi.cpp)
target_link_libraries(esp32sim_test_abi PRIVATE esp32sim_core)
esp32sim_set_warnings(esp32sim_test_abi)

add_test(NAME abi COMMAND esp32sim_test_abi)

add_executable(esp32sim_test_events test_events.cpp)
target_link_libraries(esp32sim_test_events PRIVATE esp32sim_core)
esp32sim_set_warnings(esp32sim_test_events)

add_test(NAME events COMMAND esp32sim_test_events)

# O header promete C11. Compilar como C é o teste; não roda.
add_library(esp32sim_check_abi_c11 OBJECT abi_c11.c)
target_link_libraries(esp32sim_check_abi_c11 PRIVATE esp32sim_core)
set_target_properties(esp32sim_check_abi_c11 PROPERTIES
  C_STANDARD 11
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF
)

if(MSVC)
  target_compile_options(esp32sim_check_abi_c11 PRIVATE /W4 /WX)
else()
  target_compile_options(esp32sim_check_abi_c11 PRIVATE
    -Wall -Wextra -Wpedantic -Werror)
endif()
