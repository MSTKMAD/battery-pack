# Power Bar

Esta libreria contiene las funciones y cosntantes encesarias para controlar la ultima fila de leds de matriz de leds de 15x7, con el funcion de reproducir el funcionamiento de una barra de potencia. Esta barra iluminara mas leds o menos en funcion de la potencia que se este suminsitrando por parte de la bateria a la maquina.

## Funciones

* UpdatePowerBar()
* @brief Actualiza la cantidad de leds encendidos en la barra de potencia segun el parametro de entrada "power_sample"
  
* @param power_sample
* @param ledmatrix
* @param mode_bright_display

## TESTs

* Test 0

Test para probar la actualizacion de la barra de potencia. Oscila los valores entre el maximo y el minimo de potencia que puede mostrar la barra.
