file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/inc/*{.h,.hpp})
idf_component_register(
  SRCS main.cpp ${SOURCES}
  INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/inc
  REQUIRES  esp_driver_rmt esp_driver_gpio esp_driver_ledc pthread)
