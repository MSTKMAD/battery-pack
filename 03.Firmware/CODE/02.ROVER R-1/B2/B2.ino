/**
 * @file B2.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 *      V1: Fork de la version B1_V11-RC1.  
 * 
 * @version V1
 * @date 2022-11-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#define INTEGRATED_VERSION 1

#define MAX_VOLTAGE 120
#define MIN_VOLTAGE 50
//============================================================== PINES ===========================================================//
const uint16_t C_PIN_ENABLE_LDO_VCC_2 = 1; // Enable del LDO de la alimentacion de VCC_2
const uint16_t C_PIN_OP_SWITCH = 13;       // Señal que activa/desactiva el transistor de salida en la placa DCDC. HIHG = ON, LOW = OFF
const uint16_t C_PIN_EN_DCDC = 11;         // Enable del DCDC de la placa DCDC. HIGH = OFF, LOW = ON
const uint16_t C_PIN_I_OUT = A1;           // Lectura de la tension correspondiente a la medida de corriente de salida.
const uint16_t C_PIN_V_OUT = A4;           // Lectura de la tension correspodiente a la tension de salida del DCDC.
const uint16_t C_PIN_V_IN = A5;            // Lectura de la tension correspondiente a la tension de entrada (la bateria)

//============================================================== INCLUDES ===========================================================//
#include <MilliTimer.h>
#include <DCDC.h>
#include <HealthMonitor.h>
#include <display.h>
#include <Dpad.h>
#include <Buzzer.h>
#include <diagnostic.h>
#include <power_bar.h>
#include <batt_ArduinoLowPower.h>
#include <NAMING.h>

//============================================================== CONSTANTS ===========================================================//

const uint32_t C_PROGRAM_CYCLE_PERIOD = 5000; // Duracion minima del ciclo de programa en us.

const bool C_MASK_DEACTIVATE = false; // Flag que indica que la mascara de las portecciones esta desactivada.
const bool C_MASK_ACTIVATE = true;    // Flag que indica que la mascara de las portecciones esta desactivada.

const uint16_t MAX_NUM_PROTECTION_EVENTS = 6; // Numero de vecces que tiene que saltar una proteccion para dar el aviso por pantalla.

const bool C_OUTPUT_OFF = false; // Flag que identeifica la salida como desconectada
const bool C_OUTPUT_ON = true;   // Flag que identifica la salida como conectada

const int16_t C_TIMER_ARMED = 0; // Identificador de un Timer como armado y contando.
const int16_t C_TIMER_IDLE = 1;  // Identifiador de un Timer esperando a ser iniciado.
const int16_t C_TIMER_DONE = 2;  // Identificador de un Timer que ha finalizado la cuenta

const int16_t C_SW_ST_RUN = 0x00;      // Esto Software que identifica el sistema encendido y con la salida activada.
const int16_t C_SW_ST_SLEEP = 0x01;    // Estado Software que identifica el sistema "apagado" en estado de bajo consumo.
const int16_t C_SW_ST_START_UP = 0x02; // Estado Software que identifica el sistema durante los protocolos de encendido.
const int16_t C_SW_ST_ERROR = 0x03;    // Estado Softawre que identifica a el sistema durante la atencion a un error.
const int16_t C_SW_ST_STOP = 0x04;     // Estado Software que identifica a el sistema encendido y con la salida desactivada.

const int16_t C_LIMIT_COMSUPTION_PROT = 1000;   // 1A  Limite de la proteccion de consumo.
const int16_t C_LIMIT_UNDERVOLTAGE_PROT = 1000; // -1V a la tension de salida. Diferencial de tension de la proteccion de Undervoltage de la tension de salida
const int16_t C_LIMIT_OVERPOWER_PROT = 5000;    // 5W de potencia a la sealida. Limite de la proteccion de sobre potencia.
const int16_t C_LIMIT_SHORTCIRCUIT_PROT = 3000; // 3A. Limite de la porteccion de cortocircuito.
const int16_t C_RETRY_750_COUNT = 750;

const int32_t C_TIME_IDLE_30_SEG = 30000;     // 30 s. Contador auxiliar para poder contar tiempos por encima del min.
const int32_t C_TIME_INIT_SCREEN = 1000;      // 1 s. Tiempo durante el que se muestra la pantalla de inicio al encender.
const int32_t C_TIME_DIAGNOSTIC_CHECK = 5000; // 5 s. Ventana de tiempo que tiene que mantenerse la condicion de inicio del modo diagnostico.

const uint8_t C_LOW_BATTERY_LEVEL = 15;  // Porcentaje de bateria  partir del cual no se puede asegurar un correcto funcionamiento.
const uint8_t C_EMPTY_BATTERY_LEVEL = 5; // Porcentaje de bateria  partir del cual no se permite encender la bateria.

const uint16_t C_IDLE_TIMER_COUNT = 15 * 2; // 15 Minutos para que se apage la bateria por inactividad.
                                            // NOTE: El contador es de 30seg, por eso se multiplica por 2-

const uint16_t C_EEPROM_UPDATE_TIME = 15; //min

//-------------------------------- Protections IDs -------------------------------------
const uint16_t C_ERROR_OVERCURRENT = 0x10;
const uint16_t C_ERROR_OVERPOWER = 0x11;
const uint16_t C_ERROR_OUPUT_UNDERVOLTAGE = 0x12;
const uint16_t C_ERROR_INPUT_UNDERVOLTAGE = 0x13;
const uint16_t C_ERROR_SHORTCIRCUIT = 0x14;

//--------------------------------- Configuracion UI E/S --------------------------------------
const bool C_STARTING_TEXT = true;
const bool C_STARTING_SOUND = false;
const bool C_ENDING_TEXT = true;
const bool C_ENDING_SOUND = false;

//============================================================== VARIABLES ===========================================================//
/**
 * @brief HealthMonitor de la corriente de salida para el modo boost.
 *      - Umbral
 *      - Ts = 10 ms.
 *      - T boost = 100ms.
 *      - T no boost = 1s
 * 
 */
HealthMonitor boost_check(50, 10, 1, 100);
/**
 * @brief HealthMonitor del consumo de salida de la bateria.
 *      - Umbral : 1000 mA.
 *      - Ts = 10 ms
 *      - Time spam = 1500ms
 * 
 */
HealthMonitor over_consumption_protection(C_LIMIT_COMSUPTION_PROT, 10, 10, 1500);

/**
 * @brief HealthMonitor de la potencia de salida.
 *      - Umbral : 5000 mW. (mA x mV / 1000)
 *      - Ts = 10 ms
 *      - Time spam = 2000 ms
 * 
 */
HealthMonitor over_power_protection(C_LIMIT_OVERPOWER_PROT, 10, 10, 2000);

/**
 * @brief HealthMonitor del theory_Vout de salida.
 *      - Umbral : 1000 mV
 *      - Ts = 10 ms
 *      - Time spam = 1000 ms
 * 
 */
HealthMonitor under_voltage_protection(C_LIMIT_UNDERVOLTAGE_PROT, 10, 10, 1000);
/**
 * @brief HealthMonitor del short_circuit de salida.
 *      - Umbral : 3000 mA
 *      - Ts = 10 ms
 *      - Time spam = 500 ms
 * 
 */
HealthMonitor short_current_protection(C_LIMIT_SHORTCIRCUIT_PROT, 10, 10, 500);

//--------------------------------------- Timers variables-------------------------------------
MilliTimer timer_encendido;

dcdc_controler DCDC(C_PIN_EN_DCDC);
MilliTimer protection_event_delay;  // Delay impuesto entre detecciones de erroes.
MilliTimer timer_sec_count;         // Timer contador de segundos.
MilliTimer timer_display_capacity;  // Timer para el muestreo de la capacidad en el arranque.
MilliTimer timer_recover_voltage;   // Timer que controla el tiempo que se deja para que la bateria se recupere antes de realizar una medicion de capacidad.
MilliTimer timer_wait_sleep;        // Timer que controla el tiempo entre los checkeos del boton central durante el protocolo de apagado.
MilliTimer timer_irq_button_center; // Timer que controla el tiempo tras el cual, si no hay eventos que saquen al sistema de SLEEP, el sistema vuelve al estado de bajo consumo.
MilliTimer timer_idle;              // Timer que controla el tiempo de inactividad sobre el sistema.
MilliTimer timer_display_error;     // Timer que controla el tiempo que se muestra por pantalla el aviso de error.
MilliTimer timer_init_screen;       // Timer que controla el tiempo que se muestra el logo durante el encendido.
MilliTimer timer_end_screen;        // Timer que controla el tiempo que se muestra la pantalla de apagado.
MilliTimer timer_refresh_screen;    // Timer que control el tiempo tras el cual se refresca la infomracion de la pantalla durante el estadado de Run y Stop.
MilliTimer timer_log_sec;           // Timer que cuenta hasta un seg para el logeo temporal.
MilliTimer timer_log_active;        // Timer que controla el intervalo de tiempo que tiene que pasar el sistema en el estado de RUN para realizar un log de ciertas variables de estado.
MilliTimer timer_check_diagnostic;  // Timer que controla la ventana de tiempo durante la que se comprueba los eventos necesarios para entrar en modo diagnostico.
MilliTimer timer_waiting_naming;    // Timer que controla la ventana de tiempo durante la que se comprueba los eventos necesarios para entrar en la configuracion del naming.
MilliTimer timer_active_naming;     // Timer que controla el tiempo que tiene que manterse activo los eventos necesarios para entrar en la configuracion del naming.
MilliTimer timer_blink_error;       // Periodo del parpadeo de la barra de potencia durante el aviso de un error.
MilliTimer timer_test_op_switch;    // Timer que durante el modo testeo invierte la señal de op switch.
MilliTimer timer_test_en_dcdc;      // Timer que durante el modo testeo invierte la señal de en dcdc.
MilliTimer timer_test_dac;          // Timer que durante el modo testeo invierte la señal de en dac.

//--------------------------------------- States variables-------------------------------------
int16_t sw_status = C_SW_ST_SLEEP;                                                   // Identificador del estado del sistema
bool sw_output = C_OUTPUT_OFF, hw_output = C_OUTPUT_OFF, user_output = C_OUTPUT_OFF; // Identificadores del estado de la salida del sistema.
bool output_mode = C_NON_BOOST_MODE;                                                 // Contenedor del modo de la tension de salida.
bool blink_error_state = false;                                                      // Identificador del estado del parapadeo de la barra de potencia durante el mostrado de un error.
bool mask_protection_state = C_MASK_DEACTIVATE;                                      // Identificador del estado de la mascara de las protecciones.
bool display_error_status = C_DISPLAY_ST_NOT_BUSSY;                                  // Identificador de si se esta mostrando el aviso de error por la pantalla.

//--------------------------------------- Counters variables-------------------------------------
int32_t cont_sec_log = 0;                          // Contador de los segundos en el intervalo del logeo de la EEPROM.
int16_t consmptn_event_protection_counter = 0;     // Contador del numero de eventos de error de consumo pre aviso por pantalla.
int16_t shortcircuit_event_protection_counter = 0; // Contador del numero de eventos de error de cortocircuito pre aviso por pantalla.
int16_t OP_event_protection_counter = 0;           // Contador del numero de eventos de error de sobre portencia (Over Power) pre aviso por pantalla.
int16_t UV_event_protection_counter = 0;           // Contador del numero de eventos de error de Under Voltage pre aviso por pantalla.
uint16_t long_press_events = 0;                    // Contador del numero de longpress consectivos.
int16_t cont_idle_timer = 0;                       // Contador de las veces que el timer de inactividad de 30 seg ha saltado.
uint32_t cont_log_sec = 0;                         // Contador de los segundos que lleva el sistema actvio fura del modo bajo consumo. (Reset con el cambio de pila)
uint32_t cont_log_active = 0;                      // Contador de los segundos que el sistema lleva en el estado RUN. Utilizado para el intervalo de tiempo entre logeos.

//--------------------------------------- Diagnostics variables-------------------------------------
int16_t sound = C_SOUND_MUTE;        // Contenedor de la ID de un sonido.
int16_t button_event = C_NONE_EVENT; // Contenedor del evento producido en la botonera.
uint16_t theory_Vout = 50;           // Contenedor del valor de la tension de salida fijado por el sistema.
uint16_t sample_IOut = 0;            // Muestra ponderizada la corriente de salida.
uint16_t sample_VOut = 0;            // Muestra ponderizada de la tension de salida.
uint16_t sample_POut = 0;            // Muestra de la potencia instantanea en la salida.
uint16_t sample_VIN = 0;             // Muestra de la tension de entrada-
uint16_t capacity = 0;               // Capacidad de la bateria
uint16_t error;                      // Contenedor de la ID del error que ha saltado.
bool test_op_switch = false;         // Variable del modod test que almacena el valor de opswitch
bool test_en_dcdc = false;           // Variable del modod test que almacena el valor de endcdc.
bool test_dac = false;               // Variable del modod test que almacena el valor del dac.

//-------------------------------------- FLAGS--------------------------------------------
bool flag_active_confirmation_question = false; // Flag que marca el estado de la pregunta de confirmacion de la entrada al modo diagnostico
bool flag_eeprom_init_fail = false;             // Flag que indica si se ha producido un fallo durante la inicializacion de la EEPROM

bool flag_low_battery = false;   //Flag que indica si la capacidad se encuentra por debajo del umbral de bateria baja.
bool flag_empty_battery = false; // Flag que indica si la capacidad se encuentra por debajo del umbral de bateria vacia.

bool flag_protection_event_delay = false; // Flag que indica que se tiene que realizar un delay por la deteccion de un evento de las proteccion.
volatile bool flag_error = false;         // Flag que indica la deteccion de un error.

bool flag_msg_init = false;              // Flag que indica si se ha terminado de mostrar el mensaje de encendido en la pantalla durante el protocolo de encendido.
bool flag_msg_sleep = false;             // Flag que indica si se ha terminado de mostrar el mensaje de apagado en la pantalla durante el protocolo de apagado.
bool flag_sound_init = false;            // Flag que indica si se ha terminado de sonar el sonido de encendido en la pantalla durante el protocolo de encendido.
bool flag_sound_end = false;             // Flag que indica si se ha terminado de sonar el sonido de apagado en la pantalla durante el protocolo de apagado.
bool flag_display_capacity_init = false; // Flag que indica si se esta mostrando la capacidad en la pantalla durante el protocolo de encendido.

bool trigger_Display_volt = true; // Flag que indica si se tiene que actualizar la pantalla con la informacion del voltaje.

bool flag_first_sleep = true; // Flag que indica si es la primera vez que el sistema se va al estado SLEEP.
                              // NOTE: Esto ocurre tras la insercion de la bateria.

volatile bool flag_irq_center_button = false; // Flag que indica la deteccion de una interrupcion por el boton central.

bool flag_sleep = false;               // Flag que marca el cambio al estado de SLEEP
bool go_deep_sleep = true;             // Flag que marca el inicio de la rutina para entrar en modo bajo consumo.
bool flag_initialize = false;          // Flag que marca el cambio al estado de START_UP
bool flag_init2stop = false;           // Flag que marca el cambio de estado de START_UP a STOP
bool flag_center_button_press = false; // Flag que indica si el boton central se encuentra presionado.
bool arrancado = false;                // Flag que indica si la secuencia de arrancado a finalizado.
bool flag_enable_off = false;          // Flag que indica la posibilidad de iniciar el protocolo de apagado.

uint16_t flag_waiting = C_TIMER_IDLE;          // Flag que indica el estado del timer "timer_wait_sleep"
uint16_t flag_diagnostic_timer = C_TIMER_IDLE; // Flag que indica el estado del timer "timer_check_diagnostic"
int16_t flag_timer_naming = C_TIMER_IDLE;      // Flag que indica el estado del timer "timer_active_naming"

bool flag_enable_diagnostic = false; // Flag que indica si es posible entrar el modo diagnostico.
                                     // NOTE: Esto se hace para evitar que se pueda acceder al modo diagnostico nada mas insertar la bateria.

bool flag_diagnostic_active = false; // Flag que indica si el modo diagnostico esta activo.
bool flag_waiting_naming = true;     //Flag que indica el estado de la ventana de tiempo para detectar la activacion de la entrada a la configuracion del naming.
bool flag_naming_active = false;     // Flag que indica si la configuracion del naming esta activa.

bool test_mode_activate = false;
//-------------------------------------- PROFILING --------------------------------------------
uint32_t t1; // Variables auxiliares para la medidcion de tiempos dentro del flujo del sistema.
uint32_t t2;

/*===============================================================================================================================================*/
//                                                                 SET UP
/*===============================================================================================================================================*/
void setup()
{
    Serial5.begin(57600);
    Serial5.println("START!\n\r");
    //------------------------- INIT PIN-------------------------------
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_UP, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_DOWN, INPUT_PULLUP);
    pinMode(C_PIN_OP_SWITCH, OUTPUT);
    pinMode(C_PIN_ENABLE_LDO_VCC_2, OUTPUT);

    //------------------------ INITIALITATION PERIFERICOS ----------------------------
    Wire.begin();

    digitalWrite(C_PIN_OP_SWITCH, HIGH);        // Interruptor Salida
    digitalWrite(C_PIN_ENABLE_LDO_VCC_2, HIGH); // Encendido del DCDC
    InitBuzzer(C_MODE_DEFAULT);                 // Inicializacion del Buzzer
    initDisplay();                              // Inicializacion de la pantalla
    if (!Init_local_eeprom())                   // Incializacion EEPROM
    {
        flag_eeprom_init_fail = true;
        Serial5.println("Fallo de lectura de EEPROM");
    }
    else
    {
        Serial5.println("Lectura Correcta de EEPROM");
    }

    if (local_eeprom.test_mode == true)
    {
        test_mode_activate = true;
        while (test_mode_activate)
        {
            sample_IOut = boost_check.getSample(C_PIN_I_OUT) * 3000 / 4096 * 10 / 15;               // Lectura de la Corriente de Salida
            sample_VOut = under_voltage_protection.getSample(C_PIN_V_OUT) * 208 / 39 * 3000 / 4096; // Lectura del Voltaje de salida

            OLED_display.clearDisplay();
            OLED_display.setTextSize(1);
            OLED_display.setCursor(0, 0);
            OLED_display.printf("I %d", sample_IOut);
            OLED_display.setCursor(0, 16);
            OLED_display.printf("V %d", sample_VOut);
            OLED_display.display();

            if (timer_test_op_switch.poll(300) != C_TIMER_NOT_EXPIRED)
            {
                test_op_switch = !test_op_switch;
                digitalWrite(C_PIN_OP_SWITCH, test_op_switch);
            }

            if (timer_test_en_dcdc.poll(200) != C_TIMER_NOT_EXPIRED)
            {
                test_en_dcdc = !test_en_dcdc;
                digitalWrite(C_PIN_EN_DCDC, test_en_dcdc);
            }

            if (timer_test_dac.poll(500) != C_TIMER_NOT_EXPIRED)
            {
                if (test_dac == true)
                {
                    DCDC.SetVoltage(60, C_NON_BOOST_MODE);
                    test_dac = false;
                }
                else
                {
                    DCDC.SetVoltage(120, C_NON_BOOST_MODE);
                    test_dac = true;
                }
            }

            if (digitalRead(C_PIN_BUTT_CENTER) == button_pressed)
            {
                local_eeprom.test_mode = false;
                test_mode_activate = false;
            }
        }
        SaveEeprom();
    }

    //------------------------ CALCULO INICIAL CAPACIDAD ----------------------------
    capacity = CapacityCheck(C_PIN_V_IN, &flag_low_battery, &flag_empty_battery);
    for (int i = 0; i <= capacity; i += 1)
    {
        DisplayCap(i);
        delay(200 / i);
    }
    delay(1000);
    SwitchScreenOff();
    if (ReadDiagnosticData(C_FLAG_ENABLE_NAME) == true)
    {
        ShowName();
    }
    SwitchScreenOff();
    playSound(C_SOUND_UP);

    //------------------------ SETEO DEL VOLTAJE ANTERIOR----------------------------

    theory_Vout = ReadDiagnosticData(C_THEORY_VOLTAGE);
    if (theory_Vout == 0)
    {
        LogDiagnosticData(50, C_THEORY_VOLTAGE);
        theory_Vout = 50;
    }

    //------------------------ CHECKEO PARA APP NAMING ------------------------------

    timer_waiting_naming.set(5000); // Incio de la ventana de tiempo

    while (flag_waiting_naming == true)
    {
        if ((digitalRead(C_PIN_BUTT_UP) == button_pressed) && (digitalRead(C_PIN_BUTT_DOWN) == button_pressed))
        {
            if (flag_naming_active == false) // Configuracio NO activada
            {
                if (flag_timer_naming == C_TIMER_IDLE) // Timer No iniciado
                {
                    Serial5.println("Timer Naming Activado.");
                    timer_active_naming.set(3000);
                    flag_timer_naming = C_TIMER_ARMED;
                }
                else if (flag_timer_naming == C_TIMER_ARMED) // Timer Armado
                {
                    if (timer_active_naming.poll() != C_TIMER_NOT_EXPIRED) // Timer cumplido
                    {
                        flag_naming_active = true; // Inicio de la configuracion del naming.
                        flag_timer_naming = C_TIMER_IDLE;
                    }
                }
            }
            else
            {

                Serial5.println("Naming Activado");
                Serial5.println("Loading");

                // Pantalla de carga del Naming.
                OLED_display.clearDisplay();
                OLED_display.setTextSize(1);
                OLED_display.setCursor(0, 0);
                OLED_display.print("Loading...");
                OLED_display.drawRect(0, 16, 64, 16, WHITE);
                for (uint16_t i = 0; i <= 100; i++)
                {
                    OLED_display.fillRect(0, 16, i * 64 / 100, 16, WHITE);
                    delay(8000 / 101);
                    OLED_display.display();
                }
                delay(2000);

                // Apagado Dramatico!
                SwitchScreenOff();
                delay(2000);

                // Pantalla de Presentacion
                OLED_display.clearDisplay();
                OLED_display.setTextSize(1);
                OLED_display.setCursor(8, 12);
                OLED_display.print("NICKNAME");
                OLED_display.drawRect(0, 0, 64, 32, WHITE);
                OLED_display.display();
                playSound(C_SOUND_CHARGE_IN);
                delay(4000);
                Serial5.println("Configuracion Naming");

                // Inicio de la configuracion
                Config_Naming();

                // Guardado en EEPROM
                LogDiagnosticData(true, C_FLAG_ENABLE_NAME);
                SwitchScreenOff();

                //RESETs Flags
                flag_waiting_naming = false;
            }
        }
        else if (timer_waiting_naming.poll() != C_TIMER_NOT_EXPIRED) // La ventana de tiempo cumple sin que se active la configuracion
        {
            flag_waiting_naming = false;
        }
        else if (button_event == C_LP_CENTER) // Pulsacion del boton central para skipear la ventana de tiempo.
        {
            flag_waiting_naming = false;
        }
        button_event = ReadDirPad(); // Lectura de la botonera.
    }

    /*===============================================================================================================================================*/
    //                                                                 CONTROL LOOP
    /*===============================================================================================================================================*/
    while (1)
    {
        t1 = micros(); // Control del periodo del ciclo de trabajpo.

        //============================================================== SENSADO ==================================================================//

        under_voltage_protection.threshold = (theory_Vout - 20) * 100;                          // Actualizacion de la proteccion de undervoltage
        sample_IOut = boost_check.getSample(C_PIN_I_OUT) * 3000 / 4096 * 10 / 15;               // Lectura de la Corriente de Salida
        sample_VOut = under_voltage_protection.getSample(C_PIN_V_OUT) * 201 / 39 * 3000 / 4096; // Lectura del Voltaje de salida
        sample_POut = (sample_IOut) * (sample_VOut) / 1000;                                     // Calculo de la potencia de salida
        //========================================================== BOOST MODE MONITOR ===========================================================//

        /*   Seleccion del tipo de salida en  funcion de la carga presente en la salida.   */
        if (boost_check.check(sample_IOut) == true)
        {
            output_mode = C_BOOST_MODE;
        }
        else if (boost_check.getCounter() == 0)
        {
            output_mode = C_NON_BOOST_MODE;
        }
        //============================================================================================================================================
        //                                                              STATE MACHINE
        //============================================================================================================================================

        /*________________________________________________________________ START_UP __________________________________________________________________*/
        if (sw_status == C_SW_ST_START_UP)
        {
            //---------------- SONIDO DE ENCENDIDO --------------------//
            if (C_STARTING_SOUND == false)
            {
                flag_sound_init = true;
            }
            else if (flag_sound_init == false)
            {
                playSound(C_SOUND_START);
                flag_sound_init = true;
            }
            //---------------- TEXTO DE ENCENDIDO --------------------//
            if (C_STARTING_TEXT == false)
            {
                flag_msg_init = true;
            }
            else if (flag_msg_init == false)
            {
                if (timer_init_screen.poll() != C_TIMER_NOT_EXPIRED)
                {
                    flag_msg_init = true;
                }
            }

            //-------------------- MOSTRADO DE LA CAPACIDAD AL FINAL DEL ENCENDIDO --------------------//
            if ((flag_msg_init == true) && (flag_display_capacity_init == false) && (flag_sound_init == true))
            {
                //t1_cap = micros();
                flag_display_capacity_init = true;
                capacity = CapacityCheck(C_PIN_V_IN, &flag_low_battery, &flag_empty_battery);
                DisplayCap(capacity); // Actualizacion de la pantalla con la Capacidad
                timer_display_capacity.set(1000);
            }
            else if (flag_display_capacity_init == true) // Mientras este presionado el boton central no se dejara de mostrar la capacidad.
            {
                if (timer_display_capacity.poll() != C_TIMER_NOT_EXPIRED)
                {
                    if (flag_center_button_press == true)
                    {
                        timer_display_capacity.set(1000);
                        flag_center_button_press = false;
                    }
                    else
                    {
                        flag_init2stop = true;
                    }
                }
                else
                {
                    if (button_event == C_LP_CENTER)
                    {
                        flag_center_button_press = true;
                    }
                }
            }
        }
        /*_______________________________________________________________RUN / STOP__________________________________________________________________*/
        else if ((sw_status == C_SW_ST_RUN) || (sw_status == C_SW_ST_STOP))
        {
            //t1 = micros();
            if (sw_status == C_SW_ST_RUN)
            {

                //      Shortcircuit Monitor     //
                if (short_current_protection.check(sample_IOut) == true)
                {

                    shortcircuit_event_protection_counter++;
                    if (shortcircuit_event_protection_counter == MAX_NUM_PROTECTION_EVENTS)
                    {
                        flag_error = true;
                        error = C_ERROR_SHORTCIRCUIT;
                        IncrementDiagnosticData(C_SHORT_CIRCUIT_ERROR);
                        short_current_protection.setCounter(0);
                    }
                    else
                    {
                        Serial5.printf("ShC %d\n", shortcircuit_event_protection_counter);
                        protection_event_delay.set(100);
                        flag_protection_event_delay = true;
                        short_current_protection.setCounter(C_RETRY_750_COUNT);
                    }
                }
                //------------- ARRANCADO--------------//
                if (arrancado == false)
                {
                    // planicie a 5v
                    DCDC.SetVoltage(50, C_NON_BOOST_MODE);
                    digitalWrite(C_PIN_OP_SWITCH, LOW);
                    delay(50);
                    int tiempo_arrancado = 200; // ms
                    int tiempo_bajada = 60;     // ms
                    int steps_subida = 10;
                    int steps_bajada = 10;

                    // Rampa de subida
                    for (int i = 0; i < steps_subida; i++)
                    {
                        DCDC.SetVoltage((120 - 50) / steps_subida * i + 50, C_NON_BOOST_MODE);
                        delay(tiempo_arrancado / steps_subida);
                    }
                    // Rampa de Bajada
                    for (int i = steps_bajada; i >= 0; i--)
                    {
                        DCDC.SetVoltage((120 - theory_Vout) / steps_bajada * i + theory_Vout, C_NON_BOOST_MODE);
                        delay(tiempo_bajada / steps_bajada);
                    }

                    // Voltaje Objetivo
                    DCDC.SetVoltage(theory_Vout, output_mode);
                    arrancado = true;
                }
                else if (arrancado)
                {
                    //------------ TEMPORIZADOR PARA LOGEO -------------//
                    if (timer_log_active.poll(1000) != C_TIMER_NOT_EXPIRED)
                    {
                        // Contador  de segundos
                        cont_log_active++;
                        if (cont_log_active == 60) // A la hora logeo en EEPROM.
                        {
                            PostMortemLog(sample_POut, capacity, theory_Vout, 0);
                            cont_log_active = 0;
                        }

                        // Sensado de la tension de entrada para prevenir apagado no deseado
                        sample_VIN = analogRead(C_PIN_V_IN) * 3000 / 4096 * 250 / 150;
                        Serial5.printf("Vin:%d\n", sample_VIN);
                        if (sample_VIN <= 3200)
                        {
                            DisplayLowBattery();
                            playSound(C_SOUND_LOW_BATTERY);
                        }
                    }

                    //------------ PROTECCIONES--------------//
                    if (mask_protection_state == C_MASK_DEACTIVATE)
                    {
                        //      Consumption Monitor     //
                        if (over_consumption_protection.check(sample_IOut) == true)
                        {

                            consmptn_event_protection_counter++;
                            if (consmptn_event_protection_counter == MAX_NUM_PROTECTION_EVENTS)
                            {
                                flag_error = true;
                                error = C_ERROR_OVERCURRENT;
                                IncrementDiagnosticData(C_CONSUMPTION_ERROR);
                                over_consumption_protection.setCounter(0);
                            }
                            else
                            {
                                Serial5.printf("CON %d\n", consmptn_event_protection_counter);
                                protection_event_delay.set(100);
                                flag_protection_event_delay = true;
                                over_consumption_protection.setCounter(C_RETRY_750_COUNT);
                                mask_protection_state = C_MASK_ACTIVATE;
                            }
                        }
                        //      Power Monitor       //
                        if (over_power_protection.check(sample_POut) == true)
                        {

                            OP_event_protection_counter++;
                            if (OP_event_protection_counter == MAX_NUM_PROTECTION_EVENTS)
                            {
                                flag_error = true;
                                error = C_ERROR_OVERPOWER;
                                IncrementDiagnosticData(C_POWER_ERROR);
                                over_power_protection.setCounter(0);
                            }
                            else
                            {
                                Serial5.printf("OP %d\n", OP_event_protection_counter);
                                protection_event_delay.set(100);
                                flag_protection_event_delay = true;
                                over_power_protection.setCounter(over_power_protection.limit / 2);
                                mask_protection_state = C_MASK_ACTIVATE;
                            }
                        }
                        //      Voltage Monitor        //
                        if ((under_voltage_protection.check(sample_VOut) == false) && (under_voltage_protection.getCounter() == 0))
                        {

                            UV_event_protection_counter++;
                            if (UV_event_protection_counter == MAX_NUM_PROTECTION_EVENTS)
                            {
                                flag_error = true;
                                error = C_ERROR_OUPUT_UNDERVOLTAGE;
                                IncrementDiagnosticData(C_VOLTAGE_ERROR);
                                under_voltage_protection.setCounter(under_voltage_protection.limit);
                            }
                            else
                            {
                                Serial5.printf("OUV %d\n", UV_event_protection_counter);
                                protection_event_delay.set(100);
                                flag_protection_event_delay = true;
                                under_voltage_protection.setCounter(under_voltage_protection.limit / 2);
                                mask_protection_state = C_MASK_ACTIVATE;
                            }
                        }
                    }
                    else if (mask_protection_state == C_MASK_ACTIVATE)
                    {
                        if ((flag_protection_event_delay == true) && (protection_event_delay.poll() != C_TIMER_NOT_EXPIRED))
                        {
                            mask_protection_state = C_MASK_DEACTIVATE;
                        }
                    }
                }

                //------- Actualizacion de la barra de potencia ----------//

                UpdatePowerBar(sample_POut);
            }
            else if (sw_status == C_SW_ST_STOP) // STOP
            {
                //-------  Checkeo de bateria baja -------//
                if (flag_low_battery == false)
                {
                    //  Temporizador de recupracion del voltaje antes de medir capacidad
                    if (timer_recover_voltage.poll() != C_TIMER_NOT_EXPIRED)
                    {
                        flag_enable_off = true;
                        capacity = CapacityCheck(C_PIN_V_IN, &flag_low_battery, &flag_empty_battery); // Calculo de la capacidad
                        if (flag_low_battery == true)                                                 // Aviso por bateria baja
                        {
                            DisplayCap(capacity);
                            delay(750);
                            trigger_Display_volt = true;
                        }
                    }
                }

                //-------------- TIMER DE INACTIVIDAD ----------------//
                if (button_event == C_NONE_EVENT)
                {
                    if (timer_idle.poll() != C_TIMER_NOT_EXPIRED)
                    {
                        cont_idle_timer++;                         // Incrementamos el contador.
                        if (cont_idle_timer == C_IDLE_TIMER_COUNT) // Si alcanzamos los minutos de inactividad.
                        {
                            flag_sleep = true;   // Flag ON para pasar al estado de Sleep
                            cont_idle_timer = 0; // Reset del contador de minutos.
                        }
                        else
                        {
                            timer_idle.set(C_TIME_IDLE_30_SEG); // Inicio del contador de 30 seg.
                        }
                    }
                }
                else
                {
                    timer_idle.set(C_TIME_IDLE_30_SEG); // Incio del contador de 30 seg para el Idle Timer.
                    cont_idle_timer = 0;                // Reset del contador de minutos.
                }
            }

            //--------------- CONTROL DE LOS EVENTOS DE LA BOTONERA ---------------------//

            // Si no se ha pulsado nada durante un rato se refresca la pantalla.
            if (button_event == C_NONE_EVENT)
            {
                if (timer_refresh_screen.poll() != C_TIMER_NOT_EXPIRED)
                {
                    trigger_Display_volt = true;
                    long_press_events = 0;
                }
            }
            else
            {
                sound = C_SOUND_MUTE;

                if (button_event == C_LP_CENTER) // LongPress Central
                {
                    Serial5.println("Longpress CENTER");
                    long_press_events++;
                    timer_refresh_screen.set(1000); // Inicio del timer por si se suelta el boton central.
                    if (long_press_events == 1)
                    {
                        if (flag_enable_off == true) // si estamos en el estado STOP se pasa a mostrar la capacidad
                        {
                            DisplayCap(capacity);
                        }
                        else
                        {
                            long_press_events--; // Si no estamos en el estado STOP reseteamos el contador.
                        }
                    }
                    else if ((long_press_events >= 2) && (long_press_events < 5))
                    {
                        // Pantalla de Confirmacion de Apagado.
                        OLED_display.clearDisplay();
                        OLED_display.setCursor(16, 0);
                        OLED_display.setTextSize(2);
                        OLED_display.print("OFF");
                        OLED_display.setCursor(0, 16);
                        OLED_display.setTextSize(2);
                        switch (long_press_events)
                        {
                        case 2:
                            OLED_display.print("3");
                            break;
                        case 3:
                            OLED_display.print("3 2 ");
                            break;
                        case 4:
                            OLED_display.print("3 2 1");
                            break;

                        default:
                            break;
                        }
                        OLED_display.display();
                        playSound(C_SOUND_UP); // Sonido de aviso de continuidad.
                    }
                    else if (long_press_events == 5) // Al 5 evento de LongPress nos vamos al estado SLEEP.
                    {
                        flag_sleep = true;
                        long_press_events = 0;
                    }
                }
                else
                {
                    //----- Subida de voltaje --------//
                    if (button_event == C_LP_UP)
                    {
                        Serial5.println("Longpress UP");
                        if (theory_Vout < MAX_VOLTAGE)
                        {
                            theory_Vout += 10;
                            sound = C_SOUND_UP;
                            trigger_Display_volt = true;
                        }
                    }
                    else if (button_event == C_CLICK_UP)
                    {
                        Serial5.println("Click UP");
                        if (theory_Vout < MAX_VOLTAGE)
                        {
                            theory_Vout += 1;
                            sound = C_SOUND_UP;
                            trigger_Display_volt = true;
                        }
                    }
                    //------ Bajada del Voltaje------//
                    else if (button_event == C_LP_DOWN)
                    {
                        Serial5.println("Longpress DOWN");
                        if (theory_Vout > MIN_VOLTAGE)
                        {
                            theory_Vout -= 10;
                            sound = C_SOUND_DOWN;
                            trigger_Display_volt = true;
                        }
                    }
                    else if (button_event == C_CLICK_DOWN)
                    {
                        Serial5.println("Click DOWN");
                        if (theory_Vout > MIN_VOLTAGE)
                        {
                            theory_Vout -= 1;
                            sound = C_SOUND_DOWN;
                            trigger_Display_volt = true;
                        }
                    }
                }

                theory_Vout = constrain(theory_Vout, MIN_VOLTAGE, MAX_VOLTAGE); // Constrain del voltaje de salida.
            }

            //-------------- ACTUALIZACION DEL DISPLAY (CON SONIDO)---------------------//

            if (trigger_Display_volt == true)
            {
                if (sound != C_SOUND_MUTE)
                {
                    playSound(sound);
                }
                DisplayVolt(theory_Vout);
                trigger_Display_volt = false;
            }
        }
        /*________________________________________________________________ SLEEP ____________________________________________________________________*/
        else if (sw_status == C_SW_ST_SLEEP)
        {
            // Filtrado de la primera vez que se entra en el modo SLEEP
            // Ocurre al insertar la bateria y alimentar el sistema.
            if (flag_first_sleep)
            {
                flag_msg_sleep = true;
                flag_first_sleep = false;
            }
            //----------- MENSAJE DE APAGADO --------------
            if (C_ENDING_TEXT == false)
            {
                flag_msg_sleep = true;
                SwitchScreenOff();
            }
            else if (flag_msg_sleep == false)
            {

                DisplayEndScreen();
                if (timer_end_screen.poll() != C_TIMER_NOT_EXPIRED)
                {
                    flag_msg_sleep = true;
                    SwitchScreenOff();
                }
            }
            //------------- SONIDO DE APAGADO -------------
            if (C_ENDING_SOUND == false)
            {
                flag_sound_end = true;
            }
            else if (flag_sound_end == false)
            {
                playSound(C_SOUND_END);
                flag_sound_end = true;
            }

            //------------------ Timer apagado y checkeo boton central ---------------
            if ((flag_msg_sleep == true) && (flag_waiting == C_TIMER_IDLE) && (flag_sound_end == true))
            {
                flag_waiting = C_TIMER_ARMED;
                timer_wait_sleep.set(600);
                // t1 = micros();
            }
            if (flag_waiting == C_TIMER_ARMED)
            {
                if (timer_wait_sleep.poll() != C_TIMER_NOT_EXPIRED)
                {
                    if (flag_center_button_press == true)
                    {
                        timer_wait_sleep.set(600);
                        flag_center_button_press = false;
                    }
                    else
                    {
                        flag_waiting = C_TIMER_DONE;
                    }
                }
                else
                {
                    if (button_event == C_LP_CENTER)
                    {
                        flag_center_button_press = true;
                    }
                }
            }
            else if (flag_waiting == C_TIMER_DONE)
            {
                if (go_deep_sleep == true)
                {
                    //----------- RUTINA APAGADO ---------------
                    if (flag_irq_center_button == true)
                    {
                        flag_irq_center_button = false; // Limpieza de los flags de interrupcion
                    }
                    SaveEeprom();                                                                       // Salvado en EEPROM
                    digitalWrite(C_PIN_ENABLE_LDO_VCC_2, LOW);                                          // Apagado de la alimentacion secundaria.
                    LowPower.attachInterruptWakeup(C_PIN_BUTT_CENTER, IrqCenterButtonHandler, FALLING); // Activacion de la interrupcion de despertar
                    Serial5.println("Zzz");
                    LowPower.sleep();

                    //---------- RUTINA DE DESPERTAR-----------------
                    detachInterrupt(C_PIN_BUTT_CENTER);         // Desactivar interrupcion
                    digitalWrite(C_PIN_ENABLE_LDO_VCC_2, HIGH); // Encender alimentacion secundaria.

                    // Comprobar causante del despertar.
                    if (flag_irq_center_button == true)
                    {
                        Serial5.println("Hi??");
                        go_deep_sleep = false;
                        flag_diagnostic_timer = C_TIMER_IDLE;                 // Reset del timer "timer_check_diagnostic"
                        timer_irq_button_center.set(C_TIMER_LONGPRESS + 200); // Inicio de la ventana para volver a entrar en el modo bajo consumo si no hay activacion.
                    }
                }
                else if (go_deep_sleep == false)
                {
                    if (flag_irq_center_button == true)
                    {

                        if ((digitalRead(C_PIN_BUTT_CENTER) == button_pressed) && (digitalRead(C_PIN_BUTT_DOWN) == button_pressed))
                        {
                            if (flag_enable_diagnostic == true)
                            {
                                if (flag_diagnostic_active == false) // Modo Daignostico Desactivado.
                                {
                                    if (flag_diagnostic_timer == C_TIMER_IDLE) // Timer no Activado
                                    {
                                        Serial5.println("Timer activado.");
                                        flag_diagnostic_timer = C_TIMER_ARMED;
                                        timer_check_diagnostic.set(C_TIME_DIAGNOSTIC_CHECK);
                                    }
                                    else if (flag_diagnostic_timer == C_TIMER_ARMED) // Timer Activado
                                    {
                                        if (timer_check_diagnostic.poll() != C_TIMER_NOT_EXPIRED) // Timer cumplido.
                                        {
                                            Serial5.println("Timer terminado");
                                            flag_diagnostic_active = true;        // Activacion del Modo diagnostico.
                                            flag_diagnostic_timer = C_TIMER_IDLE; //Reset del timer
                                        }
                                    }
                                }
                                else
                                {
                                    // MODO DIAGNOSTICO ACTIVADO
                                    while (flag_diagnostic_active == true)
                                    {
                                        // Ventana de presentacion
                                        initDisplay();
                                        OLED_display.clearDisplay();
                                        OLED_display.setTextSize(2);
                                        OLED_display.setCursor(10, 10);
                                        OLED_display.drawRect(0, 0, 64, 32, WHITE);
                                        OLED_display.print("TEST");
                                        OLED_display.display();
                                        playSound(C_SOUND_CHARGE_IN);
                                        delay(1000);

                                        // Pregunta sobre si diagnostico o no.
                                        Serial5.println("Pregunta activa.");
                                        OLED_display.clearDisplay();
                                        OLED_display.setCursor(0, 0);
                                        OLED_display.print("START");
                                        OLED_display.drawChar(0, 16, 0x59, WHITE, BLACK, 2);
                                        OLED_display.drawChar(52, 16, 0x4E, WHITE, BLACK, 2);
                                        OLED_display.display();
                                        flag_active_confirmation_question = true;

                                        // Espera Activa hasta confirmar que todos los boton no se estan apretando
                                        while ((digitalRead(C_PIN_BUTT_CENTER) == button_pressed) || (digitalRead(C_PIN_BUTT_DOWN) == button_pressed) || (digitalRead(C_PIN_BUTT_UP) == button_pressed))
                                        {
                                        }
                                        while (flag_active_confirmation_question == true) // Espera hasta que se reciba una respuesta.
                                        {
                                            button_event = ReadDirPad(); // Lectura de la botonera

                                            if ((button_event == C_CLICK_UP) || (button_event == C_LP_UP)) // NO
                                            {
                                                flag_active_confirmation_question = false;
                                                flag_diagnostic_active = false;
                                                Serial5.println("no");
                                                OLED_display.clearDisplay();
                                                timer_irq_button_center.set(C_TIMER_LONGPRESS + 200);
                                            }
                                            else if ((button_event == C_CLICK_DOWN) || (button_event == C_LP_DOWN)) // Yes
                                            {
                                                flag_active_confirmation_question = false;
                                                Serial5.println("yes");
                                            }

                                            OLED_display.display();
                                        }
                                        // Inicio del modo diagnostico
                                        if (flag_diagnostic_active == true)
                                        {
                                            // Cuenta atras preparatoria
                                            for (int i = 9; i >= 0; i--)
                                            {
                                                OLED_display.clearDisplay();
                                                OLED_display.setTextSize(2);
                                                OLED_display.setCursor(28, 10);
                                                OLED_display.print(i);
                                                OLED_display.display();
                                                delay(1000);
                                            }

                                            //Volcado de datos.
                                            Serial5.println("INICIO DIAGNOSTICO");
                                            DiagnosticMode();

                                            // Pantallas despedida
                                            OLED_display.clearDisplay();
                                            OLED_display.setCursor(0, 0);
                                            OLED_display.print("TEST");
                                            OLED_display.setCursor(0, 16);
                                            OLED_display.print("END");
                                            OLED_display.display();
                                            delay(2000);
                                            OLED_display.clearDisplay();
                                            OLED_display.setCursor(0, 0);
                                            OLED_display.print("THANK");
                                            OLED_display.setCursor(0, 16);
                                            OLED_display.print("YOU");
                                            OLED_display.display();
                                            flag_diagnostic_active = false;
                                            Serial5.println("FIN DIAGNOSTICO");
                                            playSound(C_SOUND_CHARGE_OUT);
                                            OLED_display.clearDisplay();

                                            timer_irq_button_center.set(C_TIMER_LONGPRESS + 200); // Reinicio del Timer para vuelta a modo bajo consumo.
                                        }
                                    }
                                }
                            }
                        }
                        else if (timer_irq_button_center.poll() != C_TIMER_NOT_EXPIRED) // Expira la ventana de tiempo para despertar o entrar en modo diagnostico.
                        {
                            go_deep_sleep = true;
                            Serial5.println("A mimir");
                        }
                        //--- Comprobacion del boton central-----
                        else if (button_event == C_LP_CENTER) // Si se mantiene apretado el boton central desde el despertar se inicia la bateria
                        {
                            flag_initialize = true;
                            Serial5.println("Bon dia");
                        }
                    }
                }
            }
        }
        /*________________________________________________________________ ERROR ____________________________________________________________________*/
        else if (sw_status == C_SW_ST_ERROR)
        {
            //------------- MENSAJE DE ERROR --------------
            if (display_error_status == C_DISPLAY_ST_BUSSY)
            {
                switch (error)
                {
                case C_ERROR_OVERPOWER:
                    OLED_display.fillCircle(31, 14, 13, WHITE);
                    OLED_display.fillRect(30, 6, 3, 12, BLACK);
                    OLED_display.fillRect(30, 20, 3, 3, BLACK);
                    PostMortemLog(sample_POut, capacity, theory_Vout, C_ERROR_OVERPOWER);
                    break;
                case C_ERROR_OVERCURRENT:
                    OLED_display.fillTriangle(31, 0, 15, 26, 47, 26, WHITE);
                    OLED_display.fillRect(30, 7, 3, 12, BLACK);
                    OLED_display.fillRect(30, 21, 3, 3, BLACK);
                    PostMortemLog(sample_POut, capacity, theory_Vout, C_ERROR_OVERCURRENT);
                    break;
                case C_ERROR_OUPUT_UNDERVOLTAGE:
                    OLED_display.drawBitmap(0, 0, error_square, 64, 30, WHITE, BLACK);
                    PostMortemLog(sample_POut, capacity, theory_Vout, C_ERROR_OUPUT_UNDERVOLTAGE);
                    break;
                case C_ERROR_INPUT_UNDERVOLTAGE:
                    PostMortemLog(sample_POut, capacity, theory_Vout, C_ERROR_INPUT_UNDERVOLTAGE);
                    break;
                case C_ERROR_SHORTCIRCUIT:
                    OLED_display.fillTriangle(31, 0, 15, 26, 47, 26, WHITE);
                    OLED_display.fillRect(30, 7, 3, 12, BLACK);
                    OLED_display.fillRect(30, 21, 3, 3, BLACK);
                    PostMortemLog(sample_POut, capacity, theory_Vout, C_ERROR_SHORTCIRCUIT);
                    break;
                default:
                    break;
                }

                // Parpadeo de la barra de potencia
                if (timer_blink_error.poll(200) != C_TIMER_NOT_EXPIRED)
                {
                    blink_error_state = !blink_error_state;
                }
                if (blink_error_state == true)
                {
                    OLED_display.fillRect(0, C_PWBAR_Y_AXE, C_DISPLAY_WIDTH, C_DISPLAY_HEIGHT, BLACK);
                }
                else
                {
                    PowerBar(LEDS_IN_POWERBAR);
                }

                // Control del tiempo que se muestra el error.
                if (timer_display_error.poll() != C_TIMER_NOT_EXPIRED)
                {
                    display_error_status = C_DISPLAY_ST_NOT_BUSSY;
                }
            }

            //------------------ FINAL DE LA ALARMA -----------------
            if ((display_error_status == C_DISPLAY_ST_NOT_BUSSY))
            {
                flag_error = false;
                consmptn_event_protection_counter = 0;
                OP_event_protection_counter = 0;
                UV_event_protection_counter = 0;
                SwitchScreenOff();
                // -------------- GUARDADO EN EEPROM -----------------
                SaveEeprom();
            }
        }
        //=============================================================================================================================================
        //                                                             END STATE MACHINE
        //=============================================================================================================================================
        //=============================================================================================================================================
        //                                                             STATUS MANAGEMENT
        //=============================================================================================================================================

        /*________________________________________________________________ START_UP __________________________________________________________________*/
        if (sw_status == C_SW_ST_START_UP)
        {
            //----------- TO STOP -----------
            if (flag_init2stop == true)
            {
                /* New State */
                sw_status = C_SW_ST_STOP;

                /* Output */
                sw_output = C_OUTPUT_OFF;
                user_output = C_OUTPUT_OFF;

                /* Clear Flags */
                flag_msg_init = false;
                flag_init2stop = false;
                flag_sound_init = false;
                flag_display_capacity_init = false;

                /* Change-State Effects */
                Serial5.println("Change TO STOP");
                trigger_Display_volt = true;
                timer_idle.set(30000);
                timer_recover_voltage.set(1000);
            }
        }
        /*________________________________________________________________ RUN __________________________________________________________________*/
        else if (sw_status == C_SW_ST_RUN)
        {
            //----------- TO STOP -----------
            if ((button_event == C_CLICK_CENTER) || (button_event == C_LP_CENTER))
            {
                /* New State */
                sw_status = C_SW_ST_STOP;

                /* Output */
                user_output = C_OUTPUT_OFF;
                sw_output = C_OUTPUT_OFF;

                /* Clear Flags */
                flag_low_battery = false;

                /* Change-State Effects */
                Serial5.println("Change TO STOP");
                playSound(C_SOUND_OFF);
                timer_idle.set(30000);
                timer_recover_voltage.set(1000);
            }
            //----------- TO ERROR -----------
            if (flag_error == true)
            {
                /* New State */
                sw_status = C_SW_ST_ERROR;

                /* Output */
                sw_output = C_OUTPUT_OFF;

                /* Clear Flags */
                flag_low_battery = false;

                /* Change-State Effects */
                Serial5.println("Change TO ERROR");
                display_error_status = C_DISPLAY_ST_BUSSY;
                timer_display_error.set(1500);
                SwitchScreenOff();
                PowerBar(0);
                playSound(C_SOUND_ERROR);
                switch (error)
                {
                case C_ERROR_OVERPOWER:
                    Serial5.printf("OverPower ERROR\n");
                    break;
                case C_ERROR_OVERCURRENT:
                    Serial5.printf("OverCurrent ERROR\n");
                    break;
                case C_ERROR_OUPUT_UNDERVOLTAGE:
                    Serial5.printf("Output UnderVoltage ERROR\n");
                    break;
                case C_ERROR_INPUT_UNDERVOLTAGE:
                    Serial5.printf("Input UnderVoltage ERROR\n");
                case C_ERROR_SHORTCIRCUIT:
                    Serial5.printf("SHORTCIRCUIT ERROR\n");
                    break;
                default:
                    break;
                }
            }
        }
        /*________________________________________________________________ STOP __________________________________________________________________*/
        else if (sw_status == C_SW_ST_STOP)
        {
            arrancado = false;
            //----------- TO RUN -----------
            if (button_event == C_CLICK_CENTER)
            {
                if (flag_empty_battery == true)
                {
                    playSound(C_SOUND_DEATH_BATTERY);
                    DisplayNoBattery();
                    trigger_Display_volt = true;
                }
                else
                {
                    /* New State */
                    sw_status = C_SW_ST_RUN;
                    /* Output */
                    sw_output = C_OUTPUT_ON;
                    user_output = C_OUTPUT_ON; // Activacion de la salida.
                    /* Clear Flags */
                    flag_enable_off = false;
                    /* Change-State Effects */
                    Serial5.printf("Change TO RUN\n");
                    playSound(C_SOUND_ON);
                }
            }
            //----------- TO SLEEP -----------
            if (flag_sleep == true)
            {
                /* New State */
                sw_status = C_SW_ST_SLEEP;

                /* Output */
                sw_output = C_OUTPUT_OFF;

                /* Clear Flags */
                flag_msg_sleep = false;
                flag_sound_end = false;
                flag_sleep = false;
                go_deep_sleep = true;

                /* Change-State Effects */
                Serial5.printf("Change TO SLEEP\n");
                timer_end_screen.set(1000);
            }
        }
        /*________________________________________________________________ SLEEP __________________________________________________________________*/
        else if (sw_status == C_SW_ST_SLEEP)
        {
            //----------- TO START_UP -----------
            if (flag_initialize == true)
            {
                /* New State */
                sw_status = C_SW_ST_START_UP;

                /* Output */
                sw_output = C_OUTPUT_OFF;

                /* Clear Flags */
                flag_initialize = false;
                flag_waiting = C_TIMER_IDLE;
                flag_msg_sleep = false;
                flag_sound_end = false;

                /* Change-State Effects */
                flag_enable_diagnostic = true;
                Serial5.printf("Change TO START\n");
                initDisplay();
                trigger_Display_volt = true;
                DisplayLogo();
                timer_init_screen.set(C_TIME_INIT_SCREEN);
            }
        }
        /*________________________________________________________________ ERROR __________________________________________________________________*/
        else if (sw_status == C_SW_ST_ERROR)
        {
            //----------- TO STOP -----------
            if (flag_error == false)
            {
                /* New State */
                sw_status = C_SW_ST_STOP;

                /* Output */
                sw_output = C_OUTPUT_OFF;
                user_output = C_OUTPUT_OFF;

                /* Clear Flags */

                /* Change-State Effects */
                Serial5.printf("Change TO STOP\n");
                timer_idle.set(30000);
                trigger_Display_volt = true;
                timer_recover_voltage.set(1000);
            }
        }
        //=============================================================================================================================================
        //                                                              END STATE MANAGEMENT
        //=============================================================================================================================================

        //============================================================== OUTPUT MANAGEMENT ===========================================================//
        if (sw_output == C_OUTPUT_OFF)
        {
            hw_output = C_OUTPUT_OFF;
        }
        else
        {
            if (user_output == C_OUTPUT_ON)
            {
                hw_output = C_OUTPUT_ON;
            }
            else
            {
                hw_output = C_OUTPUT_OFF;
            }
        }
        if (hw_output == C_OUTPUT_ON)
        {
            theory_Vout = constrain(theory_Vout, MIN_VOLTAGE, MAX_VOLTAGE); // Contrain de la tension de salida
            DCDC.SetVoltage(theory_Vout, output_mode);                      // Fijado de la tension de salida
            digitalWrite(C_PIN_OP_SWITCH, LOW);                             // Activacion del transistor de salida
            LedWork(C_OUTPUT_ON);                                           // se enciende el Led que indica que la salida esta activa
        }
        else if (hw_output == C_OUTPUT_OFF)
        {
            digitalWrite(C_PIN_OP_SWITCH, HIGH); // Desactivacion del transistor de salida.
            LedWork(C_OUTPUT_OFF);               // Apagado del led indicador de salida activada.

            // Limpieza y reset de variables de las protecciones.
            consmptn_event_protection_counter = 0;
            OP_event_protection_counter = 0;
            UV_event_protection_counter = 0;
        }
        //======================================================= ACTUALIZACION DEL DIAGNOSTICO ===========================================================//
        LogDiagnosticData(theory_Vout, C_THEORY_VOLTAGE);
        //============================================================ TIMER LOGEO EEPROM  =============================================================//
        if (timer_sec_count.poll(1000) != C_TIMER_NOT_EXPIRED)
        {
            cont_sec_log++;
            if (cont_sec_log == C_EEPROM_UPDATE_TIME * 60)
            {
                if (flag_eeprom_init_fail == true)
                {
                    if (!Init_local_eeprom()) // Incializacion EEPROM
                    {
                        flag_eeprom_init_fail = true;
                        Serial5.println("Fallo de lectura de EEPROM");
                    }
                    else
                    {
                        flag_eeprom_init_fail = false;
                        Serial5.println("Lectura Correcta de EEPROM");
                    }
                }
                else
                {
                    Serial5.println("Updateando EEPROM..");
                    IncrementDiagnosticData(C_WORK_TIME);
                    SaveEeprom();
                    Serial5.println("EEPROM Actualizada.");
                }
                cont_sec_log = 0;
            }
        }

        //====================================================== LECTURA DE EVENTOS DE LA BOTONERA ========================================================//
        button_event = ReadDirPad();

        //====================================================== Control del tiempo del ciclo de control ========================================================//
        t2 = micros();
        if (timer_log_sec.poll(1000) != C_TIMER_NOT_EXPIRED)
        {
            cont_log_sec++;

            Serial5.printf("%d\n-----------------\n", cont_log_sec);
        }
        while ((t2 - t1) < C_PROGRAM_CYCLE_PERIOD)
        {
            t2 = micros();
        }
    }
}

void loop()
{
}

void IrqCenterButtonHandler()
{
    flag_irq_center_button = true;
}

/**
 * @brief 
 * 
 * @param pin_battery 
 * @param lowbattery 
 * @return int16_t 
 */
int16_t CapacityCheck(uint16_t pin_battery, bool *lowbattery, bool *empty_batt)
{
    int16_t percent = 0;
    int32_t batt_voltage = 0;
    int16_t sample = 0;
    analogReadCorrection(12, 2055);
    analogReadResolution(12);

    // Lectura del ADC
    for (int i = 0; i < 8; i++)
    {
        sample += analogRead(pin_battery);
    }

    //Calculo del voltaje de la bateria
    batt_voltage = sample / 8 * 3000 / 4096 * 250 / 150;

    //Calculo del porcentaje  de bateria actual
    percent = constrain(((batt_voltage - 3300) * 100 / 800), 0, 100);

    if (percent <= C_EMPTY_BATTERY_LEVEL) // Porcentaje por debajo del valor de bateria vacia.
    {
        *empty_batt = true;
        *lowbattery = true;
        playSound(C_SOUND_DEATH_BATTERY);
        DisplayNoBattery();
    }
    else if (percent <= C_LOW_BATTERY_LEVEL) // Porcentaje por debajo del valor de bateria baja.
    {
        *empty_batt = false;
        *lowbattery = true;
        DisplayLowBattery();
        playSound(C_SOUND_LOW_BATTERY);
        delay(250);
    }
    else
    {
        *lowbattery = false;
        *empty_batt = false;
    }
    return percent;
}