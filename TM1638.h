/**
 **********************************************************************************
 * @file   TM1638.h
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  TM1638 chip driver
 *         Functionalities of the this file:
 *          + Display config and control functions
 *          + Keypad scan functions
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
#ifndef _TM1638_H_
#define _TM1638_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ---------------------------------------------------------------------*/
#include <stdint.h>


/* Functionality Options --------------------------------------------------------*/
/**
 * @brief  Enable support for Common Anode displays
 */   
#define TM1638_SUPPORT_COM_ANODE  1


/* Exported Constants -----------------------------------------------------------*/
#define TM1638DisplayTypeComCathode 0
#define TM1638DisplayTypeComAnode   1

#define TM1638DisplayStateOFF 0
#define TM1638DisplayStateON  1

#define TM1638DecimalPoint    0x80

  
/* Exported Data Types ----------------------------------------------------------*/
/**
 * @brief  Handler data type
 * @note   User must initialize this this functions before using library:
 *         - PlatformInit
 *         - PlatformDeInit
 *         - DioConfigOut
 *         - DioConfigIn
 *         - DioWrite
 *         - DioRead
 *         - ClkWrite
 *         - StbWrite
 *         - DelayUs
 */
typedef struct TM1638_Handler_s
{
  // Initialize the platform-dependent layer
  void (*PlatformInit)(void);
  // Uninitialize the platform-dependent layer
  void (*PlatformDeInit)(void);

  // Config the GPIO that connected to DIO PIN of SHT1x as output
  void (*DioConfigOut)(void);
  // Config the GPIO that connected to DIO PIN of SHT1x as input
  void (*DioConfigIn)(void);
  // Set level of the GPIO that connected to DIO PIN of SHT1x
  void (*DioWrite)(uint8_t);
  // Read the GPIO that connected to DIO PIN of SHT1x
  uint8_t (*DioRead)(void);

  // Set level of the GPIO that connected to CLK PIN of SHT1x
  void (*ClkWrite)(uint8_t);

  // Set level of the GPIO that connected to STB PIN of SHT1x
  void (*StbWrite)(uint8_t);

  // Delay (us)
  void (*DelayUs)(uint8_t);

  uint8_t DisplayType;

#if (TM1638_SUPPORT_COM_ANODE)
  uint8_t DisplayRegister[16];
#endif
} TM1638_Handler_t;


/**
 * @brief  Data type of library functions result
 */
typedef enum TM1638_Result_e
{
  TM1638_OK      = 0,
  TM1638_FAIL    = -1,
} TM1638_Result_t;



/**
 ==================================================================================
                           ##### Common Functions #####                           
 ==================================================================================
 */

/**
 * @brief  Initialize TM1638.
 * @param  Handler: Pointer to handler
 * @param  Type: Determine the type of display
 *         - TM1638DisplayTypeComCathode: Common-Cathode
 *         - TM1638DisplayTypeComAnode:   Common-Anode
 * @note   If 'TM1638_SUPPORT_COM_ANODE' switch is set to 0, the 'Type' argument
 *         will be ignored 
 *         
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 */
TM1638_Result_t
TM1638_Init(TM1638_Handler_t *Handler, uint8_t Type);


/**
 * @brief  De-Initialize TM1638.
 * @param  Handler: Pointer to handler
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 */
TM1638_Result_t
TM1638_DeInit(TM1638_Handler_t *Handler);
 


/**
 ==================================================================================
                           ##### Display Functions #####                           
 ==================================================================================
 */

/**
 * @brief  Config display parameters
 * @param  Handler: Pointer to handler
 * @param  Brightness: Set brightness level
 *         - 0: Display pulse width is set as 1/16
 *         - 1: Display pulse width is set as 2/16
 *         - 2: Display pulse width is set as 4/16
 *         - 3: Display pulse width is set as 10/16
 *         - 4: Display pulse width is set as 11/16
 *         - 5: Display pulse width is set as 12/16
 *         - 6: Display pulse width is set as 13/16
 *         - 7: Display pulse width is set as 14/16
 * 
 * @param  DisplayState: Set display ON or OFF
 *         - TM1638DisplayStateOFF: Set display state OFF
 *         - TM1638DisplayStateON: Set display state ON
 * 
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful
 */
TM1638_Result_t
TM1638_ConfigDisplay(TM1638_Handler_t *Handler,
                     uint8_t Brightness, uint8_t DisplayState);


/**
 * @brief  Set data to single digit in 7-segment format
 * @param  Handler: Pointer to handler
 * @param  DigitData: Digit data
 * @param  DigitPos: Digit position
 *         - 0: Seg1
 *         - 1: Seg2
 *         - .
 *         - .
 *         - .
 * 
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful
 */
TM1638_Result_t
TM1638_SetSingleDigit(TM1638_Handler_t *Handler,
                      uint8_t DigitData, uint8_t DigitPos);


/**
 * @brief  Set data to multiple digits in 7-segment format
 * @param  Handler: Pointer to handler
 * @param  DigitData: Array to Digits data
 * @param  StartAddr: First digit position
 *         - 0: Seg1
 *         - 1: Seg2
 *         - .
 *         - .
 *         - .
 * 
 * @param  Count: Number of segments to write data
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful
 */
TM1638_Result_t
TM1638_SetMultipleDigit(TM1638_Handler_t *Handler, const uint8_t *DigitData,
                        uint8_t StartAddr, uint8_t Count);


/**
 * @brief  Set data to multiple digits in 7-segment format
 * @param  Handler: Pointer to handler
 * @param  DigitData: Digit data (0, 1, ... , 15, a, A, b, B, ... , f, F) 
 * @param  DigitPos: Digit position
 *         - 0: Seg1
 *         - 1: Seg2
 *         - .
 *         - .
 *         - .
 * 
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful
 */
TM1638_Result_t
TM1638_SetSingleDigit_HEX(TM1638_Handler_t *Handler,
                          uint8_t DigitData, uint8_t DigitPos);


/**
 * @brief  Set data to multiple digits in hexadecimal format
 * @param  Handler: Pointer to handler
 * @param  DigitData: Array to Digits data. 
 *                    (0, 1, ... , 15, a, A, b, B, ... , f, F)
 * 
 * @param  StartAddr: First digit position
 *         - 0: Seg1
 *         - 1: Seg2
 *         - .
 *         - .
 *         - .
 * 
 * @param  Count: Number of segments to write data
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful
 */
TM1638_Result_t
TM1638_SetMultipleDigit_HEX(TM1638_Handler_t *Handler, const uint8_t *DigitData,
                            uint8_t StartAddr, uint8_t Count);



/** 
 ==================================================================================
                           ##### Keypad Functions #####                            
 ==================================================================================
 */

/**
 * @brief  Scan all 24 keys connected to TM1638
 * @note   
 *                   SEG1         SEG2         SEG3       ......      SEG8
 *                     |            |            |                      |
 *         K1  --  |K1_SEG1|    |K1_SEG2|    |K1_SEG3|    ......    |K1_SEG8|
 *         K2  --  |K2_SEG1|    |K2_SEG2|    |K2_SEG3|    ......    |K2_SEG8|
 *         K3  --  |K3_SEG1|    |K3_SEG2|    |K3_SEG3|    ......    |K3_SEG8|
 * 
 * @param  Handler: Pointer to handler
 * @param  Keys: pointer to save key scan result
 *         - bit0=>K1_SEG1, bit1=>K1_SEG2, ..., bit7=>K1_SEG8,
 *         - bit8=>K2_SEG1, bit9=>K2_SEG2, ..., bit15=>K2_SEG8,
 *         - bit16=>K3_SEG1, bit17=>K3_SEG2, ..., bit23=>K3_SEG8,
 * 
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful
 */
TM1638_Result_t
TM1638_ScanKeys(TM1638_Handler_t *Handler, uint32_t *Keys);



#ifdef __cplusplus
}
#endif

#endif //! _TM1638_H_
