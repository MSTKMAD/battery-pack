# BUZZER

Buzzer o Zumbador que permite realizar breves sonidos/effectos. el control de este componente se ralizara con la libreria de arduino tone.

Esta libreria utiliza los timers itnernos de la CPU para generar una señal PWM a traves del Pin seleccionado. Esta señal se puede modular en frecuencia (generando las distintas notas) y duracion.

    tone(PIN,FREQ,[DURACTION])

Para generar una melodia o sonido, es necesario controlar el tiempo que dura la señal, ya que esta sentencia no es bloqueante y por lo tanto si se ejecuta otra sentencia de tono sobre el mismo pin se sobreescribiría y se cortaría la señal previa.
Lo normal es la utilizacion de delay(), ya que se suelen reprducir de manera continuada las melodias. En este caso se utilizara la libreria millitemr y poder relaizar otras tareas mineta se reproducen los sonidos.

## Funciones

* **playSound**(int): Esta funcion reproduce el sonido indicado por el argumento.
* **getSound**(array,int):Funcion que selecciona que array de notas se devuelve para tocar.(Brazo del tocaiscos).
* **InitBuzzer**(mode): Inicializacion del zumbador y seleccion de las pistas correspondientes para cada sonido.

## TEST

* Test 0
  
Este test prueba todos los sonidos. por defecto.

El siguiente diagrama ejemplifica como se hace la reproduccion de sonidos de manera no blqueante:

!["IMAGE NOT FOUND"](Images/Buzzer.jpg)
