
# Directional PAD

Se dispone de 3 botones dispuestos 2 de ellos debajo de la pantalla y un tercero en la parte superior de la bateria. La funcion de estos botones sera:

* Control del voltage de salida.
* Activacion y desactivacion de la salida de voltaje.
* Apagado y encendido de la bateria.

## Eventos posibles

* Click.
* Pulsacion larga.

## Funcionalidades implementadas

* Click boton superior.
* Click boton Inferior.
* Click boton central.
* Pulsacion larga boton superior.
* Pulsacion larga boton inferior.
* Pulsacion larga boton central.

## Condiciones

* Solo se permite la deteccion de un evento a la vez. Si se producen 2 o mas eventos al mismo tiempo, el resutlado sera como si no se hubiera producido ningun evento. [**Pendiente de Revision.**]

## Conflictos

* Durante una pulsacion larga se producen eventos en botones distintos al de la pulsacion larga. [**Pendiente de Revision.**]

# Tests

## Test 0

En el primer test se tratara de comprobar el correcto funcionamiento de todos los botones y su funcionalidades de Click y Pulsacion larga.

Para ello, se inicializara el puerto serie, a atraves del cual podremos observar y verificar si al realizar un click o una pulsacion larga, este evento es reconocido y muestreado con un mensajo con el correspondiente evento.

Adicionalmente, se mediraá el tiempo de computa de cada evento.

