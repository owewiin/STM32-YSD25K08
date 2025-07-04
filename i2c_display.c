/**
 * @file i2c_display.c
 * @brief I2C显示板通信模块实现
 * @author Your Name
 * @date 2025-07-04
 */

/* Includes ------------------------------------------------------------------*/
#include "i2c_display.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_TIMEOUT         1000    // I2C超时时间 (ms)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static error_status i2c_wait_flag(uint32_t flag, flag_status status, uint32_t timeout);
static error_status i2c_wait_event(uint32_t event, uint32_t timeout);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  等待I2C标志位
 * @param  flag: 标志位
 * @param  status: 期望状态
 * @param  timeout: 超时时间
 * @retval SUCCESS/ERROR
 */
static error_status i2c_wait_flag(uint32_t flag, flag_status status, uint32_t timeout)
{
    uint32_t tick_start = get_tick();
    
    while(i2c_flag_get(DISPLAY_I2C, flag) != status)
    {
        if((get_tick() - tick_start) > timeout)
        {
            return ERROR;
        }
    }
    
    return SUCCESS;
}

/**
 * @brief  等待I2C事件
 * @param  event: 事件
 * @param  timeout: 超时时间
 * @retval SUCCESS/ERROR
 */
static error_status i2c_wait_event(uint32_t event, uint32_t timeout)
{
    uint32_t tick_start = get_tick();
    
    while(!i2c_event_check(DISPLAY_I2C, event))
    {
        if((get_tick() - tick_start) > timeout)
        {
            return ERROR;
        }
    }
    
    return SUCCESS;
}

/**
 * @brief  I2C显示板初始化
 * @param  None
 * @retval None
 */
void i2c_display_init(void)
{
    i2c_init_type i2c_init_struct;
    
    /* 使能I2C时钟 */
    crm_periph_clock_enable(DISPLAY_I2C_CLK, TRUE);
    
    /* I2C配置 */
    i2c_default_para_init(&i2c_init_struct);
    i2c_init_struct.mode = I2C_MODE_MASTER;
    i2c_init_struct.master_clock_speed = DISPLAY_I2C_SPEED;
    i2c_init_struct.clock_duty = I2C_CLOCK_DUTY_2;
    i2c_init_struct.address_mode = I2C_ADDRESS_MODE_7BIT;
    i2c_init_struct.own_address1 = 0x00;
    i2c_init(DISPLAY_I2C, &i2c_init_struct);
    
    /* 使能I2C */
    i2c_enable(DISPLAY_I2C, TRUE);
    
    /* 初始化控制引脚 */
    gpio_bits_reset(DISPLAY_CTRL1_GPIO_PORT, DISPLAY_CTRL1_GPIO_PIN);
    gpio_bits_reset(DISPLAY_CTRL2_GPIO_PORT, DISPLAY_CTRL2_GPIO_PIN);
    
    /* 延时等待设备稳定 */
    delay_ms(100);
}

/**
 * @brief  I2C写单个字节
 * @param  device_addr: 设备地址
 * @param  reg_addr: 寄存器地址
 * @param  data: 数据
 * @retval SUCCESS/ERROR
 */
error_status i2c_display_write_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t data)
{
    /* 生成起始信号 */
    i2c_start_generate(DISPLAY_I2C);
    
    /* 等待起始信号发送完成 */
    if(i2c_wait_event(I2C_EVENT_MASTER_START_GENERATED, I2C_TIMEOUT) != SUCCESS)
    {
        return ERROR;
    }
    
    /* 发送设备地址 */
    i2c_7bit_address_send(DISPLAY_I2C, device_addr << 1, I2C_DIRECTION_TRANSMIT);
    
    /* 等待地址发送完成 */
    if(i2c_wait_event(I2C_EVENT_MASTER_TRANSMIT_ADDRESS_MATCHED, I2C_TIMEOUT) != SUCCESS)
    {
        return ERROR;
    }
    
    /* 发送寄存器地址 */
    i2c_data_send(DISPLAY_I2C, reg_addr);
    
    /* 等待数据发送完成 */
    if(i2c_wait_event(I2C_EVENT_MASTER_DATA_TRANSMIT_COMPLETE, I2C_TIMEOUT) != SUCCESS)
    {
        return ERROR;
    }
    
    /* 发送数据 */
    i2c_data_send(DISPLAY_I2C, data);
    
    /* 等待数据发送完成 */
    if(i2c_wait_event(I2C_EVENT_MASTER_DATA_TRANSMIT_COMPLETE, I2C_TIMEOUT) != SUCCESS)
    {
        return ERROR;
    }
    
    /* 生成停止信号 */
    i2c_stop_generate(DISPLAY_I2C);
    
    return SUCCESS;
}

/**
 * @brief  I2C读单个字节
 * @param  device_addr: 设备地址
 * @param  reg_addr: 寄存器地址
 * @param  data: 数据指针
 * @retval SUCCESS/ERROR
 */
error_status i2c_display_read_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t* data)
{
    /* 生成起始信号 */
    i2c_start_generate(DISPLAY_I2C);
    
    /* 等待起始信号发送完成 */
    if(i2c_wait_event(I2C_EVENT_MASTER_START_GENERATED, I2C_TIMEOUT) != SUCCESS)
    {
        return ERROR;
    }
    
    /* 发送设备地址 (写模式) */
    i2c_7bit_address_send(DISPLAY_I2C, device_addr << 1, I2C_DIRECTION_TRANSMIT);
    
    /* 等待地址发送完成 */
    if(i2c_wait_event(I2C_EVENT_MASTER_TRANSMIT_ADDRESS_MATCHED, I2C_TIMEOUT) != SUCCESS)
    {
        return ERROR;
    }
    
    /* 发送寄存器地址 */
    i2c_data_send(DISPLAY_I2C, reg_addr);
    
    /* 等待数据发送完成 */
    if(i2c_wait_event(I2C_EVENT_MASTER_DATA_TRANSMIT_COMPLETE, I2C_TIMEOUT) != SUCCESS)
    {
        return ERROR;
    }
    
    /* 生成重复起始信号 */
    i2c_start_generate(DISPLAY_I2C);
    
    /* 等待起始信号发送完成 */
    if(i2c_wait_event(I2C_EVENT_MASTER_START_GENERATED, I2C_TIMEOUT) != SUCCESS)
    {
        return ERROR;
    }
    
    /* 发送设备地址 (读模式) */
    i2c_7bit_address_send(DISPLAY_I2C, device_addr << 1, I2C_DIRECTION_RECEIVE);
    
    /* 等待地址发送完成 */
    if(i2c_wait_event(I2C_EVENT_MASTER_RECEIVE_ADDRESS_MATCHED, I2C_TIMEOUT) != SUCCESS)
    {
        return ERROR;
    }
    
    /* 禁用应答 */
    i2c_ack_enable(DISPLAY_I2C, FALSE);
    
    /* 生成停止信号 */
    i2c_stop_generate(DISPLAY_I2C);
    
    /* 等待接收数据 */
    if(i2c_wait_flag(I2C_RDBF_FLAG, SET, I2C_TIMEOUT) != SUCCESS)
    {
        return ERROR;
    }
    
    /* 读取数据 */
    *data = i2c_data_receive(DISPLAY_I2C);
    
    /* 使能应答 */
    i2c_ack_enable(DISPLAY_I2C, TRUE);
    
    return SUCCESS;
}

/**
 * @brief  I2C写多个字节
 * @param  device_addr: 设备地址
 * @param  reg_addr: 寄存器地址
 * @param  data: 数据缓冲区
 * @param  len: 数据长度
 * @retval SUCCESS/ERROR
 */
error_status i2c_display_write_buffer(uint8_t device_addr, uint8_t reg_addr, uint8_t* data, uint16_t len)
{
    /* 生成起始信号 */
    i2c_start_generate(DISPLAY_I2C);
    
    /* 等待起始信号发送完成 */
    if(i2c_wait_event(I2C_EVENT_MASTER_START_GENERATED, I2C_TIMEOUT) != SUCCESS)
    {
        return ERROR;
