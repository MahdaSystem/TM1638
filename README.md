# TM1638 Library
Library for handling TM163 LED display driver.

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
5. Call `TM1638_ConfigDisplay()` to config display
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
  TM1638_Init(&Handler, 0);
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

#define DIO_DDR   DDRA
#define DIO_PORT  PORTA
#define DIO_PIN   PINA
#define DIO_NUM   0

#define CLK_DDR   DDRA
#define CLK_PORT  PORTA
#define CLK_NUM   1

#define STB_DDR   DDRA
#define STB_PORT  PORTA
#define STB_NUM   2


void TM1638_Platform_DioDeInit(void)
{
  DIO_PORT &= ~(1<<DIO_NUM);
  DIO_DDR &= ~(1<<DIO_NUM);
}
void TM1638_Platform_DioConfigOut(void)
{
  DIO_DDR |= (1<<DIO_NUM);
}
void TM1638_Platform_DioConfigIn(void)
{
  DIO_DDR &= ~(1<<DIO_NUM);
}
void TM1638_Platform_DioWrite(uint8_t Level)
{
  if (Level)
    DIO_PORT |= (1<<DIO_NUM);
  else
    DIO_PORT &= ~(1<<DIO_NUM);
}
uint8_t TM1638_Platform_DioRead(void)
{
  return (DIO_PIN >> DIO_NUM) & 0x01;
}


void TM1638_Platform_ClkDeInit(void)
{
  CLK_PORT &= ~(1<<CLK_NUM);
  CLK_DDR &= ~(1<<CLK_NUM);
}
void TM1638_Platform_ClkConfigOut(void)
{
  CLK_DDR |= (1<<CLK_NUM);
}
void TM1638_Platform_ClkWrite(uint8_t Level)
{
  if (Level)
    CLK_PORT |= (1<<CLK_NUM);
  else
    CLK_PORT &= ~(1<<CLK_NUM);
}

void TM1638_Platform_StbDeInit(void)
{
  STB_PORT &= ~(1<<STB_NUM);
  STB_DDR &= ~(1<<STB_NUM);
}
void TM1638_Platform_StbConfigOut(void)
{
  STB_DDR |= (1<<STB_NUM);
}
void TM1638_Platform_StbWrite(uint8_t Level)
{
  if (Level)
    STB_PORT |= (1<<STB_NUM);
  else
    STB_PORT &= ~(1<<STB_NUM);
}


void TM1638_Platform_DelayUs(uint8_t Delay)
{
  for (; Delay; --Delay)
    _delay_us(1);
}


int main(void)
{
  TM1638_Handler_t Handler;

  Handler.DioDeInit     = TM1638_Platform_DioDeInit;
  Handler.DioConfigOut  = TM1638_Platform_DioConfigOut;
  Handler.DioConfigIn   = TM1638_Platform_DioConfigIn;
  Handler.DioWrite      = TM1638_Platform_DioWrite;
  Handler.DioRead       = TM1638_Platform_DioRead;
  Handler.ClkDeInit     = TM1638_Platform_ClkDeInit;
  Handler.ClkConfigOut  = TM1638_Platform_ClkConfigOut;
  Handler.ClkWrite      = TM1638_Platform_ClkWrite;
  Handler.StbDeInit     = TM1638_Platform_StbDeInit;
  Handler.StbConfigOut  = TM1638_Platform_StbConfigOut;
  Handler.StbWrite      = TM1638_Platform_StbWrite;
  Handler.DelayUs       = TM1638_Platform_DelayUs;

  TM1638_Init(&Handler, 0);
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