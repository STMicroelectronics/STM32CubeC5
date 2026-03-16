/**
  ******************************************************************************
  * file           : stm32c5xx_ll_example.c
  * brief          : I3C IBI controller for multi-target example with LL API using interrupt-driven events with LL API.
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

/* Private define ------------------------------------------------------------*/
#define I3C_DAA_IT (LL_I3C_IER_FCIE | LL_I3C_IER_CFNFIE | LL_I3C_IER_TXFNFIE) /* Controller DAA */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
void mx_i3cx_ev_irqhandler(void);
void mx_i3cx_err_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/** brief:  Assign target dynamic address in interrupt mode for the MX_I3Cx instance.
  */
inline void I3C_AssignDynamicAddr_IT(void)
{
  LL_I3C_EnableIT(MX_I3Cx, (I3C_DAA_IT | LL_I3C_IER_ERRIE));

  LL_I3C_EnableArbitrationHeader(MX_I3Cx);

  /* Start ENTDAA CCC (Dynamic Address Assignment). */
  LL_I3C_ControllerHandleCCC(MX_I3Cx, LL_I3C_BROADCAST_ENTDAA, 0UL, LL_I3C_GENERATE_STOP);
}


/** brief:  Controller set dynamic address.
  * param:  target_address Target dynamic address.
  */
inline void I3C_SetDynamicAddr(uint64_t target_address)
{
  LL_I3C_TransmitData8(MX_I3Cx, (uint8_t)target_address);
}


/** brief: Configures the bus device entries in the controller for the MX_I3Cx instance.
  * param: p_desc    Pointer to the devices descriptor.
  * param: device_nb Number of devices.
  */
inline void I3C_ConfigureBusDevices(const target_desc_t *p_desc, uint32_t device_nb)
{
  for (uint32_t index = 0U; index < device_nb; index++)
  {
    /* Configure DEVRx entry for IBI reception. */
    LL_I3C_SetDevrx(MX_I3Cx,
                    p_desc[index].target_id,
                    p_desc[index].dynamic_addr,
                    LL_I3C_GET_IBI_CAPABLE(LL_I3C_GET_BCR(p_desc[index].target_bcr_dcr_pid)),
                    LL_I3C_GET_IBI_PAYLOAD(LL_I3C_GET_BCR(p_desc[index].target_bcr_dcr_pid)),
                    LL_I3C_SUSP_DISABLE,
                    LL_I3C_GET_CR_CAPABLE(LL_I3C_GET_BCR(p_desc[index].target_bcr_dcr_pid)));
  }
}


/** brief: Activates notification of the MX_I3Cx instance.
  */
inline void I3C_ActivateNotification(void)
{
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I3C_EnableIT(MX_I3Cx, (LL_I3C_IER_IBIIE | LL_I3C_IER_ERRIE));
#else
  LL_I3C_EnableIT(MX_I3Cx, (LL_I3C_IER_IBIIE));
#endif /* USE_LL_APP_ERROR */
}


/** brief:  Gets the number of payload bytes received after IBI on the MX_I3Cx instance.
  * retval: Number of IBI payload bytes.
  */
inline uint32_t I3C_GetIBIPayloadByteCount(void)
{
  return (LL_I3C_GetNbIBIAddData(MX_I3Cx));
}


/** brief:  Gets the IBI payload value received by the MX_I3Cx controller.
  * retval: IBI payload value.
  */
inline uint32_t I3C_GetIBIPayload(void)
{
  return LL_I3C_GetIBIPayload(MX_I3Cx);
}


/** brief: Handles the MX_I3Cx event interrupts.
  */
void mx_i3cx_ev_irqhandler(void)
{
  /* DAA (ENTDAA) events. */
  if (LL_I3C_IsActiveMaskFlag_TXFNF(MX_I3Cx) != 0U)
  {
    uint64_t target_payload = 0UL;

    /* Read 8 bytes of target payload (PID/BCR/DCR) from RX FIFO. */
    for (uint32_t index = 0U; index < 8U; index++)
    {
      target_payload |= ((uint64_t)LL_I3C_ReceiveData8(MX_I3Cx) << (index * 8U));
    }

    I3C_TargetReqDynamicAddrCallback(target_payload);
  }

  if (LL_I3C_IsActiveMaskFlag_FC(MX_I3Cx) != 0U)
  {
    LL_I3C_DisableIT(MX_I3Cx, I3C_DAA_IT);

    LL_I3C_ClearFlag_FC(MX_I3Cx);

    I3C_DAACompleteCallback();
  }

  /* IBI notification event. */
  if (LL_I3C_IsActiveMaskFlag_IBI(MX_I3Cx) != 0U)
  {
    LL_I3C_ClearFlag_IBI(MX_I3Cx);

    I3C_NotificationCallback();
  }
}


/** brief: Handles the MX_I3Cx error interrupts.
  */
void mx_i3cx_err_irqhandler(void)
{
  LL_I3C_ClearFlag_ERR(MX_I3Cx);

  I3C_ErrorCallback();
}
