#ifndef __DRIVER_UART__
#define __DRIVER_UART__

#include "stm32f103.h"
#include "stm32_hal_util.h"

void USART_Transmiter(USART_Typedef* USARTx, uint8_t data);
void USART_TransmiterString(USART_Typedef* USARTx, uint8_t* str);

#endif /* __DRIVER_UART__ */
