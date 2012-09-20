/**
 * @file        adc.h
 * @date        2012, Sptember 17
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       STM32F10x ADC device driver header file.
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */


#ifndef _ADC_H_
#define _ADC_H_

/**
 * @addtogroup BSPstm32f10x BSP STM32F10x
 * @{
 */

/**
 * @addtogroup ADC
 * @{
 *
 * This is a generic device driver for STM32F10x BSP.
 * It supports just single conversion over a single ADC channel through the
 * BR_DeviceRead() operation.
 * Every call to BR_DeviceRead() will start the ADC channel conversion and return
 * just after the end of conversion. The time to complete a conversion will depends
 * on the channel sample time that may be configured through BR_DeviceControl()
 * operation using ADC_CTRL_SET_CH_SAMPLE_TIME control command.
 *
 * The address available for read operation are:
 * 0x00: Channel 0
 * 0x02: Channel 1
 * 0x04: Channel 2
 * 0x06: Channel 3
 * 0x08: Channel 4
 * 0x0A: Channel 5
 * 0x0C: Channel 6
 * 0x0E: Channel 7
 * 0x10: Channel 8
 * 0x12: Channel 9
 * 0x14: Channel 10
 * 0x16: Channel 11
 * 0x18: Channel 12
 * 0x1A: Channel 13
 * 0x1C: Channel 14
 * 0x1E: Channel 15
 *
 * Data Alignment
 * Each converted value is 16 bits long. The value alignment may be chosen to
 * be left or right using the control command ADC_CTRL_SET_DATA_ALIGN.
 * - ADC_DATA_ALIGN_RIGHT -> data values are right aligned
 * - ADC_DATA_ALIGN_LEFT  -> data values are left aligned
 *
 * Channel Sample Time
 * The sample time may be configured individually for each ADCchannel using the
 * control command ADC_CTRL_SET_CH_SAMPLE_TIME.
 * The available sample times are:
 * - ADC_CH_SAMPLE_TIME_1_5   -> 1,5 cycles
 * - ADC_CH_SAMPLE_TIME_7_5   -> 7,5 cycles
 * - ADC_CH_SAMPLE_TIME_13_5  -> 13,5 cycles
 * - ADC_CH_SAMPLE_TIME_28_5  -> 28,5 cycles
 * - ADC_CH_SAMPLE_TIME_41_5  -> 41,5 cycles
 * - ADC_CH_SAMPLE_TIME_55_5  -> 55,5 cycles
 * - ADC_CH_SAMPLE_TIME_71_5  -> 71,5 cycles
 * - ADC_CH_SAMPLE_TIME_239_5 -> 239,5 cycles
 *
 * Device Operations
 * The supported device operations for an ADC device are:
 * - open (must be executed on every ADC device before using it)
 * - close (may be called when there is no need to use the ADC device)
 * - control (set/get configuration parameters)
 * - read (read a channel converted value)
 * - write (unsupported, has no effect over ADC devices)
 *
 * Control Commands and Parameter Format:
 * =============================================================================
 * Control Command              | Parameter Format
 * =============================================================================
 * ADC_CTRL_SET_DATA_ALIGN      | uint8_t*
 * -----------------------------------------------------------------------------
 * ADC_CTRL_SET_CH_SAMPLE_TIME  | BR_AdcChannelSampleTime_t*
 * =============================================================================
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/* ADC control commands. */
#define ADC_CTRL_SET_DATA_ALIGN     (0U)  /**< Set the data alignment. */
#define ADC_CTRL_SET_CH_SAMPLE_TIME (1U)  /**< Set the channel sample time. */

/* Data alignment. */
#define ADC_DATA_ALIGN_RIGHT        (0U)  /**< Data is right aligned. */
#define ADC_DATA_ALIGN_LEFT         (1U)  /**< Data is left aligned. */

/* Channel sample time. */
#define ADC_CH_SAMPLE_TIME_1_5      (0U)  /**< 1,5 cycles. */
#define ADC_CH_SAMPLE_TIME_7_5      (1U)  /**< 7,5 cycles. */
#define ADC_CH_SAMPLE_TIME_13_5     (2U)  /**< 13,5 cycles. */
#define ADC_CH_SAMPLE_TIME_28_5     (3U)  /**< 28,5 cycles. */
#define ADC_CH_SAMPLE_TIME_41_5     (4U)  /**< 41,5 cycles. */
#define ADC_CH_SAMPLE_TIME_55_5     (5U)  /**< 55,5 cycles. */
#define ADC_CH_SAMPLE_TIME_71_5     (6U)  /**< 71,5 cycles. */
#define ADC_CH_SAMPLE_TIME_239_5    (7U)  /**< 239,5 cycles. */

/* Read operation addresses. */
#define ADC_ADDR_CH_0   (0x00U) /* ADC Channel 0  */
#define ADC_ADDR_CH_1   (0x02U) /* ADC Channel 1  */
#define ADC_ADDR_CH_2   (0x04U) /* ADC Channel 2  */
#define ADC_ADDR_CH_3   (0x06U) /* ADC Channel 3  */
#define ADC_ADDR_CH_4   (0x08U) /* ADC Channel 4  */
#define ADC_ADDR_CH_5   (0x0AU) /* ADC Channel 5  */
#define ADC_ADDR_CH_6   (0x0CU) /* ADC Channel 6  */
#define ADC_ADDR_CH_7   (0x0EU) /* ADC Channel 7  */
#define ADC_ADDR_CH_8   (0x10U) /* ADC Channel 8  */
#define ADC_ADDR_CH_9   (0x12U) /* ADC Channel 9  */
#define ADC_ADDR_CH_10  (0x14U) /* ADC Channel 10 */
#define ADC_ADDR_CH_11  (0x16U) /* ADC Channel 11 */
#define ADC_ADDR_CH_12  (0x18U) /* ADC Channel 12 */
#define ADC_ADDR_CH_13  (0x1AU) /* ADC Channel 13 */
#define ADC_ADDR_CH_14  (0x1CU) /* ADC Channel 14 */
#define ADC_ADDR_CH_15  (0x1EU) /* ADC Channel 15 */

/* List of ADC channels. */
#define ADC_CHANNEL_0   (0U)  /* ADC Channel 0  */
#define ADC_CHANNEL_1   (1U)  /* ADC Channel 1  */
#define ADC_CHANNEL_2   (2U)  /* ADC Channel 2  */
#define ADC_CHANNEL_3   (3U)  /* ADC Channel 3  */
#define ADC_CHANNEL_4   (4U)  /* ADC Channel 4  */
#define ADC_CHANNEL_5   (5U)  /* ADC Channel 5  */
#define ADC_CHANNEL_6   (6U)  /* ADC Channel 6  */
#define ADC_CHANNEL_7   (7U)  /* ADC Channel 7  */
#define ADC_CHANNEL_8   (8U)  /* ADC Channel 8  */
#define ADC_CHANNEL_9   (9U)  /* ADC Channel 9  */
#define ADC_CHANNEL_10  (10U) /* ADC Channel 10 */
#define ADC_CHANNEL_11  (11U) /* ADC Channel 11 */
#define ADC_CHANNEL_12  (12U) /* ADC Channel 12 */
#define ADC_CHANNEL_13  (13U) /* ADC Channel 13 */
#define ADC_CHANNEL_14  (14U) /* ADC Channel 14 */
#define ADC_CHANNEL_15  (15U) /* ADC Channel 15 */

typedef struct
{
    uint8_t channel;    /**< The channel number. */
    uint8_t sampleTime; /**< The channel sample time. */
} BR_AdcChannelSampleTime_t;


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/
void __BR_AdcInit(void);
void __BR_AdcIsr1_2(void);
void __BR_AdcIsr3(void);


/** @} */

/** @} */

#endif /* _ADC_H_ */
