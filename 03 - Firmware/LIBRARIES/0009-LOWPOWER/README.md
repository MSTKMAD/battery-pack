# Arduino LowPower

La libreria Arduino LowPower permite configurar nuestra CPU para que entre en modos de bajo consumo, ideal para el proyecto cuando la bateria se encuentre "apagada".

## Funciones

* **sleep()**:
* @brief Funcion principal de esta libreria. Comanda a la CPU a un estado de Stanby (Bajo consumo) hasta que una interrupcion configurada para ello, levante la levante/despierte.

* **attachInterruptWakeup()**:
* @brief Activamos la posibilidad de disparo de una interrupcion hardward en un determinado pin ante una determianda condicion (Rise Edge, Fall Edge, High state, Low State).
* @param pin
* @param callback
* @param mode

## TESTs

* ExternalWakeUp.

  This sketch demonstrates the usage of External Interrupts (on pins) to wakeup a chip in sleep mode.
  Sleep modes allow a significant drop in the power usage of a board while it does nothing waiting for an event to happen. Battery powered application can take advantage of these modes to enhance battery life significantly.

  In this sketch, shorting pin 8 to a GND will wake up the board.
  Please note that, if the processor is sleeping, a new sketch can't be uploaded. To overcome this, manually reset the board (usually with a single or double tap to the RESET button)

  This example code is in the public domain.
