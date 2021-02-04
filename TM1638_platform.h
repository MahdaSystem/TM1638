/**
 **********************************************************************************
 * @file   TM1638_platform.h
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  TM1638 chip driver platform dependent part
 *         Functionalities of the this file:
 *          + Initialization the platform device to communicate with TM1638
 *          + Send START and STOP data
 *          + Send data to TM1638
 *          + Receive data from TM1638
 **********************************************************************************
 *
 * Copyright (c) 2021 Hossein.M (MIT License)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **********************************************************************************
 */
  
/* Define to prevent recursive inclusion ----------------------------------------*/
#ifndef _TM1638_PLATFORM_H_
#define _TM1638_PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ---------------------------------------------------------------------*/
#include "TM1638.h"
#include <stdint.h>


/* Functionality Options --------------------------------------------------------*/
/**
 * @brief  Specify the target platform
 * @note   Uncomment the line below according to the target platform
 */ 
// #define TM1638_PLATFORM_STM32        // HAL Functions
// #define TM1638_PLATFORM_ESP32_IDF    // ESP-IDF

#if defined(TM1638_PLATFORM_STM32)
#include "main.h"
#endif

#if defined(TM1638_PLATFORM_ESP32_IDF)
#include "driver/gpio.h"
#endif

#if defined(TM1638_PLATFORM_STM32)
/**
 * @brief  Specify IO Pins of STM32 connected to TM1638
 */
#define TM1638_CLK_GPIO     GPIOA
#define TM1638_CLK_PIN      GPIO_PIN_0
#define TM1638_DIO_GPIO     GPIOA
#define TM1638_DIO_PIN      GPIO_PIN_1
#define TM1638_STB_GPIO     GPIOA
#define TM1638_STB_PIN      GPIO_PIN_2
#endif

#if defined(TM1638_PLATFORM_ESP32_IDF)
/**
 * @brief  Specify IO Pins of ESP32 connected to TM1638
 */
#define TM1638_CLK_GPIO     GPIO_NUM_0
#define TM1638_DIO_GPIO     GPIO_NUM_1
#define TM1638_STB_GPIO     GPIO_NUM_2
#endif



/**
 ==================================================================================
                               ##### Functions #####                               
 ==================================================================================
 */

/**
 * @brief  Initialize platfor device to communicate TM1638.
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 *         - TM1638_FAIL: Operation failed.
 */
TM1638_Result_t
TM1638_Platform_Init(void);


/**
 * @brief  Send STOP signal to TM1638
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 *         - TM1638_FAIL: Operation failed.
 */
TM1638_Result_t
TM1638_Platform_Stop(void);


/**
 * @brief  Send START signal to TM1638 
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 *         - TM1638_FAIL: Operation failed.
 */
TM1638_Result_t
TM1638_Platform_Start(void);


/**
 * @brief  Send data bytes
 * @param  Data: Pointer to send data
 * @param  NumOfBytes: Number of bytes to send
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 *         - TM1638_FAIL: Operation failed.
 */
TM1638_Result_t
TM1638_Platform_WriteBytes(const uint8_t *Data, uint8_t NumOfBytes);


/**
 * @brief  Receive data bytes 
 * @param  Data: Pointer to save data
 * @param  NumOfBytes: Number of bytes to read
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 *         - TM1638_FAIL: Operation failed.
 */
TM1638_Result_t
TM1638_Platform_ReadBytes(uint8_t *Data, uint8_t NumOfBytes);



#ifdef __cplusplus
}
#endif

#endif // _TM1638_PLATFORM_H_
