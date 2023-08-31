#include "TM1638.h"
#include "TM1638_platform.h"

void app_main(void)
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
}