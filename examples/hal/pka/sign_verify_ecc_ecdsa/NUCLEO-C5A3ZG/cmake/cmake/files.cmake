# file-format: 1.0.0
if(CMAKE_BUILD_TYPE STREQUAL "release_GCC_NUCLEO-C5A3ZG")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/example.c ../../application/main.c ../../application/example.h ../../application/main.h ../../application/app_prime256/prime256v1.c ../../application/app_prime256/sig_gen.c ../../application/app_prime256/sig_ver.c ../../application/app_prime256/prime256v1.h ../../application/app_prime256/sig_gen.h ../../application/app_prime256/sig_ver.h)
elseif(CMAKE_BUILD_TYPE STREQUAL "debug_GCC_NUCLEO-C5A3ZG")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/example.c ../../application/main.c ../../application/example.h ../../application/main.h ../../application/app_prime256/prime256v1.c ../../application/app_prime256/sig_gen.c ../../application/app_prime256/sig_ver.c ../../application/app_prime256/prime256v1.h ../../application/app_prime256/sig_gen.h ../../application/app_prime256/sig_ver.h)
endif()
