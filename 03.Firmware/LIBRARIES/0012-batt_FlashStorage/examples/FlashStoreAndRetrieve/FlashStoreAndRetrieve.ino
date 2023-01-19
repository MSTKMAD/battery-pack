/*
  Store and retrieve an integer value in Flash memory.
  The value is increased each time the board is restarted.

  This example code is in the public domain.

  Written 30 Apr 2015 by Cristian Maglie 
*/

#include <batt_FlashStorage.h>

// Reserve a portion of flash memory to store an "int" variable
// and call it "my_flash_store".
typedef struct
{
  /* data */
  int arr[100];
} graf;
FlashStorage(num, int);
FlashStorage(num2, int);
FlashStorage(array_power, graf);

// Note: the area of flash memory reserved for the variable is
// lost every time the sketch is uploaded on the board.

void setup()
{
  Serial5.begin(9600);

  int number;
  int number2;
  graf power;

  // Read the content of "my_flash_store" and assign it to "number"
  number = num.read();
  number2 = num2.read();
  power = array_power.read();

  // Print the current number on the serial monitor
  Serial5.printf("%d,%d \n", number, number2);
  for (int i = 0; i < 100; i++)
  {
    Serial5.printf("%d ", power.arr[i]);
  }
  Serial5.println();

  // Save into "my_flash_store" the number increased by 1 for the
  // next run of the sketch
  num.write(number + 1);
  num2.write(number2 + 1);
  for (int i = 0; i < 100; i++)
  {
    power.arr[i] += 1;
  }
  array_power.write(power);
}

void loop()
{
  // Do nothing...
}
