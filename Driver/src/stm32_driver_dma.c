#include "stm32_driver_dma.h"

#define DMA_CHANNEL_IT_FLAG_MASK(ch)    (DMA_FLAG_GL(ch) | DMA_FLAG_GL(ch) | DMA_FLAG_GL(ch) | DMA_FLAG_GL(ch))
#define CFGR_CLEAR_Mask                 ((uint32_t)0xFFFF800F)

uint8_t DMA_GetITStatus(DMA_Typedef* DMAx, uint32_t DMAy_IT)
{
    uint8_t bitstatus = 0;
    uint32_t tmpreg = 0;

    tmpreg = DMAx->DMA_ISR.WORD;

    if((tmpreg & DMAy_IT) != 0)
    {
        bitstatus = 1;
    }
    else
    {
        bitstatus = 0;
    }

    return bitstatus;
}

uint8_t DMA_GetFlagStatus(DMA_Typedef* DMAx, uint32_t DMAy_FLAG)
{
    uint8_t     bitstatus = 0;
    uint32_t    tmpreg = 0;

    tmpreg = DMAx->DMA_ISR.WORD;

    if((tmpreg & DMAy_FLAG) != 0)
    {
        bitstatus = 1;
    }
    else
    {
        bitstatus = 0;
    }

    return bitstatus;
}

void DMA_StructInit(DMA_InitTypeDef *DMA_InitStruct)
{
    DMA_InitStruct->DMA_PeripheralBaseAddr = 0;
    DMA_InitStruct->DMA_MemoryBaseAddr = 0;
    DMA_InitStruct->DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct->DMA_BufferSize = 0;
    DMA_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct->DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct->DMA_Priority = DMA_Priority_Low;
    DMA_InitStruct->DMA_M2M = DMA_M2M_Disable;
}

void DMA_Init(DMA_Typedef* DMAx, uint8_t ch, DMA_InitTypeDef *DMA_InitStruct)
{
    uint32_t tmpreg = 0;

    tmpreg = DMAx->Channel[ch - 1].DMA_CCR.WORD;
    tmpreg &= CFGR_CLEAR_Mask;
    tmpreg |= DMA_InitStruct->DMA_DIR | DMA_InitStruct->DMA_Mode |
              DMA_InitStruct->DMA_PeripheralInc | DMA_InitStruct->DMA_MemoryInc |
              DMA_InitStruct->DMA_PeripheralDataSize | DMA_InitStruct->DMA_MemoryDataSize |
              DMA_InitStruct->DMA_Priority | DMA_InitStruct->DMA_M2M;

    DMAx->Channel[ch - 1].DMA_CCR.WORD = tmpreg;
    DMAx->Channel[ch - 1].DMA_CNDTR = DMA_InitStruct->DMA_BufferSize;
    DMAx->Channel[ch - 1].DMA_CPAR = DMA_InitStruct->DMA_PeripheralBaseAddr;
    DMAx->Channel[ch - 1].DMA_CMAR = DMA_InitStruct->DMA_MemoryBaseAddr;
}

void DMA_DeInit(DMA_Typedef* DMAx, uint8_t ch)
{
    DMAx->Channel[ch - 1].DMA_CCR.WORD = 0;
    DMAx->Channel[ch - 1].DMA_CNDTR = 0;
    DMAx->Channel[ch - 1].DMA_CMAR = 0;
    DMAx->Channel[ch - 1].DMA_CPAR = 0;
    DMAx->DMA_IFCR.WORD |= DMA_CHANNEL_IT_FLAG_MASK(ch);
}
