/**
  ******************************************************************************
  * file           : example.c
  * brief          : Ethernet send/receive example using HAL API
  *                  Initializes PHY + MAC, configures DMA Tx/Rx channels,
  *                  periodically broadcasts a SYNC frame, and handles
  *                  received frames with a static buffer pool and FIFO.
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
#include "mx_system.h"
#include "mx_lan8742.h"
#include "string.h" /* For memset() */
#include <stm32_hal.h>

/* Private defines -----------------------------------------------------------*/
#define ETH_TRANSMIT_INTERVAL_MS      100 /* Period (ms) between broadcast SYNC frame transmissions. */
#define ETH_LINK_CHECK_INTERVAL_MS    10000 /* Period (ms) between link status checks.*/
#define POOL_BUFF_SIZE_BYTE           1518 /* Size in bytes of each buffer in the application memory pool.*/
#define POOL_SIZE                     50U /* Number of buffers available in the application memory pool. */
#define ETH_HEADER_LEN_BYTE           14U /* Ethernet header length in bytes */
#define ETH_PKT_ETHER_TYPE            (0x88B5)
#define TX_SYNC_SEQ_OFFSET            19U
#define MAX_RECEIVED_TAGS_CNT         10U
#define ETH_MEM_SIZE_SIZE_BYTE        240U /* Region size for HAL ETH */

/* Private typedefs ----------------------------------------------------------*/
/* Application memory pool element.  */
typedef struct mem_pool_s
{
  uint32_t       packet_size;                /* Size of the received frame */
  uint8_t        data[POOL_BUFF_SIZE_BYTE];  /* Buffer to store Ethernet frame */
  struct mem_pool_s *p_next_free;            /* Pointer for free list */
} mem_pool_t;

/* FIFO structure for received buffer tags. */
typedef struct
{
  void *p_data_buff[MAX_RECEIVED_TAGS_CNT];     /**< Pointer array to memory pool entries */
  size_t head;                                  /**< Next position to write (enqueue) */
  size_t tail;                                  /**< Next position to read (dequeue) */
  size_t count;                                 /**< Current number of stored entries */
} app_buff_fifo_t;

app_buff_fifo_t  app_buff_fifo;
mem_pool_t *p_free_list = NULL;

/* Driver memory areas aligned to ETH bus width for DMA descriptors */
#if defined(__ICCARM__)
#pragma data_alignment=ETH_BUS_DATA_WIDTH_BYTE
uint32_t tx_drv_mem[ETH_MEM_SIZE_SIZE_BYTE / sizeof(uint32_t)];
#pragma data_alignment=ETH_BUS_DATA_WIDTH_BYTE
uint32_t rx_drv_mem[ETH_MEM_SIZE_SIZE_BYTE / sizeof(uint32_t)];
#pragma data_alignment=ETH_BUS_DATA_WIDTH_BYTE
mem_pool_t mem_pool[POOL_SIZE];
#elif defined(__GNUC__) || defined(__ARMCC_VERSION)
__attribute__((aligned(ETH_BUS_DATA_WIDTH_BYTE))) uint32_t tx_drv_mem[ETH_MEM_SIZE_SIZE_BYTE / sizeof(uint32_t)];
__attribute__((aligned(ETH_BUS_DATA_WIDTH_BYTE))) uint32_t rx_drv_mem[ETH_MEM_SIZE_SIZE_BYTE / sizeof(uint32_t)];
__attribute__((aligned(ETH_BUS_DATA_WIDTH_BYTE))) mem_pool_t mem_pool[POOL_SIZE];
#else /*!__ICCARM__ and !__GNUC__ and !__ARMCC_VERSION */
uint32_t tx_drv_mem[ETH_MEM_SIZE_SIZE_BYTE / sizeof(uint32_t)];
uint32_t rx_drv_mem[ETH_MEM_SIZE_SIZE_BYTE / sizeof(uint32_t)];
mem_pool_t mem_pool[POOL_SIZE];
#endif  /* __ICCARM__ */

/* Consolidated Ethernet channel runtime configuration. */
typedef struct
{
  hal_eth_handle_t              *p_eth_handler;        /**< Pointer to low-level HAL ETH handle */
  hal_eth_config_t              eth_config;            /**< (Reserved) Global Ethernet config if needed */
  uint32_t                      tx_channel_id;         /**< Active Tx channel identifier */
  hal_eth_tx_pkt_config_t       tx_pkt_conf;           /**< Per-packet Tx configuration */
  hal_eth_tx_channel_config_t   tx_channel_config;     /**< Tx channel configuration snapshot */
  uint32_t                      tx_tickRef;            /**< Reference tick for periodic Tx scheduling */
  uint32_t                      rx_channel_id;         /**< Active Rx channel identifier */
  hal_eth_rx_channel_config_t   rx_channel_config;     /**< Rx channel configuration snapshot */
  uint32_t                      input_ch_mask;         /**< (input) Bitmask of channels for data handler execution.*/
  uint32_t                      output_ch_mask;        /**< (output) Bitmask of channels for data handler execution.*/
  volatile uint32_t             data_event;            /**< Data event flag set in data callback */
  uint32_t                      link_check_tickRef;    /**< Reference tick for link status checks */

} app_config_t;

/* Private variables ---------------------------------------------------------*/

/* Predefined transmission frame buffer with broadcast MAC and payload "SYNC:00" */
uint8_t tx_frame_packet[] =
{
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* Destination MAC address (broadcast) */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* Source MAC address (to be set) */
  0x88, 0xB5,                         /* Protocol identifier */
  'S', 'Y', 'N', 'C', ':', '0', '0'   /* Payload data */
};

app_config_t   channel_0_config = {0}; /* Runtime configuration instance for channel 0. */

/** Snapshot of previously observed PHY link mode. */
lan8742_link_t prev_link_mode = { LAN8742_LINK_DOWN, LAN8742_LINK_SPEED_NONE, LAN8742_LINK_DUPLEX_NONE };

/* Private function prototypes -----------------------------------------------*/
static void EthLinkCheck(void);
static void MemPoolInit(void);
static mem_pool_t *MemPoolAlloc(void);
static void MemPoolFree(mem_pool_t *p_mem_addr);
static void AppBufferFifoInit(app_buff_fifo_t *p_app_buff_fifo);
static mem_pool_t *AppBufferFifoGet(app_buff_fifo_t *p_app_buff_fifo);
static int32_t AppBufferFifoPut(app_buff_fifo_t *p_app_buff_fifo, void *p_app_buff);
static void IncrFrameId(char *p_frameIndex);
static app_status_t TransmitSyncMsg(app_config_t *tx_config);
static void ReceiveEthPkt(void);
hal_status_t tx_channel_callback(hal_eth_handle_t *heth, uint32_t channel, void *addr,
                                 hal_eth_tx_cb_pkt_data_t meta_data);
hal_status_t rx_channel_callback(hal_eth_handle_t *heth, uint32_t channel, void *addr, uint32_t size,
                                 hal_eth_rx_cb_pkt_data_t meta_data);
void data_callback(hal_eth_handle_t *heth, uint32_t channel);
void rx_allocate_callback(hal_eth_handle_t *heth, uint32_t channel, uint32_t buff_size, void **p_buff_add,
                          void **app_context);

/* Public functions ----------------------------------------------------------*/

/**
  *  Initialize Ethernet peripheral and related interrupts.
  *  Registers callbacks for transmission and reception events.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  MemPoolInit();
  AppBufferFifoInit(&app_buff_fifo);
  channel_0_config.tx_tickRef = 0;

  /* Get ETH handler */
  channel_0_config.p_eth_handler = (hal_eth_handle_t *)mx_eth1_hal_gethandle();

  /* Init ETH Tx channel config */
  channel_0_config.tx_pkt_conf.attributes = HAL_ETH_TX_PKT_CTRL_CSUM | HAL_ETH_TX_PKT_CTRL_CRCPAD
                                            | HAL_ETH_TX_PKT_CTRL_SAIC;
  channel_0_config.tx_pkt_conf.crc_pad_ctrl = HAL_ETH_TX_PKT_CRC_PAD_INSERT;
  channel_0_config.tx_pkt_conf.csum_ctrl = HAL_ETH_TX_PKT_CSUM_PAYLOAD_HEADER_INSERT;
  channel_0_config.tx_pkt_conf.src_addr_ctrl = HAL_ETH_TX_PKT_SRC_ADDR_REPLACE;

  /* Initialize LAN8742 PHY driver */
  lan8742_init(mx_lan8742_getObj(), MX_LAN8742_0);


  /* register data callbacks */
  HAL_ETH_RegisterDataCallback(channel_0_config.p_eth_handler, data_callback);

  /* ============================ Tx Channel 0 Configuration =======================*/
  channel_0_config.tx_channel_id = HAL_ETH_TX_CHANNEL_0;
  HAL_ETH_GetConfigTxChannel(channel_0_config.p_eth_handler, channel_0_config.tx_channel_id,
                             &channel_0_config.tx_channel_config);
  channel_0_config.tx_channel_config.max_app_buffers_num = 10;
  channel_0_config.tx_channel_config.fifo_event_config.event_mode = HAL_ETH_FIFO_EVENT_ALWAYS;

  /* set Tx config */
  HAL_ETH_SetConfigTxChannel(channel_0_config.p_eth_handler, channel_0_config.tx_channel_id,
                             &channel_0_config.tx_channel_config);

  /* register Tx complete callback */
  HAL_ETH_RegisterChannelTxCptCallback(channel_0_config.p_eth_handler, channel_0_config.tx_channel_id,
                                       tx_channel_callback);
  /* ============================ Rx Channel 0 Configuration =======================*/
  channel_0_config.rx_channel_id = HAL_ETH_RX_CHANNEL_0;
  HAL_ETH_GetConfigRxChannel(channel_0_config.p_eth_handler, channel_0_config.rx_channel_id,
                             &channel_0_config.rx_channel_config);
  channel_0_config.rx_channel_config.max_app_buffers_num = 10;
  channel_0_config.rx_channel_config.fifo_event_config.event_mode = HAL_ETH_FIFO_EVENT_ALWAYS;

  /* start Tx config */
  HAL_ETH_SetConfigRxChannel(channel_0_config.p_eth_handler, channel_0_config.rx_channel_id,
                             &channel_0_config.rx_channel_config);

  /* register Rx complete callback */
  HAL_ETH_RegisterChannelRxCptCallback(channel_0_config.p_eth_handler, channel_0_config.rx_channel_id,
                                       rx_channel_callback);
  HAL_ETH_RegisterChannelRxAllocateCallback(channel_0_config.p_eth_handler, channel_0_config.rx_channel_id,
                                            rx_allocate_callback);
  /* ============================ Check and configure Ethernet link state  =======================*/
  EthLinkCheck();
  /* ============================ Start channels =======================*/
  /* start Tx channel */
  HAL_ETH_StartChannel(channel_0_config.p_eth_handler, channel_0_config.tx_channel_id, tx_drv_mem,
                       ETH_MEM_SIZE_SIZE_BYTE);
  /* start Rx channel */
  HAL_ETH_StartChannel(channel_0_config.p_eth_handler, channel_0_config.rx_channel_id, rx_drv_mem,
                       ETH_MEM_SIZE_SIZE_BYTE);

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");

  return return_status;
}

/**
  * Main processing loop.
  * Periodically sends a SYNC frame and processes received Ethernet frames.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;
  uint32_t curr_tick_value =  HAL_GetTick();

  /* ########## Step 2 : Periodic broadcast SYNC frame scheduling #######################################*/
  if ((curr_tick_value - channel_0_config.tx_tickRef) > ETH_TRANSMIT_INTERVAL_MS)
  {
    if (TransmitSyncMsg(&channel_0_config) != EXEC_STATUS_OK)
    {
      PRINTF("[ERROR] Transmit SYNC error\n");
    }
    channel_0_config.tx_tickRef = curr_tick_value;
  }

  /* ########## Step 3 : Process any received frame tags (non-blocking) ##################################*/
  ReceiveEthPkt();

  /* Data event flag set in ISR; execute HAL data handler for pending channels */
  if (channel_0_config.data_event)
  {
    channel_0_config.data_event = 0;
    channel_0_config.output_ch_mask = 0;
    channel_0_config.input_ch_mask = (channel_0_config.tx_channel_id | channel_0_config.rx_channel_id);
    HAL_ETH_ExecDataHandler(channel_0_config.p_eth_handler, channel_0_config.input_ch_mask,
                            &channel_0_config.output_ch_mask);
  }

  /* ########## Step 4 : Periodic PHY link supervision ############################################### */
  if ((curr_tick_value - channel_0_config.link_check_tickRef) > ETH_LINK_CHECK_INTERVAL_MS)
  {
    EthLinkCheck();
    channel_0_config.link_check_tickRef = curr_tick_value;
  }
  return return_status;
}
/**
  * Deinitialize the application (not used in this example).
  */
app_status_t app_deinit(void)
{
  /* This function is not used in this example and is optimized out */
  return EXEC_STATUS_ERROR;
}
/* rx allocate call back */
void rx_allocate_callback(hal_eth_handle_t *heth, uint32_t channel, uint32_t buff_size, void **p_buff_add,
                          void **app_context)
{
  mem_pool_t *rx_buff = MemPoolAlloc();
  *p_buff_add = NULL;
  *app_context = NULL;

  if (rx_buff != NULL)
  {
    *p_buff_add = rx_buff->data;
    *app_context = rx_buff;
  }
}
/* tx complete call back */
hal_status_t tx_channel_callback(hal_eth_handle_t *heth, uint32_t channel, void *addr,
                                 hal_eth_tx_cb_pkt_data_t meta_data)
{
  if (meta_data.p_data != NULL)
  {
    MemPoolFree((mem_pool_t *) meta_data.p_data);
  }
  return HAL_OK;
}
/* data event callback */
void data_callback(hal_eth_handle_t *heth, uint32_t channel)
{
  channel_0_config.data_event = 1;
}

/* Rx channel completion callback  */
hal_status_t rx_channel_callback(hal_eth_handle_t *heth, uint32_t channel, void *addr, uint32_t size,
                                 hal_eth_rx_cb_pkt_data_t meta_data)
{
  /* meta_data.p_data was set in rx_allocate_callback as the mem_pool_t context */
  mem_pool_t *p_rec_buff = (mem_pool_t *)(meta_data.p_data);
  p_rec_buff->packet_size = size; /* Record frame length for later validation */

  /* Enqueue buffer for deferred processing; avoid heavy work in ISR */
  if (AppBufferFifoPut(&app_buff_fifo, (void *)p_rec_buff) != 0)
  {
    return HAL_ERROR; /* FIFO full or pointer invalid */
  }
  return HAL_OK;
}
/* Private functions ----------------------------------------------------------*/
/* Check link status and apply MAC speed/duplex changes when detected. */
static void EthLinkCheck(void)
{
  lan8742_obj_t *pLan8742Obj = NULL;

  /* Retrieve and store the lan8742 object pointer */
  pLan8742Obj = mx_lan8742_getObj();
  lan8742_link_t link_mode;

  /* Get current PHY link state */
  lan8742_get_link_mode(pLan8742Obj, &link_mode);

  if ((link_mode.status == LAN8742_LINK_UP) && ((prev_link_mode.speed != link_mode.speed)
                                                || (prev_link_mode.duplex != link_mode.duplex)))
  {
    hal_eth_handle_t *p_eth_handle = (hal_eth_handle_t *)mx_eth1_hal_gethandle();
    hal_eth_link_config_t eth_link_config;

    /* Record new link updates */
    prev_link_mode.status = link_mode.status;
    prev_link_mode.speed = link_mode.speed;
    prev_link_mode.duplex = link_mode.duplex;

    /* Handle link event */
    if ((link_mode.speed == LAN8742_LINK_SPEED_100) && (link_mode.duplex == LAN8742_LINK_FULL_DUPLEX))
    {
      eth_link_config.duplex_mode = HAL_ETH_MAC_FULL_DUPLEX_MODE;
      eth_link_config.speed = HAL_ETH_MAC_SPEED_100M;
    }
    else if ((link_mode.speed == LAN8742_LINK_SPEED_100) && (link_mode.duplex == LAN8742_LINK_HALF_DUPLEX))
    {
      eth_link_config.duplex_mode = HAL_ETH_MAC_HALF_DUPLEX_MODE;
      eth_link_config.speed = HAL_ETH_MAC_SPEED_100M;
    }
    else if ((link_mode.speed == LAN8742_LINK_SPEED_10) && (link_mode.duplex == LAN8742_LINK_FULL_DUPLEX))
    {
      eth_link_config.duplex_mode = HAL_ETH_MAC_FULL_DUPLEX_MODE;
      eth_link_config.speed = HAL_ETH_MAC_SPEED_10M;
    }
    else if ((link_mode.speed == LAN8742_LINK_SPEED_10) && (link_mode.duplex == LAN8742_LINK_HALF_DUPLEX))
    {
      eth_link_config.duplex_mode = HAL_ETH_MAC_HALF_DUPLEX_MODE;
      eth_link_config.speed = HAL_ETH_MAC_SPEED_10M;
    }
  else
  {
      eth_link_config.duplex_mode = HAL_ETH_MAC_FULL_DUPLEX_MODE;
      eth_link_config.speed = HAL_ETH_MAC_SPEED_100M;
  }

    /* Update current ETH Link configuration */
    HAL_ETH_UpdateConfigLink(p_eth_handle, &eth_link_config);
  }
}
/* Initialize the free list (call once at startup) */
static void MemPoolInit(void)
{
  uint32_t index = 0;
  while (index < (POOL_SIZE - 1))
  {
    /* Link current node to the next sequential node */
    mem_pool[index].p_next_free = &mem_pool[index + 1];
    /* Clear any residual size info to start clean */
    mem_pool[index].packet_size = 0;
    index++;
  }
  /* Terminate list: last element's next is NULL indicating end of free chain */
  mem_pool[POOL_SIZE - 1].p_next_free = NULL;
  mem_pool[POOL_SIZE - 1].packet_size = 0;
  /* Initialize free list head to first element for subsequent MemPoolAlloc() calls */
  p_free_list = &mem_pool[0];
}

/* Release a memory pool buffer back to free list.*/
static void MemPoolFree(mem_pool_t *p_mem_addr)
{
  if (p_mem_addr != NULL)
  {
    p_mem_addr->p_next_free = p_free_list;
    p_free_list = p_mem_addr;
    p_mem_addr->packet_size = 0;
  }
}
/* Acquire a free memory pool buffer.*/
static mem_pool_t *MemPoolAlloc(void)
{
  if (p_free_list == NULL)
  {
    return NULL;
  }
  mem_pool_t *pkt = p_free_list;
  p_free_list = pkt->p_next_free;
  pkt->p_next_free = NULL;
  pkt->packet_size = 0;
  return pkt;
}
/* Transmit a SYNC frame with incremented sequence ID.*/
static app_status_t TransmitSyncMsg(app_config_t *tx_config)
{
  /* Single contiguous buffer descriptor: small control frame, no chaining needed */
  hal_eth_buffer_t hal_eth_buffer[1];
  /* Acquire deterministic static pool buffer */
  mem_pool_t *tx_buff = MemPoolAlloc();
  if (tx_buff == NULL)
  {
    /* Pool exhausted: caller will retry on next periodic tick */
    PRINTF("[ERROR] Transmit buffer not available\n");
    return EXEC_STATUS_ERROR;
  }

  /* Update ASCII sequence ("SYNC:nn") before frame copy to embed incremented counter */
  IncrFrameId((char *)(tx_frame_packet + TX_SYNC_SEQ_OFFSET));
  /* Copy fixed template frame into DMA-able buffer */
  memcpy(tx_buff->data, tx_frame_packet, sizeof(tx_frame_packet));
  tx_buff->packet_size = sizeof(tx_frame_packet);
  /* Populate HAL descriptor and attach application context for Tx completion callback */
  hal_eth_buffer[0].p_buffer = tx_buff->data;
  hal_eth_buffer[0].len_byte = tx_buff->packet_size;
  tx_config->tx_pkt_conf.p_data = (void *) tx_buff; /* freed in tx_channel_callback */

  /* Asynchronous transmit request; HAL handles CRC/pad/checksum per configured attributes */
  if (HAL_ETH_RequestTx(tx_config->p_eth_handler, tx_config->tx_channel_id, hal_eth_buffer, 1, &tx_config->tx_pkt_conf)
      != HAL_OK)
  {
    /* Immediate rejection (e.g., descriptor busy); buffer retained for future retry scenario */
    return EXEC_STATUS_ERROR;
  }
  return EXEC_STATUS_OK; /* Accepted; completion callback will recycle buffer */
}
/* Increment two ASCII digits forming a frame sequence counter. */
static void IncrFrameId(char *p_frameIndex)
{
  /* Defensive: ensure both chars are digits; if not, reset to "00" */
  if ((p_frameIndex[0] < '0') || (p_frameIndex[0] > '9')
      || (p_frameIndex[1] < '0') || (p_frameIndex[1] > '9'))
  {
    p_frameIndex[0] = '0';
    p_frameIndex[1] = '0';
    return;
  }

  if (p_frameIndex[1] < '9')
  {
    p_frameIndex[1]++;
  }
  else
  {
    p_frameIndex[1] = '0';
    if (p_frameIndex[0] < '9')
    {
      p_frameIndex[0]++;
    }
    else
    {
      p_frameIndex[0] = '0';
    }
  }
}
/* Attempt to dequeue and process a single received packet.*/
static void ReceiveEthPkt(void)
{
  mem_pool_t *p_rec_pkt = AppBufferFifoGet(&app_buff_fifo);
  if (p_rec_pkt != NULL)
  {
    if ((p_rec_pkt->packet_size > ETH_HEADER_LEN_BYTE) && (p_rec_pkt->packet_size < POOL_BUFF_SIZE_BYTE - 1U))
    {
      uint8_t *p_data = p_rec_pkt->data;
      /* Ensure the message is null-terminated for printing */
      p_data[p_rec_pkt->packet_size] = '\0';
      /* Extract EtherType (bytes 12 and 13) for protocol filtering */
      uint16_t eth_type = (p_data[12] << 8) | p_data[13];
      if (eth_type == ETH_PKT_ETHER_TYPE)
      {
        /* Print source MAC and payload message */
        PRINTF("[INFO] Received Frame MAC Addr: %02X:%02X:%02X:%02X:%02X:%02X, message: %s\n", p_data[6], p_data[7],
               p_data[8], p_data[9], p_data[10], p_data[11], &p_data[ETH_HEADER_LEN_BYTE]);
      }
    }
    MemPoolFree(p_rec_pkt);
  }
}
/* Initialize FIFO indices and count. */
static void AppBufferFifoInit(app_buff_fifo_t *p_app_buff_fifo)
{
  p_app_buff_fifo->head = 0;
  p_app_buff_fifo->tail = 0;
  p_app_buff_fifo->count = 0;
}
/* Enqueue a tag pointer into FIFO if space is available. */
static int32_t AppBufferFifoPut(app_buff_fifo_t *p_app_buff_fifo, void *p_app_buff)
{
  /* Reject enqueue if pointer invalid or FIFO full */
  if ((p_app_buff == NULL) || (p_app_buff_fifo->count >= MAX_RECEIVED_TAGS_CNT))
  {
    return -1;
  }
  p_app_buff_fifo->p_data_buff[p_app_buff_fifo->head] = p_app_buff;
  p_app_buff_fifo->head = (p_app_buff_fifo->head + 1) % MAX_RECEIVED_TAGS_CNT;
  p_app_buff_fifo->count++;
  return 0;
}

/* Dequeue one tag pointer from FIFO. */
static mem_pool_t *AppBufferFifoGet(app_buff_fifo_t *p_app_buff_fifo)
{
  mem_pool_t *result = NULL;
  if (p_app_buff_fifo->count == 0)
  {
    return NULL; /* Buffer empty */
  }

  result = (mem_pool_t *) p_app_buff_fifo->p_data_buff[p_app_buff_fifo->tail];
  p_app_buff_fifo->tail = (p_app_buff_fifo->tail + 1) % MAX_RECEIVED_TAGS_CNT;
  p_app_buff_fifo->count--;

  return result;
}
