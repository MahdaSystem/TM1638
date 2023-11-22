/**
 **********************************************************************************
 * @file   main.c
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  example code for TM1638 Driver (for ESP32-IDF)
 **********************************************************************************
 *
 * Copyright (c) 2023 Mahda Embedded System (MIT License)
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

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "TM1638.h"
#include "TM1638_platform.h"

static const char *TAG = "example";

void app_main(void)
{
  TM1638_Handler_t Handler = {0};

  ESP_LOGI(TAG, "example code for TM1638 Driver (for ESP-IDF)");

  TM1638_Platform_Init(&Handler);
  TM1638_Platform_Init(&Handler);
  TM1638_Init(&Handler, TM1638DisplayTypeComCathode);
  TM1638_ConfigDisplay(&Handler, 7, TM1638DisplayStateON);
  
  while (1)
  {
    for (uint16_t i = 0; i < 10000; i++)
    {
      uint8_t Buffer[4] = {0};
      Buffer[0] = i % 10;
      Buffer[1] = (i / 10) % 10;
      Buffer[2] = (i / 100) % 10;
      Buffer[3] = (i / 1000) % 10;
      
      Buffer[1] |= TM1638DecimalPoint;

      TM1638_SetMultipleDigit_HEX(&Handler, Buffer, 0, 4);
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }

  TM1638_DeInit(&Handler);
}
