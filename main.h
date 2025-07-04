/**
 * @file main.h
 * @brief AT32F403ARCT7 主头文件
 * @author Your Name
 * @date 2025-07-04
 */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"
#include "at32f403a_407_conf.h"
#include "usart_rs485.h"
#include "buzzer_pwm.h"
#include "i2c_display.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* 外部晶振频率 FA-128 (22FA1280023400) */
#define HEXT_VALUE    ((uint32_t)8000000)   /*!< 外部晶振频率 8MHz */

/* 系统时钟频率 */
#define SYSTEM_CORE_CLOCK    240000000      /*!< 系统时钟 240MHz */

/* RS485 引脚定义 */
#define RS485_USART                 USART2
#define RS485_USART_CLK             CRM_USART2_PERIPH_CLOCK
#define RS485_USART_IRQ             USART2_IRQn
#define RS485_USART_IRQHandler      USART2_IRQHandler

#define RS485_TX_GPIO_PORT          GPIOA
#define RS485_TX_GPIO_PIN           GPIO_PINS_2
#define RS485_TX_GPIO_PinSource     GPIO_PINS_SOURCE2
#define RS485_TX_GPIO_AF            GPIO_MUX_7

#define RS485_RX_GPIO_PORT          GPIOA
#define RS485_RX_GPIO_PIN           GPIO_PINS_3
#define RS485_RX_GPIO_PinSource     GPIO_PINS_SOURCE3
#define RS485_RX_GPIO_AF            GPIO_MUX_7

#define RS485_DE_GPIO_PORT          GPIOA
#define RS485_DE_GPIO_PIN           GPIO_PINS_4

/* BUZZER PWM 引脚定义 */
#define BUZZER_TMR                  TMR3
#define BUZZER_TMR_CLK              CRM_TMR3_PERIPH_CLOCK
#define BUZZER_TMR_CHANNEL          TMR_SELECT_CHANNEL_1
#define BUZZER_TMR_PERIOD           1000

#define BUZZER_GPIO_PORT            GPIOA
#define BUZZER_GPIO_PIN             GPIO_PINS_6
#define BUZZER_GPIO_PinSource       GPIO_PINS_SOURCE6
#define BUZZER_GPIO_AF              GPIO_MUX_2

/* I2C 显示板引脚定义 */
#define DISPLAY_I2C                 I2C1
#define DISPLAY_I2C_CLK             CRM_I2C1_PERIPH_CLOCK
#define DISPLAY_I2C_SPEED           400000
#define DISPLAY_I2C_ADDRESS         0x3C

#define DISPLAY_SCL_GPIO_PORT       GPIOB
#define DISPLAY_SCL_GPIO_PIN        GPIO_PINS_6
#define DISPLAY_SCL_GPIO_PinSource  GPIO_PINS_SOURCE6
#define DISPLAY_SCL_GPIO_AF         GPIO_MUX_4

#define DISPLAY_SDA_GPIO_PORT       GPIOB
#define DISPLAY_SDA_GPIO_PIN        GPIO_PINS_7
#define DISPLAY_SDA_GPIO_PinSource  GPIO_PINS_SOURCE7
#define DISPLAY_SDA_GPIO_AF         GPIO_MUX_4

#define DISPLAY_CTRL1_GPIO_PORT     GPIOB
#define DISPLAY_CTRL1_GPIO_PIN      GPIO_PINS_8

#define DISPLAY_CTRL2_GPIO_PORT     GPIOB
#define DISPLAY_CTRL2_GPIO_PIN      GPIO_PINS_9

/* Exported macro ------------------------------------------------------------*/
#define LED_ON(pin)     gpio_bits_reset(GPIOC, pin)
#define LED_OFF(pin)    gpio_bits_set(GPIOC, pin)
#define LED_TOGGLE(pin) gpio_bits_toggle(GPIOC, pin)

/* Exported functions prototypes ---------------------------------------------*/
void system_clock_config(void);
void gpio_config(void);
void nvic_config(void);
void delay_init(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

/* Error handler */
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
