
# DISPLAY LED

Diplay led de tamaño 15x7 leds blancos. Este display se "partira" en dos, de manerra que se quede una pantalla de 15x6 que se utilizara para la interfaz de la bateria con el uso de caracteres ASCII, y ademas quedara una "barra" de leds de 15x1 que se usara como medidor visual del consumo e potencia isntantanea de la bateria.

![Alternate text](images/I2C_frecuency.jpg "Frecuencia del I2C en ARM")

## Funciones Privadas

* **Char2Bitmap**(char character, uint8_t *bitmap, int16_t array_len):
  * @brief: Esta funcion recoge un caracter y devuelve su bitmap correspondiente.
  * @param: character.
  * @param: *bitmap.
  * @param: array_len.

## Funciones Publicas

* **SwitchScreenOff**
  * @brief: Apagado total de la pantalla.

---

* **DisplayText**(<String\>,reset, brillo):
  * @brief: Esta funcion muestra en el display una cadena de caracteres en un scroll lateral der-izq.No permite la introduccion de un cadena nueva hasta que no se haya terminado de imprimir la previa.
  * @param: cadena.
  * @param: reset. Permite reiniciar el muestreado por la pantalla.
  * @param: brillo. Ajusta el brillo de los leds.
  * @return: bool.
    * True: Si esta listo para imprimir una nueva cadena.
    * FalsE: Si se enceuntra ocupado en la impresion de una cadena.

    **Tiempo de ejecucion:**

    * Not_Busy: ~ 80us.
    * Bussy en espera: ~ 30us;
    * Bussy escribiendo: ~ 25ms.

---

* **DisplayVolt**(<integer\>,brillo):
  * @brief: Esta funcion imprime la pantalla de voltaje, que muestra le voltaje al que se encuentra la salida de la bateria, con un decimal.
  * @param: Volts.
  * @param: Brillo. Permite ajustar el brillo de la pantalla.

    **Tiempo de ejecucion:** ~20ms.

---

* **PowerBar**(<integer\>, brillo):
  * @brief: Esta funcion se encarga de encender y apagar los leds necesarios en la barra de potencia.
  * @param: Leds.
  * @param: Brillo.Permite ajustar el brillo de los leds.

    **Tiempo de ejecucion:** ~2,6ms.

---

* **DisplayCap**(<integer\>,brillo):
  * @brief: Esta fncion muestra la pantalla de porcentaje de capacidad de bateria. si la capacidad es 100% muestra la pantalla FULL.
  * @param: capacity.
  * @param: Brillo. Permite ajustar el brillo de los leds.

    **Tiempo de ejecucion:** ~20ms.

---

* **DisplayBattCharging**(<integer\>):
  * @brief: Esta funcion muestra el marco de una bateria, se encarga de ir rellenando de manera solida la capacidad recibida, y deja parpadeando la que seria el proximo 10% que se encuentra cargando.
  * @param: Capacity.

    **Tiempo de ejecucion:** 7-16 ms.
  
* **LedWork**(state,brillo):
  * @brief  Funcion que permite encender y apagar el boton que representa el estado de la salida del voltage.

  * @param state_led
  * @param mode_bright

* **DisplayUsbIn**:
  * @brief Animacion de Entrada del USB. A los 2 segundos disminuye su brillo.

  * @param ledmatrix

* **DisplayUsbOut**:
  * @brief Anuimacion de salida del USB.

  * @param ledmatrix

* **Display Diagnostic Mode** ():
  * @brief: Animacion del modo diagnostico.

## TESTs

### TEST 0

Testeo de la funcion DisplayTest. PAra ponerla a prueba se lanzara funcion con distintas cadenas de texto. El resultado debe ser la impresion de cad cadena de manera consecutiva sin ningun tipo de solapamiento.

### TEST 1

Test de la funcion DisplayVolts. Este test imprime todos los posibles valores de voltaje de manera ascendente y despues de manera descendente.

### Test 2
  
Este test simula el funcionamiento de la barra de potencia creando un movimeinto senosoidal de manera ciclica.

### Test 3

Este test pone a prueba la compatibilidad y convivencia de la barra de potencia y el display de informacion, en este caso mostrando cambios de voltaje.

### Test 4

Test de la funcion DisplayCap. Este test imprime toda la capacidad posible de la bateria ( 0-100 ).
Ascendente y descendente, de manera ciclica.

### Test 5

Test de la funcion DisplayBattCharging. Este test muestra la evolucion de la carga de la bateria, desde el 0% al 100%.

### Test 6

Testeo de la funcion DisplayTest mostrando todos los caracteres imprimibles posibles.

### Test 7

Testeo del boton que marca el estado de la salida de voltage que forma parte de la pantalla. 

### Test 8

Test que prueba las animaciones de entrada y salida de un USB. En el caso de la entrada pasados 2 segundos el brillo disminuye.
