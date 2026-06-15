/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : I3C direct polling controller example with LL API
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
#include "ll_example.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
static void I3C_ControlDataTreatment(const uint32_t **p_tc_data, uint32_t *p_tc_size_word);
static void I3C_TransmitByteTreatment(const uint8_t **p_tx_data, uint32_t *p_tx_size_byte);
static void I3C_ReceiveByteTreatment(uint8_t **p_rx_data, uint32_t *p_rx_size_byte);

/* Private functions ---------------------------------------------------------*/

/** brief: Assign target dynamic address in polling mode for the MX_I3Cx instance.
  * param:  target_dynamic_addr     Target dynamic address.
  * retval: SYSTEM_OK               Assignment success.
  * retval: SYSTEM_PERIPHERAL_ERROR Error during dynamic address assignment.
  */
inline system_status_t I3C_AssignDynamicAddress(uint8_t target_dynamic_addr)
{
  system_status_t return_status = SYSTEM_OK;

  uint64_t target_payload = 0UL;

  /* Enable arbitration header for the MX_I3Cx controller (required for ENTDAA sequence) */
  LL_I3C_EnableArbitrationHeader(MX_I3Cx);

  /* Send ENTDAA CCC to initiate dynamic address assignment for all targets */
  LL_I3C_ControllerHandleCCC(MX_I3Cx, LL_I3C_BROADCAST_ENTDAA, 0UL, LL_I3C_GENERATE_STOP);

  /* Wait for either Frame Complete, TX FIFO Not Full flag, or error */
  do
  {

    /* If an error flag is set, clear it and update return status */
    if (LL_I3C_IsActiveFlag_ERR(MX_I3Cx) != 0U)
    {
      LL_I3C_ClearFlag_ERR(MX_I3Cx);

      return_status = SYSTEM_PERIPHERAL_ERROR;
    }

  } while (((LL_I3C_READ_REG(MX_I3Cx, EVR) & (LL_I3C_EVR_FCF | I3C_EVR_TXFNFF)) == 0U) && (return_status == SYSTEM_OK));

  /* If TX FIFO is not full, read the target payload and assign the dynamic address */
  if (LL_I3C_IsActiveFlag_TXFNF(MX_I3Cx) != 0U)
  {
    /* Read 8 bytes of target payload (PID, BCR, DCR) from RX FIFO */
    for (uint32_t index = 0UL; index < 8U; index++)
    {
      target_payload |= (uint64_t)((uint64_t)LL_I3C_ReceiveData8(MX_I3Cx) << (index * 8U));
    }

    /* Transmit the dynamic address to the target */
    LL_I3C_TransmitData8(MX_I3Cx, target_dynamic_addr);
  }
  else
  {
    /* If Frame Complete flag is set, clear it (no address assigned) */
    if (LL_I3C_IsActiveFlag_FC(MX_I3Cx) != 0U)
    {
      LL_I3C_ClearFlag_FC(MX_I3Cx);
    }
  }

  return return_status;
}


/** brief: Builds the transfer context for a sequence of I3C CCC operations for the MX_I3Cx instance.
  * param: tc_data  Pointer to the control buffer to be filled with transfer descriptors.
  * param: ccc_desc Pointer to an array of 8 CCC descriptor structures (see i3c_ccc_desc_t).
  */
inline void I3C_BuildCCCTransferContext(uint32_t *p_tc_data, const i3c_ccc_desc_t *ccc_desc)
{
  const uint32_t nb_define_bytes = ((uint32_t)LL_I3C_CONTROLLER_MTYPE_DIRECT & (uint32_t)LL_I3C_DEFINE_BYTE);

  for (uint32_t i = 0U; i < I3C_DESC_OPERATION_NUM; i++)
  {
    uint32_t stop_condition = 0U;

    if (i == (I3C_DESC_OPERATION_NUM - 1U))
    {
      stop_condition = (uint32_t)LL_I3C_GENERATE_STOP;
    }

    /* Fill control buffer with CCC command and target address */
    p_tc_data[2U * i] = nb_define_bytes | ((uint32_t)ccc_desc[i].ccc << I3C_CR_CCC_Pos)
                        | (uint32_t)LL_I3C_CONTROLLER_MTYPE_CCC | (uint32_t)LL_I3C_GENERATE_RESTART;
    p_tc_data[(2U * i) + 1U] = (ccc_desc[i].data_size_byte - nb_define_bytes) | ccc_desc[i].direction |
                               ((uint32_t)ccc_desc[i].target_addr << I3C_CR_ADD_Pos)
                               | (uint32_t)LL_I3C_CONTROLLER_MTYPE_DIRECT | stop_condition;
  }
}


/** brief: Executes an I3C transfer using the provided transfer context in polling mode for the MX_I3Cx instance.
  * param:  p_tc_data     Pointer to the control buffer describing the transfer sequence.
  * param:  tc_size_word  Number of control words in the transfer context buffer.
  * param:  p_tx_data     Pointer to the transmit payload buffer.
  * param:  tx_size_byte  Number of bytes to transmit.
  * param:  rx_data       Pointer to the receive buffer.
  * param:  rx_size_byte  Number of bytes to receive.
  * retval: SYSTEM_OK               Assignment success.
  * retval: SYSTEM_PERIPHERAL_ERROR Error during Transfer.
  */
inline void I3C_Transfer(const uint32_t *p_tc_data,
                         uint32_t tc_size_word,
                         const uint8_t *p_tx_data,
                         uint32_t tx_size_byte,
                         uint8_t *p_rx_data,
                         uint32_t rx_size_byte)
{
  uint32_t exit_condition;

  /* Prepare MX_I3Cx TX and RX buffers before starting the transfer */
  I3C_TransmitByteTreatment(&p_tx_data, &tx_size_byte);
  I3C_ReceiveByteTreatment(&p_rx_data, &rx_size_byte);

  /* Write the first control word to the MX_I3Cx control register */
  LL_I3C_WRITE_REG(MX_I3Cx, CR, *p_tc_data);
  p_tc_data++;
  tc_size_word--;

  do
  {
    /* Handle control, TX, and RX data for each frame */
    I3C_ControlDataTreatment(&p_tc_data, &tc_size_word);

    I3C_TransmitByteTreatment(&p_tx_data, &tx_size_byte);

    I3C_ReceiveByteTreatment(&p_rx_data, &rx_size_byte);

    /* If frame complete and more control words remain, clear flag and request next transfer */
    if ((LL_I3C_IsActiveFlag_FC(MX_I3Cx) != 0U) && (tc_size_word > 0UL))
    {
      LL_I3C_ClearFlag_FC(MX_I3Cx);
      LL_I3C_RequestTransfer(MX_I3Cx);
    }

    /* Check exit condition: frame complete or error */
    exit_condition = (LL_I3C_READ_REG(MX_I3Cx, EVR) & (I3C_EVR_FCF | I3C_EVR_ERRF));
  } while ((exit_condition == 0U) || ((exit_condition == I3C_EVR_FCF) && (tc_size_word > 0UL)));

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (LL_I3C_IsActiveFlag_ERR(MX_I3Cx) != 0U)
  {
    LL_I3C_ClearFlag_ERR(MX_I3Cx);
  }
#endif /* USE_LL_APP_ERROR */

}


/** brief: Handles MX_I3Cx control data treatment (Control FIFO).
  * param: tc_data       Pointer to pointer to current control word in the buffer.
  * param: tc_size_word  Pointer to the number of remaining control words.
  */
static void I3C_ControlDataTreatment(const uint32_t **p_tc_data, uint32_t *p_tc_size_word)
{

  /* If Control FIFO requests data, write the next control word */
  if ((LL_I3C_IsActiveFlag_CFNF(MX_I3Cx) != 0U) && (*p_tc_size_word > 0U))
  {
    LL_I3C_WRITE_REG(MX_I3Cx, CR, **p_tc_data);
    (*p_tc_data)++;
    (*p_tc_size_word)--;
  }
}


/** brief: Handles MX_I3Cx receive byte treatment.
  * param:  p_rx_data      Pointer to pointer to current position in the receive buffer.
  * param : p_rx_size_byte Pointer to the number of remaining bytes to receive.
  */
static void I3C_ReceiveByteTreatment(uint8_t **p_rx_data, uint32_t *p_rx_size_byte)
{

  /* While RX FIFO is not empty, read received bytes into the buffer */
  while ((LL_I3C_IsActiveFlag_RXFNE(MX_I3Cx) != 0U) && (*p_rx_size_byte > 0U))
  {
    **p_rx_data = LL_I3C_ReceiveData8(MX_I3Cx);
    (*p_rx_data)++;
    (*p_rx_size_byte)--;
  }
}


/** brief: Handles MX_I3Cx transmit byte treatment.
  * param:  p_tx_data       Pointer to pointer to current position in the transmit buffer.
  * param:  p_tx_size_byte  Pointer to the number of remaining bytes to transmit.
  */
static void I3C_TransmitByteTreatment(const uint8_t **p_tx_data, uint32_t *p_tx_size_byte)
{

  /* While TX FIFO is not full and data remains, transmit bytes from the buffer */
  while ((LL_I3C_IsActiveFlag_TXFNF(MX_I3Cx) != 0U) && (*p_tx_size_byte > 0UL))
  {
    LL_I3C_TransmitData8(MX_I3Cx, **p_tx_data);
    (*p_tx_data)++;
    (*p_tx_size_byte)--;
  }
}

/**
  * brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
inline void DelayMs(uint32_t delay_ms)
{
  LL_Delay_NoISR(delay_ms);
}

