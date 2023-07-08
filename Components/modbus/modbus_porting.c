#include "modbus_porting.h"
#include "config.h"
#include "boot.h"
#include "board.h"

bool modbus_read_din_0001(modbus_coil_t *p_coils)
{
    return read_x1();
}

bool modbus_read_coil_0001(modbus_coil_t *p_coils)
{
    return read_y1();
}

void modbus_write_coil_0001(modbus_coil_t *p_coils, bool val)
{
    write_y1(val);
}

uint16_t modbus_read_reg_0001(modbus_reg_t *p_regs)
{
    return CFG.comm_addr;
}

uint8_t modbus_write_reg_0001(modbus_reg_t *p_coils, uint16_t val)
{
    if (val < 1)
        return true;

    if (val > 255)
        return true;

    if (CFG.comm_addr != val)
    {
        CFG.comm_addr = val;
        save_config_data();
    }

    return false;
}

uint16_t modbus_read_reg_x1_mode(modbus_reg_t *p_regs)
{
    return CFG.x1_mode;
}

uint8_t modbus_write_reg_x1_mode(modbus_reg_t *p_coils, uint16_t val)
{
    if (CFG.x1_mode == val)
        return false;

    CFG.x1_mode = val;
    board_gpio_set_x1_mode(CFG.x1_mode);
    save_config_data();
    return false;
}

void modbus_porting_init(void)
{
    modbus_register_din_callback(0, modbus_read_din_0001);
    modbus_register_coil_callback(0, modbus_read_coil_0001, modbus_write_coil_0001);
    modbus_register_reg_callback(0, modbus_read_reg_0001, modbus_write_reg_0001);
    modbus_register_reg_callback(1, modbus_read_reg_x1_mode, modbus_write_reg_x1_mode);
}
