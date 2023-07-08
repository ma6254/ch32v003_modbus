#include "soft_timer.h"

soft_timer_stamp_t soft_timer_uptime = 0;
bool soft_timer_uptime_overflow = 0;

#define SOFT_TIMER_CB_NONE ((soft_timer_tick_cb_t)0)

/*******************************************************************************
 * @brief 软件定时器绝对时间戳自增
 * @param inc_time 增加的时间
 * @return None
 ******************************************************************************/
inline void soft_timer_inc(uint32_t inc_time)
{
    soft_timer_uptime += inc_time;
}

/*******************************************************************************
 * @brief 初始化软件定时器
 * @param p_soft_time 软件定时器结构体指针
 * @param interval 时间间隔
 * @param fn 回调函数
 * @return None
 ******************************************************************************/
void soft_timer_init(soft_timer_t *p_soft_time,
                     uint32_t interval,
                     soft_timer_tick_cb_t fn)
{
    p_soft_time->interval = interval;
    p_soft_time->prev = soft_timer_uptime;
    p_soft_time->fn = fn;
}

/*******************************************************************************
 * @brief 判断是否到达目标时间间隔
 * @param p_soft_time 软件定时器结构体指针
 * @return true:到达时间 false:没有到达时间
 ******************************************************************************/
bool soft_timer_check(soft_timer_t *p_soft_time)
{
    // 时间变动
    if (soft_timer_uptime == p_soft_time->prev)
        return false;

    // 判定是否溢出, 并累加间隔时间
    if (p_soft_time->prev > soft_timer_uptime)
    {
        p_soft_time->acc += (uint64_t)soft_timer_uptime +
                            SOFT_TIMER_STAMP_MAX + 1 -
                            p_soft_time->prev;
    }
    else
    {
        p_soft_time->acc += soft_timer_uptime - p_soft_time->prev;
    }

    // 重置定时器时间戳
    p_soft_time->prev = soft_timer_uptime;

    // 判定是否累计到设定时间，并重置累计时间
    if (p_soft_time->acc >= p_soft_time->interval)
    {
        p_soft_time->acc = 0;

        // 检查函数指针是否为空
        if (p_soft_time->fn != SOFT_TIMER_CB_NONE)
        {
            p_soft_time->fn();
        }
        return true;
    }

    return false;
}

/*******************************************************************************
 * @brief 判断是否到达目标时间间隔
 * @param p_soft_time 软件定时器结构体指针
 * @param fn 回调函数
 * @return None
 ******************************************************************************/
void soft_timer_tick_callback(soft_timer_t *p_soft_time, soft_timer_tick_cb_t fn)
{
    if (!soft_timer_check(p_soft_time))
        return;

    // 检查函数指针是否为空
    if (fn == SOFT_TIMER_CB_NONE)
        return;

    fn();
}

/*******************************************************************************
 * @brief 定时器数组循环处理函数
 * @param p_soft_time 软件定时器组首地址
 * @param len 组中定时器个数
 * @return None
 ******************************************************************************/
void soft_timer_group_loop(soft_timer_t *p_soft_time, uint32_t len)
{
    uint32_t i = 0;
    for (i = 0; i < len; i++)
    {
        soft_timer_check(p_soft_time);
    }
}
