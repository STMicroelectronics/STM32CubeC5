/**
  **********************************************************************************************************************
  * @file           : mx_dma_nodes.h
  * @brief          : Header for mx_dma_nodes.c file.
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *********************************************************************************************************************
  */

/* Define to prevent recursive inclusion ----------------------------------------------------------------------------*/
#ifndef MX_DMA_NODES_H
#define MX_DMA_NODES_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ---------------------------------------------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported functions ---------------------------------------------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for DMA nodes in HAL layer */
/******************************************************************************/
/**
  * @brief  Initialize and configure the node1 through HAL layer.
  * @param  Node                Pointer to hal_dma_node_t node structure
  * @param  SrcBuffer           Pointer referencing the source buffer
  * @param  DstBuffer           Pointer referencing the destination buffer
  * @param  BufferSize          buffer size in words
  * @retval HAL_INVALID_PARAM   Invalid parameter return when Node or node_conf pointer  is NULL
  * @retval HAL_OK              Node is successfully generated
  */
hal_status_t mx_dma_node1_init(const uint32_t *SrcBuffer, uint32_t *DstBuffer, uint32_t BufferSize);

/**
  * @brief  Get the dma_node1 object.
  * @retval Pointer on the hal_dma_node_t object
  */
hal_dma_node_t *mx_dma_getnode1(void);

/**
  * @brief  Initialize and configure the node1 through HAL layer.
  * @param  Node                Pointer to hal_dma_node_t node structure
  * @param  SrcBuffer           Pointer referencing the source buffer
  * @param  DstBuffer           Pointer referencing the destination buffer
  * @param  BufferSize          buffer size in words
  * @retval HAL_INVALID_PARAM   Invalid parameter return when Node or node_conf pointer  is NULL
  * @retval HAL_OK              Node is successfully generated
  */
hal_status_t mx_dma_node2_init(const uint32_t *SrcBuffer, uint32_t *DstBuffer, uint32_t BufferSize);

/**
  * @brief  Get the dma_node2 object.
  * @retval Pointer on the hal_dma_node_t object
  */
hal_dma_node_t *mx_dma_getnode2(void);

/**
  * @brief  Initialize and configure the node1 through HAL layer.
  * @param  Node                Pointer to hal_dma_node_t node structure
  * @param  SrcBuffer           Pointer referencing the source buffer
  * @param  DstBuffer           Pointer referencing the destination buffer
  * @param  BufferSize          buffer size in words
  * @retval HAL_INVALID_PARAM   Invalid parameter return when Node or node_conf pointer  is NULL
  * @retval HAL_OK              Node is successfully generated
  */
hal_status_t mx_dma_node3_init(const uint32_t *SrcBuffer, uint32_t *DstBuffer, uint32_t BufferSize);

/**
  * @brief  Get the dma_node3 object.
  * @retval Pointer on the hal_dma_node_t object
  */
hal_dma_node_t *mx_dma_getnode3(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_DMA_NODES_H */
