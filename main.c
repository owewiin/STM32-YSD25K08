/**
 * @file main.c
 * @brief AT32F403ARCT7 主程序文件
 * @author Your Name
 * @date 2025-07-04
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTick;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  系统时钟配置
 * @param  None
 * @retval None
 */
void system_clock_config(void)
{
    /* 复位CRM配置 */
    crm_reset();
    
    /* 使能外部高速晶振 */
    crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);
    
    /* 等待外部晶振稳定 */
    while(crm_hext_stable_wait() == ERROR);
    
    /* 配置AHB预分频器 */
    crm_ahb_div_set(CRM_AHB_DIV_1);
    
    /* 配置APB2预分频器 */
    crm_apb2_div_set(CRM_APB2_DIV_2);
    
    /* 配置APB1预分频器 */
    crm_apb1_div_set(CRM_APB1_DIV_2);
    
    /* 配置PLL */
    crm_pll_config(CRM_PLL_SOURCE_HEXT, CRM_PLL_MULT_30);
    
    /* 使能PLL */
    crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);
    
    /* 等待PLL稳定 */
    while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET);
    
    /* 配置Flash延时 */
    flash_latency_set(FLASH_LATENCY_7);
    
    /* 选择PLL作为系统时钟 */
    crm_sysclk_switch(CRM_SCLK_PLL);
    
    /* 等待系统时钟切换成功 */
    while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL);
    
    /* 更新系统时钟 */
    system_core_clock_update();
}

/**
 * @brief  GPIO配置
 * @param  None
 * @retval None
 */
void gpio_config(void)
{
    gpio_init_type gpio_init_struct;
    
    /* 使能GPIO时钟 */
    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
    
    /* 配置RS485相关引脚 */
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins = RS485_TX_GPIO_PIN;
    gpio_init(RS485_TX_GPIO_PORT, &gpio_init_struct);
    
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init_struct.gpio_pins = RS485_RX_GPIO_PIN;
    gpio_init(RS485_RX_GPIO_PORT, &gpio_init_struct);
    
    /* RS485_DE引脚配置 */
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_pins = RS485_DE_GPIO_PIN;
    gpio_init(RS485_DE_GPIO_PORT, &gpio_init_struct);
    gpio_bits_reset(RS485_DE_GPIO_PORT, RS485_DE_GPIO_PIN); // 默认接收模式
    
    /* 配置BUZZER PWM引脚 */
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_pins = BUZZER_GPIO_PIN;
    gpio_init(BUZZER_GPIO_PORT, &gpio_init_struct);
    
    /* 配置I2C引脚 */
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
    gpio_init_struct.gpio_pull = GPIO_PULL_UP;
    gpio_init_struct.gpio_pins = DISPLAY_SCL_GPIO_PIN;
    gpio_init(DISPLAY_SCL_GPIO_PORT, &gpio_init_struct);
    
    gpio_init_struct.gpio_pins = DISPLAY_SDA_GPIO_PIN;
    gpio_init(DISPLAY_SDA_GPIO_PORT, &gpio_init_struct);
    
    /* 配置显示板控制引脚 */
    gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_pins = DISPLAY_CTRL1_GPIO_PIN;
    gpio_init(DISPLAY_CTRL1_GPIO_PORT, &gpio_init_struct);
    
    gpio_init_struct.gpio_pins = DISPLAY_CTRL2_GPIO_PIN;
    gpio_init(DISPLAY_CTRL2_GPIO_PORT, &gpio_init_struct);
    
    /* 配置引脚复用功能 */
    gpio_pin_mux_config(RS485_TX_GPIO_PORT, RS485_TX_GPIO_PinSource, RS485_TX_GPIO_AF);
    gpio_pin_mux_config(RS485_RX_GPIO_PORT, RS485_RX_GPIO_PinSource, RS485_RX_GPIO_AF);
    gpio_pin_mux_config(BUZZER_GPIO_PORT, BUZZER_GPIO_PinSource, BUZZER_GPIO_AF);
    gpio_pin_mux_config(DISPLAY_SCL_GPIO_PORT, DISPLAY_SCL_GPIO_PinSource, DISPLAY_SCL_GPIO_AF);
    gpio_pin_mux_config(DISPLAY_SDA_GPIO_PORT, DISPLAY_SDA_GPIO_PinSource, DISPLAY_SDA_GPIO_AF);
}

/**
 * @brief  中断优先级配置
 * @param  None
 * @retval None
 */
void nvic_config(void)
{
    nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
    
    /* 配置RS485 USART中断 */
    nvic_irq_enable(RS485_USART_IRQ, 0, 1);
    
    /* 配置SysTick中断 */
    nvic_irq_enable(SysTick_IRQn, 0, 0);
}

/**
 * @brief  延时初始化
 * @param  None
 * @retval None
 */
void delay_init(void)
{
    /* 配置SysTick */
    if (SysTick_Config(system_core_clock / 1000000))
    {
        while (1);
    }
    
    /* 关闭SysTick中断 */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

/**
 * @brief  毫秒延时
 * @param  ms: 延时时间(ms)
 * @retval None
 */
void delay_ms(uint32_t ms)
{
    uint32_t i;
    for(i = 0; i < ms; i++)
    {
        delay_us(1000);
    }
}

/**
 * @brief  微秒延时
 * @param  us: 延时时间(us)
 * @retval None
 */
void delay_us(uint32_t us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    
    ticks = us * (system_core_clock / 1000000);
    told = SysTick->VAL;
    
    while(1)
    {
        tnow = SysTick->VAL;
        if(tnow != told)
        {
            if(tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += SysTick->LOAD - tnow + told;
            }
            told = tnow;
            if(tcnt >= ticks)
            {
                break;
            }
        }
    }
}

/**
 * @brief  错误处理函数
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
    /* 用户可以添加错误处理代码 */
    while(1)
    {
        // 错误指示，可以点亮LED或其他操作
    }
}

/**
 * @brief  主函数
 * @param  None
 * @retval int
 */
int main(void)
{
    /* 系统初始化 */
    system_clock_config();
    gpio_config();
    nvic_config();
    delay_init();
    
    /* 外设初始化 */
    rs485_init();
    buzzer_pwm_init();
    i2c_display_init();
    
    /* 主循环 */
    while(1)
    {
        /* 测试RS485通信 */
        rs485_send_string("Hello RS485\r\n");
        delay_ms(1000);
        
        /* 测试蜂鸣器 */
        buzzer_beep(1000, 100); // 1000Hz, 100ms
        delay_ms(500);
        
        /* 测试I2C显示板 */
        i2c_display_send_data(0x01, 0x55);
        delay_ms(1000);
        
        /* 控制显示板控制引脚 */
        gpio_bits_toggle(DISPLAY_CTRL1_GPIO_PORT, DISPLAY_CTRL1_GPIO_PIN);
        gpio_bits_toggle(DISPLAY_CTRL2_GPIO_PORT, DISPLAY_CTRL2_GPIO_PIN);
        delay_ms(1000);
    }
}

/* 中断服务函数 */
void SysTick_Handler(void)
{
    uwTick++;
}

/**
 * @brief  获取系统时钟滴答数
 * @param  None
 * @retval 滴答数
 */
uint32_t get_tick(void)
{
    return uwTick;
}
