# DCDC

LA libreria del DCDC albergua la clase *dcdc_controler*. Esta clase permite inicializar un obejto para controlar el TPIC que sirve de decoder para fijar el voltaje de salida del DCDC controler.

## Contantes

* Direccion I2C (ADDR_I2C_DCDC) = 0

* BOOST MODE = True
  
* NO BOOST MODE = False

* BOOST ARRAY:_ Conjunto de valores del decodificador (TPIC) para dar una salida de 0.4v superior al valor demandado. Ej. Theorico: 4v -> Salida: 4.4v

* NO BOOST ARRAY: Conjunto de Valores de decodificador (TPIC) para dar una salida de igual valor que el valor demandado. Ej. Teorico: 4v -> Salida: 4v

## Metodos

* Constructor: dcdc_controler(pin enable).
  
* SetVoltage(voltaje, modo):

  * Modo Boost \<true> El voltaje de salida es 0.4 V mayor que el que se muestra.
  * Modo No Boost \<false> El voltaje de salida es exactamente el que se muestra.
  
* EnableDCDC (Bool): Activar o deasctiva el enable.

## TESTs

### Test 0

En este test se comprueba que el array de los comandos a decodificar por el TPIC se corresponden correctamente con los voltajes.

Para ello se utiliza la placa del makinator, donde aparte del DCDC y del TPIC, se enceuntran 3 pines de sensado: Corriente a la entrada ( I_batt ), Corriente a la salida (I_OUT) y  voltaje de salida (V_out). Por utilimos se utiliza la galga de corriente como metodo de validacion de esa corriente de entrada.

La salida de este test se trata de un conunto de graficas que muestran:

![Image Not Found](.Images/vtheryvsvreal.jpg "Grafica Voltaje Teorico vs Real")

![Image Not Found](.Images/corrientevsvreal.jpg "Grafica de la corriente en la entrada segun el voltaje de salida.")

![Image Not Found](.Images/Error.jpg "Error en el voltaje real sobre el Teorico")
