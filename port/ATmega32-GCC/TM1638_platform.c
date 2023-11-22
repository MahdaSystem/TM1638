/**
 **********************************************************************************
 * @file   TM1638_platform.c
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
  
/* Includes ---------------------------------------------------------------------*/
#include "TM1638_platform.h"
#include <avr/io.h>
#include <util/delay.h>




/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

static void
TM1638_PlatformInit(void)
{
  TM1638_CLK_DDR |= (1<<TM1638_CLK_NUM);
  TM1638_DIO_DDR |= (1<<TM1638_DIO_NUM);
  TM1638_STB_DDR |= (1<<TM1638_STB_NUM);
}

static void
TM1638_PlatformDeInit(void)
{
  TM1638_CLK_DDR &= ~(1<<TM1638_CLK_NUM);
  TM1638_CLK_PORT &= ~(1<<TM1638_CLK_NUM);
  TM1638_DIO_DDR &= ~(1<<TM1638_DIO_NUM);
  TM1638_DIO_PORT &= ~(1<<TM1638_DIO_NUM);
  TM1638_STB_DDR &= ~(1<<TM1638_STB_NUM);
  TM1638_STB_PORT &= ~(1<<TM1638_STB_NUM);
}

static void
TM1638_DioConfigOut(void)
{
  TM1638_DIO_DDR |= (1<<TM1638_DIO_NUM);
}

static void
TM1638_DioConfigIn(void)
{
  TM1638_DIO_DDR &= ~(1<<TM1638_DIO_NUM);
}

static void
TM1638_DioWrite(uint8_t Level)
{
  if (Level)
    TM1638_DIO_PORT |= (1<<TM1638_DIO_NUM);
  else
    TM1638_DIO_PORT &= ~(1<<TM1638_DIO_NUM);
}

static uint8_t
TM1638_DioRead(void)
{
  return (TM1638_DIO_PIN & (1 << TM1638_DIO_NUM)) ? 1 : 0;
}

static void
TM1638_ClkWrite(uint8_t Level)
{
  if (Level)
    TM1638_CLK_PORT |= (1<<TM1638_CLK_NUM);
  else
    TM1638_CLK_PORT &= ~(1<<TM1638_CLK_NUM);
}

static void
TM1638_StbWrite(uint8_t Level)
{
  if (Level)
    TM1638_STB_PORT |= (1<<TM1638_STB_NUM);
  else
    TM1638_STB_PORT &= ~(1<<TM1638_STB_NUM);
}

static void
TM1638_DelayUs(uint8_t Delay)
{
  for (; Delay; --Delay)
    _delay_us(1);
}



/**
 ==================================================================================
                            ##### Public Functions #####                           
 ==================================================================================
 */
 
/**
 * @brief  Initialize platform device to communicate TM1638.
 * @param  Handler: Pointer to handler
 * @retval None
 */
void
TM1638_Platform_Init(TM1638_Handler_t *Handler)
{
  Handler->PlatformInit = TM1638_PlatformInit;
  Handler->PlatformDeInit = TM1638_PlatformDeInit;
  Handler->DioConfigOut = TM1638_DioConfigOut;
  Handler->DioConfigIn = TM1638_DioConfigIn;
  Handler->DioWrite = TM1638_DioWrite;
  Handler->DioRead = TM1638_DioRead;
  Handler->ClkWrite = TM1638_ClkWrite;
  Handler->StbWrite = TM1638_StbWrite;
  Handler->DelayUs = TM1638_DelayUs;
}
