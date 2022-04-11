/**
   @file buttons_buzzer.ino
   @author Javi (Javier@musotoku.com)
   @brief Integracion de los botones direccionales y del buzzer. Servira para sentar las bvases de las funciones que atenderan ambos perifericos.

   @version 0.1
   @date 2020-09-15

   @copyright Copyright (c) 2020

*/

//---------------- CONSTANTS---------------------//
const int16_t C_LP_CENTER = 0x01;
const int16_t C_LP_RIGHT = 0x02;
const int16_t C_LP_LEFT = 0x03;
const int16_t C_LP_UP = 0x04;
const int16_t C_LP_DOWN = 0x05;
const int16_t C_CLICK_CENTER = 0x06;
const int16_t C_CLICK_RIGTH = 0x07;
const int16_t C_CLICK_LEFT = 0x08;
const int16_t C_CLICK_UP = 0x09;
const int16_t C_CLICK_DOWN = 0x0A;
const int16_t C_NONE_EVENT = 0x0B;

const int16_t C_PIN_BUTT_UP = 7;
const int16_t C_PIN_BUZZER = 8;
const int16_t C_PIN_BUTT_DOWN = 9;
const int16_t C_PIN_BUTT_RIGHT = 10;
const int16_t C_PIN_BUTT_LEFT = 11;
const int16_t C_PIN_BUTT_CENTER = 6;

const int16_t C_TIMER_REBOUND = 20;
const int32_t C_TIMER_LONGPRESS_1 = 1000;

void setup()
{
  Serial.begin(9600);
  
  float voltage = 7.0;
  int16_t button_event = C_NONE_EVENT;
  int32_t period = 0;
  pinMode(C_PIN_BUZZER, OUTPUT);
  pinMode(C_PIN_BUTT_UP, INPUT);
  pinMode(C_PIN_BUTT_DOWN, INPUT);
  pinMode(C_PIN_BUTT_RIGHT, INPUT);
  pinMode(C_PIN_BUTT_LEFT, INPUT);
  pinMode(C_PIN_BUTT_CENTER, INPUT);
  digitalWrite(C_PIN_BUZZER, LOW);
  tone(C_PIN_BUZZER, 1000, 100); //milisegundos
  while (!Serial) ;
  Serial.print("Hola!");
  while (1)
  {
    button_event = ReadDirPad();

    if (button_event == C_CLICK_UP)
    {
      voltage += 0.1;
      voltage = constrain(voltage, 5.0, 16.0);
      //Serial.print("Voltage: ");
      Serial.println(voltage);
      bip();
      //tone(C_PIN_BUZZER, 1000, 100);
      //tone(C_PIN_BUZZER, 2000, 100);
      //Serial.println(button_event);
    }

    if (button_event == C_LP_UP)
    {
      voltage += 1;
      voltage = constrain(voltage, 5.0, 16.0);
      //Serial.print("Voltage: ");
      Serial.println(voltage);
      tone(C_PIN_BUZZER, 1000, 100);
      tone(C_PIN_BUZZER, 2000, 100);
      // Serial.println(button_event);
    }
    if (button_event == C_LP_DOWN)
    {
      voltage -= 1;
      voltage = constrain(voltage, 5.0, 16.0);
      //Serial.print("Voltage: ");
      Serial.println(voltage);
      tone(C_PIN_BUZZER, 2000, 100);
      tone(C_PIN_BUZZER, 1000, 100);
      // Serial.println(button_event);
    }

    if (button_event == C_CLICK_DOWN)
    {
      voltage -= 0.1;
      voltage = constrain(voltage, 5.0, 16.0);
      //Serial.print("Voltage: ");
      Serial.println(voltage);
      tone(C_PIN_BUZZER, 2000, 100);
      tone(C_PIN_BUZZER, 1000, 100);
      //Serial.println(button_event);
    }
  }
}

void loop()
{
}

int16_t ReadDirPad()
{
  int event = C_NONE_EVENT;
  int16_t cont_changes = 0;

  static bool up_pressed = false;
  static bool up_released = false;
  static bool down_pressed = false;
  static bool down_released = false;

  static bool up_fast_mode_flag = false;
  static bool down_fast_mode_flag = false;

  static int32_t up_rebound_timer = 0;
  static int32_t up_longpress_timer = 0;
  static int32_t down_rebound_timer = 0;
  static int32_t down_longpress_timer = 0;

  static bool up_butt_last_state = false;
  static bool up_butt_state = false;
  static bool down_butt_last_state = false;
  static bool down_butt_state = false;
  static bool left_butt_last_state = false;
  static bool left_butt_state = false;
  static bool right_butt_last_state = false;
  static bool right_butt_state = false;
  static bool center_butt_last_state = false;
  static bool center_butt_state = false;

  up_butt_state = digitalRead(C_PIN_BUTT_UP);
  down_butt_state = digitalRead(C_PIN_BUTT_DOWN);
  left_butt_state = digitalRead(C_PIN_BUTT_LEFT);
  right_butt_state = digitalRead(C_PIN_BUTT_RIGHT);
  center_butt_state = digitalRead(C_PIN_BUTT_CENTER);

  //  UP
  /* Falling Edge detection.*/
  if (up_butt_state == false && up_butt_last_state == true)
  {
    up_pressed = true;
    //Serial.print("fall");
  }
  if (up_pressed == true)
  {
    if ((millis() - up_rebound_timer) >= C_TIMER_REBOUND)
    {
      if ((millis() - up_longpress_timer) < C_TIMER_LONGPRESS_1)
      {
        /* Rising Edge detection.*/
        if (up_butt_state == true && up_butt_last_state == false)
        {
          up_released = true;
          up_rebound_timer = millis();
        }
      }
      else
      {
        // Serial.print("lp");
        up_fast_mode_flag = true;
        up_longpress_timer = millis();
        event = C_LP_UP;
        cont_changes += 1;
      }
    }
  }
  else
  {
    up_rebound_timer = millis();
    up_longpress_timer = millis();
  }
  if (up_released == true)
  {
    if ((millis() - up_rebound_timer) >= C_TIMER_REBOUND)
    {
      // Serial.print("rise");
      if (up_fast_mode_flag)
      {
        event = C_NONE_EVENT;
        up_fast_mode_flag = false;
      }
      else
      {
        event = C_CLICK_UP;
      }
      up_pressed = false;
      up_released = false;
      cont_changes += 1;
    }
  }

  //  DOWN
  if (down_butt_state == false && up_butt_last_state == true)
  {
    down_pressed = true;
    //Serial.print("fall");
  }
  if (down_pressed)
  {
    if ((millis() - down_rebound_timer) >= C_TIMER_REBOUND)
    {
      if ((millis() - down_longpress_timer) < C_TIMER_LONGPRESS_1)
      {
        if (down_butt_state == true && down_butt_last_state == false)
        {
          down_released = true;
          down_rebound_timer = millis();
        }
      }
      else
      {
        // Serial.print("lp");
        down_fast_mode_flag = true;
        down_longpress_timer = millis();
        event = C_LP_DOWN;
        cont_changes += 1;
      }
    }
  }
  else
  {
    down_rebound_timer = millis();
    down_longpress_timer = millis();
  }
  if (down_released)
  {
    if ((millis() - down_rebound_timer) >= C_TIMER_REBOUND)
    {
      // Serial.print("rise");
      if (down_fast_mode_flag)
      {
        event = C_NONE_EVENT;
        down_fast_mode_flag = false;
      }
      else
      {
        event = C_CLICK_DOWN;
      }
      down_pressed = false;
      down_released = false;
      cont_changes += 1;
    }
  }
  // RIGHT
  if (right_butt_state == true && right_butt_last_state == false)
  {
    event = C_CLICK_RIGTH;
    cont_changes += 1;
  }
  if (left_butt_state == true && left_butt_last_state == false)
  {
    event = C_CLICK_LEFT;
    cont_changes += 1;
  }
  if (center_butt_state == true && center_butt_last_state == false)
  {
    event = C_CLICK_CENTER;
    cont_changes += 1;
  }
  //Serial.println(cont_changes);

  center_butt_last_state = center_butt_state;
  left_butt_last_state = left_butt_state;
  right_butt_last_state = right_butt_state;
  up_butt_last_state = up_butt_state;
  down_butt_last_state = down_butt_state;

  if (cont_changes == 1)
  {
    return event;
  }
  else
  {
    return C_NONE_EVENT;
  }
}

void bip()
{
  for (int m = 0; m < 2; m++)
  {
    digitalWrite(C_PIN_BUZZER, HIGH);
    delay(1);
    digitalWrite(C_PIN_BUZZER, LOW);
    delay(1);
  }
}
