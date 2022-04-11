# Heath Monitor

Esta libreria trata la clase *HelathMonitor* . Esta clase permita la monitorizacion o seguinto de una señal, permitiendo lanzar una alarma si  dicha señal se mantiene por encima de un umbral durante un determinado tiempo, mediante la implementacion del algotimo de *pediente de subida pendiente de bajada* .

## Metodos

* Constructor. ***HealthMonitor( Umbral, valor de incremento, valor de decremento, limite del contador)***

    Para la programacion de estos obetos es necesario tener en cuenta la siguiente formula de la que se puede desgranar cada uno de las variables. Para ello hay que definicir dos constantes previas:

  * Tiempo de muestreo.

  * Tiempo tras el que salta la alarma.

    Con ello podemos aplicar la siguiente formula:

        Limite del contador = valor de incremento * (Tiempo de alarma/Tiempo de muestreo)    
    Jugando con estos valores se obtiene el limite del contador.

* ***setCounter(valor)***: Cambia el contador a un valor deseado. Tamaño 16 bits.

* ***getCounter()***: Devuevle el valor del contador.

* ***Check( sample )***: Funcion principal que compar ala muestra con el umbral y aumenta o decrementa el contador en funcion de si lo sobrepasa o no. Ademas comparará el contador con el limite y devovera una alarma si este es superado.
  
* ***getSample( ADCpin )***: Devuelve una muestra obtenida del ADC pin indicado.

---

## Test

### Test 0

Este test inicializa un health monitor. El ciclo del programa se encargara de extraer una muestra del canal A1. Checkea la muestra y si salta la alrma se lanza un pulso de 10 ms por el pin A2. Paralelamente, por el puerto de salida analogica se esta muestreando el contador del health monitor.

* Para la creacion de la señal a muestrear, se utiliza la herramienta de LabView para la generacion de esta señal. El sketch mostrara un panel de control para modificar 2 señales senosoidales, 2 pulsos y el ruido. de menera que se imita la señal de la corriente, la frecuencia de la pulsacion deuna mano utilizando la maquina y los pulsos posibles errores de la señal  que deberian lanzar la alarma del health monitor.

![Alternate text](image/controlpanel-labview.jpg "Frecuencia del I2C en ARM")

Esta señal, se almacena en un excel. El cual se utilizara para la generacion de la señal con la yuda del Analog Discoery.

* WaveForm. Dentro del programa en la parte de WaveGen se selecciona Custom  e Import.

![Alternate text](image/waveform-wavegen.jpg "Frecuencia del I2C en ARM")

Seleccionando el excel previamente generado y configurando el separador y el simbolo decimal.

Una vez generada la señal y con las sonsas del ociloscopio colocadas correctamente:

![Alternate text](image/osciloscope.jpeg "Frecuencia del I2C en ARM")

Corroboramos el funcionamiento:

![Alternate text](image/waveform-scope.jpg "Frecuencia del I2C en ARM")

* Canal 1: Pin A0 ( Salida Analogica del ardunion con el "valor" del contador).

* Canal 2: Señal muestreada del wavegen.
  
* Canal digital 7: Pin A1 (Salida digital del arduino con el pulso de alarma.)
