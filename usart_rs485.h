/**
 * @file usart_rs485.h
 * @brief RS485通信模块头文件
 * @author Your Name
 * @date 2025-07-04
 */

#ifndef __USART_RS485_H
#define __USART_RS485_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
    RS485_MODE_TX = 0,  /*!< 发送模式 */
    RS485_MODE_RX = 1   /*!< 接收模式 */
} rs485_mode_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief  RS485初始化
 * @param  None
 * @retval None
 */
void rs485_init(void);

/**
 * @brief  设置RS485工作模式
 * @param  mode: RS485_MODE_TX 或 RS485_MODE_RX
 * @retval None
 */
void rs485_set_mode(rs485_mode_t mode);

/**
 * @brief  发送单个字节
 * @param  data: 要发送的字节
 * @retval None
 */
void rs485_send_byte(uint8_t data);

/**
 * @brief  发送字符串
 * @param  str: 要发送的字符串
 * @retval None
 */
void rs485_send_string(const char* str);

/**
 * @brief  发送数据缓冲区
 * @param  data: 数据缓冲区指针
 * @param  len: 数据长度
 * @retval None
 */
void rs485_send_buffer(uint8_t* data, uint16_t len);

/**
 * @brief  接收数据
 * @param  data: 数据缓冲区指针
 * @param  max_len: 最大接收长度
 * @retval 实际接收长度
 */
uint16_t rs485_receive_data(uint8_t* data, uint16_t max_len);

/**
 * @brief  检查是否有数据接收
 * @param  None
 * @retval 1: 有数据, 0: 无数据
 */
uint8_t rs485_data_available(void);

/**
 * @brief  获取接收数据长度
 * @param  None
 * @retval 接收数据长度
 */
uint16_t rs485_get_rx_length(void);

/**
 * @brief  清空接收缓冲区
 * @param  None
 * @retval None
 */
void rs485_clear_rx_buffer(void);

#ifdef __cplusplus
}
#endif

#endif /* __USART_RS485_H */
