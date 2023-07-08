#ifndef SOFT_TIMER_H
#define SOFT_TIMER_H

#include <stdint.h>
#include <stdbool.h>

// 时间戳格式
typedef uint32_t soft_timer_stamp_t;
#define SOFT_TIMER_STAMP_MAX (UINT32_MAX)

// 定时器回调函数
typedef void (*soft_timer_tick_cb_t)(void);

typedef struct soft_timer
{
    // 上一次触发的时间戳
    soft_timer_stamp_t prev;

    // 累计时间
    uint32_t acc;

    // 间隔时间
    uint32_t interval;

    // 回调函数
    soft_timer_tick_cb_t fn;

} soft_timer_t;

extern soft_timer_stamp_t soft_timer_uptime;

void soft_timer_inc(uint32_t inc_time);
void soft_timer_init(soft_timer_t *p_soft_time,
                     uint32_t interval,
                     soft_timer_tick_cb_t fn);
bool soft_timer_check(soft_timer_t *p_soft_time);
void soft_timer_tick_callback(soft_timer_t *p_soft_time,
                              soft_timer_tick_cb_t fn);
void soft_timer_group_loop(soft_timer_t *p_soft_time, uint32_t len);

#endif // SOFT_TIMER_H
