#ifndef __STM32_DRIVER_DMA__
#define __STM32_DRIVER_DMA__

#include "stm32f103.h"

typedef struct
{
    uint32_t DMA_PeripheralBaseAddr;
    uint32_t DMA_MemoryBaseAddr;
    uint32_t DMA_DIR;
    uint32_t DMA_BufferSize;
    uint32_t DMA_PeripheralInc;
    uint32_t DMA_MemoryInc;
    uint32_t DMA_PeripheralDataSize;
    uint32_t DMA_MemoryDataSize;
    uint32_t DMA_Mode;
    uint32_t DMA_Priority;
    uint32_t DMA_M2M;
} DMA_InitTypeDef;

/* DMA_data_transfer_direction */
#define DMA_DIR_PeripheralDST               ((uint32_t)0x00000010)
#define DMA_DIR_PeripheralSRC               ((uint32_t)0x00000000)

/* DMA_peripheral_incremented_mode */
#define DMA_PeripheralInc_Enable            ((uint32_t)0x00000040)
#define DMA_PeripheralInc_Disable           ((uint32_t)0x00000000)

/* DMA_memory_incremented_mode */
#define DMA_MemoryInc_Enable                ((uint32_t)0x00000080)
#define DMA_MemoryInc_Disable               ((uint32_t)0x00000000)

/* DMA_peripheral_data_size */
#define DMA_PeripheralDataSize_Byte         ((uint32_t)0x00000000)
#define DMA_PeripheralDataSize_HalfWord     ((uint32_t)0x00000100)
#define DMA_PeripheralDataSize_Word         ((uint32_t)0x00000200)

/* DMA_memory_data_size */
#define DMA_MemoryDataSize_Byte             ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord         ((uint32_t)0x00000400)
#define DMA_MemoryDataSize_Word             ((uint32_t)0x00000800)

/* DMA_circular_normal_mode */
#define DMA_Mode_Circular                   ((uint32_t)0x00000020)
#define DMA_Mode_Normal                     ((uint32_t)0x00000000)

/* DMA_priority_level */
#define DMA_Priority_VeryHigh               ((uint32_t)0x00003000)
#define DMA_Priority_High                   ((uint32_t)0x00002000)
#define DMA_Priority_Medium                 ((uint32_t)0x00001000)
#define DMA_Priority_Low                    ((uint32_t)0x00000000)

/* DMA_memory_to_memory */
#define DMA_M2M_Enable                      ((uint32_t)0x00004000)
#define DMA_M2M_Disable                     ((uint32_t)0x00000000)

/* DMA_interrupts_definition */
#define DMA_IT_TC                           ((uint32_t)0x00000002)
#define DMA_IT_HT                           ((uint32_t)0x00000004)
#define DMA_IT_TE                           ((uint32_t)0x00000008)

#define DMA_IT_GL_MASK(ch)                  (1u << (4*((ch)-1) + 0))
#define DMA_IT_TC_MASK(ch)                  (1u << (4*((ch)-1) + 1))
#define DMA_IT_HT_MASK(ch)                  (1u << (4*((ch)-1) + 2))
#define DMA_IT_TE_MASK(ch)                  (1u << (4*((ch)-1) + 3))

/* DMA_flags_definition */
#define DMA_FLAG_GL(ch)                     (1u << (4*((ch)-1) + 0))
#define DMA_FLAG_TC(ch)                     (1u << (4*((ch)-1) + 1))
#define DMA_FLAG_HT(ch)                     (1u << (4*((ch)-1) + 2))
#define DMA_FLAG_TE(ch)                     (1u << (4*((ch)-1) + 3))

static inline void DMA_ClearITPendingBit(DMA_Typedef* DMAx, uint32_t DMAy_IT)
{
    DMAx->DMA_IFCR.WORD = DMAy_IT;
}

static inline void DMA_ClearFlag(DMA_Typedef* DMAx, uint32_t DMAy_FLAG)
{
    DMAx->DMA_IFCR.WORD = DMAy_FLAG;
}

static inline void DMA_EnableChannel(DMA_Typedef* DMAx, uint8_t ch)
{
    DMAx->Channel[ch - 1].DMA_CCR.BITS.EN = 0x01;
}

static inline void DMA_DisableChannel(DMA_Typedef* DMAx, uint8_t ch)
{
    DMAx->Channel[ch - 1].DMA_CCR.BITS.EN = 0x00;
}

static inline void DMA_EnableIT(DMA_Typedef* DMAx, uint8_t ch, uint32_t DMA_IT)
{
    DMAx->Channel[ch - 1].DMA_CCR.WORD |= DMA_IT;
}

static inline void DMA_DisableIT(DMA_Typedef* DMAx, uint8_t ch, uint32_t DMA_IT)
{
    DMAx->Channel[ch - 1].DMA_CCR.WORD &= ~DMA_IT;
}

static inline void DMA_SetCurrDataCounter(DMA_Typedef* DMAx, uint8_t ch, uint16_t DataNumber)
{
    DMAx->Channel[ch - 1].DMA_CNDTR = DataNumber;
}

static inline uint16_t DMA_GetCurrDataCounter(DMA_Typedef* DMAx, uint8_t ch)
{
    return ((uint16_t)(DMAx->Channel[ch - 1].DMA_CNDTR));
}

extern void DMA_DeInit(DMA_Typedef* DMAx, uint8_t ch);
extern void DMA_StructInit(DMA_InitTypeDef *DMA_InitStruct);
extern void DMA_Init(DMA_Typedef* DMAx, uint8_t ch, DMA_InitTypeDef *DMA_InitStruct);
extern uint8_t DMA_GetITStatus(DMA_Typedef* DMAx, uint32_t DMAy_IT);
extern uint8_t DMA_GetFlagStatus(DMA_Typedef* DMAx, uint32_t DMAy_FLAG);

#endif /* __STM32_DRIVER_DMA__ */

