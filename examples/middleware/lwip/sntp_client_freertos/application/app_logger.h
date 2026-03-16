/**
  ******************************************************************************
  * File Name          : app_logger.h
  * Description        : This file provides the interface for logger.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_LOGGER_H
#define APP_LOGGER_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "example.h"
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#define APP_LOGGER_ERROR_STR   "ERROR"
#define APP_LOGGER_WARNING_STR "WARNING"
#define APP_LOGGER_INFO_STR    "INFO"

/**
  * brief: Initialiez the app_logger
  *
  * retval: app_status_t
  */
app_status_t app_logger_init(void);

/**
  * brief: Display Error log
  *
  * param: name app_logger name
  */
#define APP_LOG_ERROR(name, ...) APP_LOG(APP_LOGGER_ERROR_STR, name, __VA_ARGS__)

/**
  * brief: Display Warning log
  *
  * param: name app_logger name
  */
#define APP_LOG_WARNING(name, ...) APP_LOG(APP_LOGGER_WARNING_STR, name, __VA_ARGS__)

/**
  * brief: Display Info log
  *
  * param: name app_logger name
  */
#define APP_LOG_INFO(name, ...) APP_LOG(APP_LOGGER_INFO_STR, name, __VA_ARGS__)

/**
  * brief: Lock function for safe logging.
  *        Private function, must not be called by user.
  *        Prototype declared to be used in macro.
  *
  * retval: None
  */
void AppLoggerUnlock(void);

/**
  * brief: Unlock function for safe logging.
  *        Private function, must not be called by user.
  *        Prototype declared to be used in macro.
  */
void AppLoggerLock(void);

/**
  * brief: Unlock function for safe logging.
  *        Private function, must not be called by user.
  *        Prototype declared to be used in macro.
  *
  * param: level app_logger log level
  * param: name app_logger name
  */
void AppLoggerHeader(const char *level, const char *name);

#if defined(USE_TRACE) && USE_TRACE != 0
#define APP_LOG(level, name, ...)    do { \
                                          AppLoggerLock(); \
                                          AppLoggerHeader(level, name); \
                                          PRINTF(__VA_ARGS__); \
                                          AppLoggerUnlock(); \
                                        } while(0)
#else
#define APP_LOG(level, name, ...)
#endif /* defined(USE_TRACE) && USE_TRACE != 0 */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* APP_LOGGER_H */
