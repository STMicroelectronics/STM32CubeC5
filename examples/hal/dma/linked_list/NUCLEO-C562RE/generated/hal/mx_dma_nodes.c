/**
  **********************************************************************************************************************
  * @file  : mx_dma_nodes.c
  * @brief : Provides DMA nodes services
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
#include "mx_dma_nodes.h"

/* Exported variables by reference------------------------------------------------------------------------------------*/
hal_dma_node_t Node1;
hal_dma_node_t Node2;
hal_dma_node_t Node3;

/**********************************************************************************************************************/
/* Exported functions for DMA nodes in HAL layer */
/**********************************************************************************************************************/
/**
  * @brief  Initialize and configure the node1 through HAL layer.
  * @param  Node                Pointer to hal_dma_node_t node structure
  * @param  SrcBuffer           Pointer referencing the source buffer
  * @param  DstBuffer           Pointer referencing the destination buffer
  * @param  BufferSize          buffer size in words
  * @retval HAL_INVALID_PARAM   Invalid parameter return when Node or node_conf pointer is NULL
  * @retval HAL_OK              Node is successfully generated
  */
hal_status_t mx_dma_node1_init(const uint32_t *SrcBuffer, uint32_t *DstBuffer, uint32_t BufferSize)
{
  hal_dma_node_config_t node_conf;
  hal_dma_node_type_t node_type;

  node_type                                      = HAL_DMA_NODE_LINEAR_ADDRESSING;

  node_conf.xfer.request                         = HAL_DMA_REQUEST_SW;
  node_conf.hw_request_mode                      = HAL_DMA_HARDWARE_REQUEST_BURST;
  node_conf.xfer.direction                       = HAL_DMA_DIRECTION_MEMORY_TO_MEMORY;
  node_conf.xfer.src_inc                         = HAL_DMA_SRC_ADDR_INCREMENTED;
  node_conf.xfer.dest_inc                        = HAL_DMA_DEST_ADDR_INCREMENTED;
  node_conf.xfer.src_data_width                  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  node_conf.xfer.dest_data_width                 = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  node_conf.xfer.priority                        = HAL_DMA_PRIORITY_HIGH;
  node_conf.xfer_event_mode                      = HAL_DMA_LINKEDLIST_XFER_EVENT_Q;
  node_conf.trigger.source                       = HAL_LPDMA1_TRIGGER_EXTI0;
  node_conf.trigger.polarity                     = HAL_DMA_TRIGGER_POLARITY_MASKED;
  node_conf.trigger.mode                         = HAL_DMA_TRIGGER_SINGLE_BURST_TRANSFER;
  node_conf.data_handling.trunc_padd             = HAL_DMA_DEST_DATA_TRUNC_LEFT_PADD_ZERO;

  node_conf.src_addr                             = (uint32_t)SrcBuffer;
  node_conf.dest_addr                            = (uint32_t)DstBuffer;
  node_conf.size_byte                            = (BufferSize * 4U);

  if (HAL_DMA_FillNodeConfig(&Node1, &node_conf, node_type) != HAL_OK)
  {
    return HAL_INVALID_PARAM;
  }

  return HAL_OK;
}

/**
  * @brief  Get the dma_node1 object.
  * @retval Pointer on the hal_dma_node_t;
  */
hal_dma_node_t *mx_dma_getnode1(void)
{
  return &Node1;
}

/**
  * @brief  Initialize and configure the node1 through HAL layer.
  * @param  Node                Pointer to hal_dma_node_t node structure
  * @param  SrcBuffer           Pointer referencing the source buffer
  * @param  DstBuffer           Pointer referencing the destination buffer
  * @param  BufferSize          buffer size in words
  * @retval HAL_INVALID_PARAM   Invalid parameter return when Node or node_conf pointer is NULL
  * @retval HAL_OK              Node is successfully generated
  */
hal_status_t mx_dma_node2_init(const uint32_t *SrcBuffer, uint32_t *DstBuffer, uint32_t BufferSize)
{
  hal_dma_node_config_t node_conf;
  hal_dma_node_type_t node_type;

  node_type                                      = HAL_DMA_NODE_LINEAR_ADDRESSING;

  node_conf.xfer.request                         = HAL_DMA_REQUEST_SW;
  node_conf.hw_request_mode                      = HAL_DMA_HARDWARE_REQUEST_BURST;
  node_conf.xfer.direction                       = HAL_DMA_DIRECTION_MEMORY_TO_MEMORY;
  node_conf.xfer.src_inc                         = HAL_DMA_SRC_ADDR_INCREMENTED;
  node_conf.xfer.dest_inc                        = HAL_DMA_DEST_ADDR_INCREMENTED;
  node_conf.xfer.src_data_width                  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  node_conf.xfer.dest_data_width                 = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  node_conf.xfer.priority                        = HAL_DMA_PRIORITY_HIGH;
  node_conf.xfer_event_mode                      = HAL_DMA_LINKEDLIST_XFER_EVENT_Q;
  node_conf.trigger.source                       = HAL_LPDMA1_TRIGGER_EXTI0;
  node_conf.trigger.polarity                     = HAL_DMA_TRIGGER_POLARITY_MASKED;
  node_conf.trigger.mode                         = HAL_DMA_TRIGGER_SINGLE_BURST_TRANSFER;
  node_conf.data_handling.trunc_padd             = HAL_DMA_DEST_DATA_TRUNC_LEFT_PADD_ZERO;

  node_conf.src_addr                             = (uint32_t)SrcBuffer;
  node_conf.dest_addr                            = (uint32_t)DstBuffer;
  node_conf.size_byte                            = (BufferSize * 4U);

  if (HAL_DMA_FillNodeConfig(&Node2, &node_conf, node_type) != HAL_OK)
  {
    return HAL_INVALID_PARAM;
  }

  return HAL_OK;
}

/**
  * @brief  Get the dma_node2 object.
  * @retval Pointer on the hal_dma_node_t object
  */
hal_dma_node_t *mx_dma_getnode2(void)
{
  return &Node2;
}

/**
  * @brief  Initialize and configure the node1 through HAL layer.
  * @param  Node                Pointer to hal_dma_node_t node structure
  * @param  SrcBuffer           Pointer referencing the source buffer
  * @param  DstBuffer           Pointer referencing the destination buffer
  * @param  BufferSize          buffer size in words
  * @retval HAL_INVALID_PARAM   Invalid parameter return when Node or node_conf pointer is NULL
  * @retval HAL_OK              Node is successfully generated
  */
hal_status_t mx_dma_node3_init(const uint32_t *SrcBuffer, uint32_t *DstBuffer, uint32_t BufferSize)
{
  hal_dma_node_config_t node_conf;
  hal_dma_node_type_t node_type;

  node_type                                      = HAL_DMA_NODE_LINEAR_ADDRESSING;

  node_conf.xfer.request                         = HAL_DMA_REQUEST_SW;
  node_conf.hw_request_mode                      = HAL_DMA_HARDWARE_REQUEST_BURST;
  node_conf.xfer.direction                       = HAL_DMA_DIRECTION_MEMORY_TO_MEMORY;
  node_conf.xfer.src_inc                         = HAL_DMA_SRC_ADDR_INCREMENTED;
  node_conf.xfer.dest_inc                        = HAL_DMA_DEST_ADDR_INCREMENTED;
  node_conf.xfer.src_data_width                  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  node_conf.xfer.dest_data_width                 = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  node_conf.xfer.priority                        = HAL_DMA_PRIORITY_HIGH;
  node_conf.xfer_event_mode                      = HAL_DMA_LINKEDLIST_XFER_EVENT_Q;
  node_conf.trigger.source                       = HAL_LPDMA1_TRIGGER_EXTI0;
  node_conf.trigger.polarity                     = HAL_DMA_TRIGGER_POLARITY_MASKED;
  node_conf.trigger.mode                         = HAL_DMA_TRIGGER_SINGLE_BURST_TRANSFER;
  node_conf.data_handling.trunc_padd             = HAL_DMA_DEST_DATA_TRUNC_LEFT_PADD_ZERO;

  node_conf.src_addr                             = (uint32_t)SrcBuffer;
  node_conf.dest_addr                            = (uint32_t)DstBuffer;
  node_conf.size_byte                            = (BufferSize * 4U);

  if (HAL_DMA_FillNodeConfig(&Node3, &node_conf, node_type) != HAL_OK)
  {
    return HAL_INVALID_PARAM;
  }

  return HAL_OK;
}

/**
  * @brief  Get the dma_node3 object.
  * @retval Pointer on the hal_dma_node_t object
  */
hal_dma_node_t *mx_dma_getnode3(void)
{
  return &Node3;
}

