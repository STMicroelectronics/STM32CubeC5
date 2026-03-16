# file-format: 1.0.0
if(CMAKE_BUILD_TYPE STREQUAL "release_GCC_NUCLEO-C562RE")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/main.c ../../application/example_icache.c ../../application/example.h ../../application/main.h)
elseif(CMAKE_BUILD_TYPE STREQUAL "debug_GCC_NUCLEO-C562RE")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/main.c ../../application/example_icache.c ../../application/example.h ../../application/main.h)
endif()
