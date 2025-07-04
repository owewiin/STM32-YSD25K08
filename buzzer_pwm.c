/**
 * @file buzzer_pwm.c
 * @brief 蜂鸣器PWM模块实现
 * @author Your Name
 * @date 2025-07-04
 */

/* Includes ------------------------------------------------------------------*/
#include "buzzer_pwm.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t buzzer_freq = 0;
static uint8_t buzzer_duty = 50;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  蜂鸣器PWM初始化
 * @param  None
 * @retval None
 */
void buzzer_pwm_init(void)
{
    tmr_output_config_type tmr_output_struct;
    tmr_base_init_type tmr_base_struct;
    
    /* 使能定时器时钟 */
    crm_periph_clock_enable(BUZZER_TMR_CLK, TRUE);
    
    /* 定时器基本配置 */
    tmr_base_default_para_init(&tmr_base_struct);
    tmr_base_struct.tmr_clock_division = TMR_CLOCK_DIV1;
    tmr_base_struct.tmr_count_direction = TMR_COUNT_UP;
    tmr_base_struct.tmr_period = BUZZER_TMR_PERIOD - 1;
    tmr_base_struct.tmr_repetition_counter = 0;
    tmr_base_struct.tmr_div = (system_core_clock / 2 / 1000000) - 1; // 1MHz计数频率
    tmr_base_init(BUZZER_TMR, &tmr_base_struct);
    
    /* PWM输出配置 */
    tmr_output_default_para_init(&tmr_output_struct);
    tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
    tmr_output_struct.oc_output_state = TRUE;
    tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
    tmr_output_struct.oc_idle_state = FALSE;
    tmr_output_struct.occ_output_state = FALSE;
    tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
    tmr_output_struct.occ_idle_state = FALSE;
    tmr_output_channel_config(BUZZER_TMR, BUZZER_TMR_CHANNEL, &tmr_output_struct);
    
    /* 设置初始占空比为0（关闭） */
    tmr_channel_value_set(BUZZER_TMR, BUZZER_TMR_CHANNEL, 0);
    
    /* 使能定时器 */
    tmr_counter_enable(BUZZER_TMR, TRUE);
    
    /* 使能PWM输出 */
    tmr_output_enable(BUZZER_TMR, TRUE);
    
    /* 初始化完成，蜂鸣器关闭 */
    buzzer_stop();
}

/**
 * @brief  设置蜂鸣器频率
 * @param  freq: 频率 (Hz)
 * @retval None
 */
void buzzer_set_frequency(uint32_t freq)
{
    uint32_t period;
    
    if(freq == 0)
    {
        buzzer_stop();
        return;
    }
    
    /* 计算周期值 */
    period = 1000000 / freq; // 1MHz计数频率
    
    if(period > 65535)
    {
        period = 65535;
    }
    else if(period < 2)
    {
        period = 2;
    }
    
    /* 设置定时器周期 */
    tmr_period_value_set(BUZZER_TMR, period - 1);
    
    /* 更新占空比 */
    buzzer_set_duty(buzzer_duty);
    
    buzzer_freq = freq;
}

/**
 * @brief  设置蜂鸣器占空比
 * @param  duty: 占空比 (0-100)
 * @retval None
 */
void buzzer_set_duty(uint8_t duty)
{
    uint32_t pulse_value;
    
    if(duty > 100)
    {
        duty = 100;
    }
    
    buzzer_duty = duty;
    
    /* 计算脉冲值 */
    pulse_value = (tmr_period_value_get(BUZZER_TMR) + 1) * duty / 100;
    
    /* 设置比较值 */
    tmr_channel_value_set(BUZZER_TMR, BUZZER_TMR_CHANNEL, pulse_value);
}

/**
 * @brief  启动蜂鸣器
 * @param  freq: 频率 (Hz)
 * @retval None
 */
void buzzer_start(uint32_t freq)
{
    buzzer_set_frequency(freq);
    buzzer_set_duty(50); // 50%占空比
}

/**
 * @brief  停止蜂鸣器
 * @param  None
 * @retval None
 */
void buzzer_stop(void)
{
    tmr_channel_value_set(BUZZER_TMR, BUZZER_TMR_CHANNEL, 0);
    buzzer_freq = 0;
}

/**
 * @brief  蜂鸣器鸣叫
 * @param  freq: 频率 (Hz)
 * @param  duration: 持续时间 (ms)
 * @retval None
 */
void buzzer_beep(uint32_t freq, uint32_t duration)
{
    buzzer_start(freq);
    delay_ms(duration);
    buzzer_stop();
}

/**
 * @brief  获取当前频率
 * @param  None
 * @retval 当前频率 (Hz)
 */
uint32_t buzzer_get_frequency(void)
{
    return buzzer_freq;
}

/**
 * @brief  获取当前占空比
 * @param  None
 * @retval 当前占空比 (0-100)
 */
uint8_t buzzer_get_duty(void)
{
    return buzzer_duty;
}

/**
 * @brief  播放音符
 * @param  note: 音符频率数组
 * @param  duration: 持续时间数组
 * @param  count: 音符数量
 * @retval None
 */
void buzzer_play_melody(const uint32_t* note, const uint32_t* duration, uint8_t count)
{
    for(uint8_t i = 0; i < count; i++)
    {
        if(note[i] == 0)
        {
            buzzer_stop();
        }
        else
        {
            buzzer_start(note[i]);
        }
        
        delay_ms(duration[i]);
    }
    
    buzzer_stop();
}

/**
 * @brief  报警音效
 * @param  cycles: 报警循环次数
 * @retval None
 */
void buzzer_alarm(uint8_t cycles)
{
    for(uint8_t i = 0; i < cycles; i++)
    {
        buzzer_beep(1000, 200); // 1000Hz, 200ms
        delay_ms(200);
        buzzer_beep(1500, 200); // 1500Hz, 200ms
        delay_ms(200);
    }
}
