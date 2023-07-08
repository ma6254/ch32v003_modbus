#include "board.h"
#include "boot.h"

void board_gpio_set_x1_mode(uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    switch (mode)
    {
    case 0:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
        break;
    case 1:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        break;
    default:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        break;
    }

    GPIO_InitStructure.GPIO_Pin = X1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(X1_GPIO_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
 * @brief 引脚初始化
 * @param None
 * @return None
 ******************************************************************************/
void board_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // UART1 Remap
    // GPIO_PinRemapConfig(GPIO_FullRemap_USART1, DISABLE); // PD5, PD6
    // GPIO_PinRemapConfig(GPIO_PartialRemap1_USART1, ENABLE); // PD0, PD1
    // GPIO_PinRemapConfig(GPIO_PartialRemap2_USART1, ENABLE); // PD6 PD5
    GPIO_PinRemapConfig(GPIO_FullRemap_USART1, ENABLE); // PC0 PC1

    // 串口发送
    GPIO_InitStructure.GPIO_Pin = RS485_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RS485_TX_GPIO_PORT, &GPIO_InitStructure);

    // 串口接收
    GPIO_InitStructure.GPIO_Pin = RS485_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RS485_RX_GPIO_PORT, &GPIO_InitStructure);

    // 串口使能输出
    GPIO_InitStructure.GPIO_Pin = RS485_DE_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RS485_DE_GPIO_PORT, &GPIO_InitStructure);

    // LED
    GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

    // 输入1
    GPIO_InitStructure.GPIO_Pin = X1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(X1_GPIO_PORT, &GPIO_InitStructure);
    board_gpio_set_x1_mode(CFG.x1_mode);

    // 输出1
    GPIO_InitStructure.GPIO_Pin = Y1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Y1_GPIO_PORT, &GPIO_InitStructure);
}
