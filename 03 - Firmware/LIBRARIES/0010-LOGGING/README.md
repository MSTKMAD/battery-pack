# Logging Tech Note

## Arquitectura de la memoria del SAMD21G18

La memoria de un MCU SAM D21 está mapeada en cuatro secciones:

* Memoria Flash

* Memoria Flash con capacidad de lectura y escritura (RWW)

* RAM interna

* Periféricos y puertos de E/S

!["IMAGE NOT FOUND"](.Images/arq-samd21.jpg)

En nuestro caso usaremos el MCU SAMD21G18. Por lo tento tendremos una memoria de 256Kbytes.

La memoria de programa esta dividad en colummnas (*rows*) y paginas (*pages*). Cada columna contiene 4 paginas.

El tamaño de estas paginas estan predefinidas a 64 bytes, a un tamaño de 32 bits (4 bytes) por palabra, tenemos que en cada pagina hay 16 variables.

!["IMAGE NOT FOUND"](.Images/page.form.jpg)

DEbido a este formato reducimos las variables utiles a 15.

Deebido a la falta de una seccion de memoria no volatil para el ussuaria, es posible configurar una zona de emulacion de memoria EEPROM al final del stack de la memoria flash.

Este espacio puede ser de los siguientes tamaños:

!["IMAGE NOT FOUND"](.Images/eepromsizes.jpg)

En nuestro caso haremos uso de una memoeria emualda de 256 bytes. Lo que se traduce en 1 columna de 4 paginas, un total de 4 x 64 bytes = 256 bytes /4 bytes per word - 1 word per header per word = 60 variables.

Estas variables se alm,acenara en la parte no volatil de la memoria. Sin embargo esta memoria tiene una vida util limitada, por lo que es necesario que se controle muy bien el momento de la escritura.

### Memoria Externa I2C

El acceso a la memoria externa situada en la bateria, se realiza por I2C utilizando la tecnica de BitBang debido a la comparticion de los canales de I2C con el puerto USB y el pulsador. Por esta razon es aconsajable separar la comunicacion de los puertos por defecto.

## Funciones Privadas

* **Init_diagnostic_elements**()
* @brief Inicializacion del valor de los elementos de diagnostico. Lectura y comprobacion de las memorias EEPROM, tanto la del chasis como la de la bateria.
  Cada elemento posee el valor, el nombre y la categoria:
  * Serial Port (X o S). Con X no se muestra con S si.
  * Guardado en EEPROM (X,B,C). X no se guarda, B se guarda en la Bateria y C se guarda en el Chasis.
  ---
* **LogDiagnosticData** ( <integer\> , <integer\>)
  * @brief Actualizar el valor en Ram de una variable de adiagnostico
  * @param data Valor del dato.
  * @param address Direccion en la memoria RAM de la variable.
  ---
* **ReadDiagnoticData(Address)**
  * @brief: Lectura del valor de un elem,ento de diagnostico.
  * @param: Address. Direccion de almacenamiento en el array de elementos en RAM.
  * @return:  valor del elemento de diagnostico.
  ---
* **IncrementDiagnosticdata** (<integer\>,<integer\>)
  * @brief: Incremente en una cantidad el valor de un dato de diagnostico.
  * @param: Incremento, address.
  ---
* **PrintDiagnosticData** (<integer\>)
  * @brief: Muestrea por el puerto serie o guarda en EEPROM los datos de diagnostico.
  * @param: type_print.
  ---
* **PrintStaticData**
  * @brief: Impresion por el puerto serie de los datos estaticos.
  ---
* **isValid**()
  * @brief: Devuelve un boleano indicando si la memoria eemprom contiene datos o esta vacia.
  * @return: bool
    * True: La memoria contiene valores.
    * False: La memoria esta vacia.
  ---
* **Stats**(int,int)
  * @brief: Incremento de las estadisticas del valor de voltaje y potencia usado. Se incremeneta en 1 el valor de la posicion de un array correspondiente. Ej: 12.3 v -> voltages_values[ 123 ] ++
  * @param: volts
  * @param: wats
  ---
* **UpdateEeepromBatery**():
  * @brief:  Actualizacion de la memoria EEPROM de la Bateria. Antes de relaizar el guardado se comprueba que el valor a guardar sea distinto del valor ya almacenado.
  ---
* **PrintStats**():
  * @brief: Muestreo por el puerto serie de los valores acumulados de las estadisticas de potencia y voltage.Ademas de algunos datos estaticos guardados en eeprom como el numero de serio o el modelo.
  ---
* **ResetBateriaEeprom**():
  * @brief: Puesta a 0 de las primeras 1024 posiciones de la EEPROM de la Bateria.

## TESTs

### TEST_0

Testeo de las funciones de LogDiagnosticData, PrintHeader and PrintDiagnosticData (Serial Port).

### TEST_1

Testeo de la funcionalidad de guardado en EEMPROM. Prueba de las funciones isValid(), Incremeent DiagnosticData.

### TEST_2

Test del guardado y lectura de la eeprom chasis. Para ello se utiliza y prueba la funcion de estadisticas.

### TEST_3

Test que permite medir la velocidad de escritura de 1, 10 y 20 bytes, a traves de la libreria de BitBang I2c.
