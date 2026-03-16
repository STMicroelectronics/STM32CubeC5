/**
  **********************************************************************************************************************
  * @file  : mx_queue.c
  * @brief : Provides QUEUE services
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in the root directory of this software
  * component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */
/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "mx_queue.h"
#include "mx_dma_nodes.h"

/* Exported variables by reference------------------------------------------------------------------------------------*/
hal_q_t DmaQueue;

/**********************************************************************************************************************/
/* Exported functions for QUEUE in HAL layer */
/**********************************************************************************************************************/

/**
  * @brief Queue Initialization Function
  * @param None
  * @retval Pointer on the QUEUE structure, NULL in case of error
  */
hal_q_t *mx_queue1_init(void)
{
  if(HAL_Q_Init(&DmaQueue, &HAL_DMA_LinearAddressing_DescOps) != HAL_OK)
  {
    return NULL;
  }

  return &DmaQueue;
}

/**
  * @brief  DeInitialize the DmaQueue through HAL layer.
  */
void mx_queue1_deinit(void)
{
  (void)HAL_Q_DeInit(&DmaQueue);
}

/**
  * @brief  Build the DmaQueue through HAL layer.
  */
hal_q_t *mx_queue1_build(void)
{
  /* Connect node1 to Queue 1 */
  if(HAL_Q_InsertNode_Tail(&DmaQueue, mx_dma_getnode1()) != HAL_OK)
  {
    return NULL;
  }

  /* Connect node2 to Queue 1 */
  if(HAL_Q_InsertNode_Tail(&DmaQueue, mx_dma_getnode2()) != HAL_OK)
  {
    return NULL;
  }

  /* Connect node3 to Queue 1 */
  if(HAL_Q_InsertNode_Tail(&DmaQueue, mx_dma_getnode3()) != HAL_OK)
  {
    return NULL;
  }

  return &DmaQueue;
}
