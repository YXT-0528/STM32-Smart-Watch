# STM32 智能可编程手表

基于 STM32F103 标准外设库开发的多功能智能手表项目，搭载 0.96 寸 OLED 显示屏，集成时钟、传感器、游戏等多种功能。

## 硬件平台

| 模块       | 型号                  | 接口   |
| ---------- | --------------------- | ------ |
| 主控       | STM32F103VET6         | —      |
| 显示       | 0.96" OLED 128×64     | I2C    |
| 6轴传感器  | MPU6050               | I2C    |
| 实时时钟   | 板载 RTC              | —      |
| 电池监测   | ADC 分压采样          | ADC    |
| LED        | GPIO                  | —      |
| 按键       | GPIO 外部中断         | —      |

## 功能列表

- **时钟主页** — 显示日期、时间、电池电量
- **滑动菜单** — 图标式滑动切换，支持动画过渡
- **秒表** — 开始/停止/重置
- **手电筒** — LED 开关控制
- **姿态角度** — MPU6050 采集欧拉角（互补滤波），展示 Roll/Pitch/Yaw
- **水平仪** — 圆形水平仪界面，实时显示倾角
- **谷歌小恐龙游戏** — OLED 上的经典小游戏
- **趣表情** — 动态表情动画
- **日期时间设置** — RTC 时间校准

## 项目结构

```
├── User/               # 用户层 — main.c、中断服务、外设配置头文件
├── Hardware/           # 硬件驱动 — OLED、MPU6050、I2C、ADC、按键、LED
├── System/             # 系统模块 — RTC、延时、定时器、菜单逻辑
├── Start/              # 启动文件、内核文件、系统时钟
├── Labrary/            # STM32F10x 标准外设库
├── DebugConfig/        # 调试配置
├── Listings/           # 编译产物（不追踪）
├── Objects/            # 编译产物（不追踪）
└── .vscode/            # VS Code 配置（Keil Assistant）
```

## 开发环境

- **IDE**: Keil MDK-ARM V5
- **固件库**: STM32F10x Standard Peripheral Library V3.5.0
- **编译器**: ARMCC V5.06
- **辅助工具**: VS Code + Keil Assistant 插件

## 快速开始

1. 用 Keil MDK 打开 `Project.uvprojx`
2. 确认芯片型号为 `STM32F103VET6`
3. 编译（Build）工程
4. 通过 ST-Link / J-Link 将程序下载到开发板

## 按键操作

| 按键     | 功能           |
| -------- | -------------- |
| Key1     | 上一项 / 左移  |
| Key2     | 下一项 / 右移  |
| Key3     | 确认 / 进入    |
| Key4     | 保留（LED 测试）|

## 致谢

OLED 驱动及部分图形库来自 [江协科技](https://jiangxiekeji.com)。

## 许可证

本项目仅用于学习交流。
