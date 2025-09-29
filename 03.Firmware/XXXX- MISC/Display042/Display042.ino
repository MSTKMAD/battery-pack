// archivo que pruebe una pantalla oled  ssd1306 con un display 0.96 pulgadas por SPI
#include "Adafruit_SSD1306_096.h"
#include "bitmaps.h"
#include "C:\Users\javie\Team Dropbox\JRODRIGUEZ\Repositorios\01.Rover\03.Firmware\LIBRARIES\0008.Adafruit_GFX_Library\Fonts\FreeMono9pt7b.h"  // Include a larger font for better visibility
#include "C:\Users\javie\Team Dropbox\JRODRIGUEZ\Repositorios\01.Rover\03.Firmware\LIBRARIES\0008.Adafruit_GFX_Library\Fonts\FreeSans9pt7b.h"  // Include a larger font for better visibility
#include "C:\Users\javie\Team Dropbox\JRODRIGUEZ\Repositorios\01.Rover\03.Firmware\LIBRARIES\0008.Adafruit_GFX_Library\Fonts\FreeSerif9pt7b.h" // Include a larger font for better visibility
#include "C:\Users\javie\Team Dropbox\JRODRIGUEZ\Repositorios\01.Rover\03.Firmware\LIBRARIES\0008.Adafruit_GFX_Library\Fonts\Org_01.h"         // Include a larger font for better visibility
#include "C:\Users\javie\Team Dropbox\JRODRIGUEZ\Repositorios\01.Rover\03.Firmware\LIBRARIES\0008.Adafruit_GFX_Library\Fonts\Picopixel.h"      // Include a larger font for better visibility
#include "C:\Users\javie\Team Dropbox\JRODRIGUEZ\Repositorios\01.Rover\03.Firmware\LIBRARIES\0008.Adafruit_GFX_Library\Fonts\Tiny3x3a2pt7b.h"  // Include a larger font for better visibility
#include "C:\Users\javie\Team Dropbox\JRODRIGUEZ\Repositorios\01.Rover\03.Firmware\LIBRARIES\0008.Adafruit_GFX_Library\Fonts\TomThumb.h"       // Include a larger font for better visibility

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// If using software SPI (the default case):
#define OLED_MOSI 9
#define OLED_CLK 10
#define OLED_DC 11
#define OLED_CS 12
#define OLED_RESET 13
Adafruit_SSD1306 OLED_display(SCREEN_WIDTH,SCREEN_HEIGHT,OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

const uint16_t OFFSET_X = 28 + 4; // Offset X for drawing
const uint16_t OFFSET_Y = 24 + 4; // Offset Y for drawing

void setup()
{
    Serial.begin(9600);
    OLED_display.begin(SSD1306_SWITCHCAPVCC);
    OLED_display.clearDisplay();
    OLED_display.setTextSize(1);
    OLED_display.setFont(&FreeSans9pt7b); // Set a larger font for better visibility
    /*
    OLED_display.setTextColor(SSD1306_WHITE);
    OLED_display.setCursor(OFFSET_X, OFFSET_Y);
    OLED_display.print("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    
    OLED_display.setTextSize(2);
    OLED_display.setCursor(OFFSET_X, OFFSET_Y + 8);
    OLED_display.print("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    OLED_display.display();
    */  
    delay(2000); // Espera para mostrar el mensaje
    OLED_display.clearDisplay();
    OLED_display.drawRect(OFFSET_X, OFFSET_Y, 64, 32, SSD1306_WHITE);
    OLED_display.display();

    delay(2000); // Espera para mostrar el rectángulo
    DisplayEndScreen();
    delay(2000);      // Espera para mostrar la pantalla final
    DisplayVolt(123); // Muestra un voltaje de ejemplo
    delay(2000);      // Espera para mostrar la pantalla final
/*
OLED_display.clearDisplay();
OLED_display.setTextSize(2);
OLED_display.setCursor(OFFSET_X, OFFSET_Y);
OLED_display.print("AB12");
OLED_display.display();
*/
}

void loop()
{
}

void DisplayEndScreen()
{

    OLED_display.clearDisplay();
    OLED_display.setTextSize(2);
    OLED_display.setCursor(OFFSET_X, OFFSET_Y);
    OLED_display.print("GOOD");
    OLED_display.setCursor(OFFSET_X, OFFSET_Y + 16);
    OLED_display.print("JOB");
    OLED_display.display();
}
void DisplayVolt(int16_t number)
{
    int intPart;
    int decPart;
    int fracPart;
    int cursor;

    // Config the text
    OLED_display.setTextSize(2);
    OLED_display.setTextColor(WHITE);

    OLED_display.fillRect(OFFSET_X, OFFSET_Y, 72, 40, BLACK); // clears the screen and buffer
    intPart = number / 10;
    decPart = intPart / 10;
    fracPart = number - 10 * intPart;

    if (number >= 100)
    {
        OLED_display.setCursor(OFFSET_X - 4, OFFSET_Y);
        OLED_display.print(intPart);
    }
    else
    {
        OLED_display.setCursor(OFFSET_X + 15, OFFSET_Y);
        OLED_display.print(intPart);
    }
    OLED_display.fillRect(OFFSET_X + 38, OFFSET_Y + 25, 3, 3, WHITE);
    cursor = OFFSET_X + 44; //
    OLED_display.setCursor(cursor, OFFSET_Y);
    OLED_display.print(fracPart);
    OLED_display.display();
}