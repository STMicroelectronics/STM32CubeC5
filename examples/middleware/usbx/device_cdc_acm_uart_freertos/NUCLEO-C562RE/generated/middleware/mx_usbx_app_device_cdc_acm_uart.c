/**
  ******************************************************************************
  * @file    app_usbx_device_cdc_acm_uart.c
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "mx_usbx_app_device_cdc_acm_uart.h"
#include "mx_hal_def.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define APP_RX_DATA_SIZE   2048
#define APP_TX_DATA_SIZE   2048
#define DMA_ALIGNMENT     64U /* user define to adjust the data alignment to the DMA IP of the SoC */
/* Rx/TX flag */
#define RX_NEW_RECEIVED_DATA      0x01
#define TX_NEW_TRANSMITTED_DATA   0x02

/* Data length for vcp */
#define VCP_WORDLENGTH8  8
#define VCP_WORDLENGTH9  9

/* the minimum baudrate */
#define MIN_BAUDRATE    9600
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UX_SLAVE_CLASS_CDC_ACM  *cdc_acm;

/* Data to send over USB CDC are stored in this buffer   */
__attribute__((section(".non_cacheable_variables"), aligned(DMA_ALIGNMENT)))
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

uint32_t UserTxBufPtrIn;
uint32_t UserTxBufPtrOut;

hal_uart_handle_t *p_uart_handle;
hal_uart_config_t config;

UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER CDC_VCP_LineCoding =
{
  115200, /* baud rate */
  0x00,   /* stop bits-1 */
  0x00,   /* parity - none */
  0x08    /* nb. of bits 8 */
};

extern UX_EVENT_FLAGS_GROUP EventFlag;
/* Exported variables --------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern VOID USBX_APP_UART_Init(hal_uart_handle_t **huart);
static void USBD_CDC_VCP_Config(UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER *);
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  USBD_CDC_ACM_Activate
  *         This function is called when insertion of a CDC ACM device.
  * @param  cdc_acm_instance: Pointer to the cdc acm class instance.
  * @retval none
  */
VOID USBD_CDC_ACM_Activate(VOID *cdc_acm_instance)
{

  /* Save the CDC instance */
  cdc_acm = (UX_SLAVE_CLASS_CDC_ACM *) cdc_acm_instance;

  /* Get UART handle */
  p_uart_handle = mx_example_uart_gethandle();

  /* Get default UART parameters */
  CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_baudrate = config.baud_rate;

  /* Set the UART data type : only 8bits and 9bits are supported */
  switch (config.word_length)
  {
    case HAL_UART_WORD_LENGTH_8_BIT:
    {
      /* Set UART data bit to 8 */
      CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_data_bit = VCP_WORDLENGTH8;
      break;
    }

    case HAL_UART_WORD_LENGTH_9_BIT:
    {
      /* Set UART data bit to 9 */
      CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_data_bit = VCP_WORDLENGTH9;
      break;
    }

    default :
    {
      /* By default set UART data bit to 8 */
      CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_data_bit = VCP_WORDLENGTH8;
      break;
    }
  }

  /* Get UART Parity */
  CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_parity = config.parity;

  /* Get UART StopBits */
  CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_stop_bit = config.stop_bits;

  /* Set device class_cdc_acm with default parameters */
  if (ux_device_class_cdc_acm_ioctl(cdc_acm, UX_SLAVE_CLASS_CDC_ACM_IOCTL_SET_LINE_CODING,
                                    &CDC_VCP_LineCoding) != UX_SUCCESS)
  {
    while (1);
  }

  /* Receive an amount of data in interrupt mode */
  if (HAL_UART_Receive_DMA(p_uart_handle, (uint8_t *)UserTxBufferFS, 1) != HAL_OK)
  {
    /* Transfer error in reception process */
    while (1);
  }

  return;
}

/**
  * @brief  USBD_CDC_ACM_Deactivate
  *         This function is called when extraction of a CDC ACM device.
  * @param  cdc_acm_instance: Pointer to the cdc acm class instance.
  * @retval none
  */
VOID USBD_CDC_ACM_Deactivate(VOID *cdc_acm_instance)
{
  UX_PARAMETER_NOT_USED(cdc_acm_instance);

  /* Reset the cdc acm instance */
  cdc_acm = UX_NULL;

  return;
}

/**
  * @brief  USBD_CDC_ACM_ParameterChange
  *         This function is invoked to manage the CDC ACM class requests.
  * @param  cdc_acm_instance: Pointer to the cdc acm class instance.
  * @retval none
  */
VOID USBD_CDC_ACM_ParameterChange(VOID *cdc_acm_instance)
{
  UX_PARAMETER_NOT_USED(cdc_acm_instance);

  ULONG request;
  UX_SLAVE_TRANSFER *transfer_request;
  UX_SLAVE_DEVICE *device;

  /* Get the pointer to the device.  */
  device = &_ux_system_slave -> ux_system_slave_device;

  /* Get the pointer to the transfer request associated with the control endpoint. */
  transfer_request = &device -> ux_slave_device_control_endpoint.ux_slave_endpoint_transfer_request;

  request = *(transfer_request -> ux_slave_transfer_request_setup + UX_SETUP_REQUEST);

  switch (request)
  {
    case UX_SLAVE_CLASS_CDC_ACM_SET_LINE_CODING :

      /* Get the Line Coding parameters */
      if (ux_device_class_cdc_acm_ioctl(cdc_acm_instance, UX_SLAVE_CLASS_CDC_ACM_IOCTL_GET_LINE_CODING,
                                        &CDC_VCP_LineCoding) != UX_SUCCESS)
      {
        while (1);
      }

      /* Check if baudrate < 9600) then set it to 9600 */
      if (CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_baudrate < MIN_BAUDRATE)
      {
        CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_baudrate = MIN_BAUDRATE;

        /* Set the new configuration of ComPort */
        USBD_CDC_VCP_Config(&CDC_VCP_LineCoding);
      }
      else
      {
        /* Set the new configuration of ComPort */
        USBD_CDC_VCP_Config(&CDC_VCP_LineCoding);
      }

      break;

    case UX_SLAVE_CLASS_CDC_ACM_GET_LINE_CODING :

      /* Set the Line Coding parameters */
      if (ux_device_class_cdc_acm_ioctl(cdc_acm_instance, UX_SLAVE_CLASS_CDC_ACM_IOCTL_SET_LINE_CODING,
                                        &CDC_VCP_LineCoding) != UX_SUCCESS)
      {
        while (1);
      }

      break;

    case UX_SLAVE_CLASS_CDC_ACM_SET_CONTROL_LINE_STATE :
    default :
      break;
  }

  return;
}

/**
  * @brief  Function implementing usbx_cdc_acm_thread_entry.
  * @param  none
  * @retval none
  */
VOID usbx_cdc_acm_read_thread_entry(VOID *args)
{
  ULONG actual_length;
  ULONG senddataflag = 0;
  UX_SLAVE_DEVICE *device;

  device = &_ux_system_slave->ux_system_slave_device;

  while (1)
  {
    /* Check if device is configured */
    if ((device->ux_slave_device_state == UX_DEVICE_CONFIGURED) && (cdc_acm != UX_NULL))
    {

#ifndef UX_DEVICE_CLASS_CDC_ACM_TRANSMISSION_DISABLE

      /* Set transmission_status to UX_FALSE for the first time */
      cdc_acm -> ux_slave_class_cdc_acm_transmission_status = UX_FALSE;

#endif /* UX_DEVICE_CLASS_CDC_ACM_TRANSMISSION_DISABLE */

      /* Read the received data in blocking mode */
      ux_device_class_cdc_acm_read(cdc_acm, (UCHAR *)UserRxBufferFS, 512,
                                   &actual_length);
      if (actual_length != 0)
      {
        /* Send the data via UART */
        if (HAL_UART_Transmit_DMA(p_uart_handle, (uint8_t *)UserRxBufferFS, actual_length) != HAL_OK)
        {
          while (1);
        }

        /* Wait until the requested flag TX_NEW_TRANSMITTED_DATA is received */
        if (_ux_os_utility_event_flags_get(&EventFlag, TX_NEW_TRANSMITTED_DATA, UX_OR_CLEAR,
                                           &senddataflag, UX_WAIT_FOREVER) != UX_SUCCESS)
        {
          while (1);
        }
      }
      else
      {
        /* Sleep thread for 10ms if no data received */
        vTaskDelay(pdMS_TO_TICKS(10));
      }
    }
    else
    {
      /* Sleep thread for 10ms */
      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }
}

/**
  * @brief  Function implementing usbx_cdc_acm_write_thread_entry.
  * @param  none
  * @retval none
  */
VOID usbx_cdc_acm_write_thread_entry(VOID *args)
{
  ULONG receivedataflag = 0;
  ULONG actual_length, buffptr, buffsize;

  while (1)
  {
    /* Wait until the requested flag RX_NEW_RECEIVED_DATA is received */
    if (_ux_os_utility_event_flags_get(&EventFlag, RX_NEW_RECEIVED_DATA, UX_OR_CLEAR,
                                       &receivedataflag, UX_WAIT_FOREVER) != UX_SUCCESS)
    {
      while (1);
    }

#ifndef UX_DEVICE_CLASS_CDC_ACM_TRANSMISSION_DISABLE

    /* Set transmission_status to UX_FALSE for the first time */
    cdc_acm -> ux_slave_class_cdc_acm_transmission_status = UX_TRUE;

#endif /* UX_DEVICE_CLASS_CDC_ACM_TRANSMISSION_DISABLE */

    /* Check if there is a new data to send */
    if (UserTxBufPtrOut != UserTxBufPtrIn)
    {
      /* Check buffer overflow and Rollback */
      if (UserTxBufPtrOut > UserTxBufPtrIn)
      {
        buffsize = APP_RX_DATA_SIZE - UserTxBufPtrOut;
      }
      else
      {
        /* Calculate data size */
        buffsize = UserTxBufPtrIn - UserTxBufPtrOut;
      }

      /* Copy UserTxBufPtrOut in buffptr */
      buffptr = UserTxBufPtrOut;

      /* Send data over the class cdc_acm_write */
      if (ux_device_class_cdc_acm_write(cdc_acm, (UCHAR *)(&UserTxBufferFS[buffptr]),
                                        buffsize, &actual_length) == UX_SUCCESS)
      {
        /* Increment the UserTxBufPtrOut pointer */
        UserTxBufPtrOut += buffsize;

        /* Rollback UserTxBufPtrOut if it equal to APP_TX_DATA_SIZE */
        if (UserTxBufPtrOut == APP_TX_DATA_SIZE)
        {
          UserTxBufPtrOut = 0;
        }
      }
    }
  }
}

/**
  * @brief  Tx Transfer completed callback.
  * @param  huart UART handle.
  * @retval none
  */
void HAL_UART_TxCpltCallback(hal_uart_handle_t *huart)
{
  /* Set TX_NEW_TRANSMITTED_DATA flag */
  if (_ux_os_utility_event_flags_set(&EventFlag, TX_NEW_TRANSMITTED_DATA, UX_OR) != UX_SUCCESS)
  {
    while (1);
  }
}

/**
  * @brief  Rx Transfer completed callback
  * @param  huart: UART handle
  * @retval none
  */
void HAL_UART_RxCpltCallback(hal_uart_handle_t *huart, uint32_t size_byte, hal_uart_rx_event_types_t rx_event)
{
  /* Prevent unused argument(s) compilation warning */
  STM32_UNUSED(huart);
  STM32_UNUSED(size_byte);
  STM32_UNUSED(rx_event);
  /* Set RX_NEW_RECEIVED_DATA flag */
  if (_ux_os_utility_event_flags_set(&EventFlag, RX_NEW_RECEIVED_DATA, UX_OR))
  {
    while (1);
  }

  /* Increment the UserTxBufPtrIn pointer */
  UserTxBufPtrIn++;

  /* Rollback the UserTxBufPtrIn if it equal to APP_TX_DATA_SIZE */
  if (UserTxBufPtrIn == APP_TX_DATA_SIZE)
  {
    UserTxBufPtrIn = 0;
  }

  /* Start another reception: provide the buffer pointer with offset and the buffer size */
  if (HAL_UART_Receive_DMA(p_uart_handle, (uint8_t *)UserTxBufferFS + UserTxBufPtrIn, 1) != HAL_OK)
  {
    /* Transfer error in reception process */
    while (1);
  }

}

/**
  * @brief  UART error callbacks
            Transfer error occurred in reception and/or transmission process.
  * @param  huart: UART handle
  * @retval none
  */
void HAL_UART_ErrorCallback(hal_uart_handle_t *huart)
{
  while (1);
}

/**
  * @brief  USBD_CDC_VCP_Config
            Configure the COM Port with the parameters received from host.
  * @param  UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER: linecoding struct.
  * @param  CDC_VCP_LineCoding: CDC VCP line coding.
  * @retval none
  */
static VOID USBD_CDC_VCP_Config(UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER
                                *CDC_VCP_LineCoding)
{
  /* Check stop bit parameter */
  switch (CDC_VCP_LineCoding->ux_slave_class_cdc_acm_parameter_stop_bit)
  {
    case 0:

      /* Set the UART Stop bit to 1 */
      config.stop_bits = HAL_UART_STOP_BIT_1;

      break;

    case 2:

      /* Set the UART Stop bit to 2 */
      config.stop_bits = HAL_UART_STOP_BIT_2;

      break;

    default :

      /* By default set the UART Stop bit to 1 */
      config.stop_bits = HAL_UART_STOP_BIT_1;

      break;
  }

  /* Check parity parameter */
  switch (CDC_VCP_LineCoding->ux_slave_class_cdc_acm_parameter_parity)
  {
    case 0:

      /* Set the UART parity bit to none */
      config.parity = HAL_UART_PARITY_NONE;

      break;

    case 1:

      /* Set the UART parity bit to ODD */
      config.parity = HAL_UART_PARITY_ODD;

      break;

    case 2:

      /* Set the UART parity bit to even */
      config.parity = HAL_UART_PARITY_EVEN;

      break;

    default :

      /* By default set the UART parity bit to none */
      config.parity = HAL_UART_PARITY_NONE;

      break;
  }

  /* Set the UART data type : only 8bits and 9bits is supported */
  switch (CDC_VCP_LineCoding->ux_slave_class_cdc_acm_parameter_data_bit)
  {
    case 0x07:

      /* With this configuration a parity (Even or Odd) must be set */
      config.word_length = HAL_UART_WORD_LENGTH_8_BIT;

      break;

    case 0x08:

      if (config.parity == HAL_UART_PARITY_NONE)
      {
        config.word_length = HAL_UART_WORD_LENGTH_8_BIT;
      }
      else
      {
        config.word_length = HAL_UART_WORD_LENGTH_9_BIT;
      }

      break;

    default :

      config.word_length = HAL_UART_WORD_LENGTH_8_BIT;

      break;
  }

  /* Get the UART baudrate from vcp */
  config.baud_rate = CDC_VCP_LineCoding->ux_slave_class_cdc_acm_parameter_baudrate;

  /* Set the UART Hw flow control to none */
  config.hw_flow_ctl = HAL_UART_HW_CONTROL_NONE;
  /* Set the UART mode */
  config.direction = HAL_UART_DIRECTION_TX_RX;

  /* Set the UART sampling */
  config.oversampling = HAL_UART_OVERSAMPLING_16;

  /* Set Config UART */
  if (HAL_UART_SetConfig(p_uart_handle, &config) != HAL_OK)
  {
    /* Set Config Error */
    while (1);
  }

  /* Start reception: provide the buffer pointer with offset and the buffer size */
  HAL_UART_Receive_IT(p_uart_handle, (uint8_t *)(UserTxBufferFS + UserTxBufPtrIn), 1);
}
