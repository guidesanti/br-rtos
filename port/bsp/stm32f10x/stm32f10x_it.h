#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f10x.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#define WWDG_IRQHandler             (0U)
#define PVD_IRQHandler              (0U)
#define TAMPER_IRQHandler           (0U)
#define RTC_IRQHandler              __BR_RtcIsr
#define FLASH_IRQHandler            (0U)
#define RCC_IRQHandler              (0U)
#define EXTI0_IRQHandler            (0U)
#define EXTI1_IRQHandler            (0U)
#define EXTI2_IRQHandler            (0U)
#define EXTI3_IRQHandler            (0U)
#define EXTI4_IRQHandler            (0U)
#define DMA1_Channel1_IRQHandler    (0U)
#define DMA1_Channel2_IRQHandler    (0U)
#define DMA1_Channel3_IRQHandler    (0U)
#define DMA1_Channel4_IRQHandler    (0U)
#define DMA1_Channel5_IRQHandler    (0U)
#define DMA1_Channel6_IRQHandler    (0U)
#define DMA1_Channel7_IRQHandler    (0U)
#define ADC1_2_IRQHandler           (0U)
#define USB_HP_CAN_TX_IRQHandler    (0U)
#define USB_LP_CAN_RX0_IRQHandler   (0U)
#define CAN_RX1_IRQHandler          (0U)
#define CAN_SCE_IRQHandler          (0U)
#define EXTI9_5_IRQHandler          (0U)
#define TIM1_BRK_IRQHandler         (0U)
#define TIM1_UP_IRQHandler          (0U)
#define TIM1_TRG_COM_IRQHandler     (0U)
#define TIM1_CC_IRQHandler          (0U)
#define TIM2_IRQHandler             (0U)
#define TIM3_IRQHandler             (0U)
#define TIM4_IRQHandler             (0U)
#define I2C1_EV_IRQHandler          (0U)
#define I2C1_ER_IRQHandler          (0U)
#define I2C2_EV_IRQHandler          (0U)
#define I2C2_ER_IRQHandler          (0U)
#define SPI1_IRQHandler             __BR_Spi1Isr
#define SPI2_IRQHandler             __BR_Spi2Isr
#define USART1_IRQHandler           __BR_UsartIsr1
#define USART2_IRQHandler           __BR_UsartIsr2
#define USART3_IRQHandler           __BR_UsartIsr3
#define EXTI15_10_IRQHandler        (0U)
#define RTCAlarm_IRQHandler         (0U)
#define USBWakeUp_IRQHandler        (0U)
#define TIM8_BRK_IRQHandler         (0U)
#define TIM8_UP_IRQHandler          (0U)
#define TIM8_TRG_COM_IRQHandler     (0U)
#define TIM8_CC_IRQHandler          (0U)
#define ADC3_IRQ_Handler            (0U)
#define FSMC_IRQHandler             (0U)
#define SDIO_IRQHandler             (0U)
#define TIM5_IRQHandler             (0U)
#define SPI3_IRQHandler             __BR_Spi3Isr
#define UART4_IRQHandler            __BR_UartIsr4
#define UART5_IRQHandler            __BR_UartIsr5
#define TIM6_IRQHandler             (0U)
#define TIM7_IRQHandler             (0U)
#define DMA2_Channel1_IRQHandler    (0U)
#define DMA2_Channel2_IRQHandler    (0U)
#define DMA2_Channel3_IRQHandler    (0U)
#define DMA2_Channel4_5_IRQHandler  (0U)


#ifdef __cplusplus
}
#endif


#endif /* __STM32F10x_IT_H */
