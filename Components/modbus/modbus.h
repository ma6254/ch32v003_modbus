#ifndef MODBUS_H
#define MODBUS_H

#include "ch32v00x.h"
#include "ch32v00x_usart.h"
#include "boot.h"
#include "bytes.h"
#include "mbits.h"
#include "config.h"
#include "board.h"

#define MODBUS_UART (USART1)

#define MODBUS_TX_BUF_SIZE (128)
#define MODBUS_RX_BUF_SIZE (128)

//   9600波特率，1字节：1042us，3.5字节：3646us
//  38400波特率，1字节： 260us，3.5字节： 911us
// 115200波特率，1字节：  87us，3.5字节： 308us
// 921600波特率，1字节：  11us，3.5字节：  38us

// 接收超时，微秒
#define MODBUS_TX_TIMEOUT (3500)
#define MODBUS_RX_TIMEOUT (3500)

#define MODBUS_MAX_DIN_COUNT (10)
#define MODBUS_MAX_COIL_COUNT (10)
#define MODBUS_MAX_INPUT_REG_COUNT (10)
#define MODBUS_MAX_REG_COUNT (10)

// MOSBUS广播地址
#define MODBUS_BOARDCAST_ADDR (0)

/*******************************************************************************
 * @brief Modbus功能字
 ******************************************************************************/
typedef enum
{
    MODBUS_FUNC_R_COILS = 0x01,       // (0x01) Read Coils
    MODBUS_FUNC_R_READ_DIN = 0x02,    // (0x02) Read Discrete Inputs
    MODBUS_FUNC_R_MUTI_REGS = 0x03,   // (0x03) Read Holding Registers
    MODBUS_FUNC_R_INPUT_REGS = 0x04,  // (0x04) Read Input Registers
    MODBUS_FUNC_W_SINGLE_COIL = 0x05, // (0x05) Write Single Coil
    MODBUS_FUNC_W_SINGLE_REG = 0x06,  // (0x06) Write Single Register
                                      // (0x08) Diagnostics (Serial Line only)
                                      // (0x0B) Get Comm Event Counter (Serial Line only)
    MODBUS_FUNC_W_MUTI_COILS = 0x0F,  // (0x0F) Write Multiple Coils
    MODBUS_FUNC_W_MUTI_REGS = 0x10,   // (0x10) Write Multiple Registers
                                      // (0x11) Report Server ID (Serial Line only)
                                      // (0x16) Mask Write Register
                                      // (0x17) Read/Write Multiple Registers
                                      // (0x2B / 0x0E) Read Device Identification
} modbus_func_t;

/*******************************************************************************
 * @brief Modbus故障码
 ******************************************************************************/
typedef enum
{
    MODBUS_ERR_INVALID_FUNC = 0x01, // 不支持的功能码
    MODBUS_ERR_INVALID_ADDR = 0x02, // 非法数据地址
    MODBUS_ERR_INVALID_VAL = 0x03,  // 非法数据值
    MODBUS_ERR_EXEC = 0x04,         // 执行命令出错
} modbus_err_t;

/*******************************************************************************
 * @brief Modbus映射方式
 ******************************************************************************/
typedef enum
{
    MODBUS_VAL_REMAP_NONE = 0,

    // 通过读写回调函数映射
    MODBUS_VAL_REMAP_FUNC,

    // 通过指针映射
    MODBUS_VAL_REMAP_POINTER,

    // 常量
    MODBUS_VAL_REMAP_CONST,

} modbus_val_remap_type_t;

/*******************************************************************************
 * @brief Modbus线圈
 ******************************************************************************/
typedef struct MODBUS_COIL_T modbus_coil_t;

/*******************************************************************************
 * @brief Modbus线圈指针
 ******************************************************************************/
typedef modbus_coil_t *p_modbus_coil_t;

/*******************************************************************************
 * @brief Modbus寄存器
 ******************************************************************************/
typedef struct MODBUS_REG_T modbus_reg_t;

/*******************************************************************************
 * @brief Modbus寄存器指针
 ******************************************************************************/
typedef modbus_reg_t *p_modbus_reg_t;

/*******************************************************************************
 * @brief Modbus读取线圈回调函数
 ******************************************************************************/
typedef bool (*modbus_read_coil_cb_t)(modbus_coil_t *p_coils);

/*******************************************************************************
 * @brief Modbus写入线圈回调函数
 ******************************************************************************/
typedef void (*modbus_write_coil_cb_t)(modbus_coil_t *p_coils, bool val);

/*******************************************************************************
 * @brief Modbus读取寄存器回调函数
 ******************************************************************************/
typedef uint16_t (*modbus_read_reg_cb_t)(modbus_reg_t *p_regs);

/*******************************************************************************
 * @brief Modbus写入寄存器回调函数
 ******************************************************************************/
typedef uint8_t (*modbus_write_reg_cb_t)(modbus_reg_t *p_regs, uint16_t val);

struct MODBUS_COIL_T
{
    uint16_t addr;
    modbus_val_remap_type_t remap_type;

    union
    {
        uint8_t *remap_val;
        struct
        {
            modbus_read_coil_cb_t read_fn;
            modbus_write_coil_cb_t write_fn;
        };
    };
};

struct MODBUS_REG_T
{
    uint16_t addr;
    modbus_val_remap_type_t remap_type;

    union
    {
        uint16_t *remap_val;
        uint16_t remap_const;
        struct
        {
            modbus_read_reg_cb_t read_fn;
            modbus_write_reg_cb_t write_fn;
        };
    };
};

/*******************************************************************************
 * @brief Modbus接收缓存
 ******************************************************************************/
typedef struct
{
    uint8_t func;
    uint16_t start_addr;
    uint16_t data_count;
    uint8_t byte_count;
    uint16_t i;
    uint8_t err;
    union
    {
        uint16_t read_val;
        struct
        {
            uint16_t w_val;
            uint8_t set_val;
        };
    };

} modbus_rx_tmp_t;

/*******************************************************************************
 * @brief Modbus主结构体
 ******************************************************************************/
typedef struct
{
    struct
    {
        uint8_t rx_done : 1;
        uint8_t tx_done : 1;
        uint8_t is_boardcast_frame : 1;
    };

    uint8_t tx_buf[MODBUS_TX_BUF_SIZE];
    uint16_t tx_cnt;
    uint16_t tx_len;
    uint32_t tx_timeout;

    uint8_t rx_buf[MODBUS_RX_BUF_SIZE];
    uint16_t rx_cnt;
    uint32_t rx_timeout;

    modbus_rx_tmp_t rx_tmp;

    // 只读线圈
    modbus_coil_t dins[MODBUS_MAX_DIN_COUNT];
    uint16_t dins_cnt;

    // 线圈
    modbus_coil_t coils[MODBUS_MAX_COIL_COUNT];
    uint16_t coils_cnt;

    // 只读寄存器
    modbus_reg_t input_regs[MODBUS_MAX_INPUT_REG_COUNT];
    uint16_t input_regs_cnt;

    // 寄存器
    modbus_reg_t regs[MODBUS_MAX_REG_COUNT];
    uint16_t regs_cnt;

} modbus_t;

extern modbus_t MODBUS;

void modbus_init(void);
void modbus_timeout_inc(uint32_t time);
void modbus_process(void);

uint16_t modbus_crc16(uint8_t *nData, uint16_t wLength);

// 只读线圈
uint8_t modbus_register_din_callback(uint16_t addr,
                                     modbus_read_coil_cb_t read_fn);
uint8_t modbus_read_din_val(uint16_t addr, uint8_t *val);

// 线圈
uint8_t modbus_register_coil_pointer(uint16_t addr, uint8_t *val);
uint8_t modbus_register_coil_callback(uint16_t addr,
                                      modbus_read_coil_cb_t read_fn,
                                      modbus_write_coil_cb_t write_fn);
uint8_t modbus_coil_find_by_addr(p_modbus_coil_t *p_coil, uint16_t addr);
uint8_t modbus_set_coil_val(uint16_t addr, uint8_t val);
uint8_t modbus_get_coil_val(uint16_t addr, uint8_t *val);

// 输入寄存器
uint8_t modbus_register_input_reg_pointer(uint16_t addr, uint16_t *val);
uint8_t modbus_register_input_reg_const(uint16_t addr, uint16_t val);
uint8_t modbus_input_reg_find_by_addr(p_modbus_reg_t *p_reg, uint16_t addr);
uint8_t modbus_read_input_reg(uint16_t addr, uint16_t *val);

// 寄存器
uint8_t modbus_register_reg_pointer(uint16_t addr, uint16_t *val);
uint8_t modbus_register_reg_callback(uint16_t addr,
                                     modbus_read_reg_cb_t read_fn,
                                     modbus_write_reg_cb_t write_fn);
uint8_t modbus_reg_find_by_addr(p_modbus_reg_t *p_reg, uint16_t addr);
uint8_t modbus_write_reg(uint16_t addr, uint16_t val);
uint8_t modbus_read_reg(uint16_t addr, uint16_t *val);

#endif // MODBUS_H
