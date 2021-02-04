/**
 **********************************************************************************
 * @file   TM1638_platform.c
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
  
/* Includes ---------------------------------------------------------------------*/
#include "TM1638_platform.h"


/* Private Constants ------------------------------------------------------------*/
#define TM1638TM1638_DelayUs  1


/* Private Macro ----------------------------------------------------------------*/
#if defined(TM1638_PLATFORM_STM32)
#define TM1638_SetGPIO_OUT(GPIO, PIN) GPIO_InitStruct.Pin = PIN;\
                                      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;\
                                      GPIO_InitStruct.Pull = GPIO_NOPULL;\
                                      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;\
                                      HAL_GPIO_Init(GPIO, &GPIO_InitStruct);

#define TM1638_SetGPIO_IN_PU(GPIO, PIN) GPIO_InitStruct.Pin = PIN;\
                                        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;\
                                        GPIO_InitStruct.Pull = GPIO_PULLUP;\
                                        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;\
                                        HAL_GPIO_Init(GPIO, &GPIO_InitStruct);

#define TM1638_DIO_OUTPUT()     TM1638_SetGPIO_OUT(TM1638_DIO_GPIO, TM1638_DIO_PIN)
#define TM1638_DIO_INPUT()      TM1638_SetGPIO_IN_PU(TM1638_DIO_GPIO, TM1638_DIO_PIN)
#define TM1638_DIO_HIGH()       HAL_GPIO_WritePin(TM1638_DIO_GPIO, TM1638_DIO_PIN, GPIO_PIN_SET)
#define TM1638_DIO_LOW()        HAL_GPIO_WritePin(TM1638_DIO_GPIO, TM1638_DIO_PIN, GPIO_PIN_RESET)
#define TM1638_DIO_READ()       HAL_GPIO_ReadPin(TM1638_DIO_GPIO, TM1638_DIO_PIN)

#define TM1638_CLK_OUTPUT()     TM1638_SetGPIO_OUT(TM1638_CLK_GPIO, TM1638_CLK_PIN)
#define TM1638_CLK_HIGH()       HAL_GPIO_WritePin(TM1638_CLK_GPIO, TM1638_CLK_PIN, GPIO_PIN_SET)
#define TM1638_CLK_LOW()        HAL_GPIO_WritePin(TM1638_CLK_GPIO, TM1638_CLK_PIN, GPIO_PIN_RESET) 

#define TM1638_STB_OUTPUT()     TM1638_SetGPIO_OUT(TM1638_STB_GPIO, TM1638_STB_PIN)
#define TM1638_STB_HIGH()       HAL_GPIO_WritePin(TM1638_STB_GPIO, TM1638_STB_PIN, GPIO_PIN_SET)
#define TM1638_STB_LOW()        HAL_GPIO_WritePin(TM1638_STB_GPIO, TM1638_STB_PIN, GPIO_PIN_RESET)

#define TM1638_DelayUs(Delay)   for(uint32_t DelayCounter = 0; DelayCounter<1000*Delay; DelayCounter++) DelayCounter = DelayCounter
#endif

#if defined(TM1638_PLATFORM_ESP32_IDF)
#define TM1638_SetGPIO_OUT(GPIO_Pad)  TM1638_GPIO_CONF.pin_bit_mask	= GPIO_Pad;\
                                      TM1638_GPIO_CONF.mode         = GPIO_MODE_OUTPUT;\
                                      TM1638_GPIO_CONF.pull_up_en   = GPIO_PULLUP_DISABLE;\
                                      TM1638_GPIO_CONF.pull_down_en = GPIO_PULLDOWN_DISABLE;\
                                      TM1638_GPIO_CONF.intr_type    = GPIO_INTR_DISABLE;\
                                      gpio_config(&TM1638_GPIO_CONF);
              

#define TM1638_SetGPIO_IN_PU(GPIO_Pad)  TM1638_GPIO_CONF.pin_bit_mask	= GPIO_Pad;\
                                        TM1638_GPIO_CONF.mode         = GPIO_MODE_INPUT;\
                                        TM1638_GPIO_CONF.pull_up_en   = GPIO_PULLUP_ENABLE;\
                                        TM1638_GPIO_CONF.pull_down_en = GPIO_PULLDOWN_DISABLE;\
                                        TM1638_GPIO_CONF.intr_type    = GPIO_INTR_DISABLE;\
                                        gpio_config(&TM1638_GPIO_CONF);

#define TM1638_DIO_OUTPUT()   TM1638_SetGPIO_OUT(1<<TM1638_DIO_GPIO)
#define TM1638_DIO_INPUT()    TM1638_SetGPIO_IN_PU(1<<TM1638_DIO_GPIO)
#define TM1638_DIO_HIGH()     gpio_set_level(TM1638_DIO_GPIO, 1)
#define TM1638_DIO_LOW()      gpio_set_level(TM1638_DIO_GPIO, 0)
#define TM1638_DIO_READ()     gpio_get_level(TM1638_DIO_GPIO)

#define TM1638_CLK_OUTPUT()   TM1638_SetGPIO_OUT(1<<TM1638_CLK_GPIO)
#define TM1638_CLK_HIGH()     gpio_set_level(TM1638_CLK_GPIO, 1)
#define TM1638_CLK_LOW()      gpio_set_level(TM1638_CLK_GPIO, 0)

#define TM1638_STB_OUTPUT()   TM1638_SetGPIO_OUT(1<<TM1638_STB_GPIO)
#define TM1638_STB_HIGH()     gpio_set_level(TM1638_STB_GPIO, 1)
#define TM1638_STB_LOW()      gpio_set_level(TM1638_STB_GPIO, 0)

#define TM1638_DelayUs(Delay) 
#endif

/* Private variables ------------------------------------------------------------*/
#if defined(TM1638_PLATFORM_STM32)
GPIO_InitTypeDef GPIO_InitStruct = {0};
#endif

#if defined(TM1638_PLATFORM_ESP32_IDF)
gpio_config_t TM1638_GPIO_CONF;
#endif



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */
 
/**
 * @brief  Initialize platfor device to communicate TM1638.
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 *         - TM1638_FAIL: Operation failed.
 */
TM1638_Result_t
TM1638_Platform_Init(void)
{
  TM1638_CLK_OUTPUT();
  TM1638_STB_OUTPUT();
  TM1638_DIO_OUTPUT();

  TM1638_CLK_HIGH();
  TM1638_STB_HIGH();
  TM1638_DIO_HIGH();

  return TM1638_OK;
}

 
/**
 * @brief  Send STOP signal to TM1638
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 *         - TM1638_FAIL: Operation failed.
 */
TM1638_Result_t
TM1638_Platform_Stop(void)
{
  TM1638_STB_HIGH();

  // TM1638_DelayUs(TM1638TM1638_DelayUs);

  return TM1638_OK;
}


/**
 * @brief  Send START signal to TM1638 
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 *         - TM1638_FAIL: Operation failed.
 */
TM1638_Result_t
TM1638_Platform_Start(void)
{
  TM1638_STB_LOW();

  return TM1638_OK;
}


/**
 * @brief  Send data bytes
 * @param  Data: Pointer to send data
 * @param  NumOfBytes: Number of bytes to send
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 *         - TM1638_FAIL: Operation failed.
 */
TM1638_Result_t
TM1638_Platform_WriteBytes(const uint8_t *Data, uint8_t NumOfBytes)
{
  uint8_t i, j, Buff;

  TM1638_DIO_OUTPUT();

  for (j = 0; j < NumOfBytes; j++)
  {
    for (i = 0, Buff = Data[j]; i < 8; ++i, Buff >>= 1)
    {
      TM1638_CLK_LOW();
      TM1638_DelayUs(TM1638TM1638_DelayUs);

      if (Buff & 0x01)
        TM1638_DIO_HIGH();
      else
        TM1638_DIO_LOW();

      TM1638_CLK_HIGH();
      TM1638_DelayUs(TM1638TM1638_DelayUs);
    }
  }

  return TM1638_OK;
}


/**
 * @brief  Receive data bytes 
 * @param  Data: Pointer to save data
 * @param  NumOfBytes: Number of bytes to read
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 *         - TM1638_FAIL: Operation failed.
 */
TM1638_Result_t
TM1638_Platform_ReadBytes(uint8_t *Data, uint8_t NumOfBytes)
{
  uint8_t i, j, Buff;

  TM1638_DIO_INPUT();

  TM1638_DelayUs(5);

  for (j = 0; j < NumOfBytes; j++)
  {
    for (i = 0, Buff = 0; i < 8; i++)
    {
      TM1638_CLK_LOW();
      TM1638_DelayUs(TM1638TM1638_DelayUs);
      TM1638_CLK_HIGH();

      Buff |= (TM1638_DIO_READ() << i);

      TM1638_DelayUs(TM1638TM1638_DelayUs);
    }

    Data[j] = Buff;
    TM1638_DelayUs(2 * TM1638TM1638_DelayUs);
  }

  return TM1638_OK;
}
