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
#include "TM1638.h"


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
const uint8_t HexTo7Seg[40] =
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
  0x71, // F
  0x6F, // g
  0x3D, // G
  0x74, // h
  0x76, // H
  0x05, // i
  0x06, // I
  0x0D, // j
  0x30, // l
  0x38, // L
  0x54, // n
  0x37, // N
  0x5C, // o
  0x3F, // O
  0x73, // P
  0x67, // q
  0x50, // r
  0x6D, // S
  0x78, // t
  0x1C, // u
  0x3E, // U
  0x66, // y
  0x08, // _
  0x40, // -
  0x01  // Overscore
};



/**
 ==================================================================================
                           ##### Private Functions #####                           
 ==================================================================================
 */

static inline void
TM1638_StartComunication(TM1638_Handler_t *Handler)
{
  Handler->StbWrite(0);
}

static inline void
TM1638_StopComunication(TM1638_Handler_t *Handler)
{
  Handler->StbWrite(1);
}

static void
TM1638_WriteBytes(TM1638_Handler_t *Handler,
                  const uint8_t *Data, uint8_t NumOfBytes)
{
  uint8_t i, j, Buff;

  Handler->DioConfigOut();

  for (j = 0; j < NumOfBytes; j++)
  {
    for (i = 0, Buff = Data[j]; i < 8; ++i, Buff >>= 1)
    {
      Handler->ClkWrite(0);
      Handler->DelayUs(1);
      Handler->DioWrite(Buff & 0x01);
      Handler->ClkWrite(1);
      Handler->DelayUs(1);
    }
  }
}

static void
TM1638_ReadBytes(TM1638_Handler_t *Handler,
                 uint8_t *Data, uint8_t NumOfBytes)
{
  uint8_t i, j, Buff;

  Handler->DioConfigIn();

  Handler->DelayUs(5);

  for (j = 0; j < NumOfBytes; j++)
  {
    for (i = 0, Buff = 0; i < 8; i++)
    {
      Handler->ClkWrite(0);
      Handler->DelayUs(1);
      Handler->ClkWrite(1);
      Buff |= (Handler->DioRead() << i);
      Handler->DelayUs(1);
    }

    Data[j] = Buff;
    Handler->DelayUs(2);
  }
}

static void
TM1638_SetMultipleDisplayRegister(TM1638_Handler_t *Handler,
                                  const uint8_t *DigitData,
                                  uint8_t StartAddr, uint8_t Count)
{
  uint8_t Data = DataInstructionSet | WriteDataToRegister |
                 AutoAddressAdd | NormalMode;

  TM1638_StartComunication(Handler);
  TM1638_WriteBytes(Handler, &Data, 1);
  TM1638_StopComunication(Handler);

  Data = AddressInstructionSet | StartAddr;

  TM1638_StartComunication(Handler);
  TM1638_WriteBytes(Handler, &Data, 1);
  TM1638_WriteBytes(Handler, DigitData, Count);
  TM1638_StopComunication(Handler);
}

static void
TM1638_ScanKeyRegs(TM1638_Handler_t *Handler, uint8_t *KeyRegs)
{
  uint8_t Data = DataInstructionSet | ReadKeyScanData |
                 AutoAddressAdd | NormalMode;

  TM1638_StartComunication(Handler);
  TM1638_WriteBytes(Handler, &Data, 1);
  TM1638_ReadBytes(Handler, KeyRegs, 4);
  TM1638_StopComunication(Handler);
}



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
TM1638_Init(TM1638_Handler_t *Handler, uint8_t Type)
{
  Handler->DisplayType = TM1638DisplayTypeComCathode;

#if TM1638_SUPPORT_COM_ANODE
  for (uint8_t i = 0; i < 16; i++)
  {
    Handler->DisplayRegister[i] = 0;
  }
  if (Type == TM1638DisplayTypeComCathode)
    Handler->DisplayType = TM1638DisplayTypeComCathode;
  else
    Handler->DisplayType = TM1638DisplayTypeComAnode;
#endif

  Handler->PlatformInit();
  return TM1638_OK;
}

/**
 * @brief  De-Initialize TM1638.
 * @param  Handler: Pointer to handler
 * @retval TM1638_Result_t
 *         - TM1638_OK: Operation was successful.
 */
TM1638_Result_t
TM1638_DeInit(TM1638_Handler_t *Handler)
{
  Handler->PlatformDeInit();
  return TM1638_OK;
}



/**
 ==================================================================================
                        ##### Public Display Functions #####                       
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
                     uint8_t Brightness, uint8_t DisplayState)
{
  uint8_t Data = DisplayControlInstructionSet;
  Data |= Brightness & 0x07;
  Data |= (DisplayState) ? (ShowTurnOn) : (ShowTurnOff);

  TM1638_StartComunication(Handler);
  TM1638_WriteBytes(Handler, &Data, 1);
  TM1638_StopComunication(Handler);

  return TM1638_OK;
}


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
                      uint8_t DigitData, uint8_t DigitPos)
{ 
  if (Handler->DisplayType == TM1638DisplayTypeComCathode)
    TM1638_SetMultipleDisplayRegister(Handler, &DigitData, DigitPos, 1);
#if (TM1638_SUPPORT_COM_ANODE)
  else
    TM1638_SetMultipleDigit(Handler, &DigitData, DigitPos, 1);
#endif
  return TM1638_OK;
}


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
                        uint8_t StartAddr, uint8_t Count)
{
  uint8_t Shift = 0;
  uint8_t DigitDataBuff = 0;
  uint8_t i = 0, j = 0;

  if (Handler->DisplayType == TM1638DisplayTypeComCathode)
    TM1638_SetMultipleDisplayRegister(Handler, DigitData, StartAddr, Count);
#if (TM1638_SUPPORT_COM_ANODE)
  else
  {
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
          Handler->DisplayRegister[i] |= (1 << Shift);
        else
          Handler->DisplayRegister[i] &= ~(1 << Shift);
      }
    }
    TM1638_SetMultipleDisplayRegister(Handler, Handler->DisplayRegister, 0, 16);
  }
#endif

  return TM1638_OK;
}

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
                          uint8_t DigitData, uint8_t DigitPos)
{
  uint8_t DigitDataHEX = 0;
  uint8_t DecimalPoint = DigitData & 0x80;

  DigitData &= 0x7F;

  if (DigitData <= 15)
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

  return TM1638_SetSingleDigit(Handler, DigitDataHEX, DigitPos);
}


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
                            uint8_t StartAddr, uint8_t Count)
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

  return TM1638_SetMultipleDigit(Handler,
                                 (const uint8_t *)DigitDataHEX, StartAddr, Count);
}


/**
 * @brief  Set data to multiple digits in char format
 * @param  Handler: Pointer to handler
 * @param  DigitData: Array to Digits data. 
 *                    Supported chars 0,1,2,3,4,5,6,7,8,9
 *                                    A,b,C,d,E,F,g,G,h,H,i,I,j,l,L,n,N,o,O,P,q,r,S,t,u,U,y
 *                                    _,-,Overscore (use ~ to set)
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
TM1638_SetMultipleDigit_CHAR(TM1638_Handler_t *Handler, const uint8_t *DigitData,
                            uint8_t StartAddr, uint8_t Count)
{
  uint8_t DigitDataHEX[10];
  uint8_t DecimalPoint = 0;

  for (uint8_t i = 0; i < Count; i++)
  {
    DecimalPoint = DigitData[i] & 0x80;

    // numbers 0 - 9
    if ((DigitData[i] & 0x7F) >= (uint8_t)'0' && (DigitData[i] & 0x7F) <= (uint8_t)'9')
    {
      DigitDataHEX[i] = HexTo7Seg[(DigitData[i]-48) & 0x7F] | DecimalPoint;
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

      case 'g':
        DigitDataHEX[i] = HexTo7Seg[0x10] | DecimalPoint;
      break;
      
      case 'G':
        DigitDataHEX[i] = HexTo7Seg[0x11] | DecimalPoint;
      break;

      case 'h':
        DigitDataHEX[i] = HexTo7Seg[0x12] | DecimalPoint;
      break;
      
      case 'H':
        DigitDataHEX[i] = HexTo7Seg[0x13] | DecimalPoint;
      break;

      case 'i':
        DigitDataHEX[i] = HexTo7Seg[0x14] | DecimalPoint;
      break;
      
      case 'I':
        DigitDataHEX[i] = HexTo7Seg[0x15] | DecimalPoint;
      break;

      case 'j':
      case 'J':
        DigitDataHEX[i] = HexTo7Seg[0x16] | DecimalPoint;
      break;

      case 'l':
        DigitDataHEX[i] = HexTo7Seg[0x17] | DecimalPoint;
      break;

      case 'L':
        DigitDataHEX[i] = HexTo7Seg[0x18] | DecimalPoint;
      break;

      case 'n':
        DigitDataHEX[i] = HexTo7Seg[0x19] | DecimalPoint;
      break;
      
      case 'N':
        DigitDataHEX[i] = HexTo7Seg[0x1A] | DecimalPoint;
      break;

      case 'o':
        DigitDataHEX[i] = HexTo7Seg[0x1B] | DecimalPoint;
      break;
      
      case 'O':
        DigitDataHEX[i] = HexTo7Seg[0x1C] | DecimalPoint;
      break;

      case 'p':
      case 'P':
        DigitDataHEX[i] = HexTo7Seg[0x1D] | DecimalPoint;
      break;

      case 'q':
      case 'Q':
        DigitDataHEX[i] = HexTo7Seg[0x1E] | DecimalPoint;
      break;

      case 'r':
      case 'R':
        DigitDataHEX[i] = HexTo7Seg[0x1F] | DecimalPoint;
      break;

      case 's':
      case 'S':
        DigitDataHEX[i] = HexTo7Seg[0x20] | DecimalPoint;
      break;

      case 't':
      case 'T':
        DigitDataHEX[i] = HexTo7Seg[0x21] | DecimalPoint;
      break;

      case 'u':
        DigitDataHEX[i] = HexTo7Seg[0x22] | DecimalPoint;
      break;

      case 'U':
        DigitDataHEX[i] = HexTo7Seg[0x23] | DecimalPoint;
      break;

      case 'y':
      case 'Y':
        DigitDataHEX[i] = HexTo7Seg[0x24] | DecimalPoint;
      break;

      case '_':
        DigitDataHEX[i] = HexTo7Seg[0x25] | DecimalPoint;
      break;

      case '-':
        DigitDataHEX[i] = HexTo7Seg[0x26] | DecimalPoint;
      break;

      case '~':
        DigitDataHEX[i] = HexTo7Seg[0x27] | DecimalPoint;
      break;

      default:
        DigitDataHEX[i] = 0;
        break;
      }
    }
  }

  return TM1638_SetMultipleDigit(Handler,
                                 (const uint8_t *)DigitDataHEX, StartAddr, Count);
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
TM1638_ScanKeys(TM1638_Handler_t *Handler, uint32_t *Keys)
{
  uint8_t KeyRegs[4];
  uint32_t KeysBuff = 0;
  uint8_t Kn = 0x01;

  TM1638_ScanKeyRegs(Handler, KeyRegs);

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
