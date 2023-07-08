#ifndef CONFIG_H
#define CONFIG_H

// MODBUS串口通讯波特率
#define MODBUS_UART_BAUD (9600)

// <o> APP固件地址
// <i> 【APP_ADDR】
// <i> BOOT完成后，跳转到该地址执行
#define APP_ADDR (0x08002000)

// <o> 数据区地址
// <i> 【DATA_ADDR】
// <i> 从该地址中取得运行数据、设置参数等
// <i> CH32V003为16KB, 1页1KB
#define DATA_ADDR (0x08003C00)

#endif // CONFIG_H
