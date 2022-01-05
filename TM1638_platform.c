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

#if defined(TM1638_PLATFORM_AVR)
#include <avr/io.h>
#define F_CPU TM1638_AVR_CLK
#include <util/delay.h>
#elif defined(TM1638_PLATFORM_STM32)
#include "main.h"
#elif defined(TM1638_PLATFORM_ESP32_IDF)
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"
#endif



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

#if defined(TM1638_PLATFORM_STM32)
void TM1638_SetGPIO_OUT(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
									
void TM1638_SetGPIO_IN_PU(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
#elif defined(TM1638_PLATFORM_ESP32_IDF)
void TM1638_SetGPIO_OUT(uint64_t GPIO_Pad)
{
  gpio_config_t TM1638_GPIO_CONF;
  TM1638_GPIO_CONF.pin_bit_mask = GPIO_Pad;
  TM1638_GPIO_CONF.mode = GPIO_MODE_OUTPUT;
  TM1638_GPIO_CONF.pull_up_en = GPIO_PULLUP_DISABLE;
  TM1638_GPIO_CONF.pull_down_en = GPIO_PULLDOWN_DISABLE;
  TM1638_GPIO_CONF.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&TM1638_GPIO_CONF);
}

void TM1638_SetGPIO_IN_PU(uint64_t GPIO_Pad)
{
  gpio_config_t TM1638_GPIO_CONF;
  TM1638_GPIO_CONF.pin_bit_mask = GPIO_Pad;
  TM1638_GPIO_CONF.mode = GPIO_MODE_DEF_INPUT;
  TM1638_GPIO_CONF.pull_up_en = GPIO_PULLUP_ENABLE;
  TM1638_GPIO_CONF.pull_down_en = GPIO_PULLDOWN_DISABLE;
  TM1638_GPIO_CONF.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&TM1638_GPIO_CONF);
}
#endif

static void
TM1638_PlatformInit(void)
{
#if defined(TM1638_PLATFORM_AVR)
  TM1638_CLK_DDR |= (1<<TM1638_CLK_NUM);
  TM1638_DIO_DDR |= (1<<TM1638_DIO_NUM);
  TM1638_STB_DDR |= (1<<TM1638_STB_NUM);
#elif defined(TM1638_PLATFORM_STM32)
  TM1638_SetGPIO_OUT(TM1638_CLK_GPIO, TM1638_CLK_PIN);
  TM1638_SetGPIO_OUT(TM1638_STB_GPIO, TM1638_STB_PIN);
  TM1638_SetGPIO_OUT(TM1638_DIO_GPIO, TM1638_DIO_PIN);
#elif defined(TM1638_PLATFORM_ESP32_IDF)
  uint64_t GPIO_Pad = 0;
  GPIO_Pad = (1 << TM1638_CLK_GPIO) |
             (1 << TM1638_STB_GPIO);
  TM1638_SetGPIO_OUT(GPIO_Pad);
#endif
}

static void
TM1638_PlatformDeInit(void)
{
#if defined(TM1638_PLATFORM_AVR)
  TM1638_CLK_DDR &= ~(1<<TM1638_CLK_NUM);
  TM1638_CLK_PORT &= ~(1<<TM1638_CLK_NUM);
  TM1638_DIO_DDR &= ~(1<<TM1638_DIO_NUM);
  TM1638_DIO_PORT &= ~(1<<TM1638_DIO_NUM);
  TM1638_STB_DDR &= ~(1<<TM1638_STB_NUM);
  TM1638_STB_PORT &= ~(1<<TM1638_STB_NUM);
#elif defined(TM1638_PLATFORM_STM32)

#elif defined(TM1638_PLATFORM_ESP32_IDF)
  gpio_reset_pin(TM1638_CLK_GPIO);
  gpio_reset_pin(TM1638_STB_GPIO);
  gpio_reset_pin(TM1638_DIO_GPIO);
#endif
}

static void
TM1638_DioConfigOut(void)
{
#if defined(TM1638_PLATFORM_AVR)
  TM1638_DIO_DDR |= (1<<TM1638_DIO_NUM);
#elif defined(TM1638_PLATFORM_STM32)
  TM1638_SetGPIO_OUT(TM1638_DIO_GPIO, TM1638_DIO_PIN);
#elif defined(TM1638_PLATFORM_ESP32_IDF)
  uint64_t GPIO_Pad = 0;
  GPIO_Pad = (1 << TM1638_DIO_GPIO);
  TM1638_SetGPIO_OUT(GPIO_Pad);
#endif
}

static void
TM1638_DioConfigIn(void)
{
#if defined(TM1638_PLATFORM_AVR)
  TM1638_DIO_DDR &= ~(1<<TM1638_DIO_NUM);
#elif defined(TM1638_PLATFORM_STM32)
  TM1638_SetGPIO_IN_PU(TM1638_DIO_GPIO, TM1638_DIO_PIN);
#elif defined(TM1638_PLATFORM_ESP32_IDF)
  uint64_t GPIO_Pad = 0;
  GPIO_Pad = (1 << TM1638_DIO_GPIO);
  TM1638_SetGPIO_IN_PU(GPIO_Pad);
#endif
}

static void
TM1638_DioWrite(uint8_t Level)
{
#if defined(TM1638_PLATFORM_AVR)
  if (Level)
    TM1638_DIO_PORT |= (1<<TM1638_DIO_NUM);
  else
    TM1638_DIO_PORT &= ~(1<<TM1638_DIO_NUM);
#elif defined(TM1638_PLATFORM_STM32)
  HAL_GPIO_WritePin(TM1638_DIO_GPIO, TM1638_DIO_PIN, Level);
#elif defined(TM1638_PLATFORM_ESP32_IDF)
  gpio_set_level(TM1638_DIO_GPIO, Level);
#endif
}

static uint8_t
TM1638_DioRead(void)
{
  uint8_t Result = 1;
#if defined(TM1638_PLATFORM_AVR)
  Result = (TM1638_DIO_PIN & (1 << TM1638_DIO_NUM)) ? 1 : 0;
#elif defined(TM1638_PLATFORM_STM32)
  Result = HAL_GPIO_ReadPin(TM1638_DIO_GPIO, TM1638_DIO_PIN);
#elif defined(TM1638_PLATFORM_ESP32_IDF)
  Result = gpio_get_level(TM1638_DIO_GPIO);
#endif
  return Result;
}

static void
TM1638_ClkWrite(uint8_t Level)
{
#if defined(TM1638_PLATFORM_AVR)
  if (Level)
    TM1638_CLK_PORT |= (1<<TM1638_CLK_NUM);
  else
    TM1638_CLK_PORT &= ~(1<<TM1638_CLK_NUM);
#elif defined(TM1638_PLATFORM_STM32)
  HAL_GPIO_WritePin(TM1638_CLK_GPIO, TM1638_CLK_PIN, Level);
#elif defined(TM1638_PLATFORM_ESP32_IDF)
  gpio_set_level(TM1638_CLK_GPIO, Level);
#endif
}

static void
TM1638_StbWrite(uint8_t Level)
{
#if defined(TM1638_PLATFORM_AVR)
  if (Level)
    TM1638_STB_PORT |= (1<<TM1638_STB_NUM);
  else
    TM1638_STB_PORT &= ~(1<<TM1638_STB_NUM);
#elif defined(TM1638_PLATFORM_STM32)
  HAL_GPIO_WritePin(TM1638_STB_GPIO, TM1638_STB_PIN, Level);
#elif defined(TM1638_PLATFORM_ESP32_IDF)
  gpio_set_level(TM1638_STB_GPIO, Level);
#endif
}

static void
TM1638_DelayUs(uint8_t Delay)
{
#if defined(TM1638_PLATFORM_AVR)
  for (; Delay; --Delay)
    _delay_us(1);
#elif defined(TM1638_PLATFORM_STM32)
  for (uint32_t DelayCounter = 0; DelayCounter < 100 * Delay; DelayCounter++)
    DelayCounter = DelayCounter;
#elif defined(TM1638_PLATFORM_ESP32_IDF)
  ets_delay_us(Delay);
#endif
}

/**
 ==================================================================================
                            ##### Public Functions #####                           
 ==================================================================================
 */
 
/**
 * @brief  Initialize platfor device to communicate TM1638.
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
