/**
 * @file usart_rs485.c
 * @brief RS485通信模块实现
 * @author Your Name
 * @date 2025-07-04
 */

/* Includes ------------------------------------------------------------------*/
#include "usart_rs485.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RS485_RX_BUFFER_SIZE    256

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t rs485_rx_buffer[RS485_RX_BUFFER_SIZE];
static uint16_t rs485_rx_count = 0;
static uint8_t rs485_rx_flag = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  RS485初始化
 * @param  None
 * @retval None
 */
void rs485_init(void)
{
    usart_init_type usart_init_struct;
    
    /* 使能USART2时钟 */
    crm_periph_clock_enable(RS485_USART_CLK, TRUE);
    
    /* USART2配置 */
    usart_default_para_init(&usart_init_struct);
    usart_init_struct.baudrate = 115200;
    usart_init_struct.data_bit = USART_DATA_8BITS;
    usart_init_struct.stop_bit = USART_STOP_1_BIT;
    usart_init_struct.parity = USART_PARITY_NONE;
    usart_init_struct.hardware_flow_control = USART_HARDWARE_FLOW_NONE;
    usart_init_struct.mode = USART_MODE_TX | USART_MODE_RX;
    usart_init(RS485_USART, &usart_init_struct);
    
    /* 使能USART2接收中断 */
    usart_interrupt_enable(RS485_USART, USART_RDBF_INT, TRUE);
    
    /* 使能USART2 */
    usart_enable(RS485_USART, TRUE);
    
    /* 设置RS485为接收模式 */
    rs485_set_mode(RS485_MODE_RX);
}

/**
 * @brief  设置RS485工作模式
 * @param  mode: RS485_MODE_TX 或 RS485_MODE_RX
 * @retval None
 */
void rs485_set_mode(rs485_mode_t mode)
{
    if(mode == RS485_MODE_TX)
    {
        gpio_bits_set(RS485_DE_GPIO_PORT, RS485_DE_GPIO_PIN);
    }
    else
    {
        gpio_bits_reset(RS485_DE_GPIO_PORT, RS485_DE_GPIO_PIN);
    }
    
    /* 模式切换延时 */
    delay_us(10);
}

/**
 * @brief  发送单个字节
 * @param  data: 要发送的字节
 * @retval None
 */
void rs485_send_byte(uint8_t data)
{
    rs485_set_mode(RS485_MODE_TX);
    
    /* 等待发送缓冲区空 */
    while(usart_flag_get(RS485_USART, USART_TDC_FLAG) == RESET);
    
    /* 发送数据 */
    usart_data_transmit(RS485_USART, data);
    
    /* 等待发送完成 */
    while(usart_flag_get(RS485_USART, USART_TDC_FLAG) == RESET);
    
    rs485_set_mode(RS485_MODE_RX);
}

/**
 * @brief  发送字符串
 * @param  str: 要发送的字符串
 * @retval None
 */
void rs485_send_string(const char* str)
{
    rs485_set_mode(RS485_MODE_TX);
    
    while(*str)
    {
        /* 等待发送缓冲区空 */
        while(usart_flag_get(RS485_USART, USART_TDC_FLAG) == RESET);
        
        /* 发送数据 */
        usart_data_transmit(RS485_USART, *str++);
    }
    
    /* 等待发送完成 */
    while(usart_flag_get(RS485_USART, USART_TDC_FLAG) == RESET);
    
    rs485_set_mode(RS485_MODE_RX);
