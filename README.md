# TM1638 Library
Library for handling TM163 LED display driver.

## Library Features
-   Support for both Common Anode and Common Cathode Seven-segment displays
-   Support for dimming display
-   Support for scan Keypad

## Hardware Support
-   STM32 (HAL)
-   ESP32 (esp-idf)

## How To Use
 1. Add library files to your project
 2. Config `TM1638_platform.h`
 3. Specify the display type in `TM1638.h`
 4. Call `TM1638_Init()` to initialize library
 5. Call `TM1638_ConfigDisplay` to config display
 6. Call other functions and enjoy

## Example
```C
#include "TM1638.h"

int main(void)
{
  // Put some code here
  
  TM1638_Init();
  TM1638_ConfigDisplay(7, TM1638DisplayStateON);
  
  while(1)
  {
    // Display the number 8 and Decimal Point in the SEG1 
    TM1638_SetSingleDigit_HEX(8 | TM1638DecimalPoint, 0);
  }
}
```