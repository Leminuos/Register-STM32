#ifndef __HAL_GPIO__
#define __HAL_GPIO__

#include <stdint.h>
#include "stm32f103.h"

typedef enum
{ 
  GPIO_Speed_10MHz  = 0x01,
  GPIO_Speed_2MHz   = 0x02,
  GPIO_Speed_50MHz  = 0x03
} GPIOSpeed_TypeDef;

typedef enum
{
  GPIO_Mode_AIN = 0x00,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_AF_PP = 0x18,
  GPIO_Mode_AF_OD = 0x1C
} GPIOMode_TypeDef;

typedef struct
{
  uint16_t GPIO_Pin;
  GPIOSpeed_TypeDef GPIO_Speed;
  GPIOMode_TypeDef GPIO_Mode;
} GPIO_InitTypeDef;

#define GPIO_PIN_0          ((uint16_t) 0x0001U)
#define GPIO_PIN_1          ((uint16_t) 0x0002U)
#define GPIO_PIN_2          ((uint16_t) 0x0004U)
#define GPIO_PIN_3          ((uint16_t) 0x0008U)
#define GPIO_PIN_4          ((uint16_t) 0x0010U)
#define GPIO_PIN_5          ((uint16_t) 0x0020U)
#define GPIO_PIN_6          ((uint16_t) 0x0040U)
#define GPIO_PIN_7          ((uint16_t) 0x0080U)
#define GPIO_PIN_8          ((uint16_t) 0x0100U)
#define GPIO_PIN_9          ((uint16_t) 0x0200U)
#define GPIO_PIN_10         ((uint16_t) 0x0400U)
#define GPIO_PIN_11         ((uint16_t) 0x0800U)
#define GPIO_PIN_12         ((uint16_t) 0x1000U)
#define GPIO_PIN_13         ((uint16_t) 0x2000U)
#define GPIO_PIN_14         ((uint16_t) 0x4000U)
#define GPIO_PIN_15         ((uint16_t) 0x8000U)

#endif /* __HAL_GPIO__ */
