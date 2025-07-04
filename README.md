# STM32-YSD25K08
# AT32F403ARCT7 项目

基于雅特力AT32F403ARCT7微控制器的嵌入式项目，实现RS485通信、PWM蜂鸣器控制和I2C显示板通信功能。

## 项目概述

本项目使用AT32F403ARCT7 (ARM Cortex-M4F, 240MHz) 微控制器，实现以下功能：

- **RS485通信**: 基于USART2的半双工RS485通信
- **PWM蜂鸣器**: 基于定时器3的PWM音频输出
- **I2C显示板**: 基于I2C1的显示板通信及控制
- **外部晶振**: 使用8MHz FA-128晶振

## 硬件配置

### 微控制器规格
- **型号**: AT32F403ARCT7
- **内核**: ARM Cortex-M4F with FPU
- **主频**: 240MHz
- **Flash**: 256KB
- **SRAM**: 96KB
- **封装**: LQFP64

### 外设配置

#### 1. RS485通信模块
- **接口**: USART2
- **引脚配置**:
  - PA2: USART2_TX (发送)
  - PA3: USART2_RX (接收)
  - PA4: RS485_DE (发送使能)
- **波特率**: 115200
- **数据位**: 8位
- **停止位**: 1位
- **校验位**: 无

#### 2. PWM蜂鸣器
- **接口**: TMR3_CH1
- **引脚配置**:
  - PA6: TMR3_CH1 (PWM输出)
- **特性**:
  - 可调频率范围: 20Hz - 20kHz
  - 可调占空比: 0% - 100%
  - 支持音符播放和报警音效

#### 3. I2C显示板通信
- **接口**: I2C1
- **引脚配置**:
  - PB6: I2C1_SCL (时钟线)
  - PB7: I2C1_SDA (数据线)
  - PB8: 显示板控制信号1
  - PB9: 显示板控制信号2
- **速率**: 400kHz (快速模式)
- **默认地址**: 0x3C (可配置)

#### 4. 外部晶振
- **型号**: FA-128 (22FA1280023400)
- **频率**: 8MHz
- **负载电容**: 18pF
- **精度**: ±20ppm

## 软件架构

### 文件结构
```
AT32F403_Project/
├── Core/
│   ├── Inc/
│   │   ├── main.h              # 主头文件
│   │   ├── usart_rs485.h       # RS485通信头文件
│   │   ├── buzzer_pwm.h        # PWM蜂鸣器头文件
│   │   └── i2c_display.h       # I2C显示板头文件
│   └── Src/
│       ├── main.c              # 主程序
│       ├── usart_rs485.c       # RS485通信实现
│       ├── buzzer_pwm.c        # PWM蜂鸣器实现
│       └── i2c_display.c       # I2C显示板实现
├── Drivers/
│   └── AT32F403A_407_Firmware_Library/
└── Project/
    └── AT32F403A_407_IAR/
```

### 主要功能模块

#### 1. 系统初始化
- 外部8MHz晶振配置
- 系统时钟倍频至240MHz
- GPIO引脚配置
- 中断优先级设置

#### 2. RS485通信模块
- 半双工通信控制
- 发送/接收模式自动切换
- 中断接收处理
- 数据缓冲区管理

#### 3. PWM蜂鸣器模块
- 频率和占空比可调
- 音符播放功能
- 报警音效生成
- 音乐旋律播放

#### 4. I2C显示板模块
- 标准I2C通信协议
- 多字节读写支持
- 设备地址扫描
- 控制引脚管理

## 开发环境

### 推荐IDE
- **AT32 IDE**: 官方集成开发环境
- **IAR EWARM**: 支持AT32F403系列
- **MDK-ARM**: Keil µVision5

### 编译工具链
- **GCC**: ARM GCC工具链
- **IAR**: IAR C/C++ Compiler
- **ARMCC**: ARM Compiler 5/6

### 调试工具
- **AT-Link**: 官方调试器
- **J-Link**: Segger J-Link调试器
- **ST-Link**: ST-Link/V2调试器

## 快速开始

### 1. 硬件连接
1. 连接外部8MHz晶振到OSC_IN/OSC_OUT引脚
2. 按照引脚配置连接RS485接口、蜂鸣器和I2C显示板
3. 连接调试器到SWD接口

### 2. 软件配置
1. 打开AT32 IDE
2. 导入项目文件
3. 配置编译选项
4. 编译并下载到目标板

### 3. 功能测试
1. **RS485测试**: 发送"Hello RS485"消息
2. **蜂鸣器测试**: 播放1000Hz, 100ms的蜂鸣声
3. **I2C测试**: 向显示板发送数据
4. **控制引脚测试**: 切换PB8、PB9引脚状态

## API参考

### RS485通信
```c
void rs485_init(void);                                    // 初始化
void rs485_send_string(const char* str);                // 发送字符串
uint16_t rs485_receive_data(uint8_t* data, uint16_t len); // 接收数据
```

### PWM蜂鸣器
```c
void buzzer_pwm_init(void);                              // 初始化
void buzzer_beep(uint32_t freq, uint32_t duration);     // 蜂鸣
void buzzer_play_melody(const uint32_t* note, const uint32_t* duration, uint8_t count); // 播放旋律
```

### I2C显示板
```c
void i2c_display_init(void);                             // 初始化
error_status i2c_display_send_data(uint8_t reg, uint8_t data); // 发送数据
void i2c_display_set_ctrl1(uint8_t state);             // 设置控制引脚1
```

## 注意事项

### 硬件注意事项
1. 外部晶振需要匹配的负载电容
2. RS485接口需要正确的终端电阻
3. I2C总线需要上拉电阻
4. 蜂鸣器需要适当的驱动电路

### 软件注意事项
1. 确保系统时钟配置正确
2. 中断优先级设置合理
3. I2C通信需要处理超时
4. RS485收发切换需要适当延时

## 故障排除

### 常见问题
1. **系统不启动**: 检查外部晶振连接
2. **RS485通信失败**: 检查DE引脚控制
3. **I2C通信失败**: 检查上拉电阻和设备地址
4. **蜂鸣器无声**: 检查PWM频率和占空比设置

### 调试方法
1. 使用示波器检查信号波形
2. 使用逻辑分析仪监控数字信号
3. 通过调试器查看寄存器状态
4. 添加LED指示灯监控程序状态

## 版本历史

- **v1.0.0** (2025-07-04): 初始版本
  - 实现基本的RS485、PWM、I2C功能
  - 完整的硬件抽象层
  - 基础测试程序

## 许可证

本项目采用MIT许可证 - 详见 [LICENSE](LICENSE) 文件

## 贡献

欢迎提交Issue和Pull Request来改进项目。

## 联系方式

- 项目维护者: [Jason]
- 邮箱: [jason@ysd.tw]
- GitHub: [https://github.com/owewiin/STM32-YSD25K08](https://github.com/owewiin/STM32-YSD25K08)

## 参考资料

- [AT32F403/407 Reference Manual](https://www.arterytek.com/download/RM/RM_AT32F403A_407_CH_V2.04.pdf)
- [AT32F403/407 Datasheet](https://www.arterytek.com/download/DS/DS_AT32F403A_407_CH_V2.03.pdf)
- [AT32 IDE User Guide](https://www.arterytek.com/cn/support/index.jsp)
