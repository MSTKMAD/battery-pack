/**
 * @file bitmaps.h
 * @author Javi (Javier@musotoku.com)
 * @brief Conjunto de Bitmaps, funciones y constantes realcionadas con la el display Musotoku 15x7.
 * @version 3
 * @date 2020-8-4
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#define DISPLAY_SIZE 6      //Altura del caracter del display
#define POWERBAR_LOCATION 1 // Posicion de la barra de potencia: 0 = Top, 1 = Bot.
#define C_DISPLAY_WIDTH 15  // Anchura del display.
#define C_DISPLAY_HEIGHT 6  // Altura del display.

#if DISPLAY_SIZE == 7

const int16_t C_TEXT_CHAR_HEIGHT = 7; // Altrua del caracter.

volt_dec_number[] =
    {B00000000,
     B00000000,
     B00000000,
     B00000000,
     B00000000,
     B00000000,
     B00000000},
    volt_cent_number[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    volt_unit_number[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    cap_dec_number[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    cap_unit_number[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000};
const uint8_t
    SPACEchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    EXCLMARKchar[] =
        {B11000000,
         B11000000,
         B11000000,
         B11000000,
         B00000000,
         B11000000,
         B1000000},
    QUOTMARKchar[] =
        {B10100000,
         B10100000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    HASTAGchar[] =
        {B00000000,
         B01001000,
         B11111100,
         B01001000,
         B01001000,
         B11111100,
         B01001000},
    DOLLARchar[] =
        {B01110000,
         B10101000,
         B10100000,
         B01110000,
         B00101000,
         B10101000,
         B01110000},
    PERCENTchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B10010000,
         B00100000,
         B01000000,
         B10010000},
    ANPERSANchar[] =
        {B01000000,
         B10100000,
         B10100000,
         B01000000,
         B10101000,
         B10010000,
         B01101000},
    APOSTROPHEchar[] =
        {B10000000,
         B10000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    OPENPARENTESISchar[] =
        {B01000000,
         B10000000,
         B10000000,
         B10000000,
         B10000000,
         B10000000,
         B01000000},
    CLOSEPARENTESISchar[] =
        {B10000000,
         B01000000,
         B01000000,
         B01000000,
         B01000000,
         B01000000,
         B10000000},
    ASTERISKchar[] =
        {B01000000,
         B11100000,
         B01000000,
         B10100000,
         B00000000,
         B00000000,
         B00000000},
    PLUSchar[] =
        {B00000000,
         B00000000,
         B01000000,
         B11100000,
         B01000000,
         B00000000,
         B00000000},
    COMMAchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B01000000,
         B10000000},
    HYPHENchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B11100000,
         B00000000,
         B00000000,
         B00000000},
    DOTchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B11000000,
         B11000000},
    SLASHchar[] =
        {B00100000,
         B00100000,
         B01000000,
         B01000000,
         B01000000,
         B10000000,
         B10000000},
    ZEROchar[] =
        {B01100000,
         B10010000,
         B10010000,
         B10010000,
         B10010000,
         B10010000,
         B01100000},
    ONEchar[] =
        {B01000000,
         B11000000,
         B01000000,
         B01000000,
         B01000000,
         B01000000,
         B01000000},
    TWOchar[] =
        {B01100000,
         B10010000,
         B00010000,
         B01100000,
         B10000000,
         B10000000,
         B11110000},
    THREEchar[] =
        {B01100000,
         B10010000,
         B00010000,
         B00100000,
         B00010000,
         B10010000,
         B01100000},
    FOURchar[] =
        {B10000000,
         B10000000,
         B10010000,
         B11110000,
         B00010000,
         B00010000,
         B00010000},
    FIVEchar[] =
        {B11110000,
         B10000000,
         B10000000,
         B11100000,
         B00010000,
         B10010000,
         B01100000},
    SIXchar[] =
        {B01100000,
         B10010000,
         B10000000,
         B11100000,
         B10010000,
         B10010000,
         B01100000},
    SEVENchar[] =
        {B11110000,
         B00010000,
         B00010000,
         B00100000,
         B01000000,
         B10000000,
         B10000000},
    EIGHTchar[] =
        {B01100000,
         B10010000,
         B10010000,
         B01100000,
         B10010000,
         B10010000,
         B01100000},
    NINEchar[] =
        {B01100000,
         B10010000,
         B10010000,
         B01110000,
         B00010000,
         B10010000,
         B01100000},
    COLONchar[] =
        {B00000000,
         B00000000,
         B11000000,
         B11000000,
         B00000000,
         B11000000,
         B11000000},
    SEMICOLONchar[] =
        {B00000000,
         B00000000,
         B11000000,
         B11000000,
         B00000000,
         B01000000,
         B10000000},
    LESSTHANchar[] =
        {B00000000,
         B00000000,
         B00100000,
         B01000000,
         B10000000,
         B01000000,
         B00100000},
    EQUALchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B11100000,
         B00000000,
         B11100000},
    GREATERTHANchar[] =
        {B00000000,
         B00000000,
         B10000000,
         B01000000,
         B00100000,
         B01000000,
         B10000000},
    QUESTIONchar[] =
        {B01100000,
         B10010000,
         B00010000,
         B00100000,
         B00100000,
         B00000000,
         B00100000},
    ATchar[] =
        {B01111100,
         B10000010,
         B10111010,
         B10101010,
         B10110100,
         B10000010,
         B01111100},
    Achar[] =
        {B01000000,
         B10100000,
         B11100000,
         B10100000,
         B10100000,
         B10100000,
         B10100000},
    Bchar[] =
        {B11000000,
         B10100000,
         B10100000,
         B11000000,
         B10100000,
         B10100000,
         B11000000},
    Cchar[] =
        {B01000000,
         B10100000,
         B10000000,
         B10000000,
         B10000000,
         B10100000,
         B01000000},
    Dchar[] =
        {B11000000,
         B10100000,
         B10100000,
         B10100000,
         B10100000,
         B10100000,
         B11000000},
    Echar[] =
        {B11100000,
         B10000000,
         B10000000,
         B11000000,
         B10000000,
         B10000000,
         B11100000},
    Fchar[] =
        {B11100000,
         B10000000,
         B10000000,
         B11000000,
         B10000000,
         B10000000,
         B10000000},
    Gchar[] =
        {B01000000,
         B10000000,
         B10000000,
         B10000000,
         B10100000,
         B10100000,
         B01000000},
    Hchar[] =
        {B10100000,
         B10100000,
         B10100000,
         B11100000,
         B10100000,
         B10100000,
         B10100000},
    Ichar[] =
        {B11100000,
         B01000000,
         B01000000,
         B01000000,
         B01000000,
         B01000000,
         B11100000},
    Jchar[] =
        {B11100000,
         B00100000,
         B00100000,
         B00100000,
         B00100000,
         B10100000,
         B11100000},
    Kchar[] =
        {B10000000,
         B10000000,
         B10100000,
         B11000000,
         B10000000,
         B11000000,
         B10100000},
    Lchar[] =
        {B10000000,
         B10000000,
         B10000000,
         B10000000,
         B10000000,
         B10000000,
         B11000000},
    Mchar[] =
        {B10001000,
         B11011000,
         B10101000,
         B10001000,
         B10001000,
         B10001000,
         B10001000},
    Nchar[] =
        {B10010000,
         B11010000,
         B10110000,
         B10010000,
         B10010000,
         B10010000,
         B10010000},
    Ochar[] =
        {B01000000,
         B10100000,
         B10100000,
         B10100000,
         B10100000,
         B10100000,
         B01000000},
    Pchar[] =
        {B11000000,
         B10100000,
         B10100000,
         B11000000,
         B10000000,
         B10000000,
         B10000000},
    Qchar[] =
        {B01100000,
         B10010000,
         B10010000,
         B10010000,
         B10010000,
         B10110000,
         B01101000},
    Rchar[] =
        {B11000000,
         B10100000,
         B10100000,
         B10100000,
         B11000000,
         B11000000,
         B10100000},
    Schar[] =
        {B01000000,
         B10100000,
         B10000000,
         B01000000,
         B00100000,
         B10100000,
         B01000000},
    Tchar[] =
        {B11100000,
         B01000000,
         B01000000,
         B01000000,
         B01000000,
         B01000000,
         B01000000},
    Uchar[] =
        {B10100000,
         B10100000,
         B10100000,
         B10100000,
         B10100000,
         B10100000,
         B11100000},
    Vchar[] =
        {B10100000,
         B10100000,
         B10100000,
         B10100000,
         B10100000,
         B10100000,
         B01000000},
    Wchar[] =
        {B10001000,
         B10001000,
         B10001000,
         B10001000,
         B10001000,
         B10101000,
         B01010000},
    Xchar[] =
        {B00000000,
         B00000000,
         B10001000,
         B01010000,
         B00100000,
         B01010000,
         B10001000},
    Ychar[] =
        {B10001000,
         B10001000,
         B10001000,
         B01010000,
         B00100000,
         B00100000,
         B00100000},
    Zchar[] =
        {B00000000,
         B11110000,
         B00010000,
         B00100000,
         B01000000,
         B10000000,
         B11110000},
    SQUAREBRAQUETchar[] =
        {B11100000,
         B10000000,
         B10000000,
         B10000000,
         B10000000,
         B10000000,
         B11100000},
    BACKSLASHchar[] =
        {B10000000,
         B10000000,
         B01000000,
         B01000000,
         B01000000,
         B00100000,
         B00100000},
    CLOSESQUAREBRAQUETchar[] =
        {B11100000,
         B00100000,
         B00100000,
         B00100000,
         B00100000,
         B00100000,
         B11100000},
    CIRCUMFLEXchar[] =
        {B01000000,
         B10100000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    UNDERSCOREchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B11100000},
    ACCENTchar[] =
        {B10000000,
         B01000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    OPENCURLYBRAchar[] =
        {B01100000,
         B01000000,
         B01000000,
         B11000000,
         B01000000,
         B01000000,
         B01100000},
    CLOSECURLYBRAchar[] =
        {B11000000,
         B01000000,
         B01100000,
         B01000000,
         B01000000,
         B01000000,
         B11000000},
    VERTICALBARchar[] =
        {B10000000,
         B10000000,
         B10000000,
         B10000000,
         B10000000,
         B10000000,
         B10000000},
    SWUNGchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B01010000,
         B10100000,
         B00000000,
         B00000000};

void Char2Bitmap(char character, uint8_t *bitmap, int16_t array_len)
{
    if (array_len < sizeof(SPACEchar))
    {
        for (int i = 0; i < array_len; i++)
        {
            bitmap[i] = 0xFF;
        }
    }
    else if (character == 32)
    {
        memcpy(bitmap, SPACEchar, array_len);
    }
    else if (character == 33)
    {
        memcpy(bitmap, EXCLMARKchar, array_len);
    }
    else if (character == 34)
    {
        memcpy(bitmap, QUOTMARKchar, array_len);
    }
    else if (character == 35)
    {
        memcpy(bitmap, HASTAGchar, array_len);
    }
    else if (character == 36)
    {
        memcpy(bitmap, DOLLARchar, array_len);
    }
    else if (character == 37)
    {
        memcpy(bitmap, PERCENTchar, array_len);
    }
    else if (character == 38)
    {
        memcpy(bitmap, ANPERSANchar, array_len);
    }
    else if (character == 39)
    {
        memcpy(bitmap, APOSTROPHEchar, array_len);
    }
    else if (character == 40)
    {
        memcpy(bitmap, OPENPARENTESISchar, array_len);
    }
    else if (character == 41)
    {
        memcpy(bitmap, CLOSEPARENTESISchar, array_len);
    }
    else if (character == 42)
    {
        memcpy(bitmap, ASTERISKchar, array_len);
    }
    else if (character == 43)
    {
        memcpy(bitmap, PLUSchar, array_len);
    }
    else if (character == 44)
    {
        memcpy(bitmap, COMMAchar, array_len);
    }
    else if (character == 45)
    {
        memcpy(bitmap, HYPHENchar, array_len);
    }
    else if (character == 46)
    {
        memcpy(bitmap, DOTchar, array_len);
    }
    else if (character == 47)
    {
        memcpy(bitmap, SLASHchar, array_len);
    }
    else if (character == 48)
    {
        memcpy(bitmap, ZEROchar, array_len);
    }
    else if (character == 49)
    {
        memcpy(bitmap, ONEchar, array_len);
    }
    else if (character == 50)
    {
        memcpy(bitmap, TWOchar, array_len);
    }
    else if (character == 51)
    {
        memcpy(bitmap, THREEchar, array_len);
    }
    else if (character == 52)
    {
        memcpy(bitmap, FOURchar, array_len);
    }
    else if (character == 53)
    {
        memcpy(bitmap, FIVEchar, array_len);
    }
    else if (character == 54)
    {
        memcpy(bitmap, SIXchar, array_len);
    }
    else if (character == 55)
    {
        memcpy(bitmap, SEVENchar, array_len);
    }
    else if (character == 56)
    {
        memcpy(bitmap, EIGHTchar, array_len);
    }
    else if (character == 57)
    {
        memcpy(bitmap, NINEchar, array_len);
    }
    else if (character == 58)
    {
        memcpy(bitmap, COLONchar, array_len);
    }
    else if (character == 59)
    {
        memcpy(bitmap, SEMICOLONchar, array_len);
    }
    else if (character == 60)
    {
        memcpy(bitmap, LESSTHANchar, array_len);
    }
    else if (character == 61)
    {
        memcpy(bitmap, EQUALchar, array_len);
    }
    else if (character == 62)
    {
        memcpy(bitmap, GREATERTHANchar, array_len);
    }
    else if (character == 63)
    {
        memcpy(bitmap, QUESTIONchar, array_len);
    }
    else if (character == 64)
    {
        memcpy(bitmap, ATchar, array_len);
    }
    else if (character == 65)
    {
        memcpy(bitmap, Achar, array_len);
    }
    else if (character == 66)
    {
        memcpy(bitmap, Bchar, array_len);
    }
    else if (character == 67)
    {
        memcpy(bitmap, Cchar, array_len);
    }
    else if (character == 68)
    {
        memcpy(bitmap, Dchar, array_len);
    }
    else if (character == 69)
    {
        memcpy(bitmap, Echar, array_len);
    }
    else if (character == 70)
    {
        memcpy(bitmap, Fchar, array_len);
    }
    else if (character == 71)
    {
        memcpy(bitmap, Gchar, array_len);
    }
    else if (character == 72)
    {
        memcpy(bitmap, Hchar, array_len);
    }
    else if (character == 73)
    {
        memcpy(bitmap, Ichar, array_len);
    }
    else if (character == 74)
    {
        memcpy(bitmap, Jchar, array_len);
    }
    else if (character == 75)
    {
        memcpy(bitmap, Kchar, array_len);
    }
    else if (character == 76)
    {
        memcpy(bitmap, Lchar, array_len);
    }
    else if (character == 77)
    {
        memcpy(bitmap, Mchar, array_len);
    }
    else if (character == 78)
    {
        memcpy(bitmap, Nchar, array_len);
    }
    else if (character == 79)
    {
        memcpy(bitmap, Ochar, array_len);
    }
    else if (character == 80)
    {
        memcpy(bitmap, Pchar, array_len);
    }
    else if (character == 81)
    {
        memcpy(bitmap, Qchar, array_len);
    }
    else if (character == 82)
    {
        memcpy(bitmap, Rchar, array_len);
    }
    else if (character == 83)
    {
        memcpy(bitmap, Schar, array_len);
    }
    else if (character == 84)
    {
        memcpy(bitmap, Tchar, array_len);
    }
    else if (character == 85)
    {
        memcpy(bitmap, Uchar, array_len);
    }
    else if (character == 86)
    {
        memcpy(bitmap, Vchar, array_len);
    }
    else if (character == 87)
    {
        memcpy(bitmap, Wchar, array_len);
    }
    else if (character == 88)
    {
        memcpy(bitmap, Xchar, array_len);
    }
    else if (character == 89)
    {
        memcpy(bitmap, Ychar, array_len);
    }
    else if (character == 90)
    {
        memcpy(bitmap, Zchar, array_len);
    }
    else if (character == 91)
    {
        memcpy(bitmap, SQUAREBRAQUETchar, array_len);
    }
    else if (character == 92)
    {
        memcpy(bitmap, BACKSLASHchar, array_len);
    }
    else if (character == 93)
    {
        memcpy(bitmap, CLOSESQUAREBRAQUETchar, array_len);
    }
    else if (character == 94)
    {
        memcpy(bitmap, CIRCUMFLEXchar, array_len);
    }
    else if (character == 95)
    {
        memcpy(bitmap, UNDERSCOREchar, array_len);
    }
    else if (character == 96)
    {
        memcpy(bitmap, ACCENTchar, array_len);
    }
    else if (character == 123)
    {
        memcpy(bitmap, OPENCURLYBRAchar, array_len);
    }
    else if (character == 124)
    {
        memcpy(bitmap, VERTICALBARchar, array_len);
    }
    else if (character == 125)
    {
        memcpy(bitmap, CLOSECURLYBRAchar, array_len);
    }
    else if (character == 126)
    {
        memcpy(bitmap, SWUNGchar, array_len);
    }
}
const uint16_t width_bitmap[] = {4, 3, 4, 6, 6, 5, 6, 2, 4, 4, 4, 4, 3, 4, 3, 4, 5, 3, 5, 5, 5, 5, 5, 5, 5, 5, 3, 3, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 6, 5, 4, 4, 6, 4, 4, 4, 4, 4, 6, 6, 6, 5, 4, 4, 4, 4, 4, 3, 4, 2, 4, 5};

#elif DISPLAY_SIZE == 6

const int16_t C_TEXT_CHAR_HEIGHT = 6; // Altrua del caracter.

uint8_t
    bitmap_char[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    volt_dec_number[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    volt_cent_number[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    volt_unit_number[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    cap_dec_number[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    cap_unit_number[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000};
uint8_t
    SPACEchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    EXCLMARKchar[] =
        {B11000000,
         B11000000,
         B11000000,
         B00000000,
         B11000000,
         B11000000},
    QUOTMARKchar[] =
        {B10100000,
         B10100000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    HASTAGchar[] =
        {B01001000,
         B11111100,
         B01001000,
         B01001000,
         B11111100,
         B01001000},
    DOLLARchar[] =
        {B01111000,
         B10100000,
         B01110000,
         B00101000,
         B00101000,
         B11110000},
    PERCENTchar[] =
        {B00000000,
         B00000000,
         B10010000,
         B00100000,
         B01000000,
         B10010000},
    ANPERSANchar[] =
        {B01000000,
         B10100000,
         B01000000,
         B10101000,
         B10010000,
         B01101000},
    APOSTROPHEchar[] =
        {B10000000,
         B10000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    OPENPARENTESISchar[] =
        {B01000000,
         B10000000,
         B10000000,
         B10000000,
         B10000000,
         B01000000},
    CLOSEPARENTESISchar[] =
        {B10000000,
         B01000000,
         B01000000,
         B01000000,
         B01000000,
         B10000000},
    ASTERISKchar[] =
        {B01000000,
         B11100000,
         B01000000,
         B10100000,
         B00000000,
         B00000000},
    PLUSchar[] =
        {B00000000,
         B00000000,
         B01000000,
         B11100000,
         B01000000,
         B00000000},
    COMMAchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B01000000,
         B10000000},
    HYPHENchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B11100000,
         B00000000,
         B00000000},
    DOTchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B10000000},
    SLASHchar[] =
        {B00100000,
         B00100000,
         B01000000,
         B01000000,
         B10000000,
         B10000000},
    ZEROchar[] =
        {B01100000,
         B10010000,
         B10010000,
         B10010000,
         B10010000,
         B01100000},
    ONEchar[] =
        {B01000000,
         B11000000,
         B01000000,
         B01000000,
         B01000000,
         B01000000},
    TWOchar[] =
        {B01100000,
         B10010000,
         B00100000,
         B01000000,
         B10000000,
         B11110000},
    THREEchar[] =
        {B01100000,
         B10010000,
         B00100000,
         B00010000,
         B10010000,
         B01100000},
    FOURchar[] =
        {B10000000,
         B10000000,
         B10010000,
         B11110000,
         B00010000,
         B00010000},
    FIVEchar[] =
        {B11110000,
         B10000000,
         B11100000,
         B00010000,
         B10010000,
         B01100000},
    SIXchar[] =
        {B01100000,
         B10010000,
         B10000000,
         B11100000,
         B10010000,
         B01100000},
    SEVENchar[] =
        {B11110000,
         B00010000,
         B00100000,
         B01000000,
         B10000000,
         B10000000},
    EIGHTchar[] =
        {B01100000,
         B10010000,
         B01100000,
         B10010000,
         B10010000,
         B01100000},
    NINEchar[] =
        {B01100000,
         B10010000,
         B01110000,
         B00010000,
         B10010000,
         B01100000},
    COLONchar[] =
        {B00000000,
         B11000000,
         B11000000,
         B00000000,
         B11000000,
         B11000000},
    SEMICOLONchar[] =
        {B00000000,
         B11000000,
         B11000000,
         B00000000,
         B01000000,
         B10000000},
    LESSTHANchar[] =
        {B00000000,
         B00100000,
         B01000000,
         B10000000,
         B01000000,
         B00100000},
    EQUALchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B11100000,
         B00000000,
         B11100000},
    GREATERTHANchar[] =
        {B00000000,
         B10000000,
         B01000000,
         B00100000,
         B01000000,
         B10000000},
    QUESTIONchar[] =
        {B01100000,
         B10010000,
         B00010000,
         B00100000,
         B00000000,
         B00100000},
    ATchar[] =
        {B01111000,
         B10000100,
         B10110100,
         B10111000,
         B10000100,
         B01111000},
    Achar[] =
        {B01000000,
         B10100000,
         B11100000,
         B10100000,
         B10100000,
         B10100000},
    Bchar[] =
        {B11000000,
         B10100000,
         B11000000,
         B10100000,
         B10100000,
         B11000000},
    Cchar[] =
        {B01000000,
         B10100000,
         B10000000,
         B10000000,
         B10100000,
         B01000000},
    Dchar[] =
        {B11000000,
         B10100000,
         B10100000,
         B10100000,
         B10100000,
         B11000000},
    Echar[] =
        {B11100000,
         B10000000,
         B11000000,
         B10000000,
         B10000000,
         B11100000},
    Fchar[] =
        {B11100000,
         B10000000,
         B11000000,
         B10000000,
         B10000000,
         B10000000},
    Gchar[] =
        {B01000000,
         B10100000,
         B10000000,
         B10100000,
         B10100000,
         B01000000},
    Hchar[] =
        {B10100000,
         B10100000,
         B11100000,
         B10100000,
         B10100000,
         B10100000},
    Ichar[] =
        {B11100000,
         B01000000,
         B01000000,
         B01000000,
         B01000000,
         B11100000},
    Jchar[] =
        {B11100000,
         B00100000,
         B00100000,
         B00100000,
         B10100000,
         B11100000},
    Kchar[] =
        {B10000000,
         B10100000,
         B11000000,
         B10000000,
         B11000000,
         B10100000},
    Lchar[] =
        {B10000000,
         B10000000,
         B10000000,
         B10000000,
         B10000000,
         B11000000},
    Mchar[] =
        {B10001000,
         B11011000,
         B10101000,
         B10001000,
         B10001000,
         B10001000},
    Nchar[] =
        {B10010000,
         B11010000,
         B10110000,
         B10010000,
         B10010000,
         B10010000},
    Ochar[] =
        {B01000000,
         B10100000,
         B10100000,
         B10100000,
         B10100000,
         B01000000},
    Pchar[] =
        {B11000000,
         B10100000,
         B10100000,
         B11000000,
         B10000000,
         B10000000},
    Qchar[] =
        {B01100000,
         B10010000,
         B10010000,
         B10010000,
         B10110000,
         B01101000},
    Rchar[] =
        {B11000000,
         B10100000,
         B10100000,
         B11000000,
         B11000000,
         B10100000},
    Schar[] =
        {B01100000,
         B10010000,
         B01000000,
         B00100000,
         B10010000,
         B01100000},
    Tchar[] =
        {B11100000,
         B01000000,
         B01000000,
         B01000000,
         B01000000,
         B01000000},
    Uchar[] =
        {B10100000,
         B10100000,
         B10100000,
         B10100000,
         B10100000,
         B11100000},
    Vchar[] =
        {B10100000,
         B10100000,
         B10100000,
         B10100000,
         B10100000,
         B01000000},
    Wchar[] =
        {B10001000,
         B10001000,
         B10001000,
         B10001000,
         B10101000,
         B01010000},
    Xchar[] =
        {B10100000,
         B10100000,
         B01000000,
         B01000000,
         B10100000,
         B10100000},
    Ychar[] =
        {B10100000,
         B10100000,
         B10100000,
         B01000000,
         B01000000,
         B01000000},
    Zchar[] =
        {B11100000,
         B00100000,
         B01000000,
         B10000000,
         B10000000,
         B11100000},
    SQUAREBRAQUETchar[] =
        {B11100000,
         B10000000,
         B10000000,
         B10000000,
         B10000000,
         B11100000},
    BACKSLASHchar[] =
        {B10000000,
         B10000000,
         B01000000,
         B01000000,
         B00100000,
         B00100000},
    CLOSESQUAREBRAQUETchar[] =
        {B11100000,
         B00100000,
         B00100000,
         B00100000,
         B00100000,
         B11100000},
    CIRCUMFLEXchar[] =
        {B01000000,
         B10100000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    UNDERSCOREchar[] =
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B11100000},
    ACCENTchar[] =
        {B10000000,
         B01000000,
         B00000000,
         B00000000,
         B00000000,
         B00000000},
    OPENCURLYBRAchar[] =
        {B01100000,
         B01000000,
         B11000000,
         B01000000,
         B01000000,
         B01100000},
    CLOSECURLYBRAchar[] =
        {B11000000,
         B01000000,
         B01100000,
         B01000000,
         B01000000,
         B11000000},
    VERTICALBARchar[] =
        {B10000000,
         B10000000,
         B10000000,
         B10000000,
         B10000000,
         B10000000},
    SWUNGchar[] =
        {B00000000,
         B00000000,
         B01010000,
         B10100000,
         B00000000,
         B00000000};
void Char2Bitmap(char character, uint8_t *bitmap, uint16_t array_len)
{
    if (array_len < sizeof(SPACEchar))
    {
        for (int i = 0; i < array_len; i++)
        {
            bitmap[i] = 0xFF;
        }
    }
    else if (character == 32)
    {
        memcpy(bitmap, SPACEchar, array_len);
    }
    else if (character == 33)
    {
        memcpy(bitmap, EXCLMARKchar, array_len);
    }
    else if (character == 34)
    {
        memcpy(bitmap, QUOTMARKchar, array_len);
    }
    else if (character == 35)
    {
        memcpy(bitmap, HASTAGchar, array_len);
    }
    else if (character == 36)
    {
        memcpy(bitmap, DOLLARchar, array_len);
    }
    else if (character == 37)
    {
        memcpy(bitmap, PERCENTchar, array_len);
    }
    else if (character == 38)
    {
        memcpy(bitmap, ANPERSANchar, array_len);
    }
    else if (character == 39)
    {
        memcpy(bitmap, APOSTROPHEchar, array_len);
    }
    else if (character == 40)
    {
        memcpy(bitmap, OPENPARENTESISchar, array_len);
    }
    else if (character == 41)
    {
        memcpy(bitmap, CLOSEPARENTESISchar, array_len);
    }
    else if (character == 42)
    {
        memcpy(bitmap, ASTERISKchar, array_len);
    }
    else if (character == 43)
    {
        memcpy(bitmap, PLUSchar, array_len);
    }
    else if (character == 44)
    {
        memcpy(bitmap, COMMAchar, array_len);
    }
    else if (character == 45)
    {
        memcpy(bitmap, HYPHENchar, array_len);
    }
    else if (character == 46)
    {
        memcpy(bitmap, DOTchar, array_len);
    }
    else if (character == 47)
    {
        memcpy(bitmap, SLASHchar, array_len);
    }
    else if (character == 48)
    {
        memcpy(bitmap, ZEROchar, array_len);
    }
    else if (character == 49)
    {
        memcpy(bitmap, ONEchar, array_len);
    }
    else if (character == 50)
    {
        memcpy(bitmap, TWOchar, array_len);
    }
    else if (character == 51)
    {
        memcpy(bitmap, THREEchar, array_len);
    }
    else if (character == 52)
    {
        memcpy(bitmap, FOURchar, array_len);
    }
    else if (character == 53)
    {
        memcpy(bitmap, FIVEchar, array_len);
    }
    else if (character == 54)
    {
        memcpy(bitmap, SIXchar, array_len);
    }
    else if (character == 55)
    {
        memcpy(bitmap, SEVENchar, array_len);
    }
    else if (character == 56)
    {
        memcpy(bitmap, EIGHTchar, array_len);
    }
    else if (character == 57)
    {
        memcpy(bitmap, NINEchar, array_len);
    }
    else if (character == 58)
    {
        memcpy(bitmap, COLONchar, array_len);
    }
    else if (character == 59)
    {
        memcpy(bitmap, SEMICOLONchar, array_len);
    }
    else if (character == 60)
    {
        memcpy(bitmap, LESSTHANchar, array_len);
    }
    else if (character == 61)
    {
        memcpy(bitmap, EQUALchar, array_len);
    }
    else if (character == 62)
    {
        memcpy(bitmap, GREATERTHANchar, array_len);
    }
    else if (character == 63)
    {
        memcpy(bitmap, QUESTIONchar, array_len);
    }
    else if (character == 64)
    {
        memcpy(bitmap, ATchar, array_len);
    }
    else if (character == 65)
    {
        memcpy(bitmap, Achar, array_len);
    }
    else if (character == 66)
    {
        memcpy(bitmap, Bchar, array_len);
    }
    else if (character == 67)
    {
        memcpy(bitmap, Cchar, array_len);
    }
    else if (character == 68)
    {
        memcpy(bitmap, Dchar, array_len);
    }
    else if (character == 69)
    {
        memcpy(bitmap, Echar, array_len);
    }
    else if (character == 70)
    {
        memcpy(bitmap, Fchar, array_len);
    }
    else if (character == 71)
    {
        memcpy(bitmap, Gchar, array_len);
    }
    else if (character == 72)
    {
        memcpy(bitmap, Hchar, array_len);
    }
    else if (character == 73)
    {
        memcpy(bitmap, Ichar, array_len);
    }
    else if (character == 74)
    {
        memcpy(bitmap, Jchar, array_len);
    }
    else if (character == 75)
    {
        memcpy(bitmap, Kchar, array_len);
    }
    else if (character == 76)
    {
        memcpy(bitmap, Lchar, array_len);
    }
    else if (character == 77)
    {
        memcpy(bitmap, Mchar, array_len);
    }
    else if (character == 78)
    {
        memcpy(bitmap, Nchar, array_len);
    }
    else if (character == 79)
    {
        memcpy(bitmap, Ochar, array_len);
    }
    else if (character == 80)
    {
        memcpy(bitmap, Pchar, array_len);
    }
    else if (character == 81)
    {
        memcpy(bitmap, Qchar, array_len);
    }
    else if (character == 82)
    {
        memcpy(bitmap, Rchar, array_len);
    }
    else if (character == 83)
    {
        memcpy(bitmap, Schar, array_len);
    }
    else if (character == 84)
    {
        memcpy(bitmap, Tchar, array_len);
    }
    else if (character == 85)
    {
        memcpy(bitmap, Uchar, array_len);
    }
    else if (character == 86)
    {
        memcpy(bitmap, Vchar, array_len);
    }
    else if (character == 87)
    {
        memcpy(bitmap, Wchar, array_len);
    }
    else if (character == 88)
    {
        memcpy(bitmap, Xchar, array_len);
    }
    else if (character == 89)
    {
        memcpy(bitmap, Ychar, array_len);
    }
    else if (character == 90)
    {
        memcpy(bitmap, Zchar, array_len);
    }
    else if (character == 91)
    {
        memcpy(bitmap, SQUAREBRAQUETchar, array_len);
    }
    else if (character == 92)
    {
        memcpy(bitmap, BACKSLASHchar, array_len);
    }
    else if (character == 93)
    {
        memcpy(bitmap, CLOSESQUAREBRAQUETchar, array_len);
    }
    else if (character == 94)
    {
        memcpy(bitmap, CIRCUMFLEXchar, array_len);
    }
    else if (character == 95)
    {
        memcpy(bitmap, UNDERSCOREchar, array_len);
    }
    else if (character == 96)
    {
        memcpy(bitmap, ACCENTchar, array_len);
    }
    else if (character == 123)
    {
        memcpy(bitmap, OPENCURLYBRAchar, array_len);
    }
    else if (character == 124)
    {
        memcpy(bitmap, VERTICALBARchar, array_len);
    }
    else if (character == 125)
    {
        memcpy(bitmap, CLOSECURLYBRAchar, array_len);
    }
    else if (character == 126)
    {
        memcpy(bitmap, SWUNGchar, array_len);
    }
}
const uint16_t width_bitmap[] = {4, 3, 4, 6, 6, 5, 6, 2, 4, 4, 4, 4, 3, 4, 2, 4, 5, 3, 5, 5, 5, 5, 5, 5, 5, 5, 3, 3, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 6, 5, 4, 4, 6, 4, 5, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 2, 4, 5};
#endif

uint8_t
    Frame_Batt_1[] =
        {B01111111,
         B01000000,
         B01000000,
         B01000000,
         B01000000,
         B01111111},

    Frame_Batt_2[] =
        {B11111000,
         B00001000,
         B00001100,
         B00001100,
         B00001000,
         B11111000};
uint8_t
    Frame_USB_1[] =
        {B00000111,
         B00001000,
         B11111000,
         B00001000,
         B00000111,
         B00000000},

    Frame_USB_2[] =
        {B11100000,
         B00111000,
         B00101000,
         B00111000,
         B11100000,
         B00000000};

uint8_t
    EYES_1[] =
        {B01111100,
         B10000010,
         B10011010,
         B10011010,
         B10000010,
         B01111100},

    EYES_2[] =
        {B01111100,
         B10000010,
         B10110010,
         B10110010,
         B10000010,
         B01111100},

    EYES2_1[] =
        {B01111100,
         B11111110,
         B11110010,
         B11110010,
         B11111110,
         B01111100},

    EYES2_2[] =
        {B01111100,
         B11111110,
         B10011110,
         B10011110,
         B11111110,
         B01111100};

uint8_t
    FACE_1[] =
        {B11100000,
         B00000001,
         B01000001,
         B00000001,
         B00000100,
         B00000011},

    FACE_2[] =
        {B00001110,
         B00000000,
         B00000100,
         B00000000,
         B01000000,
         B10000000};
uint8_t
    Frame_Batt_Low_1[] =
        {B00000000,
         B00111111,
         B00100001,
         B00100010,
         B00111111,
         B00000100},

    Frame_Batt_Low_2[] =
        {B10000000,
         B11110000,
         B00011000,
         B00011000,
         B11110000,
         B00000000};
uint8_t
    Frame_Error_1[] =
        {B00001000,
         B00010001,
         B00010001,
         B00010000,
         B00010001,
         B00001000},

    Frame_Error_2[] =
        {B00100000,
         B00010000,
         B00010000,
         B00010000,
         B00010000,
         B00100000};