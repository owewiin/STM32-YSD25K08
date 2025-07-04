/**
 * @file buzzer_pwm.h
 * @brief 蜂鸣器PWM模块头文件
 * @author Your Name
 * @date 2025-07-04
 */

#ifndef __BUZZER_PWM_H
#define __BUZZER_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* 常用音符频率定义 */
#define NOTE_C4     262     // Do
#define NOTE_D4     294     // Re
#define NOTE_E4     330     // Mi
#define NOTE_F4     349     // Fa
#define NOTE_G4     392     // Sol
#define NOTE_A4     440     // La
#define NOTE_B4     494     // Si
#define NOTE_C5     523     // Do (高八度)
#define NOTE_REST   0       // 休止符

/* 预定义的报警频率 */
#define ALARM_FREQ_LOW      800
#define ALARM_FREQ_HIGH     1200
#define BEEP_FREQ_NORMAL    1000
#define BEEP_FREQ_HIGH      2000

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief  蜂鸣器PWM初始化
 * @param  None
 * @retval None
 */
void buzzer_pwm_init(void);

/**
 * @brief  设置蜂鸣器频率
 * @param  freq: 频率 (Hz)
 * @retval None
 */
void buzzer_set_frequency(uint32_t freq);

/**
 * @brief  设置蜂鸣器占空比
 * @param  duty: 占空比 (0-100)
 * @retval None
 */
void buzzer_set_duty(uint8_t duty);

/**
 * @brief  启动蜂鸣器
 * @param  freq: 频率 (Hz)
 * @retval None
 */
void buzzer_start(uint32_t freq);

/**
 * @brief  停止蜂鸣器
 * @param  None
 * @retval None
 */
void buzzer_stop(void);

/**
 * @brief  蜂鸣器鸣叫
 * @param  freq: 频率 (Hz)
 * @param  duration: 持续时间 (ms)
 * @retval None
 */
void buzzer_beep(uint32_t freq, uint32_t duration);

/**
 * @brief  获取当前频率
 * @param  None
 * @retval 当前频率 (Hz)
 */
uint32_t buzzer_get_frequency(void);

/**
 * @brief  获取当前占空比
 * @param  None
 * @retval 当前占空比 (0-100)
 */
uint8_t buzzer_get_duty(void);

/**
 * @brief  播放音符
 * @param  note: 音符频率数组
 * @param  duration: 持续时间数组
 * @param  count: 音符数量
 * @retval None
 */
void buzzer_play_melody(const uint32_t* note, const uint32_t* duration, uint8_t count);

/**
 * @brief  报警音效
 * @param  cycles: 报警循环次数
 * @retval None
 */
void buzzer_alarm(uint8_t cycles);

#ifdef __cplusplus
}
#endif

#endif /* __BUZZER_PWM_H */
