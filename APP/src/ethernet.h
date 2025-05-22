#ifndef _ETHERNET_H_
#define _ETHERNET_H_

#include "stm32_driver_spi.h"

#define MAX_ETHERNET_BUFFER_LENGTH  2048

#define COMMON_REGISTER_BLOCK       0x00
#define SOCKET0_REGISTER_BLOCK      0x01
#define SOCKET0_BUFFER_TX_BLOCK     0x02
#define SOCKET0_BUFFER_RX_BLOCK     0x03
#define SOCKET1_REGISTER_BLOCK      0x05
#define SOCKET1_BUFFER_TX_BLOCK     0x06
#define SOCKET1_BUFFER_RX_BLOCK     0x07
#define SOCKET2_REGISTER_BLOCK      0x09
#define SOCKET2_BUFFER_TX_BLOCK     0x0A
#define SOCKET2_BUFFER_RX_BLOCK     0x0B
#define SOCKET3_REGISTER_BLOCK      0x0D
#define SOCKET3_BUFFER_TX_BLOCK     0x0E
#define SOCKET3_BUFFER_RX_BLOCK     0x0F
#define SOCKET4_REGISTER_BLOCK      0x11
#define SOCKET4_BUFFER_TX_BLOCK     0x12
#define SOCKET4_BUFFER_RX_BLOCK     0x13
#define SOCKET5_REGISTER_BLOCK      0x15
#define SOCKET5_BUFFER_TX_BLOCK     0x16
#define SOCKET5_BUFFER_RX_BLOCK     0x17
#define SOCKET6_REGISTER_BLOCK      0x19
#define SOCKET6_BUFFER_TX_BLOCK     0x1A
#define SOCKET6_BUFFER_RX_BLOCK     0x1B
#define SOCKET7_REGISTER_BLOCK      0x1D
#define SOCKET7_BUFFER_TX_BLOCK     0x1E
#define SOCKET7_BUFFER_RX_BLOCK     0x1F

/* Common Register Block*/
#define MODE_REGISTER               0x0000
    #define FORCE_ARP               0x02
    #define PPPOE_MODE              0x08
    #define PING_BLOCK_MODE         0x10
    #define WAKE_ON_LAN             0x20
    #define SOFT_RST                0x80
#define GATEWAY_IP_ADDR0_REGISTER   0x0001
#define GATEWAY_IP_ADDR1_REGISTER   0x0002
#define GATEWAY_IP_ADDR2_REGISTER   0x0003
#define GATEWAY_IP_ADDR3_REGISTER   0x0004
#define GATEWAY_IP_ADDR_REGISTER    GATEWAY_IP_ADDR0_REGISTER
#define SUBNET_MASK0_REGISTER       0x0005
#define SUBNET_MASK1_REGISTER       0x0006
#define SUBNET_MASK2_REGISTER       0x0007
#define SUBNET_MASK3_REGISTER       0x0008
#define SUBNET_MASK_REGISTER        SUBNET_MASK0_REGISTER
#define SRC_HW_ADDR0_REGISTER       0x0009       
#define SRC_HW_ADDR1_REGISTER       0x000A    
#define SRC_HW_ADDR2_REGISTER       0x000B       
#define SRC_HW_ADDR3_REGISTER       0x000C 
#define SRC_HW_ADDR4_REGISTER       0x000D 
#define SRC_HW_ADDR5_REGISTER       0x000E
#define SRC_HW_ADDR_REGISTER        SRC_HW_ADDR0_REGISTER
#define SRC_IP_ADDR0_REGISTER       0x000F
#define SRC_IP_ADDR1_REGISTER       0x0010
#define SRC_IP_ADDR2_REGISTER       0x0011
#define SRC_IP_ADDR3_REGISTER       0x0012
#define SRC_IP_ADDR_REGISTER        SRC_IP_ADDR0_REGISTER
#define INT_LOW_LVL_TMR0_REGISTER   0x0013
#define INT_LOW_LVL_TMR1_REGISTER   0x0014
#define INT_LOW_LVL_TMR_REGISTER    INT_LOW_LVL_TMR0_REGISTER
#define INT_REGISTER                0x0015
    #define MAGIC_PACKET            0x10
    #define PPPOE_CONNECTION_CLOSE  0x20
    #define DEST_UNREACH            0x40
    #define IP_CONFLICT             0x80
#define INT_MASK_REGISTER           0x0016
    #define MAGIC_PACKET_INT_MASK   0x10
    #define PPPOE_CLOSE_INT_MASK    0x20
    #define DEST_UNREACH_INT_MASK   0x40
    #define IP_CONFLICT_INT_MASK    0x80
#define COMMON_INT_REGISTER         0x0017
    #define S0_INT                  0x01
    #define S1_INT                  0x02
    #define S2_INT                  0x04
    #define S3_INT                  0x08
    #define S4_INT                  0x10
    #define S5_INT                  0x20
    #define S6_INT                  0x40
    #define S7_INT                  0x80
#define SOCKET_INT_MASK_REGISTER    0x0018
    #define S0_INT_MASK             0x01
    #define S1_INT_MASK             0x02
    #define S2_INT_MASK             0x04
    #define S3_INT_MASK             0x08
    #define S4_INT_MASK             0x10
    #define S5_INT_MASK             0x20
    #define S6_INT_MASK             0x40
    #define S7_INT_MASK             0x80
#define RETRY_TIME_VAL0_REGISTER    0x0019
#define RETRY_TIME_VAL1_REGISTER    0x001A
#define RETRY_TIME_VAL_REGISTER     RETRY_TIME_VAL0_REGISTER
#define RETRY_COUNTER_REGISTER      0x001B
#define PTIMER_REGISTER             0x001C
#define PMAGIC_REGISTER             0x001D
#define DEST_HW_ADDR0_REGISTER      0x001E
#define DEST_HW_ADDR1_REGISTER      0x001F
#define DEST_HW_ADDR2_REGISTER      0x0020
#define DEST_HW_ADDR3_REGISTER      0x0021
#define DEST_HW_ADDR4_REGISTER      0x0022
#define DEST_HW_ADDR5_REGISTER      0x0023
#define DEST_HW_ADDR_REGISTER       DEST_HW_ADDR0_REGISTER
#define SESSION_ID0_REGISTER        0x0024
#define SESSION_ID1_REGISTER        0x0025
#define SESSION_ID_REGISTER         SESSION_ID0_REGISTER
#define MAX_RECV_UNIT0_REGISTER     0x0026
#define MAX_RECV_UNIT1_REGISTER     0x0027
#define MAX_RECV_UNIT_REGISTER      MAX_RECV_UNIT0_REGISTER
#define UNREACH_IP_ADDR0_REGISTER   0x0028
#define UNREACH_IP_ADDR1_REGISTER   0x0029
#define UNREACH_IP_ADDR2_REGISTER   0x002A
#define UNREACH_IP_ADDR3_REGISTER   0x002B
#define UNREACH_IP_ADDR_REGISTER    UNREACH_IP_ADDR0_REGISTER
#define UNREACH_PORT0_REGITER       0x002C
#define UNREACH_PORT1_REGITER       0x002D
#define UNREACH_PORT_REGITER        UNREACH_PORT0_REGITER
#define PHY_CONFIG_REGISTER         0x002E
    #define PHY_RST                 0x80
    #define PHY_FULL_DUPLEX         0x04
    #define PHY_HALF_DUPLEX         0x00
    #define PHY_SPEED_100MPBS       0x02
    #define PHY_SPEED_10MPBS        0x00
    #define PHY_LNK_UP              0x01
    #define PHY_LNK_DOWN            0x00
#define CHIP_VER_REGISTER           0x0039
#define W5500_ID                    0x04

/* Socket register */
#define SOCKET_MODE_REGISTER        0x0000
    #define EN_MULTICASTING         0x80
    #define EN_MAC_FILTER           0x80
    #define EN_BROADCAST_BLOCK      0x40
    #define EN_USE_NO_DELAY_ACK     0x20
    #define MULTICAST_IGMP_VER_1    0x20
    #define MULTICAST_IGMP_VER_2    0x00
    #define EN_MULTICAST_BLOCK      0x20
    #define EN_UNICAST_BLOCK        0x10
    #define EN_IPV6_BLOCK           0x10
    #define CLOSED_MODE             0x00
    #define TCP_MODE                0x01
    #define UDP_MODE                0x02
    #define MACRAM_MODE             0x04
#define SOCKER_CMD_REGISTER         0x0001
    #define OPEN_CMD                0x01
    #define LISTEN_CMD              0x02
    #define CONNECT_CMD             0x04
    #define DISCON_CMD              0x08
    #define CLOSE_CMD               0x10
    #define SEND_CMD                0x20
    #define SEND_MAC_CMD            0x21
    #define SEND_MAC_KEEP           0x22
    #define RECV_CMD                0x40
#define SOCKET_INT_REGISTER         0x0002
    #define SOCKET_SENDOK_INT       0x10
    #define SOCKET_TIMEOUT_INT      0x08
    #define SOCKET_RECV_INT         0x04
    #define SOCKET_DISCON_INT       0x02
    #define SOCKET_CON_INT          0x01
#define SOCKET_STATUS_REGISTER      0x0003
    #define SOCK_CLOSED             0x00
    #define SOCK_INIT               0x13
    #define SOCK_LISTEN             0x14
    #define SOCK_ESTABLISHED        0x17
    #define SOCK_CLOSE_WAIT         0x1C
    #define SOCK_UDP                0x22
    #define SOCK_MACRAW             0x42
    #define SOCK_SYNSENT            0x15
    #define SOCK_SYNRECV            0x16
    #define SOCK_FIN_WAIT           0x18
    #define SOCK_CLOSING            0x1A
    #define SOCK_TIME_WAIT          0x1B
    #define SOCK_LAST_ACK           0x1D
#define SOCKET_RX_BUFF_SIZE         0x001E
#define SOCKET_TX_BUFF_SIZE         0x001F
#define SOCKET_TX_FREE_SIZE0        0x0020
#define SOCKET_TX_FREE_SIZE1        0x0021
#define SOCKET_TX_FREE_SIZE         SOCKET_TX_FREE_SIZE0
#define SOCKET_TX_READ_POINTER0     0x0022
#define SOCKET_TX_READ_POINTER1     0x0023
#define SOCKET_TX_READ_POINTER      SOCKET_TX_READ_POINTER0
#define SOCKET_TX_WRITE_POINTER0    0x0024
#define SOCKET_TX_WRITE_POINTER1    0x0025
#define SOCKET_TX_WRITE_POINTER     SOCKET_TX_WRITE_POINTER0
#define SOCKET_RX_RECV_SIZE0        0x0026
#define SOCKET_RX_RECV_SIZE1        0x0027
#define SOCKET_RX_RECV_SIZE         SOCKET_RX_RECV_SIZE0
#define SOCKET_RX_READ_POINTER0     0x0028
#define SOCKET_RX_READ_POINTER1     0x0029
#define SOCKET_RX_READ_POINTER      SOCKET_RX_READ_POINTER0
#define SOCKET_RX_WRITE_POINTER0    0x002A
#define SOCKET_RX_WRITE_POINTER1    0x002B
#define SOCKET_RX_WRITE_POINTER     SOCKET_RX_WRITE_POINTER0

/* Socket Buffer
   Mặc định kích thước của mỗi RX Socket Buffer là 2KB.
   Nhưng, ta có thể cấu hình lại kích thước bằng thanh ghi Sn_RXBUF_SIZE.
   Với điều kiện tổng kích thước các RX Socket Buffer không được vượt quá 16KB.
   Tương tự đối với TX Socket Buffer.
*/
#define S0_RXBUF_SIZE               0x02
#define S1_RXBUF_SIZE               0x02
#define S2_RXBUF_SIZE               0x02
#define S3_RXBUF_SIZE               0x02
#define S4_RXBUF_SIZE               0x02
#define S5_RXBUF_SIZE               0x02
#define S6_RXBUF_SIZE               0x02
#define S7_RXBUF_SIZE               0x02

#define S0_TXBUF_SIZE               0x02
#define S1_TXBUF_SIZE               0x02
#define S2_TXBUF_SIZE               0x02
#define S3_TXBUF_SIZE               0x02
#define S4_TXBUF_SIZE               0x02
#define S5_TXBUF_SIZE               0x02
#define S6_TXBUF_SIZE               0x02
#define S7_TXBUF_SIZE               0x02

#define CS_LOW      GPIOB->ODR.BITS.ODR12 = 0x00
#define CS_HIGH     GPIOB->ODR.BITS.ODR12 = 0x01

extern void EthernetInit(void);

#endif /* _ETHERNET_H_ */

