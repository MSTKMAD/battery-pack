/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>
#include <HealthMonitor.h>
#include "MilliTimer.h"

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 100;            // variable to store the servo position
String inputString = ""; // a String to hold incoming data
bool stringComplete = false;
char vectorChar[10];
HealthMonitor current(1000, 10, 10, 200);
const uint16_t limit_inferior = 90;

MilliTimer timer_rate;
int32_t t0 = 0, t1 = 0;
uint16_t sample_I = 0;

bool direction = true;

void setup()
{
  Serial.begin(9600);
  inputString.reserve(200);
  while (!Serial)
  {
  }

  myservo.attach(9); // attaches the servo on pin 9 to the servo object
  Serial.println("SEND POSITION FOR THE SERVO IN DEGREES");
}

void loop()
{
  t0 = micros();
  // while (Serial.available())
  // {
  //   // get the new byte:
  //   char inChar = (char)Serial.read();
  //   // add it to the inputString:
  //   inputString += inChar;
  //   if (inChar == '\n')
  //   {
  //     stringComplete = true;
  //   }
  // }
  // if (stringComplete)
  // {
  //   inputString.toCharArray(vectorChar, 10);
  //   pos = atoi(vectorChar);
  // if (timer_rate.poll(10) != C_TIMER_NOT_EXPIRED)
  // {

  //   if (pos == 180)
  //   {
  //     direction = false;
  //   }
  //   else if (pos == limit_inferior)
  //   {
  //     direction = true;
  //   }

  //   if (direction == true)
  //   {
  //     pos += 1;
  //   }
  //   else
  //   {
  //     pos -= 1;
  //   }

  //   Serial.print(pos);
  // }

  myservo.write(pos);
  for (int i = 180; i > limit_inferior; i--)
  {
    myservo.write(i);
    
    delay(20);
  }
  for (int i = 0; i < 5; i++)
  {
    for (int i = 0; i < 360; i += 5)
    {
      pos = 3 * abs(sin(radians(i))) + limit_inferior;
      myservo.write(pos);
      
      delay(20);
    }
  }
  for (int i = limit_inferior; i < 150; i++)
  {
    myservo.write(i);
    
    delay(10);
  }
  for (int i = 150; i > limit_inferior; i--)
  {
    myservo.write(i);
    
    delay(20);
  }
  for (int i = 0; i < 5; i++)
  {
    for (int i = 0; i < 360; i += 5)
    {
      pos = 3 * abs(sin(radians(i))) + limit_inferior;
      myservo.write(pos);
      
      delay(20);
    }
  }
  for (int i = limit_inferior; i < 180; i++)
  {
    myservo.write(i);
    
    delay(20);
  }

  myservo.write(pos); // tell servo to go to position in variable 'pos'
  // clear the string:
  //inputString = "";
  //stringComplete = false;
  // }
  //sample_I = current.getSample(A1);
  t1 = micros();

  // Serial.print(";");
  //Serial.println(sample_I);
  // while (t1 - t0 >= 10000) // Control del Periodo del ciclo de ejecucion (10ms)
  // {
  //   t1 = micros();
  // };
}