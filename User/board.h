#ifndef BOARD_H
#define BOARD_H

#include "ch32v00x.h"
#include "ch32v00x_gpio.h"

// 输入信号
#define X1_GPIO_PORT (GPIOD)
#define X1_GPIO_PIN (GPIO_Pin_5)

// 输出信号
#define Y1_GPIO_PORT (GPIOD)
#define Y1_GPIO_PIN (GPIO_Pin_6)

// 输出信号
#define LED_GPIO_PORT (GPIOD)
#define LED_GPIO_PIN (GPIO_Pin_3)

// 串口发送
#define RS485_TX_GPIO_PORT (GPIOC)
#define RS485_TX_GPIO_PIN (GPIO_Pin_0)

// 串口接收
#define RS485_RX_GPIO_PORT (GPIOC)
#define RS485_RX_GPIO_PIN (GPIO_Pin_1)

// 串口收发使能
#define RS485_DE_GPIO_PORT (GPIOD)
#define RS485_DE_GPIO_PIN (GPIO_Pin_4)

#define set_rs485_de_enable() (GPIO_SetBits(RS485_DE_GPIO_PORT, RS485_DE_GPIO_PIN))
#define set_rs485_de_disable() (GPIO_ResetBits(RS485_DE_GPIO_PORT, RS485_DE_GPIO_PIN))

#define read_x1() (GPIO_ReadInputDataBit(X1_GPIO_PORT, X1_GPIO_PIN))

#define write_y1(val) (GPIO_WriteBit(Y1_GPIO_PORT, Y1_GPIO_PIN, (val)))
#define read_y1() (GPIO_ReadOutputDataBit(Y1_GPIO_PORT, Y1_GPIO_PIN))

void board_gpio_init(void);
void board_gpio_set_x1_mode(uint8_t mode);

#endif // BOARD_H
