# file-format: 1.0.0
if(CMAKE_BUILD_TYPE STREQUAL "release_GCC_NUCLEO-C5A3ZG")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/example.c ../../application/main.c ../../application/example.h ../../application/main.h ../../application/app_config.h ../../application/app_logger.h ../../application/app_lwip.h ../../application/app_tcp_echo_client.h ../../application/app_tcp_echo_server.h ../../application/app_logger.c ../../application/app_lwip.c ../../application/app_tcp_echo_client.c ../../application/app_tcp_echo_server.c)
elseif(CMAKE_BUILD_TYPE STREQUAL "debug_GCC_NUCLEO-C5A3ZG")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/example.c ../../application/main.c ../../application/example.h ../../application/main.h ../../application/app_config.h ../../application/app_logger.h ../../application/app_lwip.h ../../application/app_tcp_echo_client.h ../../application/app_tcp_echo_server.h ../../application/app_logger.c ../../application/app_lwip.c ../../application/app_tcp_echo_client.c ../../application/app_tcp_echo_server.c)
endif()
