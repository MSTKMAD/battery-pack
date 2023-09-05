// Note: Uses pin 13 as the LED pin; may need changing for other boards

#include "MilliTimer.h"
#include <SAMD21turboPWM.h>
#include <Dpad.h>
#include <display.h>

TurboPWM pwm;
int16_t button_event;

int16_t duty = 500;
void setup()
{
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  pinMode(8, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pwm.setClockDivider(48, false); // Main clock divided by 200 => 240KHz
  pwm.timer(2, 1, 50, false);     // Use timer 2 for pin 13, divide clock by 4, resolution 60000, dual-slope PWM
  pwm.analogWrite(13, duty);      // PWM frequency is now 0.5Hz, dutycycle is 500 / 1000 * 100% = 50%
  pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
  pinMode(C_PIN_BUTT_UP, INPUT_PULLUP);
  pinMode(C_PIN_BUTT_DOWN, INPUT_PULLUP);
  initDisplay();
}

void loop()
{
  button_event = ReadDirPad();

  if (button_event == C_CLICK_DOWN)
  {
    duty -= 10;
  }
  else if (button_event == C_CLICK_UP)
  {
    duty += 10;
  }
  duty = constrain(duty, 50, 950);
  pwm.analogWrite(13, duty); // PWM frequency is now 0.5Hz, dutycycle is 500 / 1000 * 100% = 50%
  DisplayCap(duty/10);
}