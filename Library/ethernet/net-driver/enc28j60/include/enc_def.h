#ifndef __ENC_DEF__
#define __ENC_DEF__

/* ================= SPI OPCODES ================= */
#define ENC_OP_RCR               0x00U  /* Read Control Reg   */
#define ENC_OP_RBM               0x3AU  /* Read Buffer Memory */
#define ENC_OP_WCR               0x40U  /* Write Control Reg  */
#define ENC_OP_WBM               0x7AU  /* Write Buffer Memory*/
#define ENC_OP_BFS               0x80U  /* Bit Field Set      */
#define ENC_OP_BFC               0xA0U  /* Bit Field Clear    */
#define ENC_OP_SRC               0xFFU  /* Soft Reset Command */

/* ================= BANK SELECT ================ */
#define ENC_BANK0                0x00U
#define ENC_BANK1                0x01U
#define ENC_BANK2                0x02U
#define ENC_BANK3                0x03U
#define ENC_MAC_MII              0x80U

/* Thanh ghi nhóm MAC/MII cần 1 byte DUMMY khi đọc */
#define ENC_IS_MAC_MII(addr)     (((addr) & 0x80U) != 0U)

/* ============== COMMON REGISTERS ============== */
#define ENC_REG_EIE              0x1BU
#define ENC_REG_EIR              0x1CU
#define ENC_REG_ESTAT            0x1DU
#define ENC_REG_ECON2            0x1EU
#define ENC_REG_ECON1            0x1FU

/* EIE bits */
#define ENC_EIE_INTIE            (1U << 7)
#define ENC_EIE_PKTIE            (1U << 6)
#define ENC_EIE_DMAIE            (1U << 5)
#define ENC_EIE_LINKIE           (1U << 4)
#define ENC_EIE_TXIE             (1U << 3)
#define ENC_EIE_TXERIE           (1U << 1)
#define ENC_EIE_RXERIE           (1U << 0)

/* EIR bits */
#define ENC_EIR_PKTIF            (1U << 6)
#define ENC_EIR_DMAIF            (1U << 5)
#define ENC_EIR_LINKIF           (1U << 4)
#define ENC_EIR_TXIF             (1U << 3)
#define ENC_EIR_TXERIF           (1U << 1)
#define ENC_EIR_RXERIF           (1U << 0)

/* ESTAT bits */
#define ENC_ESTAT_INT            (1U << 7)
#define ENC_ESTAT_BUFER          (1U << 6)
#define ENC_ESTAT_LATECOL        (1U << 4)
#define ENC_ESTAT_RXBUSY         (1U << 2)
#define ENC_ESTAT_TXABRT         (1U << 1)
#define ENC_ESTAT_CLKRDY         (1U << 0)

/* ECON2 bits */
#define ENC_ECON2_AUTOINC        (1U << 7)
#define ENC_ECON2_PKTDEC         (1U << 6)
#define ENC_ECON2_PWRSV          (1U << 5)
#define ENC_ECON2_VRPS           (1U << 3)

/* ECON1 bits */
#define ENC_ECON1_TXRST          (1U << 7)
#define ENC_ECON1_RXRST          (1U << 6)
#define ENC_ECON1_DMAST          (1U << 5)
#define ENC_ECON1_CSUMEN         (1U << 4)
#define ENC_ECON1_TXRTS          (1U << 3)
#define ENC_ECON1_RXEN           (1U << 2)
#define ENC_ECON1_BSEL0          (1U << 0)
#define ENC_ECON1_BSEL1          (1U << 1)
#define ENC_ECON1_BSEL_MASK      (ENC_ECON1_BSEL0 | ENC_ECON1_BSEL1)

/* ================ BANK 0 ====================== */
#define ENC_REG_ERDPTL           0x00U
#define ENC_REG_ERDPTH           0x01U
#define ENC_REG_EWRPTL           0x02U
#define ENC_REG_EWRPTH           0x03U
#define ENC_REG_ETXSTL           0x04U
#define ENC_REG_ETXSTH           0x05U
#define ENC_REG_ETXNDL           0x06U
#define ENC_REG_ETXNDH           0x07U
#define ENC_REG_ERXSTL           0x08U
#define ENC_REG_ERXSTH           0x09U
#define ENC_REG_ERXNDL           0x0AU
#define ENC_REG_ERXNDH           0x0BU
#define ENC_REG_ERXRDPTL         0x0CU
#define ENC_REG_ERXRDPTH         0x0DU
#define ENC_REG_ERXWRPTL         0x0EU
#define ENC_REG_ERXWRPTH         0x0FU
#define ENC_REG_EDMASTL          0x10U
#define ENC_REG_EDMASTH          0x11U
#define ENC_REG_EDMANDL          0x12U
#define ENC_REG_EDMANDH          0x13U
#define ENC_REG_EDMADSTL         0x14U
#define ENC_REG_EDMADSTH         0x15U
#define ENC_REG_EDMACSL          0x16U
#define ENC_REG_EDMACSH          0x17U

/* ================ BANK 1 ====================== */
#define ENC_REG_EHT0             (0x00U | (ENC_BANK1 << 5))
#define ENC_REG_EHT1             (0x01U | (ENC_BANK1 << 5))
#define ENC_REG_EHT2             (0x02U | (ENC_BANK1 << 5))
#define ENC_REG_EHT3             (0x03U | (ENC_BANK1 << 5))
#define ENC_REG_EHT4             (0x04U | (ENC_BANK1 << 5))
#define ENC_REG_EHT5             (0x05U | (ENC_BANK1 << 5))
#define ENC_REG_EHT6             (0x06U | (ENC_BANK1 << 5))
#define ENC_REG_EHT7             (0x07U | (ENC_BANK1 << 5))
#define ENC_REG_EPMM0            (0x08U | (ENC_BANK1 << 5))
#define ENC_REG_EPMM1            (0x09U | (ENC_BANK1 << 5))
#define ENC_REG_EPMM2            (0x0AU | (ENC_BANK1 << 5))
#define ENC_REG_EPMM3            (0x0BU | (ENC_BANK1 << 5))
#define ENC_REG_EPMM4            (0x0CU | (ENC_BANK1 << 5))
#define ENC_REG_EPMM5            (0x0DU | (ENC_BANK1 << 5))
#define ENC_REG_EPMM6            (0x0EU | (ENC_BANK1 << 5))
#define ENC_REG_EPMM7            (0x0FU | (ENC_BANK1 << 5))
#define ENC_REG_EPMCSL           (0x10U | (ENC_BANK1 << 5))
#define ENC_REG_EPMCSH           (0x11U | (ENC_BANK1 << 5))
#define ENC_REG_EPMOL            (0x14U | (ENC_BANK1 << 5))
#define ENC_REG_EPMOH            (0x15U | (ENC_BANK1 << 5))
#define ENC_REG_ERXFCON          (0x18U | (ENC_BANK1 << 5))
#define ENC_REG_EPKTCNT          (0x19U | (ENC_BANK1 << 5))

/* ERXFCON bits */
#define ENC_ERXFCON_UCEN         (1U << 7)
#define ENC_ERXFCON_ANDOR        (1U << 6)
#define ENC_ERXFCON_CRCEN        (1U << 5)
#define ENC_ERXFCON_PMEN         (1U << 4)
#define ENC_ERXFCON_MPEN         (1U << 3)
#define ENC_ERXFCON_HTEN         (1U << 2)
#define ENC_ERXFCON_MCEN         (1U << 1)
#define ENC_ERXFCON_BCEN         (1U << 0)

/* ================ BANK 2 (MAC/MII) ============ */
#define ENC_REG_MACON1           (0x00U | (ENC_BANK2 << 5) | ENC_MAC_MII)
#define ENC_REG_MACON3           (0x02U | (ENC_BANK2 << 5) | ENC_MAC_MII)
#define ENC_REG_MACON4           (0x03U | (ENC_BANK2 << 5) | ENC_MAC_MII)
#define ENC_REG_MABBIPG          (0x04U | (ENC_BANK2 << 5) | ENC_MAC_MII)
#define ENC_REG_MAIPGL           (0x06U | (ENC_BANK2 << 5) | ENC_MAC_MII)
#define ENC_REG_MAIPGH           (0x07U | (ENC_BANK2 << 5) | ENC_MAC_MII)
#define ENC_REG_MAMXFLL          (0x0AU | (ENC_BANK2 << 5) | ENC_MAC_MII)
#define ENC_REG_MAMXFLH          (0x0BU | (ENC_BANK2 << 5) | ENC_MAC_MII)
#define ENC_REG_MICMD            (0x12U | (ENC_BANK2 << 5) | ENC_MAC_MII)
#define ENC_REG_MIREGADR         (0x14U | (ENC_BANK2 << 5) | ENC_MAC_MII)
#define ENC_REG_MIWRL            (0x16U | (ENC_BANK2 << 5) | ENC_MAC_MII)
#define ENC_REG_MIWRH            (0x17U | (ENC_BANK2 << 5) | ENC_MAC_MII)
#define ENC_REG_MIRDL            (0x18U | (ENC_BANK2 << 5) | ENC_MAC_MII)
#define ENC_REG_MIRDH            (0x19U | (ENC_BANK2 << 5) | ENC_MAC_MII)

/* MACON1 bits */
#define ENC_MACON1_TXPAUS        (1U << 3)
#define ENC_MACON1_RXPAUS        (1U << 2)
#define ENC_MACON1_PASSALL       (1U << 1)
#define ENC_MACON1_MARXEN        (1U << 0)

/* MACON3 bits */
#define ENC_MACON3_PADCFG2       (1U << 7)
#define ENC_MACON3_PADCFG1       (1U << 6)
#define ENC_MACON3_PADCFG0       (1U << 5)
#define ENC_MACON3_TXCRCEN       (1U << 4)
#define ENC_MACON3_PHDREN        (1U << 3)
#define ENC_MACON3_HFRMEN        (1U << 2)
#define ENC_MACON3_FRMLNEN       (1U << 1)
#define ENC_MACON3_FULDPX        (1U << 0)

/* MACON4 bits */
#define ENC_MACON4_DEFER         (1U << 6)
#define ENC_MACON4_BPEN          (1U << 5)
#define ENC_MACON4_NOBKOFF       (1U << 4)

/* MICMD bits */
#define ENC_MICMD_MIISCAN        (1U << 1)
#define ENC_MICMD_MIIRD          (1U << 0)

/* ================ BANK 3 ====================== */
#define ENC_REG_MAADR5           (0x00U | (ENC_BANK3 << 5) | 0x80U)
#define ENC_REG_MAADR6           (0x01U | (ENC_BANK3 << 5) | 0x80U)
#define ENC_REG_MAADR3           (0x02U | (ENC_BANK3 << 5) | 0x80U)
#define ENC_REG_MAADR4           (0x03U | (ENC_BANK3 << 5) | 0x80U)
#define ENC_REG_MAADR1           (0x04U | (ENC_BANK3 << 5) | 0x80U)
#define ENC_REG_MAADR2           (0x05U | (ENC_BANK3 << 5) | 0x80U)
#define ENC_REG_EBSTSD           (0x06U | (ENC_BANK3 << 5))
#define ENC_REG_EBSTCON          (0x07U | (ENC_BANK3 << 5))
#define ENC_REG_EBSTCSL          (0x08U | (ENC_BANK3 << 5))
#define ENC_REG_EBSTCSH          (0x09U | (ENC_BANK3 << 5))
#define ENC_REG_MISTAT           (0x0AU | (ENC_BANK3 << 5) | 0x80U)
#define ENC_REG_EREVID           (0x12U | (ENC_BANK3 << 5))
#define ENC_REG_ECOCON           (0x15U | (ENC_BANK3 << 5))
#define ENC_REG_EFLOCON          (0x17U | (ENC_BANK3 << 5))
#define ENC_REG_EPAUSL           (0x18U | (ENC_BANK3 << 5))
#define ENC_REG_EPAUSH           (0x19U | (ENC_BANK3 << 5))

/* EBSTCON bits */
#define ENC_EBSTCON_PSV2         (1U << 7)
#define ENC_EBSTCON_PSV1         (1U << 6)
#define ENC_EBSTCON_PSV0         (1U << 5)
#define ENC_EBSTCON_PSEL         (1U << 4)
#define ENC_EBSTCON_TMSEL1       (1U << 3)
#define ENC_EBSTCON_TMSEL0       (1U << 2)
#define ENC_EBSTCON_TME          (1U << 1)
#define ENC_EBSTCON_BISTST       (1U << 0)

/* MISTAT bits */
#define ENC_MISTAT_NVALID        (1U << 2)
#define ENC_MISTAT_SCAN          (1U << 1)
#define ENC_MISTAT_BUSY          (1U << 0)

/* ================ PHY (via MII) ============== */
#define ENC_PHY_PHCON1           0x00U
#define ENC_PHY_PHSTAT1          0x01U
#define ENC_PHY_PHID1            0x02U
#define ENC_PHY_PHID2            0x03U
#define ENC_PHY_PHCON2           0x10U
#define ENC_PHY_PHSTAT2          0x11U
#define ENC_PHY_PHIE             0x12U
#define ENC_PHY_PHIR             0x13U
#define ENC_PHY_PHLCON           0x14U

/* PHCON1 bits */
#define ENC_PHCON1_PRST          (1U << 15)
#define ENC_PHCON1_PLOOPBK       (1U << 14)
#define ENC_PHCON1_PPWRSV        (1U << 11)
#define ENC_PHCON1_PDPXMD        (1U << 8)

/* PHSTAT1 bits */
#define ENC_PHSTAT1_PFDPX        (1U << 12)
#define ENC_PHSTAT1_PHDPX        (1U << 11)
#define ENC_PHSTAT1_LLSTAT       (1U << 2)
#define ENC_PHSTAT1_JBSTAT       (1U << 1)

/* PHCON2 bits */
#define ENC_PHCON2_FRCLNK        (1U << 14)
#define ENC_PHCON2_TXDIS         (1U << 13)
#define ENC_PHCON2_JABBER        (1U << 10)
#define ENC_PHCON2_HDLDIS        (1U << 8)

/* PHSTAT2 bits */
#define ENC_PHSTAT2_TXSTAT       (1U << 13)
#define ENC_PHSTAT2_RXSTAT       (1U << 12)
#define ENC_PHSTAT2_COLSTAT      (1U << 11)
#define ENC_PHSTAT2_LSTAT        (1U << 10)
#define ENC_PHSTAT2_DPXSTAT      (1U << 9)
#define ENC_PHSTAT2_PLRITY       (1U << 5)

/* PHIE bits */
#define ENC_PHIE_PLNKIE          (1U << 4)
#define ENC_PHIE_PGEIE           (1U << 1)

/* PHIR bits */
#define ENC_PHIR_PLNKIE          (1U << 4)
#define ENC_PHIR_PGIF            (1U << 2)

/* PHLCON bits */
#define ENC_PHLCON_LACFG3        (1U << 11)
#define ENC_PHLCON_LACFG2        (1U << 10)
#define ENC_PHLCON_LACFG1        (1U << 9)
#define ENC_PHLCON_LACFG0        (1U << 8)
#define ENC_PHLCON_LBCFG3        (1U << 7)
#define ENC_PHLCON_LBCFG2        (1U << 6)
#define ENC_PHLCON_LBCFG1        (1U << 5)
#define ENC_PHLCON_LBCFG0        (1U << 4)
#define ENC_PHLCON_LFRQ1         (1U << 3)
#define ENC_PHLCON_LFRQ0         (1U << 2)
#define ENC_PHLCON_STRCH         (1U << 1)

/* ============== RX PACKET HEADER ============= */
#define ENC_RXHDR_NEXTPTR_L      0U
#define ENC_RXHDR_NEXTPTR_H      1U
#define ENC_RXHDR_LEN_L          2U
#define ENC_RXHDR_LEN_H          3U
#define ENC_RXHDR_STATUS_L       4U
#define ENC_RXHDR_STATUS_H       5U
#define ENC_RXHDR_SIZE           6U

/* Receive Status Vector (RSV) bits */

#define ENC_RSV_VLAN_TYPE            (1U << 15)
#define ENC_RSV_UNKNOWN_OPCODE       (1U << 14)
#define ENC_RSV_PAUSE_CONTROL_FRAME  (1U << 13)
#define ENC_RSV_CONTROL_FRAME        (1U << 12)
#define ENC_RSV_DRIBBLE_NIBBLE       (1U << 11)
#define ENC_RSV_BROADCAST_PACKET     (1U << 10)
#define ENC_RSV_MULTICAST_PACKET     (1U <<  9)
#define ENC_RSV_RECEIVED_OK          (1U <<  8)
#define ENC_RSV_LENGTH_OUT_OF_RANGE  (1U <<  6)
#define ENC_RSV_LENGTH_CHECK_ERROR   (1U <<  5)
#define ENC_RSV_CRC_ERROR            (1U <<  4)
#define ENC_RSV_CARRIER_EVENT        (1U <<  2)
#define ENC_RSV_DROP_EVENT           (1U <<  0)

#endif /* __ENC_DEF__ */
