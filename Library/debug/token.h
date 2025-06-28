#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <stdint.h>
#include "stm32_driver_uart.h"

extern uint32_t GetCounterTick(void);

#define CONFIG_LOG_COLOR            1
#define DEBUG_MODE                  1
#define WRITE_OUT(_data)            USART_Transmiter(USART2, _data)
#define TIMESTAMP                   GetCounterTick()

#endif /* __TOKEN_H__ */

