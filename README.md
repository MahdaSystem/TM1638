# TM1638 Library
Library for handling TM1638 LED display driver.

## Library Features
-   Support for both Common Anode and Common Cathode Seven-segment displays
-   Support for dimming display
-   Support for scan Keypad

## Hardware Support
It is easy to port this library to any platform. But now it is ready for use in:
- AVR (ATmega32)
- STM32 (HAL)
- ESP32 (esp-idf)

## How To Use
1. Add `TM1638.h` and `TM1638.c` files to your project.  It is optional to use `TM1638_platform.h` and `TM1638_platform.c` files (open and config `TM1638_platform.h` file).
2. Initialize platform-dependent part of handler.
4. Call `TM1638_Init()`.
5. Call `TM1638_ConfigDisplay()` to config display.
6. Call other functions and enjoy.

## Example
<details>
<summary>Using TM1638_platform files</summary>

```c
#include <stdio.h>
#include "TM1638.h"
#include "TM1638_platform.h"

int main(void)
{
  TM1638_Handler_t Handler;

  TM1638_Platform_Init(&Handler);
  TM1638_Init(&Handler, TM1638DisplayTypeComCathode);
  TM1638_ConfigDisplay(&Handler, 7, TM1638DisplayStateON);

  while (1)
  {
    // Display the number 8 and Decimal Point in the SEG1 
    TM1638_SetSingleDigit_HEX(&Handler, 8 | TM1638DecimalPoint, 0);
  }

  TM1638_DeInit(&Handler);
  return 0;
}
```
</details>


<details>
<summary>Without using TM1638_platform files (AVR)</summary>

```c
#include <stdio.h>
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include "TM1638.h"

#define TM1638_DIO_DDR   DDRA
#define TM1638_DIO_PORT  PORTA
#define TM1638_DIO_PIN   PINA
#define TM1638_DIO_NUM   0

#define TM1638_CLK_DDR   DDRA
#define TM1638_CLK_PORT  PORTA
#define TM1638_CLK_NUM   1

#define TM1638_STB_DDR   DDRA
#define TM1638_STB_PORT  PORTA
#define TM1638_STB_NUM   2


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
  uint8_t Result = 1;
  Result = (TM1638_DIO_PIN & (1 << TM1638_DIO_NUM)) ? 1 : 0;
  return Result;
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


int main(void)
{
  TM1638_Handler_t Handler;

  Handler.PlatformInit = TM1638_PlatformInit;
  Handler.PlatformDeInit = TM1638_PlatformDeInit;
  Handler.DioConfigOut = TM1638_DioConfigOut;
  Handler.DioConfigIn = TM1638_DioConfigIn;
  Handler.DioWrite = TM1638_DioWrite;
  Handler.DioRead = TM1638_DioRead;
  Handler.ClkWrite = TM1638_ClkWrite;
  Handler.StbWrite = TM1638_StbWrite;
  Handler.DelayUs = TM1638_DelayUs;

  TM1638_Init(&Handler, TM1638DisplayTypeComCathode);
  TM1638_ConfigDisplay(&Handler, 7, TM1638DisplayStateON);

  while (1)
  {
    // Display the number 8 and Decimal Point in the SEG1 
    TM1638_SetSingleDigit_HEX(&Handler, 8 | TM1638DecimalPoint, 0);
  }

  TM1638_DeInit(&Handler);
  return 0;
}
```
</details>
