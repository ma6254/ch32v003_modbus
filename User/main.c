/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 Multiprocessor communication mode routine:
 Master:USART1_Tx(PD5)\USART1_Rx(PD6).
 This routine demonstrates that USART1 receives the data sent by CH341 and inverts
 it and sends it (baud rate 115200).

 Hardware connection:PD5 -- Rx
                     PD6 -- Tx

*/

#include "debug.h"
#include "config.h"
#include "boot.h"
#include "board.h"
#include "soft_timer.h"
#include "modbus.h"
#include "modbus_porting.h"

/* Global define */

/* Global Variable */
soft_timer_t test_timer;
volatile uint8_t aaa = 0;

/*******************************************************************************
 * @brief 主循环
 * @param None
 * @return None
 ******************************************************************************/
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    Delay_Init();
    read_config_data();
    board_gpio_init();
    modbus_init();
    modbus_porting_init();

    // 1ms tick
    NVIC_EnableIRQ(SysTicK_IRQn);
    SysTick->SR &= ~(1 << 0);
    SysTick->CMP = SystemCoreClock / (1000000 / 10) - 1;
    SysTick->CNT = 0;
    SysTick->CTLR = 0xF;

    soft_timer_init(&test_timer, 1000000, 0);

    while (1)
    {
        modbus_process();

        if (soft_timer_check(&test_timer))
        {
            if (aaa)
            {
                aaa = 0;
                GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);
            }
            else
            {
                aaa = 1;
                GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN);
            }
        }
    }
}

/*******************************************************************************
 * @brief 系统滴答中断
 * @param None
 * @return None
 ******************************************************************************/
void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SysTick_Handler(void)
{
    soft_timer_inc(10);
    modbus_timeout_inc(10);
    SysTick->SR = 0;
}
