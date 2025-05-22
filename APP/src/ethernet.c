#include <stdio.h>
#include <string.h>
#include "ethernet.h"

// Cấu hình địa chỉ mạng
uint8_t gateway[4] = {192, 168, 1, 1};
uint8_t subnet[4]  = {255, 255, 255, 0};
uint8_t mac[6]     = {0x00, 0x08, 0xDC, 0x01, 0x02, 0x03};
uint8_t ip[4]      = {192, 168, 0, 100};

uint8_t NetBuffer[MAX_ETHERNET_BUFFER_LENGTH];

static void W5500_WriteByte(uint16_t addr, uint8_t block, uint8_t data)
{
    CS_LOW; // CS low

    SPI_Transfer(SPI2, (addr >> 8) & 0xFF);       // Address high
    SPI_Transfer(SPI2, addr & 0xFF);              // Address low
    SPI_Transfer(SPI2, (block << 3) | 0x04);      // Write 1 byte
    SPI_Transfer(SPI2, data);

    CS_HIGH; // CS high
}

static uint8_t W5500_ReadByte(uint16_t addr, uint8_t block)
{
    uint8_t data;

    CS_LOW;

    SPI_Transfer(SPI2, (addr >> 8) & 0xFF);
    SPI_Transfer(SPI2, addr & 0xFF);
    SPI_Transfer(SPI2, (block << 3) | 0x00); // Read, 1 byte mode

    data = SPI_Transfer(SPI2, 0x00); // dummy write

    CS_HIGH;

    return data;
}

static void W5500_WriteBuffer(uint16_t addr, uint8_t block, const uint8_t *buf, uint16_t len)
{
    uint16_t i = 0;

    CS_LOW; // CS low

    SPI_Transfer(SPI2, (addr >> 8) & 0xFF);       // Address high
    SPI_Transfer(SPI2, addr & 0xFF);              // Address low
    SPI_Transfer(SPI2, (block << 3) | 0x04);      // Write 1 byte

    for (i = 0; i < len; i++)
    {
        SPI_Transfer(SPI2, buf[i]);
    }

    CS_HIGH; // CS high
}

static void W5500_ReadBuffer(uint16_t addr, uint8_t block, uint8_t *buf, uint16_t len)
{
    uint16_t i = 0;

    CS_LOW; // CS low

    SPI_Transfer(SPI2, (addr >> 8) & 0xFF);     // Address high
    SPI_Transfer(SPI2, addr & 0xFF);            // Address low
    SPI_Transfer(SPI2, (block << 3) | 0x00);    // Read 1 byte

    for (i = 0; i < len; i++)
    {
        buf[i] = SPI_Transfer(SPI2, 0x00);
    }

    CS_HIGH; // CS high
}

static inline uint16_t W5500_Read16(uint16_t addr, uint8_t block)
{
    uint8_t buf[2] = {0};

    W5500_ReadBuffer(addr, block, buf, 2);
        
    return buf[0] << 8 | buf[1];
}

static inline void W5500_Write16(uint16_t addr, uint8_t block, uint16_t data)
{
    uint8_t buf[2] = {0};
    buf[0] = (data >> 8) & 0xFF;
    buf[1] = data & 0xFF;
    W5500_WriteBuffer(addr, block, buf, 2);
}

void W5500_MACRAW_Init(void) {
    // Đặt Socket 0 về MACRAW mode
    W5500_WriteByte(SOCKET_MODE_REGISTER, SOCKET0_REGISTER_BLOCK, MACRAM_MODE);    // Sn_MR = MACRAW
    W5500_WriteByte(SOCKER_CMD_REGISTER, SOCKET0_REGISTER_BLOCK, OPEN_CMD);        // Sn_CR = OPEN
    while (W5500_ReadByte(SOCKER_CMD_REGISTER, SOCKET0_REGISTER_BLOCK));           // Đợi cho lệnh thực thi xong
}

void W5500_SendPacket(uint8_t* packet, uint16_t len)
{
    uint16_t tx_wr = 0;
    uint16_t tx_fsr = 0;

    tx_fsr = W5500_Read16(SOCKET_TX_FREE_SIZE, SOCKET0_REGISTER_BLOCK);

    if (tx_fsr > len)
    {
        return;
    }

    tx_wr = W5500_Read16(SOCKET_TX_WRITE_POINTER, SOCKET0_REGISTER_BLOCK);
    W5500_WriteBuffer(tx_wr, SOCKET0_BUFFER_TX_BLOCK, packet, len);
    tx_wr += len;
    W5500_Write16(SOCKET_TX_WRITE_POINTER, SOCKET0_REGISTER_BLOCK, tx_wr);

    // Gửi lệnh SEND
    W5500_WriteByte(SOCKER_CMD_REGISTER, SOCKET0_REGISTER_BLOCK, SEND_CMD);

    // Chờ lệnh hoàn tất
    while (W5500_ReadByte(SOCKER_CMD_REGISTER, SOCKET0_REGISTER_BLOCK));
}

void W5500_RevcPacket(uint8_t* buff, uint16_t* len)
{
    uint16_t rx_rd = 0;
    uint16_t rx_size = 0;
    
    rx_size = W5500_Read16(SOCKET_RX_RECV_SIZE, SOCKET0_REGISTER_BLOCK);
    rx_size = rx_size < MAX_ETHERNET_BUFFER_LENGTH ? rx_size : MAX_ETHERNET_BUFFER_LENGTH;
    *len = rx_size;

    rx_rd = W5500_Read16(SOCKET_RX_READ_POINTER, SOCKET0_REGISTER_BLOCK);
    W5500_ReadBuffer(rx_rd, SOCKET0_BUFFER_RX_BLOCK, buff, rx_size);

    // Cập nhật con trỏ RX
    rx_rd += rx_size;
    W5500_Write16(SOCKET_RX_READ_POINTER, SOCKET0_REGISTER_BLOCK, rx_rd);
    W5500_WriteByte(SOCKER_CMD_REGISTER, SOCKET0_REGISTER_BLOCK, RECV_CMD);
}

void SendARPRequest(uint8_t *src_mac, uint8_t *src_ip, uint8_t *target_ip) {
    uint8_t arp_packet[42];

    // Ethernet Header
    memset(arp_packet, 0xFF, 6);                    // Destination MAC: broadcast
    memcpy(&arp_packet[6], src_mac, 6);             // Source MAC
    arp_packet[12] = 0x08; arp_packet[13] = 0x06;   // EtherType: ARP (0x0806)

    // ARP Payload
    arp_packet[14] = 0x00; arp_packet[15] = 0x01;   // Hardware type: Ethernet
    arp_packet[16] = 0x08; arp_packet[17] = 0x00;   // Protocol type: IPv4
    arp_packet[18] = 0x06;                          // Hardware size
    arp_packet[19] = 0x04;                          // Protocol size
    arp_packet[20] = 0x00; arp_packet[21] = 0x01;   // Opcode: request
    memcpy(&arp_packet[22], src_mac, 6);            // Sender MAC
    memcpy(&arp_packet[28], src_ip, 4);             // Sender IP
    memset(&arp_packet[32], 0x00, 6);               // Target MAC = 00:00:00:00:00:00
    memcpy(&arp_packet[38], target_ip, 4);          // Target IP

    // Gửi gói ARP thông qua TX buffer
    W5500_SendPacket(arp_packet, 42);
}

uint8_t ReceiveARPReply(uint8_t *reply_mac, uint8_t *reply_ip) {

    uint16_t rx_size = 0;
    uint8_t MAC_source[6];
    uint8_t MAC_target[6];
    uint8_t IP_source[4];
    uint8_t IP_target[4];
    
    W5500_RevcPacket(NetBuffer, &rx_size);

    if (NetBuffer[12])
    {
        printf("ARP: 0x%02X 0x%02X\r\n", NetBuffer[12], NetBuffer[13]);

        memcpy(MAC_source, &NetBuffer[6], 6);  //lay MAC cua nguon gui packet
        memcpy(MAC_target, &NetBuffer[32], 6);  //lay MAC cua nguon nhan packet
        memcpy(IP_source, &NetBuffer[28], 4); //lay IP cua nguon gui packet
        memcpy(IP_target, &NetBuffer[38], 4); //lay IP cua nguon nhan packet
        printf("Source MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", MAC_source[0], MAC_source[1], MAC_source[2], MAC_source[3], MAC_source[4], MAC_source[5]);
        printf("Target MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", MAC_target[0], MAC_target[1], MAC_target[2], MAC_target[3], MAC_target[4], MAC_target[5]);
        printf("Source IP: %d.%d.%d.%d\r\n", IP_source[0], IP_source[1], IP_source[2], IP_source[3]);
        printf("Target IP: %d.%d.%d.%d\r\n", IP_target[0], IP_target[1], IP_target[2], IP_target[3]);
    }

    // Kiểm tra có phải ARP reply không
    if (NetBuffer[12] == 0x08 && NetBuffer[13] == 0x06 &&  // EtherType: ARP
        NetBuffer[20] == 0x00 && NetBuffer[21] == 0x02) {  // Opcode: reply
        memcpy(reply_mac, &NetBuffer[22], 6);        // MAC của người trả lời
        memcpy(reply_ip, &NetBuffer[28], 4);         // IP của người trả lời

        return 1;
    }

    memset(NetBuffer, 0, rx_size);

    return 0;
}

extern void delay(uint16_t mDelay);

void EthernetInit(void)
{
    uint8_t ver = 0;
    uint8_t mymac[6] = {0};

    SPI_Init(SPI2);

    ver = W5500_ReadByte(0x0039, 0x00);

    if (ver != W5500_ID) {
        printf("Failed version\r\n");
        return;
    }

    printf("Ethernet init...\r\n");

    // Reset phần mềm (MR = 0x80)
    W5500_WriteByte(0x0000, 0x00, 0x80);
    delay(2);

    W5500_WriteBuffer(SRC_HW_ADDR_REGISTER, COMMON_REGISTER_BLOCK, mac, 6);

    W5500_ReadBuffer(SRC_HW_ADDR_REGISTER, COMMON_REGISTER_BLOCK, mymac, 6);
    
    printf("Your MAC address: %02X:%02X:%02X:%02X:%02X:%02X\r\n", mymac[0],mymac[1],mymac[2],mymac[3],mymac[4],mymac[5]);

    W5500_MACRAW_Init();

    SendARPRequest(mac, ip, (uint8_t[]) {
        192, 168, 0, 1
    });

    uint8_t found_mac[6], found_ip[4];
    while (1)
    {
        if (ReceiveARPReply(found_mac, found_ip)) {
            printf("Found MAC of IP %d.%d.%d.%d là %02X:%02X:%02X:%02X:%02X:%02X\n",
                found_ip[0], found_ip[1], found_ip[2], found_ip[3],
                found_mac[0], found_mac[1], found_mac[2],
                found_mac[3], found_mac[4], found_mac[5]);
        }

        delay(200);
    }
}

