/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
/**
  ******************************************************************************
  * @file  lwipopts.h
  * @brief LwIP options file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_lwip_license.md file
  * in the same directory as the generated code.
  * If no mx_lwip_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#ifndef LWIPOPTS_H
#define LWIPOPTS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*
   ------------------------------------
   -------------- NO SYS --------------
   ------------------------------------
*/

/**
  * NO_SYS==1: Use lwIP without OS-awareness (no thread, semaphores, mutexes or
  * mboxes). This means threaded APIs cannot be used (socket, netconn,
  * i.e. everything in the 'api' folder), only the callback-style raw API is
  * available (and you have to watch out for yourself that you don't access
  * lwIP functions/structures from more than one context at a time!)
  */
#define NO_SYS                          0

/*
   ------------------------------------
   ----------- Core locking -----------
   ------------------------------------
*/
/**
  * LWIP_TCPIP_CORE_LOCKING
  * Creates a global mutex that is held during TCPIP thread operations.
  * Can be locked by client code to perform lwIP operations without changing
  * into TCPIP thread using callbacks. See LOCK_TCPIP_CORE() and
  * UNLOCK_TCPIP_CORE().
  * Your system must provide mutexes supporting priority inversion to use this.
  */
#define LWIP_TCPIP_CORE_LOCKING         1

/**
  * SYS_LIGHTWEIGHT_PROT==1: enable inter-task protection (and task-vs-interrupt
  * protection) for certain critical regions during buffer allocation, deallocation
  * and memory allocation and deallocation.
  * ATTENTION: This is required when using lwIP from more than one context! If
  * you disable this, you must be sure what you are doing!
  */
#define SYS_LIGHTWEIGHT_PROT            1

/*
   ------------------------------------
   ---------- Memory options ----------
   ------------------------------------
*/
#define MEM_LIBC_MALLOC                 0
#define MEMP_MEM_MALLOC                 0

/**
  * MEMP_MEM_INIT==1: Force use of memset to initialize pool memory.
  * Useful if pool are moved in uninitialized section of memory. This will ensure
  * default values in pcbs struct are well initialized in all conditions.
  */
#define MEMP_MEM_INIT                   1

/**
 * MEMP_NUM_SYS_TIMEOUT: the number of simultaneously active timeouts.
 */
#define MEMP_NUM_SYS_TIMEOUT            17

/* Number of "TCPIP_MSG_INPKT" structures in the pool.
 *
 * These objects are used by the tcpip_thread to queue incoming packets
 * (from the Ethernet/low-level input) for deferred processing in
 * thread context when NO_SYS == 0.
 *
 * A higher value allows more packets to be queued concurrently, which
 * improves throughput on high-traffic links or when the tcpip_thread
 * is busy, at the cost of additional RAM.
 * Typical values: 8-64 depending on link speed and memory size.
 */
#define MEMP_NUM_TCPIP_MSG_INPKT        16

/**
  * MEM_ALIGNMENT: must be set to the alignment of the CPU
  * and must correspond to the STM32 Ethernet interface bus width.
  * If the Ethernet IP is on an AHB bus: 4 bytes.
  * If the Ethernet IP is on an AXI bus: 8 bytes
  *    4 byte alignment -> \#define MEM_ALIGNMENT 4
  *    8 byte alignment -> \#define MEM_ALIGNMENT 8
  */
#define MEM_ALIGNMENT                   8

/**
  * MEM_SIZE: the size of the heap memory. If the application will send
  * a lot of data that needs to be copied, this must be set high.
  */
#define MEM_SIZE                        4096

/*
   ------------------------------------------------
   ---------- Internal Memory Pool Sizes ----------
   ------------------------------------------------
*/

/**
  * MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
  * per active UDP "connection".
  * (requires the LWIP_UDP option)
  */
#define MEMP_NUM_UDP_PCB                4

/**
  * MEMP_NUM_TCP_PCB: the number of simultaneously active TCP connections.
  * (requires the LWIP_TCP option)
  */
#define MEMP_NUM_TCP_PCB                15

/**
  * MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP connections.
  * (requires the LWIP_TCP option)
  */
#define MEMP_NUM_TCP_PCB_LISTEN         4

/**
  * MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP segments.
  * (requires the LWIP_TCP option)
  */
#define MEMP_NUM_TCP_SEG                16

/**
  * MEMP_NUM_NETCONN: the number of struct netconns.
  * (only needed if you use the sequential API, like api_lib.c)
  */
#define MEMP_NUM_NETCONN                8

/**
  * PBUF_POOL_SIZE: the number of buffers in the general pbuf pool.
  */
#define PBUF_POOL_SIZE                  40

/*
   ---------------------------------
   ---------- ARP options ----------
   ---------------------------------
*/
/**
  * LWIP_ARP==1: Enable ARP functionality.
  */
#define LWIP_ARP                        1
/*
   --------------------------------
   ---------- IP options ----------
   --------------------------------
*/
/* Internet Protocol version 4 */
#define LWIP_IPV4                       1

/*
   ----------------------------------
   ---------- ICMP options ----------
   ----------------------------------
*/
#define LWIP_ICMP                       1

/*
   ----------------------------------
   ---------- DHCP options ----------
   ----------------------------------
*/
/**
  * LWIP_DHCP==1: Enable DHCP module.
  */
#define LWIP_DHCP                       1

/*
   ----------------------------------
   ---------- DNS options -----------
   ----------------------------------
*/
/**
  * LWIP_DNS==1: Turn on DNS module. UDP must be available for DNS
  * transport.
  */
#define LWIP_DNS                        1

/** The maximum of DNS servers
  * The first server can be initialized automatically by defining
  * DNS_SERVER_ADDRESS(ipaddr), where 'ipaddr' is an 'ip_addr_t*'
  */
#define DNS_MAX_SERVERS                 3

/* mDNS responder */
#define LWIP_MDNS_RESPONDER             1
#define MDNS_DEBUG                      LWIP_DBG_OFF
#define LWIP_IGMP                       1
#define LWIP_NUM_NETIF_CLIENT_DATA      1

/*
   ---------------------------------
   ---------- UDP options ----------
   ---------------------------------
*/

#define LWIP_UDP                        1

/**
  * UDP_TTL: Default Time-To-Live value.
  */
#define UDP_TTL                         255

/*
   ---------------------------------
   ---------- TCP options ----------
   ---------------------------------
*/
#define LWIP_TCP                        1

/**
  * TCP_TTL: Default Time-To-Live value.
  */
#define TCP_TTL                         255

/**
  * TCP_WND: The size of a TCP window.  This must be at least
  * (2 * TCP_MSS) for things to work well.
  * ATTENTION: when using TCP_RCV_SCALE, TCP_WND is the total size
  * with scaling applied. Maximum window value in the TCP header
  * will be TCP_WND >> TCP_RCV_SCALE
  */
#define TCP_WND                         (16 * TCP_MSS)

/**
  * TCP_MSS: TCP Maximum segment size. (default is 536, a conservative default,
  * you might want to increase this.)
  * For the receive side, this MSS is advertised to the remote side
  * when opening a connection. For the transmit size, this MSS sets
  * an upper limit on the MSS advertised by the remote host.
  */
/* TCP_MSS = (Ethernet MTU - IP header size(20) - TCP header size(20)) */
#define TCP_MSS                 (1500 - 40)


/**
  * TCP_SND_BUF: TCP sender buffer space (bytes).
  * To achieve good performance, this must be at least 2 * TCP_MSS.
  */
#define TCP_SND_BUF             (4 * TCP_MSS)

/**
  * TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
  * as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work.
  */
#define TCP_SND_QUEUELEN        (4 * TCP_SND_BUF/TCP_MSS)

/**
  * TCP_LISTEN_BACKLOG: Enable the backlog option for tcp listen pcb.
  */
#define TCP_LISTEN_BACKLOG              5

/**
  * TCP_OVERSIZE: The maximum number of bytes that tcp_write may
  * allocate ahead of time in an attempt to create shorter pbuf chains
  * for transmission. The meaningful range is 0 to TCP_MSS. Some
  * suggested values are:
  *
  * 0:         Disable oversized allocation. Each tcp_write() allocates a new
              pbuf (old behaviour).
  * 1:         Allocate size-aligned pbufs with minimal excess. Use this if your
  *            scatter-gather DMA requires aligned fragments.
  * 128:       Limit the pbuf/memory overhead to 20%.
  * TCP_MSS:   Try to create unfragmented TCP packets.
  * TCP_MSS/4: Try to create 4 fragments or less per TCP packet.
  */
#define TCP_OVERSIZE                    1

/*
   ------------------------------------------------
   ---------- Network Interfaces options ----------
   ------------------------------------------------
*/

/**
  * LWIP_NETIF_HOSTNAME==1: use DHCP_OPTION_HOSTNAME with netif's hostname
  * field.
  */
#define LWIP_NETIF_HOSTNAME             1

/**
  * LWIP_NETIF_API==1: Support netif api (in netifapi.c)
  */
#define LWIP_NETIF_API                  1

/**
  * LWIP_NETIF_STATUS_CALLBACK==1: Support a callback function whenever an interface
  * changes its up/down status (i.e., due to DHCP IP acquisition)
  */
#define LWIP_NETIF_STATUS_CALLBACK      1

/**
  * LWIP_NETIF_EXT_STATUS_CALLBACK==1: Support an extended callback function
  * for several netif related event that supports multiple subscribers.
  * @see netif_ext_status_callback
  */
#define LWIP_NETIF_EXT_STATUS_CALLBACK  1

/**
  * LWIP_NETIF_LINK_CALLBACK==1: Support a callback function from an interface
  * whenever the link changes (i.e., link down)
  */
#define LWIP_NETIF_LINK_CALLBACK        1

/**
  * LWIP_NETIF_REMOVE_CALLBACK==1: Support a callback function that is called
  * when a netif has been removed
  */
#define LWIP_NETIF_REMOVE_CALLBACK      1

/**
  * LWIP_NETIF_TX_SINGLE_PBUF: if this is set to 1, lwIP *tries* to put all data
  * to be sent into one single pbuf. This is for compatibility with DMA-enabled
  * MACs that do not support scatter-gather.
  * Beware that this might involve CPU-memcpy before transmitting that would not
  * be needed without this flag! Use this only if you need to!
  *
  * ATTENTION: a driver must *NOT* rely on getting single pbufs but check TX
  * pbufs for being in one piece. If not, @ref pbuf_clone can be used to get
  * a single pbuf:
  *   if (p->next != NULL) {
  *     struct pbuf *q = pbuf_clone(PBUF_RAW, PBUF_RAM, p);
  *     if (q == NULL) {
  *       return ERR_MEM;
  *     }
  *     p = q; ATTENTION: do NOT free the old 'p' as the ref belongs to the caller!
  *   }
  */
#define LWIP_NETIF_TX_SINGLE_PBUF       1

/*
   ------------------------------------
   ---------- Thread options ----------
   ------------------------------------
*/
/**
  * TCPIP_THREAD_NAME: The name assigned to the main tcpip thread.
  */
#define TCPIP_THREAD_NAME               "TCP/IP"

/**
  * TCPIP_THREAD_STACKSIZE: The stack size used by the main tcpip thread.
  * The stack size value itself is platform-dependent, but is passed to
  * sys_thread_new() when the thread is created.
  */
#define TCPIP_THREAD_STACKSIZE          (2048)

/**
  * TCPIP_THREAD_PRIO: The priority assigned to the main tcpip thread.
  * The priority value itself is platform-dependent, but is passed to
  * sys_thread_new() when the thread is created.
  */
#define TCPIP_THREAD_PRIO               25

/**
  * TCPIP_MBOX_SIZE: The mailbox size for the tcpip thread messages
  * The queue size value itself is platform-dependent, but is passed to
  * sys_mbox_new() when tcpip_init is called.
  */
#define TCPIP_MBOX_SIZE                 20

#define DEFAULT_UDP_RECVMBOX_SIZE       10
#define DEFAULT_TCP_RECVMBOX_SIZE       20
#define DEFAULT_ACCEPTMBOX_SIZE         10

/**
  * DEFAULT_THREAD_NAME: The name assigned to any other lwIP thread.
  */
#define DEFAULT_THREAD_NAME             "lwIP"

/**
  * DEFAULT_THREAD_STACKSIZE: The stack size used by any other lwIP thread.
  * The stack size value itself is platform-dependent, but is passed to
  * sys_thread_new() when the thread is created.
  */
#define DEFAULT_THREAD_STACKSIZE        (512)

/**
  * DEFAULT_THREAD_PRIO: The priority assigned to any other lwIP thread.
  * The priority value itself is platform-dependent, but is passed to
  * sys_thread_new() when the thread is created.
  */
#define DEFAULT_THREAD_PRIO             16

/**
  * DEFAULT_RAW_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
  * NETCONN_RAW. The queue size value itself is platform-dependent, but is passed
  * to sys_mbox_new() when the recvmbox is created.
  */
#define DEFAULT_RAW_RECVMBOX_SIZE       3 /* for ICMP PING */

/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/**
  * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
  */
#define LWIP_NETCONN                    0

/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/**
  * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
  */
#define LWIP_SOCKET                     1

/**
  * LWIP_COMPAT_SOCKETS==1: Enable BSD-style sockets functions names through defines.
  * LWIP_COMPAT_SOCKETS==2: Same as ==1 but correctly named functions are created.
  * While this helps code completion, it might conflict with existing libraries.
  * (only used if you use sockets.c)
  */
#define LWIP_COMPAT_SOCKETS             2

/* Keep the TCP link active. Important for MQTT/TLS */
/**
  * LWIP_TCP_KEEPALIVE==1: Enable TCP_KEEPIDLE, TCP_KEEPINTVL and TCP_KEEPCNT
  * options processing. Note that TCP_KEEPIDLE and TCP_KEEPINTVL have to be set
  * in seconds. (does not require sockets.c, and will affect tcp.c)
  */
#define LWIP_TCP_KEEPALIVE                  1

/**
  * LWIP_SO_SNDTIMEO==1: Enable send timeout for sockets/netconns and
  * SO_SNDTIMEO processing.
  */
#define LWIP_SO_SNDTIMEO                1

/**
  * LWIP_SO_RCVTIMEO==1: Enable receive timeout for sockets/netconns and
  * SO_RCVTIMEO processing.
  */
#define LWIP_SO_RCVTIMEO                1

/**
  * LWIP_SO_SNDRCVTIMEO_NONSTANDARD==1: SO_RCVTIMEO/SO_SNDTIMEO take an int
  * (milliseconds, much like winsock does) instead of a struct timeval (default).
  */
#define LWIP_SO_SNDRCVTIMEO_NONSTANDARD 1

/*
   ----------------------------------------
   ---------- Statistics options ----------
   ----------------------------------------
*/
/**
  * LWIP_STATS==1: Enable statistics collection in lwip_stats.
  */
#define LWIP_STATS                      0

/*
   --------------------------------------
   ---------- Checksum options ----------
   --------------------------------------
*/

/* Use CPU to compute the checksums. */

#ifdef CHECKSUM_BY_HARDWARE
#define CHECKSUM_GEN_IP                 0
#define CHECKSUM_GEN_UDP                0
#define CHECKSUM_GEN_TCP                0
#define CHECKSUM_GEN_ICMP               0
#define CHECKSUM_GEN_ICMP6              0
#define CHECKSUM_CHECK_IP               0
#define CHECKSUM_CHECK_UDP              0
#define CHECKSUM_CHECK_TCP              0
#define CHECKSUM_CHECK_ICMP             0
#define CHECKSUM_CHECK_ICMP6            0
#else
#define CHECKSUM_GEN_IP                 1
#define CHECKSUM_GEN_UDP                1
#define CHECKSUM_GEN_TCP                1
#define CHECKSUM_GEN_ICMP               1
#define CHECKSUM_GEN_ICMP6              1
#define CHECKSUM_CHECK_IP               1
#define CHECKSUM_CHECK_UDP              1
#define CHECKSUM_CHECK_TCP              1
#define CHECKSUM_CHECK_ICMP             1
#define CHECKSUM_CHECK_ICMP6            1
#endif /* CHECKSUM_BY_HARDWARE */

/*
   ---------------------------------------
   ---------- Debugging options ----------
   ---------------------------------------
*/
/* #define LWIP_DEBUG */

/**
  * LWIP_DBG_MIN_LEVEL: After masking, the value of the debug is
  * compared against this value. If it is smaller, then debugging
  * messages are written.
  * @see debugging_levels
  */
#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_WARNING

#if defined(LWIP_DEBUG)
#define NETIF_DEBUG                     LWIP_DBG_ON
#define SOCKETS_DEBUG                   LWIP_DBG_ON
#define SYS_DEBUG                       LWIP_DBG_ON
#define DHCP_DEBUG                      LWIP_DBG_ON
#define DHCP6_DEBUG                     LWIP_DBG_ON
#define API_LIB_DEBUG                   LWIP_DBG_ON
#define API_MSG_DEBUG                   LWIP_DBG_ON
#define INET_DEBUG                      LWIP_DBG_ON
#define IP_DEBUG                        LWIP_DBG_ON
#define IP6_DEBUG                       LWIP_DBG_ON
#define RAW_DEBUG                       LWIP_DBG_ON
#define MEM_DEBUG                       LWIP_DBG_ON
#define MEMP_DEBUG                      LWIP_DBG_ON
#define UDP_DEBUG                       LWIP_DBG_ON
#define TCP_DEBUG                       LWIP_DBG_ON
#define TCP_INPUT_DEBUG                 LWIP_DBG_ON
#define TCP_OUTPUT_DEBUG                LWIP_DBG_ON
#endif /* LWIP_DEBUG */

/*
   ---------------------------------
   ---------- OS options ----------
   ---------------------------------
*/
#if (defined(__GNUC__) && !defined(__ARMCC_VERSION))
#define LWIP_ERRNO_STDINCLUDE
#else
#define LWIP_PROVIDE_ERRNO
#endif /* __GNUC__ */

#define LWIP_COMPAT_MUTEX               0
#define LWIP_COMPAT_MUTEX_ALLOWED       0

#define LWIP_FREERTOS_THREAD_STACKSIZE_IS_STACKWORDS 1

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* LWIPOPTS_H */
