
#include <tinyNeoPixelmod.h>

#define PIN_STRIP 3
#define PIN_CHARGER 5
#define PIN_STATUS 10
#define PIN_ADC_SENSE 15

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
tinyNeoPixel strip = tinyNeoPixel(5, PIN_STRIP, NEO_RGBW + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

uint32_t v_sense;

void setup()
{
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    pinMode(PIN_CHARGER, OUTPUT);
    pinMode(PIN_STATUS, INPUT);
    pinMode(PIN_ADC_SENSE, INPUT);
    digitalWrite(PIN_CHARGER, HIGH); // Habilita la carga
}

void loop()
{
    //analogSetResolution(12); // 12 bits de resolucion (0-4095)
    v_sense = analogRead(PIN_ADC_SENSE);
    v_sense = v_sense * 5000 / 1024; // Conversion a mV
    if (v_sense < 1500)
    {
        colorWipe(strip.Color(0, 0, 255), 50); // Blue
    }
    else if (v_sense >= 1500 && v_sense < 2500)
    {
        colorWipe(strip.Color(0, 255, 0), 50); // Green
    }
    else if (v_sense >= 2500)
    {
        colorWipe(strip.Color(255, 0, 0), 50); // Red
    }

    /*
    if (digitalRead(PIN_STATUS) == HIGH)
    {

    colorWipe(strip.Color(255, 0, 0), 50);    // Red
    colorWipe(strip.Color(0, 255, 0), 50);    // Green
    colorWipe(strip.Color(0, 0, 255), 50);    // Blue
    colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
    }
    else
    {
        colorWipe(strip.Color(0, 0, 0), 50); // Apaga todos los leds.
    }
    */
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}
