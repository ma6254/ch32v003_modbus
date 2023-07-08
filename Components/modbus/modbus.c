#include <string.h>
#include "modbus.h"

modbus_t MODBUS;
uint8_t rs485_rx_tmp;

#define MODBUS_ADDR (CFG.comm_addr)

/*******************************************************************************
 * @brief 发送Modbus返回错误帧
 * @param None
 * @return None
 ******************************************************************************/
void modbus_resp_err(modbus_err_t err)
{
    uint16_t calc_crc;

    MODBUS.tx_buf[0] = MODBUS_ADDR;
    MODBUS.tx_buf[1] = MODBUS.rx_tmp.func | 0x80;
    MODBUS.tx_buf[2] = err;

    calc_crc = modbus_crc16(MODBUS.tx_buf, 3);
    pack_little_16(MODBUS.tx_buf + 3, calc_crc);

    MODBUS.tx_len = 5;
    MODBUS.tx_cnt = 0;
    set_rs485_de_enable();
    MODBUS.tx_done = false;
    USART_ITConfig(MODBUS_UART, USART_IT_TC, ENABLE);

    // 启动发送
}

/*******************************************************************************
 * @brief Modbus接收处理
 * @param None
 * @return None
 ******************************************************************************/
void modbus_rx_process(void)
{
    uint16_t rx_crc = unpack_little_16(MODBUS.rx_buf + MODBUS.rx_cnt - 2);
    uint16_t calc_crc = modbus_crc16(MODBUS.rx_buf, MODBUS.rx_cnt - 2);

    // 检查校验码是否符合
    if (rx_crc != calc_crc)
        return;

    memset(&MODBUS.rx_tmp, 0, sizeof(MODBUS.rx_tmp));

    uint8_t rx_addr = MODBUS.rx_buf[0];
    MODBUS.rx_tmp.func = MODBUS.rx_buf[1];

    // 检查地址字是否符合
    if ((rx_addr != MODBUS_BOARDCAST_ADDR) && (rx_addr != MODBUS_ADDR))
    {
        return;
    }

    // 地址范围为1-247
    if (rx_addr > 247)
        return;

    // 根据地址字是否为0，判定该帧是否为广播帧
    if (rx_addr == MODBUS_BOARDCAST_ADDR)
        MODBUS.is_boardcast_frame = true;
    else
        MODBUS.is_boardcast_frame = false;

    switch (MODBUS.rx_tmp.func)
    {
    case MODBUS_FUNC_R_READ_DIN:
    {
        /***********************************************************************
         * 2(0x02) 读取输入离散量
         **********************************************************************/
        uint8_t byte_count;
        uint16_t i;
        uint8_t err;
        uint8_t read_val;

        MODBUS.rx_tmp.start_addr = unpack_big_16(MODBUS.rx_buf + 2);
        MODBUS.rx_tmp.data_count = unpack_big_16(MODBUS.rx_buf + 4);

        if (MODBUS.rx_tmp.data_count > 64)
        {
            modbus_resp_err(MODBUS_ERR_INVALID_VAL);
            break;
        }

        // 计算字节数
        byte_count = MODBUS.rx_tmp.data_count / 8;
        if (MODBUS.rx_tmp.data_count % 8)
            byte_count += 1;

        memset(MODBUS.tx_buf + 3, 0x00, byte_count);

        for (i = 0; i < MODBUS.rx_tmp.data_count; i++)
        {
            err = modbus_read_din_val(MODBUS.rx_tmp.start_addr + i, &read_val);
            if (err)
            {
                modbus_resp_err(MODBUS_ERR_INVALID_ADDR);
                break;
            }

            if (read_val)
            {
                Set8Bit(*(MODBUS.tx_buf + 3 + (i / 8)), i % 8);
            }
        }

        uint16_t calc_crc;
        MODBUS.tx_buf[0] = MODBUS_ADDR;
        MODBUS.tx_buf[1] = MODBUS.rx_tmp.func;
        MODBUS.tx_buf[2] = byte_count;

        calc_crc = modbus_crc16(MODBUS.tx_buf, 3 + byte_count);
        pack_little_16(MODBUS.tx_buf + 3 + byte_count, calc_crc);

        MODBUS.tx_len = 5 + byte_count;
        MODBUS.tx_cnt = 0;
        set_rs485_de_enable();
        MODBUS.tx_done = false;
        USART_ITConfig(MODBUS_UART, USART_IT_TC, ENABLE);
    }
    break;
    case MODBUS_FUNC_R_COILS:
    {
        /***********************************************************************
         * 1(0x01) 读取线圈
         **********************************************************************/
        uint16_t start_addr;
        uint16_t data_count;
        uint8_t byte_count;
        uint16_t i;
        uint8_t err;
        uint8_t read_val;

        start_addr = unpack_big_16(MODBUS.rx_buf + 2);
        data_count = unpack_big_16(MODBUS.rx_buf + 4);

        if (data_count > 64)
        {
            modbus_resp_err(MODBUS_ERR_INVALID_VAL);
            break;
        }

        // 计算字节数
        byte_count = data_count / 8;
        if (data_count % 8)
            byte_count += 1;

        memset(MODBUS.tx_buf + 3, 0x00, byte_count);

        for (i = 0; i < data_count; i++)
        {
            err = modbus_get_coil_val(start_addr + i, &read_val);
            if (err)
            {
                modbus_resp_err(MODBUS_ERR_INVALID_ADDR);
                break;
            }

            if (read_val)
            {
                Set8Bit(*(MODBUS.tx_buf + 3 + (i / 8)), i % 8);
            }
        }

        uint16_t calc_crc;
        MODBUS.tx_buf[0] = MODBUS_ADDR;
        MODBUS.tx_buf[1] = MODBUS.rx_tmp.func;
        MODBUS.tx_buf[2] = byte_count;

        calc_crc = modbus_crc16(MODBUS.tx_buf, 3 + byte_count);
        pack_little_16(MODBUS.tx_buf + 3 + byte_count, calc_crc);

        MODBUS.tx_len = 5 + byte_count;
        MODBUS.tx_cnt = 0;
        set_rs485_de_enable();
        MODBUS.tx_done = false;
        USART_ITConfig(MODBUS_UART, USART_IT_TC, ENABLE);
    }
    break;
    case MODBUS_FUNC_W_SINGLE_COIL:
    {
        /***********************************************************************
         * 5(0x05) 写入单个线圈
         **********************************************************************/
        uint16_t start_addr;
        uint16_t w_val;
        uint8_t err;
        uint8_t set_val;

        start_addr = unpack_big_16(MODBUS.rx_buf + 2);
        w_val = unpack_big_16(MODBUS.rx_buf + 4);

        if (w_val == 0x0000)
            set_val = 0;
        else if (w_val == 0xFF00)
            set_val = 1;
        else
        {
            modbus_resp_err(MODBUS_ERR_EXEC);
            break;
        }

        err = modbus_set_coil_val(start_addr, set_val);
        if (err == true)
        {
            modbus_resp_err(MODBUS_ERR_EXEC);
            break;
        }

        uint16_t calc_crc;

        MODBUS.tx_buf[0] = MODBUS_ADDR;
        MODBUS.tx_buf[1] = MODBUS.rx_tmp.func;
        pack_big_16(MODBUS.tx_buf + 2, start_addr);

        if (set_val == 0x0000)
            pack_big_16(MODBUS.tx_buf + 4, 0x0000);
        else
            pack_big_16(MODBUS.tx_buf + 4, 0xFF00);

        calc_crc = modbus_crc16(MODBUS.tx_buf, 6);
        pack_little_16(MODBUS.tx_buf + 6, calc_crc);

        MODBUS.tx_len = 8;
        MODBUS.tx_cnt = 0;
        set_rs485_de_enable();
        MODBUS.tx_done = false;
        USART_ITConfig(MODBUS_UART, USART_IT_TC, ENABLE);
    }
    break;
    case MODBUS_FUNC_W_MUTI_COILS:
    {
        /***********************************************************************
         * 15(0x0F) 写入多个线圈
         **********************************************************************/
    }
    break;
    case MODBUS_FUNC_R_INPUT_REGS:
    {
        /***********************************************************************
         * 4(0x04) 读取输入寄存器
         **********************************************************************/
    }
    break;
    case MODBUS_FUNC_R_MUTI_REGS:
    {
        /***********************************************************************
         * 3(0x03) 读多个寄存器
         **********************************************************************/
        uint16_t start_addr;
        uint16_t data_count;
        uint8_t byte_count;
        uint16_t i;
        uint8_t err;
        uint16_t read_val;

        start_addr = unpack_big_16(MODBUS.rx_buf + 2);
        data_count = unpack_big_16(MODBUS.rx_buf + 4);

        if (data_count > 32)
        {
            modbus_resp_err(MODBUS_ERR_INVALID_VAL);
            break;
        }

        // 计算字节数
        byte_count = data_count * 2;

        memset(MODBUS.tx_buf + 3, 0x00, byte_count);

        for (i = 0; i < data_count; i++)
        {
            err = modbus_read_reg(start_addr + i, &read_val);
            if (err)
            {
                modbus_resp_err(MODBUS_ERR_INVALID_ADDR);
                break;
            }

            pack_big_16(MODBUS.tx_buf + 3 + (2 * i), read_val);
        }

        uint16_t calc_crc;
        MODBUS.tx_buf[0] = MODBUS_ADDR;
        MODBUS.tx_buf[1] = MODBUS.rx_tmp.func;
        MODBUS.tx_buf[2] = byte_count;

        calc_crc = modbus_crc16(MODBUS.tx_buf, 3 + byte_count);
        pack_little_16(MODBUS.tx_buf + 3 + byte_count, calc_crc);

        MODBUS.tx_len = 5 + byte_count;
        MODBUS.tx_cnt = 0;
        set_rs485_de_enable();
        MODBUS.tx_done = false;
        USART_ITConfig(MODBUS_UART, USART_IT_TC, ENABLE);
    }
    break;
    case MODBUS_FUNC_W_SINGLE_REG:
    {
        /***********************************************************************
         * 6(0x06) 写入单个寄存器
         **********************************************************************/
        uint16_t start_addr;
        uint16_t w_val;
        uint8_t err;

        start_addr = unpack_big_16(MODBUS.rx_buf + 2);
        w_val = unpack_big_16(MODBUS.rx_buf + 4);

        err = modbus_write_reg(start_addr, w_val);
        if (err == true)
        {
            modbus_resp_err(MODBUS_ERR_EXEC);
            break;
        }

        uint16_t calc_crc;

        MODBUS.tx_buf[0] = MODBUS_ADDR;
        MODBUS.tx_buf[1] = MODBUS.rx_tmp.func;
        pack_big_16(MODBUS.tx_buf + 2, start_addr);
        pack_big_16(MODBUS.tx_buf + 4, w_val);

        calc_crc = modbus_crc16(MODBUS.tx_buf, 6);
        pack_little_16(MODBUS.tx_buf + 6, calc_crc);

        MODBUS.tx_len = 8;
        MODBUS.tx_cnt = 0;
        set_rs485_de_enable();
        MODBUS.tx_done = false;
        USART_ITConfig(MODBUS_UART, USART_IT_TC, ENABLE);
    }
    break;
    default:
    {
        /***********************************************************************
         * 不支持的功能字
         **********************************************************************/
        modbus_resp_err(MODBUS_ERR_INVALID_FUNC);
    }
    break;
    }
}

void modbus_process(void)
{
    if (!MODBUS.tx_done)
        return;

    if (!MODBUS.rx_done)
        return;

    modbus_rx_process();
    MODBUS.rx_done = 0;
    MODBUS.rx_cnt = 0;
}

/*******************************************************************************
 * @brief Modbus初始化
 * @param None
 * @return None
 ******************************************************************************/
void modbus_init(void)
{
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    USART_InitStructure.USART_BaudRate = MODBUS_UART_BAUD;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(MODBUS_UART, &USART_InitStructure);
    USART_ITConfig(MODBUS_UART, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    memset(&MODBUS, 0, sizeof(MODBUS));
    MODBUS.tx_done = true;

    USART_Cmd(MODBUS_UART, ENABLE);
}

/*******************************************************************************
 * @brief 串口收发超时
 * @param time 单次时间
 * @return None
 ******************************************************************************/
void modbus_timeout_inc(uint32_t time)
{
    if (!MODBUS.rx_done && (MODBUS.rx_cnt != 0))
    {
        MODBUS.rx_timeout += time;
        if (MODBUS.rx_timeout >= MODBUS_RX_TIMEOUT)
            MODBUS.rx_done = true;
    }

    if (!MODBUS.tx_done && (MODBUS.tx_cnt == MODBUS.tx_len))
    {
        MODBUS.tx_timeout += time;
        if (MODBUS.tx_timeout >= MODBUS_TX_TIMEOUT)
            MODBUS.tx_done = true;
    }
}

/*******************************************************************************
 * @brief 串口中断
 * @param None
 * @return None
 ******************************************************************************/
void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART1_IRQHandler(void)
{
    // 接收中断
    if (USART_GetITStatus(MODBUS_UART, USART_IT_RXNE) != RESET)
    {
        rs485_rx_tmp = USART_ReceiveData(MODBUS_UART);
        if (MODBUS.rx_cnt == MODBUS_RX_BUF_SIZE)
            goto END_IT_RXNE;
        // 队列满了
        MODBUS.rx_buf[MODBUS.rx_cnt] = rs485_rx_tmp;
        MODBUS.rx_cnt++;
        MODBUS.rx_timeout = 0;
    }
END_IT_RXNE:

    // 发送中断
    if (USART_GetITStatus(MODBUS_UART, USART_IT_TC) != RESET)
    {
        if (MODBUS.tx_cnt == MODBUS.tx_len)
        {
            set_rs485_de_disable();
            USART_ITConfig(MODBUS_UART, USART_IT_TC, DISABLE);
            return;
        }

        USART_SendData(MODBUS_UART, MODBUS.tx_buf[MODBUS.tx_cnt]);
        MODBUS.tx_timeout = 0;
        MODBUS.tx_cnt++;
    }
    // END_IT_TC:
}

uint16_t modbus_crc16(uint8_t *nData, uint16_t wLength)
{
    static const uint16_t wCRCTable[] = {
        0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
        0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
        0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
        0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
        0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
        0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
        0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
        0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
        0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
        0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
        0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
        0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
        0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
        0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
        0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
        0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
        0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
        0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
        0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
        0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
        0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
        0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
        0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
        0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
        0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
        0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
        0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
        0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
        0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
        0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
        0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
        0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040};

    uint8_t nTemp;
    uint16_t wCRCWord = 0xFFFF;

    while (wLength--)
    {
        nTemp = *nData++ ^ wCRCWord;
        wCRCWord >>= 8;
        wCRCWord ^= wCRCTable[nTemp];
    }
    return wCRCWord;
}

/*******************************************************************************
 * @brief Modbus查找该地址上是否有定义离散输入
 * @param p_coil 接收线圈指针
 * @param addr 线圈地址
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_din_find_by_addr(p_modbus_coil_t *p_coil, uint16_t addr)
{
    uint16_t i = 0;
    for (i = 0; i < MODBUS.dins_cnt; i++)
    {
        if (MODBUS.dins[i].addr == addr)
        {
            if (p_coil != 0)
                *p_coil = &MODBUS.dins[i];
            return false;
        }
    }

    return true;
}

/*******************************************************************************
 * @brief Modbus查找该地址上是否有定义线圈
 * @param p_coil 接收线圈指针
 * @param addr 线圈地址
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_coil_find_by_addr(p_modbus_coil_t *p_coil, uint16_t addr)
{
    uint16_t i = 0;
    for (i = 0; i < MODBUS.coils_cnt; i++)
    {
        if (MODBUS.coils[i].addr == addr)
        {
            if (p_coil != 0)
                *p_coil = &MODBUS.coils[i];
            return false;
        }
    }

    return true;
}

/*******************************************************************************
 * @brief Modbus查找该地址上是否有定义输入寄存器
 * @param p_reg 接收寄存器指针
 * @param addr 寄存器地址
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_input_reg_find_by_addr(p_modbus_reg_t *p_reg, uint16_t addr)
{
    uint16_t i = 0;
    for (i = 0; i < MODBUS.input_regs_cnt; i++)
    {
        if (MODBUS.input_regs[i].addr == addr)
        {

            if (p_reg != 0)
                *p_reg = &MODBUS.input_regs[i];
            return false;
        }
    }

    return true;
}

/*******************************************************************************
 * @brief Modbus查找该地址上是否有定义寄存器
 * @param p_reg 接收寄存器指针
 * @param addr 寄存器地址
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_reg_find_by_addr(p_modbus_reg_t *p_reg, uint16_t addr)
{
    uint16_t i = 0;
    for (i = 0; i < MODBUS.regs_cnt; i++)
    {
        if (MODBUS.regs[i].addr == addr)
        {

            if (p_reg != 0)
                *p_reg = &MODBUS.regs[i];
            return false;
        }
    }

    return true;
}

/*******************************************************************************
 * @brief Modbus注册线圈，映射方式为指针
 * @param addr 线圈地址
 * @param val 数据指针
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_register_coil_pointer(uint16_t addr, uint8_t *val)
{
    // 定义的线圈满了
    if (MODBUS.coils_cnt == MODBUS_MAX_COIL_COUNT)
        return true;

    // 检查地址是否重复
    if (modbus_coil_find_by_addr(0, addr) == false)
        return true;

    modbus_coil_t coil;
    memset(&coil, 0, sizeof(coil));
    coil.addr = addr;
    coil.remap_type = MODBUS_VAL_REMAP_POINTER;
    coil.remap_val = val;
    memcpy(&MODBUS.coils[MODBUS.coils_cnt], &coil, sizeof(modbus_coil_t));
    MODBUS.coils_cnt++;

    return false;
}

/*******************************************************************************
 * @brief Modbus注册线圈，映射方式为回调函数
 * @param addr 线圈地址
 * @param read_fn 读取函数
 * @param write_fn 写入函数
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_register_coil_callback(uint16_t addr,
                                      modbus_read_coil_cb_t read_fn,
                                      modbus_write_coil_cb_t write_fn)
{

    // 定义的寄存器满了
    if (MODBUS.coils_cnt == MODBUS_MAX_COIL_COUNT)
        return true;

    // 检查地址是否重复
    if (modbus_coil_find_by_addr(0, addr) == false)
        return true;

    modbus_coil_t coil;
    memset(&coil, 0, sizeof(coil));
    coil.addr = addr;
    coil.remap_type = MODBUS_VAL_REMAP_FUNC;
    coil.read_fn = read_fn;
    coil.write_fn = write_fn;
    memcpy(&MODBUS.coils[MODBUS.coils_cnt], &coil, sizeof(modbus_coil_t));
    MODBUS.coils_cnt++;

    return false;
}

/*******************************************************************************
 * @brief Modbus注册离散输入，映射方式为回调函数
 * @param addr 线圈地址
 * @param read_fn 读取函数
 * @param write_fn 写入函数
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_register_din_callback(uint16_t addr,
                                     modbus_read_coil_cb_t read_fn)
{

    // 定义的寄存器满了
    if (MODBUS.dins_cnt == MODBUS_MAX_DIN_COUNT)
        return true;

    // 检查地址是否重复
    if (modbus_din_find_by_addr(0, addr) == false)
        return true;

    modbus_coil_t coil;
    memset(&coil, 0, sizeof(coil));
    coil.addr = addr;
    coil.remap_type = MODBUS_VAL_REMAP_FUNC;
    coil.read_fn = read_fn;
    coil.write_fn = 0;
    memcpy(&MODBUS.dins[MODBUS.dins_cnt], &coil, sizeof(modbus_coil_t));
    MODBUS.dins_cnt++;

    return false;
}

/*******************************************************************************
 * @brief Modbus注册输入寄存器，映射方式为指针
 * @param addr 寄存器地址
 * @param val 数据指针
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_register_input_reg_pointer(uint16_t addr, uint16_t *val)
{
    // 定义的寄存器满了
    if (MODBUS.input_regs_cnt == MODBUS_MAX_INPUT_REG_COUNT)
        return true;

    // 检查地址是否重复
    if (modbus_input_reg_find_by_addr(0, addr) == false)
        return true;

    modbus_reg_t reg;
    memset(&reg, 0, sizeof(reg));
    reg.addr = addr;
    reg.remap_type = MODBUS_VAL_REMAP_POINTER;
    reg.remap_val = val;
    memcpy(&MODBUS.input_regs[MODBUS.input_regs_cnt], &reg, sizeof(modbus_reg_t));
    MODBUS.regs_cnt++;

    return false;
}

/*******************************************************************************
 * @brief Modbus注册输入寄存器，映射方式为常亮
 * @param addr 寄存器地址
 * @param val 常量数据
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_register_input_reg_const(uint16_t addr, uint16_t val)
{
    // 定义的寄存器满了
    if (MODBUS.input_regs_cnt == MODBUS_MAX_INPUT_REG_COUNT)
        return true;

    // 检查地址是否重复
    if (modbus_input_reg_find_by_addr(0, addr) == false)
        return true;

    modbus_reg_t reg;
    memset(&reg, 0, sizeof(reg));
    reg.addr = addr;
    reg.remap_type = MODBUS_VAL_REMAP_CONST;
    reg.remap_const = val;
    memcpy(&MODBUS.input_regs[MODBUS.input_regs_cnt], &reg, sizeof(modbus_reg_t));
    MODBUS.regs_cnt++;

    return false;
}

/*******************************************************************************
 * @brief Modbus注册寄存器，映射方式为指针
 * @param addr 寄存器地址
 * @param val 数据指针
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_register_reg_pointer(uint16_t addr, uint16_t *val)
{
    // 定义的寄存器满了
    if (MODBUS.regs_cnt == MODBUS_MAX_REG_COUNT)
        return true;

    // 检查地址是否重复
    if (modbus_reg_find_by_addr(0, addr) == false)
        return true;

    modbus_reg_t reg;
    memset(&reg, 0, sizeof(reg));
    reg.addr = addr;
    reg.remap_type = MODBUS_VAL_REMAP_POINTER;
    reg.remap_val = val;
    memcpy(&MODBUS.regs[MODBUS.regs_cnt], &reg, sizeof(modbus_reg_t));
    MODBUS.regs_cnt++;

    return false;
}

/*******************************************************************************
 * @brief Modbus注册寄存器，映射方式为回调函数
 * @param addr 寄存器地址
 * @param read_fn 读取函数
 * @param write_fn 写入函数
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_register_reg_callback(uint16_t addr,
                                     modbus_read_reg_cb_t read_fn,
                                     modbus_write_reg_cb_t write_fn)
{

    // 定义的寄存器满了
    if (MODBUS.regs_cnt == MODBUS_MAX_REG_COUNT)
        return true;

    // 检查地址是否重复
    if (modbus_reg_find_by_addr(0, addr) == false)
        return true;

    modbus_reg_t reg;
    memset(&reg, 0, sizeof(reg));
    reg.addr = addr;
    reg.remap_type = MODBUS_VAL_REMAP_FUNC;
    reg.read_fn = read_fn;
    reg.write_fn = write_fn;
    memcpy(&MODBUS.regs[MODBUS.regs_cnt], &reg, sizeof(modbus_reg_t));
    MODBUS.regs_cnt++;

    return false;
}

/*******************************************************************************
 * @brief Modbus设置该地址线圈的值
 * @param addr 线圈地址
 * @param val 值
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_set_coil_val(uint16_t addr, uint8_t val)
{
    p_modbus_coil_t p_coil;

    // 查找该地址上是否有线圈定义
    if (modbus_coil_find_by_addr(&p_coil, addr))
        return true;

    switch (p_coil->remap_type)
    {
    case MODBUS_VAL_REMAP_FUNC:

        if (p_coil->write_fn == 0)
            return true;

        p_coil->write_fn(p_coil, val);

        break;
    case MODBUS_VAL_REMAP_POINTER:

        *(p_coil->remap_val) = val;

        break;
    default:
        return true;
    }

    return false;
}

/*******************************************************************************
 * @brief Modbus获取该地址线圈的值
 * @param addr 线圈地址
 * @param val 接收指针
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_get_coil_val(uint16_t addr, uint8_t *val)
{
    p_modbus_coil_t p_coil;

    // 查找该地址上是否有线圈定义
    if (modbus_coil_find_by_addr(&p_coil, addr))
        return true;

    switch (p_coil->remap_type)
    {
    case MODBUS_VAL_REMAP_FUNC:

        if (p_coil->read_fn == 0)
            return true;

        *val = p_coil->read_fn(p_coil);

        break;
    case MODBUS_VAL_REMAP_POINTER:
        *val = *(p_coil->remap_val);
        break;
    default:
        return true;
    }

    return false;
}

/*******************************************************************************
 * @brief Modbus获取该地址离散输入的值
 * @param addr 线圈地址
 * @param val 接收指针
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_read_din_val(uint16_t addr, uint8_t *val)
{
    p_modbus_coil_t p_coil;

    // 查找该地址上是否有线圈定义
    if (modbus_din_find_by_addr(&p_coil, addr))
        return true;

    if (p_coil->read_fn == 0)
        return true;

    *val = p_coil->read_fn(p_coil);

    return false;
}

/*******************************************************************************
 * @brief Modbus读取该地址输入寄存器的值
 * @param addr 线圈地址
 * @param val 接收指针
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_read_input_reg(uint16_t addr, uint16_t *val)
{
    p_modbus_reg_t p_reg;

    // 查找该地址上是否有寄存器定义
    if (modbus_input_reg_find_by_addr(&p_reg, addr))
        return true;

    switch (p_reg->remap_type)
    {
    case MODBUS_VAL_REMAP_FUNC:

        if (p_reg->read_fn == 0)
            return true;

        *val = p_reg->read_fn(p_reg);

        break;
    case MODBUS_VAL_REMAP_POINTER:
        *val = *(p_reg->remap_val);
        break;
    case MODBUS_VAL_REMAP_CONST:
        *val = p_reg->remap_const;
        break;
    default:
        return true;
    }

    return false;
}

/*******************************************************************************
 * @brief Modbus设置该地址寄存器的值
 * @param addr 寄存器地址
 * @param val 值
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_write_reg(uint16_t addr, uint16_t val)
{
    p_modbus_reg_t p_reg;

    // 查找该地址上是否有寄存器定义
    if (modbus_reg_find_by_addr(&p_reg, addr))
        return true;

    switch (p_reg->remap_type)
    {
    case MODBUS_VAL_REMAP_FUNC:

        if (p_reg->write_fn == 0)
            return true;

        return p_reg->write_fn(p_reg, val);

        break;
    case MODBUS_VAL_REMAP_POINTER:

        *(p_reg->remap_val) = val;

        break;
    default:
        return true;
    }

    return false;
}

/*******************************************************************************
 * @brief Modbus读取该地址寄存器的值
 * @param addr 线圈地址
 * @param val 接收指针
 * @return 0：成功，1：失败
 ******************************************************************************/
uint8_t modbus_read_reg(uint16_t addr, uint16_t *val)
{
    p_modbus_reg_t p_reg;

    // 查找该地址上是否有寄存器定义
    if (modbus_reg_find_by_addr(&p_reg, addr))
        return true;

    switch (p_reg->remap_type)
    {
    case MODBUS_VAL_REMAP_FUNC:

        if (p_reg->read_fn == 0)
            return true;

        *val = p_reg->read_fn(p_reg);

        break;
    case MODBUS_VAL_REMAP_POINTER:
        *val = *(p_reg->remap_val);
        break;
    default:
        return true;
    }

    return false;
}
