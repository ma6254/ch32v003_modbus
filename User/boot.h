#ifndef BOOT_H
#define BOOT_H

#include "ch32v00x.h"
#include "ch32v00x_flash.h"
#include "config.h"
#include "bytes.h"

#define CONFIG_AREA_SIZE (16)

typedef struct
{
    // 固件大小
    uint32_t firmware_size;

    // 固件区CRC校验码
    uint16_t firmware_crc;

    // 固件区CRC校验码
    uint16_t flash_crc;

    // 通信地址
    uint8_t comm_addr;

    // 波特率
    uint8_t serial_baud;

    // 输入1的IO模式
    uint8_t x1_mode;
} cfg_t;

extern cfg_t CFG;

uint8_t read_config_data(void);
uint8_t save_config_data(void);

#endif // BOOT_H
