/**
  ******************************************************************************
  * file           : example.c
  * brief          : spi adc com polling.
  ******************************************************************************
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

/* Includes ------------------------------------------------------------------*/
#include "example.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TX_RX_BUFFER_SIZE      2U
#define AVCC_MV          3300U   /* Reference voltage in mV */
#define AVCC_12BITS      0xFFFU  /* Reference voltage coded on 12bits */
/* @user: Set the timeout period in milliseconds for the transmission and the reception of data */
#define TIMEOUT 1000U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_spi_handle_t *pSPI; /* Pointer referencing the SPI handle from the generated code */

uint32_t AdcValueMv;     /* Voltage on IN0 in mV */

/* Private functions prototype -----------------------------------------------*/
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t spi_error_code);

/** ########## Step 1 ##########
  * The init of the SPI instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status;

  pSPI = mx_example_spi_init();

  if (pSPI != NULL)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR.\n");
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;   /* memorizes the HAL status of the SPI TX/RX operations */
  uint32_t spi_error_code;   /* memorizes the SPI error code limited to the last process */
  uint16_t raw_adc_value;    /* Voltage on IN0 input coded in 12 bits */


  /** ########## Step 2 ##########
    * Initiate communication in full duplex blocking mode by exchanging data (transmitting and receiving) with
    * the ADC sensor within a specific timeout period.
    */
  /* Buffer used for transmission */
  uint8_t tx_buffer[TX_RX_BUFFER_SIZE] = {0x00, 0x00};
  /* Buffer used for reception */
  uint8_t rx_buffer[TX_RX_BUFFER_SIZE];

  hal_status = HAL_SPI_TransmitReceive(pSPI, tx_buffer, rx_buffer, TX_RX_BUFFER_SIZE, TIMEOUT);
  if (hal_status != HAL_OK)
  {
    spi_error_code = HAL_SPI_GetLastErrorCodes(pSPI);
    return_status  = HandleTransferError(hal_status, spi_error_code);

  }

  else
  {
    /** ########## Step 3 ##########
      * Convert input voltage and display it.
      */
    raw_adc_value = (rx_buffer[0] << 8) | rx_buffer[1];

    /* Retrieve voltage on IN0 input from RxBuffer */
    AdcValueMv = (raw_adc_value * AVCC_MV) / AVCC_12BITS;

    PRINTF("[INFO] Controller - Transfer COMPLETED. Input voltage is %" PRId32 " mV \n", AdcValueMv);
    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end app_process */


/** De-initializes the SPI before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_spi_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  This function is executed in case of a data transfer error.
  * param hal_status:  HAL status of the SPI TX/RX operations.
  * param spi_error_code:  SPI Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t spi_error_code)
{
  PRINTF("[ERROR] Controller - Communication ERROR: hal_status = %" PRIu32 ", \
  HAL_SPI_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, spi_error_code);

  /* Take the time to check the setup before restarting the communication */
  HAL_Delay(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
