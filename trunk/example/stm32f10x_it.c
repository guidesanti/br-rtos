/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "port.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  asm ("BKPT 0x01");
  asm ("MRS     R0,MSP                  ; Read PSP");
  asm ("LDR     R1,[R0,#24]             ; Read Saved PC from Stack");


  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  *
  * The SysTick is configured to execute every millisecond and it is used as the system
  * time base unit.
  */
//void SysTick_Handler(void)
//{
//  extern uint32_t tickCounter;
//  extern uint32_t flag;
//  tickCounter++;
//  if (0U == (tickCounter % 10U))
//  {
//    flag = 1U;
//  }
//}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @}
  */


/******************************************************************************/
/*                             The Vector Table                               */
/******************************************************************************/
typedef void( *intfunc )( void );
typedef union
{
    intfunc __fun;
    void*   __ptr;
} intvec_elem;

#pragma language=extended
#pragma segment="CSTACK"

void __iar_program_start( void );

#pragma location = ".intvec"
const intvec_elem __vector_table[] =
{
    /* Interrupt Name           Interrupt Handler */
    /* MSP */                   { .__ptr = __sfe( "CSTACK" ) },
    /* Reset */                 &__iar_program_start,
    /* NMI */                   NMI_Handler,
    /* HardFault */             HardFault_Handler,
    /* MemManage */             MemManage_Handler,
    /* BusFault */              BusFault_Handler,
    /* UsageFault */            UsageFault_Handler,
    /* Reserved */              0U,
    /* Reserved */              0U,
    /* Reserved */              0U,
    /* Reserved */              0U,
    /* SVCall */                SVC_Handler,
    /* DebugMonitor */          DebugMon_Handler,
    /* Reserved */              0U,
    /* PendSV */                BR_PortPendSVCHandler,
    /* SysTick */               BR_PortSysTickHandler,
    /* 0: WWDG */               WWDG_IRQHandler,
    /* 1: PVD */                PVD_IRQHandler,
    /* 2: TAMPER */             TAMPER_IRQHandler,
    /* 3: RTC */                RTC_IRQHandler,
    /* 4: FLASH */              FLASH_IRQHandler,
    /* 5: RCC */                RCC_IRQHandler,
    /* 6: EXTI0 */              EXTI0_IRQHandler,
    /* 7: EXTI1 */              EXTI1_IRQHandler,
    /* 8: EXTI2 */              EXTI2_IRQHandler,
    /* 9: EXTI3 */              EXTI3_IRQHandler,
    /* 10: EXTI4 */             EXTI4_IRQHandler,
    /* 11: DMA1_Channel1 */     DMA1_Channel1_IRQHandler,
    /* 12: DMA1_Channel2 */     DMA1_Channel2_IRQHandler,
    /* 13: DMA1_Channel3 */     DMA1_Channel3_IRQHandler,
    /* 14: DMA1_Channel4 */     DMA1_Channel4_IRQHandler,
    /* 15: DMA1_Channel5 */     DMA1_Channel5_IRQHandler,
    /* 16: DMA1_Channel6 */     DMA1_Channel6_IRQHandler,
    /* 17: DMA1_Channel7 */     DMA1_Channel7_IRQHandler,
    /* 18: ADC1_2 */            ADC1_2_IRQHandler,
    /* 19: USB_HP_CAN_TX */     USB_HP_CAN_TX_IRQHandler,
    /* 20: USB_LP_CAN_RX0 */    USB_LP_CAN_RX0_IRQHandler,
    /* 21: CAN_RX1 */           CAN_RX1_IRQHandler,
    /* 22: CAN_SCE */           CAN_SCE_IRQHandler,
    /* 23: EXTI9_5 */           EXTI9_5_IRQHandler,
    /* 24: TIM1_BRK */          TIM1_BRK_IRQHandler,
    /* 25: TIM1_UP */           TIM1_UP_IRQHandler,
    /* 26: TIM1_TR_COM */       TIM1_TRG_COM_IRQHandler,
    /* 27: TIM1_CC */           TIM1_CC_IRQHandler,
    /* 28: TIM2 */              TIM2_IRQHandler,
    /* 29: TIM3 */              TIM3_IRQHandler,
    /* 30: TIM4 */              TIM4_IRQHandler,
    /* 31: I2C1_EV */           I2C1_EV_IRQHandler,
    /* 32: I2C1_ER */           I2C1_ER_IRQHandler,
    /* 33: I2C2_EV */           I2C2_EV_IRQHandler,
    /* 34: I2C2_ER */           I2C2_ER_IRQHandler,
    /* 35: SPI1 */              SPI1_IRQHandler,
    /* 36: SPI2 */              SPI2_IRQHandler,
    /* 37: USART1 */            USART1_IRQHandler,
    /* 38: USART2 */            USART2_IRQHandler,
    /* 39: USART3 */            USART3_IRQHandler,
    /* 40: EXTI15_10 */         EXTI15_10_IRQHandler,
    /* 41: RTCAlarm */          RTCAlarm_IRQHandler,
    /* 42: USBWakeUp */         USBWakeUp_IRQHandler,
    /* 43: TIM8_BRK */          TIM8_BRK_IRQHandler,
    /* 44: TIM8_UP */           TIM8_UP_IRQHandler,
    /* 45: TIM8_TRG_COM */      TIM8_TRG_COM_IRQHandler,
    /* 46: TIM8_CC */           TIM8_CC_IRQHandler,
    /* 47: ADC3 */              ADC3_IRQ_Handler,
    /* 48: FSMC */              FSMC_IRQHandler,
    /* 49: SDIO */              SDIO_IRQHandler,
    /* 50: TIM5 */              TIM5_IRQHandler,
    /* 51: SPI3 */              SPI3_IRQHandler,
    /* 52: UART4 */             UART4_IRQHandler,
    /* 53: UART5 */             UART5_IRQHandler,
    /* 54: TIM6 */              TIM6_IRQHandler,
    /* 55: TIM7 */              TIM7_IRQHandler,
    /* 56: DMA2_Channel1 */     DMA2_Channel1_IRQHandler,
    /* 57: DMA2_Channel2 */     DMA2_Channel2_IRQHandler,
    /* 58: DMA2_Channel3 */     DMA2_Channel3_IRQHandler,
    /* 59: DMA2_Channel4_5 */   DMA2_Channel4_5_IRQHandler,
};


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
