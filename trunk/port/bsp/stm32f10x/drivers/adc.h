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
 * It supports single and continuous conversions over a group of 16 channels
 * for each ADC device.
 * The supported device operations for an ADC device are:
 * - open (must be executed on every ADC device before using it)
 * - close (may be called when there is no need to use the ADC device)
 * - control (set/get configuration parameters)
 * - read (read last converted channel values
 * - write (unsupported, has no effect over ADC devices)
 *
 * This driver will always covert a sequence of channels defined by a group.
 * The group may have 1 to 16 channels and may be converted in single mode or
 * in continuous mode.
 * The ADC device driver has an internal buffer to store the last converted
 * value for each ADC channel of the selected group.
 * These values may be read using the BR_DeviceRead() operation passing a valid
 * ADC device pointer.
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
 * Conversion Modes
 * The generic ADC driver supports the following conversion modes:
 * a) Single: the selected group of channels will be converted once, after a
 *    control command ADC_CTRL_START_CONV has been issued.
 * b) Continuous: the selected group of channels will be converted continuously,
 *    after a control command ADC_CTRL_START_CONV has been issued and will stop
 *    the conversion just after a ADC_CTRL_STOP_CONV control command.
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
 * Control Commands and Parameter Format:
 * =============================================================================
 * Control Command              | Parameter Format
 * =============================================================================
 * ADC_CTRL_SET_CONV_MODE       | uint8_t
 * -----------------------------------------------------------------------------
 * ADC_CTRL_SET_DATA_ALIGN      | uint8_t
 * -----------------------------------------------------------------------------
 * ADC_CTRL_SET_CH_SAMPLE_TIME  | uint8_t
 * -----------------------------------------------------------------------------
 * ADC_CTRL_SET_CONV_GROUP      | uint8_t[16]
 * -----------------------------------------------------------------------------
 * ADC_CTRL_SET_CONV_GROUP_LEN  | uint8_t
 * -----------------------------------------------------------------------------
 * ADC_CTRL_START_CONV          | NULL
 * -----------------------------------------------------------------------------
 * ADC_CTRL_STOP_CONV           | NULL
 * =============================================================================
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/* ADC control commands. */
#define ADC_CTRL_SET_CONV_MODE      (0U)  /**< Set the conversion mode. */
#define ADC_CTRL_SET_DATA_ALIGN     (1U)  /**< Set the data alignment. */
#define ADC_CTRL_SET_CH_SAMPLE_TIME (2U)  /**< Set the channel sample time. */
#define ADC_CTRL_SET_CONV_GROUP     (3U)  /**< Set the conversion group sequence. */
#define ADC_CTRL_SET_CONV_GROUP_LEN (4U)  /**< Set the conversion group length. */
#define ADC_CTRL_START_CONV         (5U)  /**< Start conversion. */
#define ADC_CTRL_STOP_CONV          (6U)  /**< Stop conversion. */

/* Conversion modes. */
#define ADC_CONV_MODE_SINGLE        (0U)  /**< Single conversion mode. */
#define ADC_CONV_MODE_CONT          (1U)  /**< Continuous conversion mode. */

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
#define ADC_ADDR_CH_0   (0x00U) /* adc Channel 0  */
#define ADC_ADDR_CH_1   (0x02U) /* adc Channel 1  */
#define ADC_ADDR_CH_2   (0x04U) /* adc Channel 2  */
#define ADC_ADDR_CH_3   (0x06U) /* adc Channel 3  */
#define ADC_ADDR_CH_4   (0x08U) /* adc Channel 4  */
#define ADC_ADDR_CH_5   (0x0AU) /* adc Channel 5  */
#define ADC_ADDR_CH_6   (0x0CU) /* adc Channel 6  */
#define ADC_ADDR_CH_7   (0x0EU) /* adc Channel 7  */
#define ADC_ADDR_CH_8   (0x10U) /* adc Channel 8  */
#define ADC_ADDR_CH_9   (0x12U) /* adc Channel 9  */
#define ADC_ADDR_CH_10  (0x14U) /* adc Channel 10 */
#define ADC_ADDR_CH_11  (0x16U) /* adc Channel 11 */
#define ADC_ADDR_CH_12  (0x18U) /* adc Channel 12 */
#define ADC_ADDR_CH_13  (0x1AU) /* adc Channel 13 */
#define ADC_ADDR_CH_14  (0x1CU) /* adc Channel 14 */
#define ADC_ADDR_CH_15  (0x1EU) /* adc Channel 15 */


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/
void __BR_AdcInit(void);
void __BR_AdcIsr1_2(void);
void __BR_AdcIsr3(void);


/** @} */

/** @} */

#endif /* _ADC_H_ */
