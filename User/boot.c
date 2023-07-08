#include <string.h>
#include "boot.h"
#include "modbus.h"

cfg_t CFG = {0};

/*******************************************************************************
 * @brief 读取配置数据，并校验
 * @param None
 * @return 0:校验成功 1:校验失败
 ******************************************************************************/
uint8_t read_config_data(void)
{
    uint8_t *cfg_buf = (uint8_t *)DATA_ADDR;

    // uint16_t cfg_crc = *(uint16_t *)(cfg_buf + CONFIG_AREA_SIZE - 2);

    // 校验魔术字
    if ((cfg_buf[0] != 0xAA) || (cfg_buf[1] != 0x55))
        return 1;

    CFG.firmware_size = unpack_big_32(cfg_buf + 2);
    CFG.firmware_crc = unpack_big_16(cfg_buf + 6);
    CFG.comm_addr = *(cfg_buf + 8);
    CFG.serial_baud = *(cfg_buf + 9);
    CFG.x1_mode = *(cfg_buf + 10);

    CFG.flash_crc = modbus_crc16((uint8_t *)APP_ADDR, CFG.firmware_size);

    // 校验固件区
    if (CFG.firmware_crc != CFG.flash_crc)
        return 1;

    return 0;
}

/*******************************************************************************
 * @brief 写入配置数据到flash
 * @param None
 * @return None
 ******************************************************************************/
uint8_t save_config_data(void)
{
    uint8_t cfg_buf[CONFIG_AREA_SIZE];
    uint32_t Counter_Num = 0;

    memset(cfg_buf, 0, CONFIG_AREA_SIZE);
    cfg_buf[0] = 0xAA;
    cfg_buf[1] = 0x55;
    pack_big_32(cfg_buf + 2, CFG.firmware_size);
    pack_big_16(cfg_buf + 6, CFG.firmware_crc);
    *(cfg_buf + 8) = CFG.comm_addr;
    *(cfg_buf + 9) = CFG.serial_baud;
    *(cfg_buf + 10) = CFG.x1_mode;

    FLASH_Unlock();
    FLASH_ErasePage(DATA_ADDR);

    for (Counter_Num = 0; Counter_Num < CONFIG_AREA_SIZE; Counter_Num += 4)
    {
        FLASH_ProgramWord(
            DATA_ADDR + Counter_Num,
            *(uint32_t *)(cfg_buf + Counter_Num));
    }

    FLASH_Lock();

    return 0;
}
