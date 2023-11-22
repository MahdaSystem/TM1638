/**
 **********************************************************************************
 * @file   TM1638_platform.h
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  A sample Platform dependent layer for TM1638 Driver
 **********************************************************************************
 *
 * Copyright (c) 2021 Mahda Embedded System (MIT License)
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
 * @brief  Specify IO Pins of AVR connected to TM1638
 */
#define TM1638_AVR_CLK      8000000UL
#define TM1638_CLK_DDR      DDRA
#define TM1638_CLK_PORT     PORTA
#define TM1638_CLK_NUM      0
#define TM1638_DIO_DDR      DDRA
#define TM1638_DIO_PORT     PORTA
#define TM1638_DIO_PIN      PINA
#define TM1638_DIO_NUM      1
#define TM1638_STB_DDR      DDRA
#define TM1638_STB_PORT     PORTA
#define TM1638_STB_NUM      2



/**
 ==================================================================================
                               ##### Functions #####                               
 ==================================================================================
 */

/**
 * @brief  Initialize platform device to communicate TM1638.
 * @param  Handler: Pointer to handler
 * @retval None
 */
void
TM1638_Platform_Init(TM1638_Handler_t *Handler);



#ifdef __cplusplus
}
#endif

#endif //! _TM1638_PLATFORM_H_
