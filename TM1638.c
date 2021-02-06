/**
 **********************************************************************************
 * @file   TM1638.c
 * @author Hossein.M (https://github.com/Hossein-M98)
 * @brief  TM1638 chip driver
 *         Functionalities of the this file:
 *          + Display config and control functions
 *          + Keypad scan functions
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
#include "TM1638.h"
#include "TM1638_platform.h"


/* Private Constants ------------------------------------------------------------*/
/**
 * @brief  Instruction description
 */
#define DataInstructionSet            0x40  // 0b01000000
#define DisplayControlInstructionSet  0x80  // 0b10000000
#define AddressInstructionSet         0xC0  // 0b11000000

/**
 * @brief  Data instruction set
 */
#define WriteDataToRegister   0x00  // 0b00000000
#define ReadKeyScanData       0x02  // 0b00000010
#define AutoAddressAdd        0x00  // 0b00000000
#define FixedAddress          0x04  // 0b00000100
#define NormalMode            0x00  // 0b00000000
#define TestMode              0x08  // 0b00001000

/**
 * @brief  Display ControlInstruction Set
 */
#define ShowTurnOff   0x00  // 0b00000000
#define ShowTurnOn    0x08  // 0b00001000


/* Private variables ------------------------------------------------------------*/
/**
 * @brief  Convert HEX number to Seven-Segment code
 */
const uint8_t HexTo7Seg[16] =
{
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F, // 9
  0x77, // A
  0x7c, // b
  0x39, // C
  0x5E, // d
  0x79, // E
  0x71  // F
};

#if (TM1638SegType)
uint8_t Tm1638DisplayRegister[16] = { 0 };
#endif // (TM1638SegType)



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

static void
TM1638_SetMultipleDisplayRegister(const uint8_t *DigitData, uint8_t StartAddr, uint8_t Count)
{
  uint8_t Data = DataInstructionSet | WriteDataToRegister |
                 AutoAddressAdd | NormalMode;

  TM1638_Platform_Start();
  TM1638_Platform_WriteBytes(&Data, 1);
  TM1638_Platform_Stop();

  Data = AddressInstructionSet | StartAddr;

  TM1638_Platform_Start();
  TM1638_Platform_WriteBytes(&Data, 1);
  TM1638_Platform_WriteBytes(DigitData, Count);
  TM1638_Platform_Stop();
}

static TM1638_Result_t
TM1638_ScanKeyRegs(uint8_t *KeyRegs)
{
  uint8_t Data = DataInstructionSet | ReadKeyScanData |
                 AutoAddressAdd | NormalMode;

  TM1638_Platform_Start();
  TM1638_Platform_WriteBytes(&Data, 1);
  TM1638_Platform_ReadBytes(KeyRegs, 4);
  TM1638_Platform_Stop();

  return TM1638_OK;
}



/**
 ==================================================================================
                           ##### Common Functions #####                            
 ==================================================================================
 */

/**
 * @brief  Initialize TM1638.  
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 *         - TM1638_FAIL: Operation failed.
 */
TM1638_Result_t
TM1638_Init(void)
{
  return TM1638_Platform_Init();
}



/**
 ==================================================================================
                        ##### Public Display Functions #####                       
 ==================================================================================
 */

/**
 * @brief  Config display parameters
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
 *         - TM1638_FAIL: Operation failed
 */
TM1638_Result_t
TM1638_ConfigDisplay(uint8_t Brightness, uint8_t DisplayState)
{
  uint8_t Data = DisplayControlInstructionSet;
  Data |= Brightness & 0x07;
  Data |= (DisplayState) ? (ShowTurnOn) : (ShowTurnOff);

  TM1638_Platform_Start();
  TM1638_Platform_WriteBytes(&Data, 1);
  TM1638_Platform_Stop();

  return TM1638_OK;
}


/**
 * @brief  Set data to single digit in 7-segment format
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
 *         - TM1638_FAIL: Operation failed
 */
TM1638_Result_t
TM1638_SetSingleDigit(uint8_t DigitData, uint8_t DigitPos)
{
#if (TM1638SegType)
  TM1638_SetMultipleDigit(&DigitData, DigitPos, 1);
#else
  TM1638_SetMultipleDisplayRegister(DigitPos, 1, &DigitData);
#endif

  return TM1638_OK;
}


/**
 * @brief  Set data to multiple digits in 7-segment format
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
 *         - TM1638_FAIL: Operation failed
 */
TM1638_Result_t
TM1638_SetMultipleDigit(const uint8_t *DigitData, uint8_t StartAddr, uint8_t Count)
{
  uint8_t Shift = 0;
  uint8_t DigitDataBuff = 0;
  uint8_t i = 0, j = 0;

#if (TM1638SegType)
  for (j = 0; j < Count; j++)
  {
    DigitDataBuff = DigitData[j];

    if ((j + StartAddr) >= 0 && (j + StartAddr) <= 7)
    {
      Shift = j + StartAddr;
      i = 0;
    }
    else if ((j + StartAddr) == 8 || (j + StartAddr) == 9)
    {
      Shift = (j + StartAddr) - 8;
      i = 1;
    }
    else
    {
      i = 16;
    }

    for (; i < 16; i += 2, DigitDataBuff >>= 1)
    {
      if (DigitDataBuff & 0x01)
        Tm1638DisplayRegister[i] |= (1 << Shift);
      else
        Tm1638DisplayRegister[i] &= ~(1 << Shift);
    }
  }
  TM1638_SetMultipleDisplayRegister(Tm1638DisplayRegister, 0, 16);

#else
  TM1638_SetMultipleDisplayRegister(StartAddr, Count, DigitData);
#endif

  return TM1638_OK;
}


/**
 * @brief  Set data to multiple digits in 7-segment format
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
 *         - TM1638_FAIL: Operation failed
 */
TM1638_Result_t
TM1638_SetSingleDigit_HEX(uint8_t DigitData, uint8_t DigitPos)
{
  uint8_t DigitDataHEX = 0;
  uint8_t DecimalPoint = DigitData & 0x80;

  DigitData &= 0x7F;

  if (DigitData >= 0 && DigitData <= 15)
  {
    DigitDataHEX = HexTo7Seg[DigitData] | DecimalPoint;
  }
  else
  {
    switch (DigitData)
    {
    case 'A':
    case 'a':
      DigitDataHEX = HexTo7Seg[0x0A] | DecimalPoint;
      break;

    case 'B':
    case 'b':
      DigitDataHEX = HexTo7Seg[0x0B] | DecimalPoint;
      break;

    case 'C':
    case 'c':
      DigitDataHEX = HexTo7Seg[0x0C] | DecimalPoint;
      break;

    case 'D':
    case 'd':
      DigitDataHEX = HexTo7Seg[0x0D] | DecimalPoint;
      break;

    case 'E':
    case 'e':
      DigitDataHEX = HexTo7Seg[0x0E] | DecimalPoint;
      break;

    case 'F':
    case 'f':
      DigitDataHEX = HexTo7Seg[0x0F] | DecimalPoint;
      break;

    default:
      DigitDataHEX = 0;
      break;
    }
  }

  return TM1638_SetSingleDigit(DigitDataHEX, DigitPos);
}


/**
 * @brief  Set data to multiple digits in hexadecimal format
 * @param  DigitData: Array to Digits data. 
 *                    (0, 1, ... , 15, a, A, b, B, ... , f, F)
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
 *         - TM1638_FAIL: Operation failed
 */
TM1638_Result_t
TM1638_SetMultipleDigit_HEX(const uint8_t *DigitData, uint8_t StartAddr, uint8_t Count)
{
  uint8_t DigitDataHEX[10];
  uint8_t DecimalPoint = 0;

  for (uint8_t i = 0; i < Count; i++)
  {
    DecimalPoint = DigitData[i] & 0x80;

    if ((DigitData[i] & 0x7F) >= 0 && (DigitData[i] & 0x7F) <= 15)
    {
      DigitDataHEX[i] = HexTo7Seg[DigitData[i] & 0x7F] | DecimalPoint;
    }
    else
    {
      switch (DigitData[i] & 0x7F)
      {
      case 'A':
      case 'a':
        DigitDataHEX[i] = HexTo7Seg[0x0A] | DecimalPoint;
        break;

      case 'B':
      case 'b':
        DigitDataHEX[i] = HexTo7Seg[0x0B] | DecimalPoint;
        break;

      case 'C':
      case 'c':
        DigitDataHEX[i] = HexTo7Seg[0x0C] | DecimalPoint;
        break;

      case 'D':
      case 'd':
        DigitDataHEX[i] = HexTo7Seg[0x0D] | DecimalPoint;
        break;

      case 'E':
      case 'e':
        DigitDataHEX[i] = HexTo7Seg[0x0E] | DecimalPoint;
        break;

      case 'F':
      case 'f':
        DigitDataHEX[i] = HexTo7Seg[0x0F] | DecimalPoint;
        break;

      default:
        DigitDataHEX[i] = 0;
        break;
      }
    }
  }

  return TM1638_SetMultipleDigit((const uint8_t *)DigitDataHEX, StartAddr, Count);
}


/** 
 ==================================================================================
                      ##### Public Keypad Functions #####                         
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
 * @param  Keys: pointer to save key scan result
 *         - bit0=>K1_SEG1, bit1=>K1_SEG2, ..., bit7=>K1_SEG8,
 *         - bit8=>K2_SEG1, bit9=>K2_SEG2, ..., bit15=>K2_SEG8,
 *         - bit16=>K3_SEG1, bit17=>K3_SEG2, ..., bit23=>K3_SEG8,
 * 
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful
 *         - TM1638_FAIL: Operation failed
 */
TM1638_Result_t
TM1638_ScanKeys(uint32_t *Keys)
{
  uint8_t KeyRegs[4];
  uint32_t KeysBuff = 0;
  uint8_t Kn = 0x01;

  TM1638_ScanKeyRegs(KeyRegs);

  for (uint8_t i = 0; i < 3; i++)
  {
    for (int8_t i = 3; i >= 0; i--)
    {
      KeysBuff <<= 1;

      if (KeyRegs[i] & (Kn << 4))
        KeysBuff |= 1;

      KeysBuff <<= 1;

      if (KeyRegs[i] & Kn)
        KeysBuff |= 1;
    }

    Kn <<= 1;
  }

  *Keys = KeysBuff;

  return TM1638_OK;
}
