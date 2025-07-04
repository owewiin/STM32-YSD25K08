/**
 * @file i2c_display.h
 * @brief I2C显示板通信模块头文件
 * @author Your Name
 * @date 2025-07-04
 */

#ifndef __I2C_DISPLAY_H
#define __I2C_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* 常用显示板I2C地址 */
#define OLED_I2C_ADDRESS        0x3C    // OLED显示屏地址
#define LCD_I2C_ADDRESS         0x27    // LCD显示屏地址
#define LED_MATRIX_I2C_ADDRESS  0x70    // LED矩阵地址

/* 显示板寄存器地址定义 */
#define DISPLAY_REG_CTRL        0x00    // 控制寄存器
#define DISPLAY_REG_DATA        0x01    // 数据寄存器
#define DISPLAY_REG_STATUS      0x02    // 状态寄存器
#define DISPLAY_REG_CONFIG      0x03    // 配置寄存器

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief  I2C显示板初始化
 * @param  None
 * @retval None
 */
void i2c_display_init(void);

/**
 * @brief  I2C写单个字节
 * @param  device_addr: 设备地址
 * @param  reg_addr: 寄存器地址
 * @param  data: 数据
 * @retval SUCCESS/ERROR
 */
error_status i2c_display_write_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t data);

/**
 * @brief  I2C读单个字节
 * @param  device_addr: 设备地址
 * @param  reg_addr: 寄存器地址
 * @param  data: 数据指针
 * @retval SUCCESS/ERROR
 */
error_status i2c_display_read_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t* data);

/**
 * @brief  I2C写多个字节
 * @param  device_addr: 设备地址
 * @param  reg_addr: 寄存器地址
 * @param  data: 数据缓冲区
 * @param  len: 数据长度
 * @retval SUCCESS/ERROR
 */
error_status i2c_display_write_buffer(uint8_t device_addr, uint8_t reg_addr, uint8_t* data, uint16_t len);

/**
 * @brief  I2C读多个字节
 * @param  device_addr: 设备地址
 * @param  reg_addr: 寄存器地址
 * @param  data: 数据缓冲区
 * @param  len: 数据长度
 * @retval SUCCESS/ERROR
 */
error_status i2c_display_read_buffer(uint8_t device_addr, uint8_t reg_addr, uint8_t* data, uint16_t len);

/**
 * @brief  发送数据到显示板
 * @param  reg_addr: 寄存器地址
 * @param  data: 数据
 * @retval SUCCESS/ERROR
 */
error_status i2c_display_send_data(uint8_t reg_addr, uint8_t data);

/**
 * @brief  从显示板读取数据
 * @param  reg_addr: 寄存器地址
 * @param  data: 数据指针
 * @retval SUCCESS/ERROR
 */
error_status i2c_display_read_data(uint8_t reg_addr, uint8_t* data);

/**
 * @brief  发送缓冲区数据到显示板
 * @param  reg_addr: 寄存器地址
 * @param  data: 数据缓冲区
 * @param  len: 数据长度
 * @retval SUCCESS/ERROR
 */
error_status i2c_display_send_buffer(uint8_t reg_addr, uint8_t* data, uint16_t len);

/**
 * @brief  从显示板读取缓冲区数据
 * @param  reg_addr: 寄存器地址
 * @param  data: 数据缓冲区
 * @param  len: 数据长度
 * @retval SUCCESS/ERROR
 */
error_status i2c_display_read_buffer(uint8_t reg_addr, uint8_t* data, uint16_t len);

/**
 * @brief  设置显示板控制引脚1
 * @param  state: 引脚状态 (0/1)
 * @retval None
 */
void i2c_display_set_ctrl1(uint8_t state);

/**
 * @brief  设置显示板控制引脚2
 * @param  state: 引脚状态 (0/1)
 * @retval None
 */
void i2c_display_set_ctrl2(uint8_t state);

/**
 * @brief  获取显示板控制引脚1状态
 * @param  None
 * @retval 引脚状态 (0/1)
 */
uint8_t i2c_display_get_ctrl1(void);

/**
 * @brief  获取显示板控制引脚2状态
 * @param  None
 * @retval 引脚状态 (0/1)
 */
uint8_t i2c_display_get_ctrl2(void);

/**
 * @brief  I2C总线扫描
 * @param  None
 * @retval None
 */
void i2c_display_scan(void);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_DISPLAY_H */
