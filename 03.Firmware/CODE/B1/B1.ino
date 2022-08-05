/**
 * @file B1.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 *      V1: - Gestion y deteccion del modo Boost/No boost. Cuando la corriente de salida sea 
 *            superior a un umbral durante un determinado timep, se debera activar la salida en 
 *            modo BOOST, mientras que si se encuentra por debajo del umbral durante otro 
 *            determinado tiempo, se debera desactivar la salida modo BOOSt.
 *             - Umbral: 15 mA
 *             - Tiempo Boost: 100 ms
 *             - Tiempo No Boost: 1s. 
 *      V2: - Proteccion de sobreconsumo: La proteccion va a enfocada a detectar cuando la bateria
 *            tiene un consumo por encima de lo normal, indicativo de una posible situacion de error.
 *             Ej: Fallo en el arranque de la maquina de tatuar.
 *              - Umbral: 450 mA.
 *              - Tiempo: 1500 ms.
 *              - Sistema de doble pendiente.
 *      V3: - Se creean 2 helathMonitor independientes para cada proteccion.El funcionamiento 
 *            de estas protecciones son equivalentes a la proteccion de sobreconsumo.
 *             Proteccion de UnderVoltage:
 *              - Umbral: 2 Voltios
 *              - Tiempo: 1 segundos
 *              - Sistema de doble pendiente.
 *             Proteccion de SobrePotencia.
 *              - Umbral: 4 Watios
 *              - Tiempo: 2 segundos
 *              - Sistema de doble pendiente.
 *      V4: - La parte del display integrara 2 funcionalidades.
 *             Funcionalidades Display:
 *              - Mensaje de Inicio: Al encendido de la bateria se mostrara en la pantalla una cadena de caracteres predeterminada.
 *              - Visualizar Voltaje: En el ciclo de control se mostrara en la pantalla el voltaje con el siguiente formato:
 *             State Machine: Diferenciacion de distintos estados y las condiciones de cambio correspondientes:
 *              - Initialization: Mesnaje de bienvenida y arranque.
 *                  - Run: Voltaje por display y activacion de la salida.
 *                  - Stop: Voltaje por Display y salida desactivada.
 *                  - Sleep: Mensaje de despedida y bateria "apagada".
 *                  - ERROR: Mensaje de error.
 *              - Subida y bajada de voltaje con los botones.
 *              - Activacion y desactavacion de la salida con los botones.
 *      V5: - Se decide añadir el modulo del control del Zumbador/Buzzer. Con este modulo se incorporan sonidos a determinados eventos.
 *            Los sonidos se seleccionaran entra alternativas en una funcion de inicializacion, segun el atributo de entrada.
 *            Los eventos que iran acompañados de sonidos en esta integracion son:
 *                  - Inicio.
 *                  - Apagado.
 *                  - Subida de Voltaje.
 *                  - Bajada de Voltaje.
 *                  - Run/Stop.
 *      V6: - Se decide realizar un seguimeinto de ciertas variables que se consideran de interes. 
 *              Para ello se utuilizan 2 memorias EEPROM, una situada en la parte del chasis y otra en la parte de la bateria.
 *              En la bateria se guardaran datos relacionados con la identidad y vida de la bateria como son:
 *                  - Modelo.
 *                  - Numero de serie.
 *                  - Numero de ciclos/Tiempo de Uso.
 *                  - Esstadisticas de uso en voltage y potencia. 
 *                  - Log de errores que ha podido sufrir.
 *              En el chasis se almacena informacion relacionada con la configuracion del chasis como son:
 *                  - Seleccion de los sonidos(Encendido,Run/Stop,Up,Down).
 *                  - Voltaje de funcionamiento.
 *                  - Numero de serie del chasis.
 *      V7: - Se incluye el calculo de la capacidad de la bateria partiendo de la lectura del voltaje.
 *              Esta lectura de realiza siempre cuando la salida se encuentra descoenctada para evitar que la carga afecte al voltaje, realizando la medida en vacio.
 *              Se situan los extremos en 4,2V y 3,5 como maximo y minimo respectivamente.
 *              Propuesta de muestra de la informacion de la capacidaden 2 lugares:
 *                  - En la inicializacion.
 *                  - Si se mantiene apretado el boton central, se muestra la capacidad durante un breve tiempo antes de mostrar un mensaje de apagado. Si se mantiene apretado termina entrando en modo sleep.
 *              Se incluye la proteccion del voltage de entrada pera prevenir que la bateria baje por debajo de un umbral que no asegure un correcto funcionamiento del sistema.
 *              Caracteristicas:
 *                  - Umbral: 3300 mV.
 *                  - Tiempo: 500 ms.
 *                  - Sistema de doble pendiente.
 *                  - Fix Error
 *              Se ha detectado que si no se levanta el boton central, el sistema se enciende y se apaga constantemente.
 *              Como solucion, al apagar se espera a que se levante el boton para permitir volver a pulsar y encencer el sistema.
 *      V8: - Manejo de la barra de potencia, formada por 14 leds puestos en linea recta. Colocada en la parte inferior del Display.
 *              La barra de potencia mostrara de forma grafica el valor de la potencia instantanea, de forma que con un fondo de escala ajustable (incialmente en 2500 mW) y una granularidad de 13 leds, se encienden los leds de manera fluida.
 *              En los estados de inicio, apagado, error y diagnostico, la barra de potencia estara completamente apagada.
 *              Por otro lado, se ha determiando la necesaidad de incluir un modo de brillo tenue que se active a los 5 segundos de que no se produzca ninguna interaccion con la bateria, que disminuya considerablemente el brillo del Display.
 *              En el momenot que se pulse cualquiera de los botones, la pantalla recupera el brillo.
 *              La disminucion del brillo solo afecta al estado de Work y Capaciti, deminuyendo el brillo del display y la barra de potencia.
 *      V9: - Para poder realizar un perfil de tiempos, se utilizan 6 pines que actualmente no tienen uso para, utilizando un analizador logico, se pueda obersvar el consumo temporal de las distintas funciones como:
 *                  - Duracion del ciclo de control.
 *                  - Lectura de la botonera.
 *                  - Funciones del Buzzer.
 *                  - Funciones del Display.
 *                  - Funciones de Diagnostico.
 *                  - Funcion de la Power Bar.
 *      V10: - Con el fin de aumentar la vida de la bateria, durante el estado de sleep, se introducira al micro en un estado de lowpower o Standby.
 *              Para despertar el micro se utilizara una interrupcion harward provocada por el boton central. 
 *              Tras el despertar se analiza la rutina de atencion de la interrupcion y se continuara el rpograma justo despues de la intruccion que duerme al micro.
 *      V11: - Cuando se conecte el USB al battery pack el chasis reacciona estado en un estadoq ue unicamente mostrara una animacion por el display indicando la conexion, y seguidamente se quedara permanentemente intentando conectarse al puerto serie. 
 *              Si se consigue conectar al puerto serie,  empezara a retransmitir la informacion del modo diagnostico (Aun por concretar esta informacion.)
 *              Se volvera al modo sleep tras la desconexion del USB.
 *              La deteccion de la conexion y desconexion se realizara mediante una interrupcion hardward.
 * 
 * @version V11-RC1
 * @date 2021-3-5
 * 
 * @copyright Copyright (c) 2020
 * 
 */

//============================================================== PINES ===========================================================//
const uint16_t C_PIN_ENABLE_LDO_VCC_2 = 0;
const uint16_t C_PIN_FLAG_CHARG = 5;
const uint16_t C_PIN_OP_SWITCH = 6;
const uint16_t C_PIN_USB_SEL = 7;
const uint16_t C_PIN_TEST_MODE = 9;
const uint16_t C_PIN_SDA_2 = 10;
const uint16_t C_PIN_EN_DCDC = 11;
const uint16_t C_PIN_FLAG_BATTLOW = 12;
const uint16_t C_PIN_SCL_2 = 13;

const uint16_t C_PIN_I_OUT = A1;
const uint16_t C_PIN_V_OUT = A4;
const uint16_t C_PIN_V_IN = A5;

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

//============================================================== CONSTANTS ===========================================================//

const uint32_t C_PROGRAM_CYCLE_PERIOD = 5000; // Duracion minima del ciclo de programa en us.

const bool C_MASK_DEACTIVATE = false;
const bool C_MASK_ACTIVATE = true;

const int16_t C_MIN_VOLT = 50;
const int16_t C_MAX_VOLT = 160;

const uint16_t MAX_NUM_PROTECTION_EVENTS = 6;

const bool C_OFF = false;
const bool C_ON = true;

const int16_t C_TIMER_ARMED = 0;
const int16_t C_TIMER_IDLE = 1;
const int16_t C_TIMER_DONE = 2;

const int16_t C_SW_ST_RUN = 0x00;
const int16_t C_SW_ST_SLEEP = 0x01;
const int16_t C_SW_ST_START_UP = 0x02;
const int16_t C_SW_ST_ERROR = 0x03;
const int16_t C_SW_ST_STOP = 0x04;
const int16_t C_SW_ST_DIAGNOSTIC = 0x05;
const int16_t C_SW_ST_USB = 0x07;

const int16_t C_LIMIT_COMSUPTION_PROT = 450;
const int16_t C_LIMIT_UNDERVOLTAGE_PROT = 2000;
const int16_t C_LIMIT_OVERPOWER_PROT = 4000;
const int16_t C_RETRY_750_COUNT = 750;

const int32_t C_TIME_TO_LIGHT_DOWN = 3000; // 3s

const uint32_t C_WATCH_DOG_TIME_MS = 1000;

const String C_DIAGNOSTIC_PASSWORD = "FPO";

const bool C_USB_CONNECTED = true;
const bool C_USB_DISCONNECTED = false;

const uint8_t C_NUMB_PRINTS_STATIC_DATA = 5;

const uint8_t C_LOW_BATTERY_LEVEL = 10; // Porcentaje de bateria  partir del cual no se puede asegurar un correcto funcionamiento.

const uint16_t C_IDLE_TIMER_COUNT = 15 * 2; // Minutos antes de que se apage la bateria por inactividad. NOTE: El contador es de 30seg, por eso se multiplica por 2-

const uint16_t C_EEPROM_UPDATE_TIME = 6; //min

//-------------------------------- Diagnostic Commands-------------------------------------
const String C_CMD_DISPLAY_STARTING_OFF = "DspStOFF";
const String C_CMD_DISPLAY_STARTING_ON = "DspStON";
const String C_CMD_DISPLAY_ENDING_OFF = "DspEndOFF";
const String C_CMD_DISPLAY_ENDING_ON = "DspEndON";
const String C_CMD_SOUND_STARTING_OFF = "SndStrOFF";
const String C_CMD_SOUND_STARTING_ON = "SndSrtON";
const String C_CMD_SOUND_ENDING_OFF = "SndEndOFF";
const String C_CMD_SOUND_ENDING_ON = "SndEndON";
const String C_CMD_SOUND_FULL_CHARGE_OFF = "SndFChgOFF";
const String C_CMD_SOUND_FULL_CHARGE_ON = "SndFChgOFF";
const String C_CMS_SOUND_DEATH_BATTERY_OFF = "SndDthBttOFF";
const String C_CMS_SOUND_DEATH_BATTERY_ON = "SndDthBttON";
const String C_CMS_SOUND_CHARGE_START_OFF = "SndChgStOFF";
const String C_CMS_SOUND_CHARGE_START_ON = "SndChgStON";
const String C_CMD_LIVE_OFF = "LiveOFF";
const String C_CMD_LIVE_ON = "LiveON";
const String C_CMD_DIAGNOSTIC_STOP = "GO_SLEEP";

const String C_LIVE_CMD_OUT_ON = "EnableOut";
const String C_LIVE_CMD_OUT_OFF = "DisnableOut";
const String C_LIVE_CMD_VOLT_UP_1 = "VoltUp1";
const String C_LIVE_CMD_VOLT_UP_10 = "VoltUp10";
const String C_LIVE_CMD_VOLT_DOWN_1 = "VoltDown1";
const String C_LIVE_CMD_VOLT_DOWN_10 = "VoltDown10";
const String C_LIVE_CMD_OFF_LIVE_VIEW = "LiveViewOff";

String C_TXT_START = "MSTK";
String C_TXT_SLEEP = "GOOD JOB";
String C_TXT_ASK_OFF = "OFF";

const uint16_t C_ERROR_OVERCURRENT = 0x10;
const uint16_t C_ERROR_OVERPOWER = 0x11;
const uint16_t C_ERROR_OUPUT_UNDERVOLTAGE = 0x12;
const uint16_t C_ERROR_INPUT_UNDERVOLTAGE = 0x13;

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
 *      - Umbral : 450 mah.
 *      - Ts = 10 ms
 *      - Time spam = 1500ms
 * 
 */
HealthMonitor over_consumption_protection(C_LIMIT_COMSUPTION_PROT, 10, 10, 1500);

/**
 * @brief HealthMonitor de la potencia de salida.
 *      - Umbral : 4000 mW. (mA x mV / 1000)
 *      - Ts = 10 ms
 *      - Time spam = 2000 ms
 * 
 */
HealthMonitor over_power_protection(C_LIMIT_OVERPOWER_PROT, 10, 10, 2000);

/**
 * @brief HealthMonitor del theory_Vout de salida.
 *      - Umbral : 2000 mV
 *      - Ts = 10 ms
 *      - Time spam = 1000 ms
 * 
 */
HealthMonitor under_voltage_protection(C_LIMIT_UNDERVOLTAGE_PROT, 10, 10, 1000);

Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();

dcdc_controler DCDC(C_PIN_EN_DCDC);

MilliTimer timer_encendido;
MilliTimer protection_event_delay;
MilliTimer timer_diagnostic_querist;
MilliTimer timer_sec_count;
MilliTimer timer_print_diagnostic;
MilliTimer timer_display_capacity;
MilliTimer timer_recover_voltage;
MilliTimer timer_wait_sleep;
MilliTimer timer_low_bright;
MilliTimer timer_irq_button_center;
MilliTimer timer_idle;
MilliTimer timer_display_error;
MilliTimer timer_info_error;
MilliTimer debug_serial_timer;
MilliTimer timer_retry_upgrade_eeprom;

//--------------------------------------- States variables-------------------------------------
int16_t sw_status = C_SW_ST_START_UP;
int16_t sound = C_SOUND_UP;
int16_t button_event = C_NONE_EVENT;
bool usb_status = C_USB_DISCONNECTED;
bool sw_output = C_OFF, hw_output = C_OFF, user_output = C_OFF;
bool output_mode = C_NON_BOOST_MODE;

//--------------------------------------- Counters variables-------------------------------------
int32_t cont_sec = 0;
int16_t consmptn_event_protection_counter = 0;
int16_t OP_event_protection_counter = 0;
int16_t UV_event_protection_counter = 0;
uint16_t click_events = 0;
int16_t cont_idle_timer = 0;

//--------------------------------------- Diagnostics variables-------------------------------------
uint16_t sample_IOut = 0;
uint16_t sample_VOut = 0;
uint16_t sample_POut = 0;
uint16_t sample_Vin = 0;
uint16_t theory_Vout = 50;
uint16_t capacity = 0;
uint8_t counter_prints_static_data = 0;
uint8_t afk_counter = 0;
String data;
bool enable_starting_text = false;
bool enable_starting_sound = false;
bool enable_ending_text = false;
bool enable_ending_sound = false;
bool enable_charge_sound = false;
bool enable_death_battery_sound = false;
bool enable_full_charge_sound = false;
bool enable_live_view = false;
MilliTimer delay_live_view;
MilliTimer afk_timer;
bool new_text_received = false;
bool change_text_answered = false;
bool cmd_go_sleep = false;
bool print_diagnostic_static_data = false;
bool diag_check = false;
bool test_enable = false;
bool flag_eeprom_init_fail = false;
bool flag_eeprom_upgrade_fail = false;
uint16_t error;

//-------------------------------------- FLAGS--------------------------------------------
bool low_batt_sound = false;
bool low_batt_display = false;
bool flag_low_battery = false;

bool reset_text = true;

bool protection_event_delay_flag = false;
bool mask_protection_state = C_MASK_DEACTIVATE;
volatile bool flag_error = false;

bool display_status = C_DISPLAY_ST_NOT_BUSSY;
bool display_error_status = C_DISPLAY_ST_NOT_BUSSY;
bool mode_bright_display = C_MODE_HIGH_BRIGHT;
bool flag_msg_init = false;
bool flag_msg_sleep = false;
bool trigger_Display_volt = true;
bool flag_display_capacity = false;

volatile bool flag_irq_center_button = false;

bool flag_sound_init = false;
bool flag_sound_end = false;

bool flag_sleep = false;
bool go_sleep = true;
bool flag_initialize = false;
bool flag_usb_change = false;
bool flag_work = false;
bool flag_center_button_press = false;
bool arrancado = false;
bool flag_stop = false;

//int16_t flag_encendido = C_TIMER_IDLE;
int16_t flag_waiting = C_TIMER_IDLE;
int16_t flag_timer_low_bright = C_TIMER_IDLE;

//-------------------------------------- PROFILING --------------------------------------------
uint32_t t1;
uint32_t t2;

uint32_t t1_text;
uint32_t t2_text;
uint32_t t1_encendido;
uint32_t t2_encendido;
uint32_t t1_sound;
uint32_t t2_sound;
uint32_t t1_cap;
uint32_t t2_cap;
uint32_t t1_state;
uint32_t t2_state;

/*===============================================================================================================================================*/
//                                                                 SET UP
/*===============================================================================================================================================*/
void setup()
{
    Serial.begin(9600);
    Serial5.begin(57600);
    Serial5.println("START!");
    //------------------------- INIT PIN-------------------------------
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_UP, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_DOWN, INPUT_PULLUP);
    pinMode(C_PIN_FLAG_CHARG, INPUT_PULLUP);
    pinMode(C_PIN_FLAG_BATTLOW, INPUT_PULLUP);
    pinMode(C_PIN_TEST_MODE, INPUT_PULLUP);

    pinMode(C_PIN_OP_SWITCH, OUTPUT);
    pinMode(C_PIN_ENABLE_LDO_VCC_2, OUTPUT);
    pinMode(C_PIN_SCL_2, OUTPUT);
    pinMode(C_PIN_SDA_2, OUTPUT);
    pinMode(C_PIN_USB_SEL, OUTPUT);

    //------------------------ INITIALITATION----------------------------
    digitalWrite(C_PIN_OP_SWITCH, LOW);
    digitalWrite(C_PIN_ENABLE_LDO_VCC_2, HIGH);
    attachInterrupt(C_PIN_FLAG_CHARG, Irq_USB_Handler, CHANGE);
    attachInterrupt(C_PIN_FLAG_BATTLOW, Irq_BATTLOW__Handler, FALLING);

    Wire.begin();

    ledmatrix.begin();
    SwitchScreenOff(ledmatrix);
    PowerBar(0, ledmatrix);

    InitBuzzer(C_MODE_DEFAULT);

    capacity = CapacityCheck(C_PIN_V_IN, &flag_low_battery);
    DisplayCap(capacity, ledmatrix);

    if (!Init_diagnostic_elements())
    {
        flag_eeprom_init_fail = true;
    }
    theory_Vout = ReadDiagnosticData(C_RAM_THEORY_VOLTAGE);
    if (theory_Vout == 0)
    {
        LogDiagnosticData(50, C_RAM_THEORY_VOLTAGE);
        theory_Vout = 50;
    }

    //============================================================== TEST MODE ==================================================================//
    for (int i = 0; i < 5; i++)
    {
        test_enable = digitalRead(C_PIN_TEST_MODE);
        if (test_enable == true)
        {
            break;
        }
        delay(1000);
    }
    if (test_enable == true)
    {
        while (1)
        {
            for (int i = 0; i < 8; i++)
            {
                sample_IOut += analogRead(C_PIN_I_OUT);
                sample_Vin += analogRead(C_PIN_V_IN);
                sample_VOut += analogRead(C_PIN_V_OUT);
            }

            sample_IOut = sample_IOut / 8;
            sample_Vin = sample_Vin / 8;
            sample_VOut = sample_VOut / 8;

            digitalRead(C_PIN_FLAG_CHARG);
            digitalRead(C_PIN_SCL_2);
            digitalRead(C_PIN_SDA_2);
            digitalRead(C_PIN_FLAG_BATTLOW);
            digitalWrite(C_PIN_USB_SEL, HIGH); // <--- La idea es ir conmutando el multiplexor y comprobar que el multiplexor funciona, y los pines de la CPU funciona.

            Serial5.print("\t");
            Serial5.print(sample_IOut);
            Serial5.print(";");
            Serial5.print(sample_Vin);
            Serial5.print(";");
            Serial5.print(sample_VOut);
            Serial5.print(";");
            Serial5.print(C_PIN_FLAG_CHARG);
            Serial5.print(";");
            Serial5.print(C_PIN_SCL_2); // ?
            Serial5.print(";");
            Serial5.print(C_PIN_SDA_2); // ?
            Serial5.print(";");
            Serial5.print(C_PIN_FLAG_BATTLOW);
            Serial5.print("\n");
            delay(1000);
        }
    }

    /*===============================================================================================================================================*/
    //                                                                 CONTROL LOOP
    /*===============================================================================================================================================*/
    //t1_text = micros();
    //t1_state = micros();
    //t1_sound = micros();
    while (1)
    {
        t1 = micros();
        //============================================================== SENSADO ==================================================================//
        // t1 = micros();
        under_voltage_protection.threshold = (theory_Vout - 20) * 100; // Actualizacion de la proteccion de undervoltage

        sample_IOut = boost_check.getSample(C_PIN_I_OUT) * 3000 / 4096 * 100 / 259;
        sample_VOut = under_voltage_protection.getSample(C_PIN_V_OUT) * 189 / 39 * 3000 / 4096;
        sample_POut = (sample_IOut) * (sample_VOut) / 1000;
        // t2 = micros();
        // Serial5.printf(" Sensado: %d us \t", t2 - t1);

        //======================================================== DEBBUG POR PUERTO SERIE=========================================================//
        if (debug_serial_timer.poll(100))
        {
            Serial5.print(sw_status);
            Serial5.print("\t");
            Serial5.print(sample_VOut);
            Serial5.print("\t");
            Serial5.print(sample_IOut);
            Serial5.print("\t");
            Serial5.println(sample_POut);
        }

        //========================================================== BOOST MODE MONITOR ===========================================================//
        // t1 = micros();
        if (boost_check.check(sample_IOut) == true)
        {
            output_mode = C_BOOST_MODE;
        }
        else if (boost_check.getCounter() == 0)
        {
            output_mode = C_NON_BOOST_MODE;
        }
        //t2 = micros();
        //Serial5.printf(" Boost: %d us \t", t2 - t1);
        //Serial5.println();

        //============================================================================================================================================
        //                                                              STATE MACHINE
        //============================================================================================================================================

        /*________________________________________________________________ START_UP __________________________________________________________________*/
        if (sw_status == C_SW_ST_START_UP)
        {
            //----------------TIEMPO MINIMO DE ENCENDIDO --------------------//
            //if (flag_encendido == C_TIMER_IDLE)
            //{
            // t1_encendido = micros();
            //timer_encendido.set(2000);
            //flag_encendido = C_TIMER_ARMED;
            //}
            //else if (flag_encendido == C_TIMER_ARMED)
            //{
            //if (timer_encendido.poll() != C_TIMER_NOT_EXPIRED)
            //{
            // flag_encendido = C_TIMER_DONE;
            // t2_encendido = micros();
            // Serial5.printf(" Timer Encendido: %d us \t", t2_encendido - t1_encendido);
            //Serial5.println();
            //}
            //}

            //---------------- TEXTO DE ENCENDIDO --------------------//
            if (enable_starting_text == false)
            {
                flag_msg_init = true;
            }
            else if (flag_msg_init == false)
            {
                display_status = DisplayText(C_TXT_START, ledmatrix, reset_text, mode_bright_display);
                reset_text = false;
                if (display_status == C_DISPLAY_ST_NOT_BUSSY)
                {
                    flag_msg_init = true;
                    reset_text = true;
                    //t2_text = micros();
                    // Serial5.printf(" MSG_init: %d us \t", t2_text - t1_text);
                    // Serial5.println();
                }
            }

            //---------------- SONIDO DE ENCENDIDO --------------------//
            if (enable_starting_sound == false)
            {
                flag_sound_init = true;
            }
            else if (flag_sound_init == false)
            {
                if (enable_starting_text == false)
                {
                    DisplayCap(capacity, ledmatrix);
                }
                playSound(C_SOUND_START);
                flag_sound_init = true;
                //t2_sound = micros();
                //Serial5.printf(" Sound_init: %d us \t", t2_sound - t1_sound);
                //Serial5.println();
            }

            //-------------------- MOSTRADO DE LA CAPACIDAD AL FINAL DEL ENCENDIDO --------------------//
            if ((flag_msg_init == true) && (flag_display_capacity == false) && (flag_sound_init == true))
            {
                //t1_cap = micros();
                flag_display_capacity = true;
                DisplayCap(capacity, ledmatrix); // Actualizacion de la pantalla con la Capacidad
                timer_display_capacity.set(1000);
            }
            if (flag_display_capacity == true) // Mientras este presionado el boton central no se dejara de mostrar la capacidad.
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
                        flag_work = true;
                        //t2_cap = micros();
                        //Serial5.printf(" Cap: %d us \t", t2_cap - t1_cap);
                        //Serial5.println();
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
                //------------- ARRANCADO--------------//
                if (arrancado == false)
                {
                    //t1_encendido = micros();

                    DCDC.SetVoltage(50, C_NON_BOOST_MODE);
                    digitalWrite(C_PIN_OP_SWITCH, HIGH);
                    delay(200);
                    int tiempo_arrancado = 200; // ms
                    int steps = 10;
                    for (int i = 0; i < steps; i++)
                    {
                        DCDC.SetVoltage((theory_Vout - 50) / steps * i + 50, C_NON_BOOST_MODE);
                        delay(tiempo_arrancado / steps);
                    }

                    arrancado = true;
                    //t2_encendido = micros();
                    //Serial5.printf(" Arranque: %d us \t", t2_encendido - t1_encendido);
                    //Serial5.println();
                }
                else if (arrancado)
                {

                    //------------ PROTECCIONES--------------//
                    //t1 = micros();
                    if (mask_protection_state == C_MASK_DEACTIVATE)
                    {

                        /*      Consumption Monitor     */
                        if (over_consumption_protection.check(sample_IOut) == true)
                        {

                            consmptn_event_protection_counter++;
                            if (consmptn_event_protection_counter == MAX_NUM_PROTECTION_EVENTS)
                            {
                                flag_error = true;
                                error = C_ERROR_OVERCURRENT;
                                IncrementDiagnosticData(1, C_RAM_CONSUMPTION_ERROR);
                                over_consumption_protection.setCounter(0);
                            }
                            else
                            {
                                Serial5.printf("OC %d-%d\n\r", consmptn_event_protection_counter, sample_IOut);
                                protection_event_delay.set(100);
                                protection_event_delay_flag = true;
                                over_consumption_protection.setCounter(C_RETRY_750_COUNT);
                                mask_protection_state = C_MASK_ACTIVATE;
                            }
                        }
                        /*      Power Monitor       */
                        if (over_power_protection.check(sample_POut) == true)
                        {

                            OP_event_protection_counter++;
                            if (OP_event_protection_counter == MAX_NUM_PROTECTION_EVENTS)
                            {
                                flag_error = true;
                                error = C_ERROR_OVERPOWER;
                                IncrementDiagnosticData(1, C_RAM_POWER_ERROR);
                                over_power_protection.setCounter(0);
                            }
                            else
                            {
                                Serial5.printf("OP %d\n\r", consmptn_event_protection_counter);
                                protection_event_delay.set(100);
                                protection_event_delay_flag = true;
                                over_power_protection.setCounter(over_power_protection.limit / 2);
                                mask_protection_state = C_MASK_ACTIVATE;
                            }
                        }

                        /*      Voltage Monitor      */
                        if ((under_voltage_protection.check(sample_VOut) == false) && (under_voltage_protection.getCounter() == 0))
                        {

                            UV_event_protection_counter++;
                            if (UV_event_protection_counter == MAX_NUM_PROTECTION_EVENTS)
                            {
                                flag_error = true;
                                error = C_ERROR_OUPUT_UNDERVOLTAGE;
                                IncrementDiagnosticData(1, C_RAM_VOLTAGE_ERROR);
                                under_voltage_protection.setCounter(under_voltage_protection.limit);
                            }
                            else
                            {
                                Serial5.printf("OUV %d\n\r", consmptn_event_protection_counter);
                                protection_event_delay.set(100);
                                protection_event_delay_flag = true;
                                under_voltage_protection.setCounter(under_voltage_protection.limit / 2);
                                mask_protection_state = C_MASK_ACTIVATE;
                            }
                        }
                    }
                    else if (mask_protection_state == C_MASK_ACTIVATE)
                    {
                        if ((protection_event_delay_flag == true) && (protection_event_delay.poll() != C_TIMER_NOT_EXPIRED))
                        {
                            mask_protection_state = C_MASK_DEACTIVATE;
                        }
                    }
                    //t2 = micros();
                    //Serial5.printf(" Protecciones: %d us \t", t2 - t1);
                }
            }

            else if (sw_status == C_SW_ST_STOP) // STOP
            {
                //-------  Checkeo de bateria baja -------//
                if (flag_low_battery == false)
                {
                    if (timer_recover_voltage.poll() != C_TIMER_NOT_EXPIRED)
                    {
                        //t1_cap = micros();
                        flag_stop = true;
                        capacity = CapacityCheck(C_PIN_V_IN, &flag_low_battery);
                        if (flag_low_battery == true)
                        {
                            DisplayCap(capacity, ledmatrix);
                            delay(500);
                        }
                        //t2_cap = micros();
                        //Serial5.printf(" Check Batt Low: %d us \t", t2_cap - t1_cap);
                        //Serial5.println();
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
                            timer_idle.set(30000); // Inicio del contador de 30 seg.
                        }
                    }
                }
                else
                {
                    timer_idle.set(30000); // Incio del contador de 30 seg para el Idle Timer.
                    cont_idle_timer = 0;   // Reset del contador de minutos.
                }
            }

            //--------------- CONTROL DE LOS EVENTOS DE LA BOTONERA ---------------------//
            //t1 = micros();
            if (button_event == C_NONE_EVENT)
            {
                if (flag_timer_low_bright == C_TIMER_ARMED)
                {
                    if (timer_low_bright.poll() != C_TIMER_NOT_EXPIRED) // Si pasan X seg sin accion en la botonera, el brillo disminuye.
                    {
                        mode_bright_display = C_MODE_LOW_BRIGHT;
                        click_events = 0;
                        flag_timer_low_bright = C_TIMER_DONE;
                        trigger_Display_volt = true; // Manda actualizar el display con el nuevo brillo
                        sound = C_SOUND_MUTE;
                        LogDiagnosticData(theory_Vout, C_RAM_THEORY_VOLTAGE);
                        SaveEepromChasis();
                    }
                }
                else if (flag_timer_low_bright == C_TIMER_IDLE)
                {
                    timer_low_bright.set(C_TIME_TO_LIGHT_DOWN);
                    flag_timer_low_bright = C_TIMER_ARMED;
                }
                else if (flag_timer_low_bright == C_TIMER_DONE)
                {
                    mode_bright_display = C_MODE_LOW_BRIGHT;
                }
            }
            else
            {
                sound = C_SOUND_MUTE;
                if (flag_timer_low_bright == C_TIMER_ARMED)
                {
                    timer_low_bright.set(C_TIME_TO_LIGHT_DOWN);
                }
                else if (flag_timer_low_bright == C_TIMER_DONE)
                {
                    flag_timer_low_bright = C_TIMER_IDLE;
                }
                mode_bright_display = C_MODE_HIGH_BRIGHT;

                if (button_event == C_LP_CENTER)
                {
                    click_events++;
                    if (click_events == 1)
                    {
                        if (flag_stop == true)
                        {
                            //t1_text = micros();
                            DisplayCap(capacity, ledmatrix);
                        }
                        else
                        {
                            click_events--;
                        }

                        reset_text = true;
                        mode_bright_display = C_MODE_HIGH_BRIGHT;
                    }
                    else if ((click_events >= 2) && (click_events < 5))
                    {
                        //t2_text = micros();
                        //Serial5.printf(" Fase 1 % : %d us \t", t2_text - t1_text);
                        //Serial5.println();
                        //t1_text = micros();
                        DisplayText(C_TXT_ASK_OFF, ledmatrix, reset_text, mode_bright_display);
                        reset_text = false;
                        if (click_events == 3)
                        {
                            playSound(C_SOUND_UP);
                        }
                    }
                    else if (click_events == 5)
                    {
                        //t2_text = micros();
                        //Serial5.printf(" Fase 2 OFF: %d us \t", t2_text - t1_text);
                        //Serial5.println();
                        flag_sleep = true;
                        click_events = 0;
                    }
                }
                else
                {
                    click_events = 0;
                    //----- Subida de voltaje --------//
                    if (button_event == C_LP_UP)
                    {
                        if (theory_Vout < 160)
                        {
                            theory_Vout += 10;
                            sound = C_SOUND_UP;
                            trigger_Display_volt = true;
                        }
                    }
                    else if (button_event == C_CLICK_UP)
                    {
                        if (theory_Vout < 160)
                        {
                            theory_Vout += 1;
                            sound = C_SOUND_UP;
                            trigger_Display_volt = true;
                        }
                    }
                    //------ Bajada del Voltaje------//
                    else if (button_event == C_LP_DOWN)
                    {
                        if (theory_Vout > 50)
                        {
                            theory_Vout -= 10;
                            sound = C_SOUND_DOWN;
                            trigger_Display_volt = true;
                        }
                    }
                    else if (button_event == C_CLICK_DOWN)
                    {
                        if (theory_Vout > 50)
                        {
                            theory_Vout -= 1;
                            sound = C_SOUND_DOWN;
                            trigger_Display_volt = true;
                        }
                    }
                }

                theory_Vout = constrain(theory_Vout, C_MIN_VOLT, C_MAX_VOLT);
            }
            //t2 = micros();
            //Serial5.printf(" Buttons: %d us \t", t2 - t1);

            //-------------- ACTUALIZACION DEL DISPLAY (CON SONIDO)---------------------//
            //t1 = micros();
            if (trigger_Display_volt == true)
            {
                if (sound != C_SOUND_MUTE)
                {
                    playSound(sound);
                }
                DisplayVolt(theory_Vout, ledmatrix, mode_bright_display);
                trigger_Display_volt = false;
            }
            //t2 = micros();
            //Serial5.printf(" Pantalla: %d us \t", t2 - t1);

            //------- Actualizacion de la barra de potencia ----------//
            //t1 = micros();
            UpdatePowerBar(sample_POut, ledmatrix, C_HIGH_BRIGHTNESS);
            //t2 = micros();
            //Serial5.printf(" PowerBar: %d us \t", t2 - t1);
            //Serial5.println();

            //t2 = micros();
            //Serial5.printf(" Ciclo Standard: %d us \t", t2 - t1);
            // Serial5.println();
        }
        /*________________________________________________________________ SLEEP ____________________________________________________________________*/
        else if (sw_status == C_SW_ST_SLEEP)
        {
            //----------- MENSAJE DE APAGADO --------------
            if (enable_ending_text == false)
            {
                flag_msg_sleep = true;
                SwitchScreenOff(ledmatrix);
                PowerBar(0, ledmatrix);
            }
            else if (flag_msg_sleep == false)
            {
                PowerBar(0, ledmatrix, mode_bright_display);
                display_status = DisplayText(C_TXT_SLEEP, ledmatrix, reset_text, mode_bright_display);
                reset_text = false;
                if (display_status == C_DISPLAY_ST_NOT_BUSSY)
                {
                    flag_msg_sleep = true;
                    SwitchScreenOff(ledmatrix);
                    PowerBar(0, ledmatrix);
                }
            }
            //------------- SONIDO DE APAGADO -------------
            if (enable_ending_sound == false)
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
                if (go_sleep == true)
                {
                    //----------- RUTINA APAGADO ---------------
                    if (flag_irq_center_button == true)
                    {
                        flag_irq_center_button = false; // Limpieza de los flags de interrupcion
                    }
                    SaveEepromChasis();                                                                 // Salvado en EEPROM
                    digitalWrite(C_PIN_ENABLE_LDO_VCC_2, LOW);                                          // Apagado de la alimentacion secundaria.
                    LowPower.attachInterruptWakeup(C_PIN_BUTT_CENTER, IrqCenterButtonHandler, FALLING); // Activacion de la interrupcion de despertar
                    //t2 = micros();
                    LowPower.sleep();
                    //Serial5.printf(" Apagado: %d us \t", t2 - t1);
                    //Serial5.println();
                    //t1_state = micros();
                    //---------- RUTINA DE DESPERTAR-----------------
                    detachInterrupt(C_PIN_BUTT_CENTER);         // Desactivar interrupcion
                    digitalWrite(C_PIN_ENABLE_LDO_VCC_2, HIGH); // Encender alimentacion secundaria.
                    // Comprobar causante del despertar.
                    if (flag_irq_center_button == true)
                    {
                        go_sleep = false;
                        timer_irq_button_center.set(1200);
                    }
                }
                else if (go_sleep == false)
                {
                    //--- Comprobacion del boton central-----
                    if (flag_irq_center_button == true)
                    {
                        if (timer_irq_button_center.poll() != C_TIMER_NOT_EXPIRED)
                        {
                            go_sleep = true;
                        }
                        else
                        {
                            if (button_event == C_LP_CENTER) // Si se mantiene apretado el botoncentral desde el despertar se inicia la bateria
                            {
                                flag_initialize = true;
                                //t2_state = micros();
                                //Serial5.printf(" Wake up: %d us \t", t2_state - t1_state);
                                //Serial5.println();
                            }
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
                    DisplayText("OP", ledmatrix, false, C_HIGH_BRIGHTNESS);
                    break;
                case C_ERROR_OVERCURRENT:
                    DisplayText("OC", ledmatrix, false, C_HIGH_BRIGHTNESS);
                    break;
                case C_ERROR_OUPUT_UNDERVOLTAGE:
                    DisplayText("OUV", ledmatrix, false, C_HIGH_BRIGHTNESS);
                    break;
                case C_ERROR_INPUT_UNDERVOLTAGE:
                    DisplayText("IUV", ledmatrix, false, C_HIGH_BRIGHTNESS);
                    break;
                default:
                    DisplayError(ledmatrix);
                    break;
                }
                if (timer_display_error.poll() != C_TIMER_NOT_EXPIRED)
                {
                    timer_info_error.set(1000);
                }
                if (timer_info_error.poll() != C_TIMER_NOT_EXPIRED)
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
                // -------------- GUARDADO EN EEPROM -----------------
                SaveEepromChasis();
                UpdateEepromBatery();
            }
        }

        /*___________________________________________________________________ USB ____________________________________________________________________*/
        else if (sw_status == C_SW_ST_USB) //
        {
            /*      Serial Port Looking For     */
            if (diag_check == false)
            {
                user_output = C_OFF;
                if (Serial.available())
                {
                    data = Serial.readString();
                    data.trim();
                    Serial.print(data);
                    if (data == C_DIAGNOSTIC_PASSWORD)
                    {
                        print_diagnostic_static_data = true;
                        timer_print_diagnostic.set(500);
                        data = "";
                        diag_check = true;
                        mode_bright_display = C_MODE_LOW_BRIGHT;
                        PowerBar(0, ledmatrix, mode_bright_display);
                        DisplayDiagnosticMode(ledmatrix);
                    }
                }
            }
            else if (diag_check == true)
            {
                if (print_diagnostic_static_data == true)
                {
                    user_output = C_OFF;
                    if (counter_prints_static_data < C_NUMB_PRINTS_STATIC_DATA)
                    {
                        if (timer_print_diagnostic.poll() != C_TIMER_NOT_EXPIRED)
                        {
                            timer_print_diagnostic.set(500);
                            PrintStaticData();
                            counter_prints_static_data++;
                        }
                    }
                    else
                    {
                        counter_prints_static_data = 0;
                        print_diagnostic_static_data = false;
                    }
                }
                else if (print_diagnostic_static_data == false)
                {
                    if (enable_live_view == false)
                    {
                        user_output = C_OFF;
                        if (Serial.available())
                        {
                            data = Serial.readString();
                            data.trim();
                            Serial.println(data);
                            if (data == C_CMD_DISPLAY_STARTING_OFF)
                            {
                                enable_starting_text = false;
                                LogDiagnosticData(enable_starting_text, C_RAM_HACK_START_DISPLAY);
                                Serial.println("Starting Text Disable.");
                            }
                            else if (data == C_CMD_DISPLAY_STARTING_ON)
                            {
                                enable_starting_text = true;
                                LogDiagnosticData(enable_starting_text, C_RAM_HACK_START_DISPLAY);
                                Serial.println("Starting Text Enable");
                                Serial.println("Change Text?");
                                Serial.print("Actual:");
                                Serial.println(C_TXT_START);
                                while (change_text_answered == false)
                                {
                                    if (Serial.available())
                                    {
                                        data = Serial.readString();
                                        data.trim();
                                        Serial.println(data);
                                        if (data == "yes")
                                        {
                                            Serial.println("Insert new starting Text:");
                                            afk_timer.set(30000);
                                            while (new_text_received == false)
                                            {
                                                if (afk_timer.poll() != C_TIMER_NOT_EXPIRED)
                                                {
                                                    afk_counter++;
                                                    if (afk_counter == (5 * 2))
                                                    {
                                                        new_text_received = true;
                                                    }
                                                }

                                                if (Serial.available())
                                                {
                                                    data = Serial.readString();
                                                    data.trim();
                                                    Serial.println(data);
                                                    C_TXT_START = data;
                                                    new_text_received = true;
                                                }
                                            }
                                            new_text_received = false;
                                            change_text_answered = true;
                                        }
                                        else if (data == "no")
                                        {
                                            change_text_answered = true;
                                        }
                                    }
                                }
                                change_text_answered = false;
                            }
                            else if (data == C_CMD_DISPLAY_ENDING_OFF)
                            {
                                enable_ending_text = false;
                                LogDiagnosticData(enable_ending_text, C_RAM_HACK_END_DISPLAY);
                                Serial.println("Ending Text Disable");
                            }
                            else if (data == C_CMD_DISPLAY_ENDING_ON)
                            {
                                enable_ending_text = true;
                                LogDiagnosticData(enable_ending_text, C_RAM_HACK_END_DISPLAY);
                                Serial.println("Ending Text Enable");
                                Serial.println("Change Text?");
                                Serial.print("Actual:");
                                Serial.println(C_TXT_SLEEP);
                                while (change_text_answered == false)
                                {
                                    if (Serial.available())
                                    {
                                        data = Serial.readString();
                                        data.trim();
                                        Serial.println(data);
                                        if (data == "yes")
                                        {
                                            Serial.println("Insert new ending Text:");
                                            afk_timer.set(30000);
                                            while (new_text_received == false)
                                            {
                                                if (afk_timer.poll() != C_TIMER_NOT_EXPIRED)
                                                {
                                                    afk_counter++;
                                                    if (afk_counter == (5 * 2))
                                                    {
                                                        new_text_received = true;
                                                    }
                                                }

                                                if (Serial.available())
                                                {
                                                    data = Serial.readString();
                                                    data.trim();
                                                    Serial.println(data);
                                                    C_TXT_SLEEP = data;
                                                    new_text_received = true;
                                                }
                                            }
                                            new_text_received = false;
                                            change_text_answered = true;
                                        }
                                        else if (data == "no")
                                        {
                                            change_text_answered = true;
                                        }
                                    }
                                }
                                change_text_answered = false;
                            }
                            else if (data == C_CMD_SOUND_STARTING_OFF)
                            {
                                enable_starting_sound = false;
                                LogDiagnosticData(enable_starting_sound, C_RAM_HACK_START_SOUND);
                                Serial.println("Starting Sound Disable");
                            }
                            else if (data == C_CMD_SOUND_STARTING_ON)
                            {
                                enable_starting_sound = true;
                                LogDiagnosticData(enable_starting_sound, C_RAM_HACK_START_SOUND);
                                Serial.println("Staring Sound Enable");
                            }
                            else if (data == C_CMD_SOUND_ENDING_OFF)
                            {
                                enable_ending_sound = false;
                                LogDiagnosticData(enable_ending_sound, C_RAM_HACK_END_SOUND);
                                Serial.println("Ending Sound Disable");
                            }
                            else if (data == C_CMD_SOUND_ENDING_ON)
                            {
                                enable_ending_sound = true;
                                LogDiagnosticData(enable_ending_sound, C_RAM_HACK_END_SOUND);
                                Serial.println("Ending Sound Enable");
                            }
                            else if (data == C_CMD_SOUND_FULL_CHARGE_OFF)
                            {
                                enable_full_charge_sound = false;
                                LogDiagnosticData(enable_full_charge_sound, C_RAM_HACK_FULL_CHARGE_SOUND);
                                Serial.println("Received and Applied");
                            }
                            else if (data == C_CMD_SOUND_FULL_CHARGE_ON)
                            {
                                enable_full_charge_sound = true;
                                LogDiagnosticData(enable_full_charge_sound, C_RAM_HACK_FULL_CHARGE_SOUND);
                                Serial.println("Received and Applied");
                            }
                            else if (data == C_CMS_SOUND_CHARGE_START_OFF)
                            {
                                enable_charge_sound = true;
                                LogDiagnosticData(enable_charge_sound, C_RAM_HACK_CHARGE_SOUND);
                                Serial.println("Received and Applied");
                            }
                            else if (data == C_CMS_SOUND_CHARGE_START_ON)
                            {
                                enable_charge_sound = true;
                                LogDiagnosticData(enable_charge_sound, C_RAM_HACK_CHARGE_SOUND);
                                Serial.println("Received and Applied");
                            }
                            else if (data == C_CMD_LIVE_ON)
                            {
                                enable_live_view = true;
                                delay_live_view.set(1000);
                                Serial.println("Received and Applied");
                            }
                            else
                            {
                                Serial.println("Command No identified");
                            }
                        }
                    }
                    else if (enable_live_view == true)
                    {
                        if (delay_live_view.poll() != C_TIMER_NOT_EXPIRED)
                        {
                            delay_live_view.set(1000);
                            PrintDiagnosticData();
                        }

                        if (Serial.available())
                        {
                            data = Serial.readString();
                            data.trim();
                            Serial.println(data);
                            if (data == C_LIVE_CMD_OUT_ON)
                            {
                                user_output = C_ON;
                            }
                            else if (data == C_LIVE_CMD_OUT_OFF)
                            {
                                user_output = C_OFF;
                            }
                            else if (data == C_LIVE_CMD_VOLT_UP_1)
                            {
                                theory_Vout += 1;
                            }
                            else if (data == C_LIVE_CMD_VOLT_UP_10)
                            {
                                theory_Vout += 10;
                            }
                            else if (data == C_LIVE_CMD_VOLT_DOWN_1)
                            {
                                theory_Vout -= 1;
                            }
                            else if (data == C_LIVE_CMD_VOLT_DOWN_10)
                            {
                                theory_Vout -= 10;
                            }
                            else if (data == C_LIVE_CMD_OFF_LIVE_VIEW)
                            {
                                enable_live_view = false;
                            }
                        }
                    }
                }
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
            if (flag_work == true)
            {
                //t1 = micros();
                sw_status = C_SW_ST_STOP;

                sw_output = C_OFF;
                user_output = C_OFF;

                trigger_Display_volt = true;
                timer_idle.set(30000);
                timer_recover_voltage.set(1000);
                //t2 = micros();
                //Serial5.printf(" Stup -> Stop: %d us \t", t2 - t1);
                //Serial5.println();
            }
            //----------- TO USB -----------
            if (usb_status == C_USB_CONNECTED)
            {
                if (enable_charge_sound == true)
                {
                    playSound(C_SOUND_CHARGE_IN);
                }
                sw_output = C_ON;
                sw_status = C_SW_ST_USB;
                DisplayUsbIn(ledmatrix);
            }
        }
        /*________________________________________________________________ RUN __________________________________________________________________*/
        else if (sw_status == C_SW_ST_RUN)
        {
            //----------- TO STOP -----------
            if ((button_event == C_CLICK_CENTER) || (button_event == C_LP_CENTER))
            {
                //t1 = micros();
                sw_status = C_SW_ST_STOP;

                user_output = C_OFF;
                sw_output = C_OFF;
                playSound(C_SOUND_OFF);
                timer_idle.set(30000);
                flag_low_battery = false;
                timer_recover_voltage.set(1000);
                //t2 = micros();
                //Serial5.printf(" Run -> Stop: %d us \t", t2 - t1);
                // Serial5.println();
            }
            ///----------- TO USB -----------
            if (usb_status == C_USB_CONNECTED)
            {
                sw_status = C_SW_ST_USB;

                sw_output = C_ON;

                if (enable_charge_sound == true)
                {
                    playSound(C_SOUND_CHARGE_IN);
                }

                DisplayUsbIn(ledmatrix);
            }
            //----------- TO ERROR -----------
            if (flag_error == true)
            {
                sw_output = C_OFF;
                digitalWrite(C_PIN_OP_SWITCH, LOW);
                sw_status = C_SW_ST_ERROR;
                display_error_status = C_DISPLAY_ST_BUSSY;
                timer_display_error.set(500);
                SwitchScreenOff(ledmatrix);
                PowerBar(0, ledmatrix, mode_bright_display);
                mode_bright_display = C_MODE_HIGH_BRIGHT;
                playSound(C_SOUND_ERROR);
                switch (error)
                {
                case C_ERROR_OVERPOWER:
                    DisplayText("OP", ledmatrix, true, C_HIGH_BRIGHTNESS);
                    Serial5.printf("OP ERROR\n\r");
                    break;
                case C_ERROR_OVERCURRENT:
                    DisplayText("OC", ledmatrix, true, C_HIGH_BRIGHTNESS);
                    Serial5.printf("OC ERROR\n\r");
                    break;
                case C_ERROR_OUPUT_UNDERVOLTAGE:
                    DisplayText("OUV", ledmatrix, true, C_HIGH_BRIGHTNESS);
                    Serial5.printf("OUV ERROR\n\r");
                    break;
                case C_ERROR_INPUT_UNDERVOLTAGE:
                    DisplayText("IUV", ledmatrix, true, C_HIGH_BRIGHTNESS);
                    Serial5.printf("IUV ERROR\n\r");
                    break;
                default:
                    DisplayError(ledmatrix);
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
                //t1 = micros();
                flag_stop = false;
                sw_output = C_ON;
                user_output = C_ON; // Activacion de la salida.
                sw_status = C_SW_ST_RUN;
                playSound(C_SOUND_ON);

                //t2 = micros();
                //Serial5.printf(" Stop -> Run: %d us \t", t2 - t1);
                //Serial5.println();
                //digitalWrite(C_PIN_PROFILE_BUZZER, LOW);
            }
            //----------- TO USB -----------
            if (usb_status == C_USB_CONNECTED)
            {
                if (enable_charge_sound == true)
                {
                    playSound(C_SOUND_CHARGE_IN);
                }
                sw_output = C_ON;
                sw_status = C_SW_ST_USB;
                DisplayUsbIn(ledmatrix);
            }
            //----------- TO SLEEP -----------
            if (flag_sleep == true)
            {
                //t1 = micros();
                go_sleep = true;
                sw_output = C_OFF;
                sw_status = C_SW_ST_SLEEP;
                flag_msg_sleep = true;
                flag_sound_end = true;
                flag_sleep = false;
                mode_bright_display = C_MODE_HIGH_BRIGHT;
                //t2 = micros();
                //Serial5.printf(" Stop -> Sleep: %d us \t", t2 - t1);
                //Serial5.println();
            }
        }
        /*________________________________________________________________ SLEEP __________________________________________________________________*/
        else if (sw_status == C_SW_ST_SLEEP)
        {
            //----------- TO START_UP -----------
            if (flag_initialize == true)
            {
                //t1 = micros();
                sw_output = C_OFF;
                flag_waiting = C_TIMER_IDLE;
                sw_status = C_SW_ST_START_UP;
                reset_text = true;
                flag_msg_sleep = false;
                flag_sound_init = false;
                flag_initialize = false;

                ledmatrix.begin(); // Inicializar pantalla de leds.
                SwitchScreenOff(ledmatrix);
                PowerBar(0, ledmatrix);
                trigger_Display_volt = true;
                capacity = CapacityCheck(C_PIN_V_IN, &flag_low_battery);
                //flag_encendido = C_TIMER_IDLE;
                flag_msg_init = false;
                flag_work = false;
                flag_display_capacity = false;
                flag_sound_init = false;
                //t2 = micros();
                //Serial5.printf(" Sleep -> Start up: %d us \t", t2 - t1);
                //Serial5.println();
            }
            //----------- TO USB -----------
            if (usb_status == C_USB_CONNECTED)
            {
                if (enable_charge_sound == true)
                {
                    playSound(C_SOUND_CHARGE_IN);
                }
                sw_output = C_ON;
                sw_status = C_SW_ST_USB;
                DisplayUsbIn(ledmatrix);
            }
        }
        /*________________________________________________________________ ERROR __________________________________________________________________*/
        else if (sw_status == C_SW_ST_ERROR)
        {
            //----------- TO STOP -----------
            if (flag_error == false)
            {
                sw_output = C_OFF;
                user_output = C_OFF;
                sw_status = C_SW_ST_STOP;
                timer_idle.set(30000);
                timer_recover_voltage.set(1000);
            }
        }
        /*________________________________________________________________ USB __________________________________________________________________*/
        else if (sw_status == C_SW_ST_USB)
        {
            //----------- TO SLEEP -----------
            if (usb_status == C_USB_DISCONNECTED)
            {
                sw_output = C_OFF;
                digitalWrite(C_PIN_OP_SWITCH, LOW);
                if (enable_charge_sound == true)
                {
                    playSound(C_SOUND_CHARGE_OUT);
                }
                DisplayUsbOut(ledmatrix);
                sw_status = C_SW_ST_SLEEP;
                flag_msg_sleep = true;
                flag_sound_end = true;
                diag_check = false;
                mode_bright_display = C_MODE_HIGH_BRIGHT;
                go_sleep = true;
            }
        }
        //=============================================================================================================================================
        //                                                              END STATE MANAGEMENT
        //=============================================================================================================================================

        //============================================================== CHECKEO USB STATE ===========================================================//
        //t1 = micros();
        if (flag_usb_change == true)
        {
            delay(100);
            if (usb_status == C_USB_DISCONNECTED)
            {
                usb_status = C_USB_CONNECTED;
            }
            else if (usb_status == C_USB_CONNECTED)
            {
                usb_status = C_USB_DISCONNECTED;
            }
            flag_usb_change = false;
        }
        //t2 = micros();
        //Serial5.printf(" USB: %d us \t", t2 - t1);
        //============================================================== OUTPUT MANAGEMENT ===========================================================//
        //t1 = micros();
        if (sw_output == C_OFF)
        {
            hw_output = C_OFF;
        }
        else
        {
            if (user_output == C_ON)
            {
                hw_output = C_ON;
            }
            else
            {
                hw_output = C_OFF;
            }
        }
        if (hw_output == C_ON)
        {
            theory_Vout = constrain(theory_Vout, C_MIN_VOLT, C_MAX_VOLT);
            DCDC.SetVoltage(theory_Vout, output_mode);
            digitalWrite(C_PIN_OP_SWITCH, HIGH);
            LedWork(C_ON, ledmatrix);
        }
        else if (hw_output == C_OFF)
        {
            digitalWrite(C_PIN_OP_SWITCH, LOW);
            consmptn_event_protection_counter = 0;
            OP_event_protection_counter = 0;
            UV_event_protection_counter = 0;
            //LedWork(C_OFF, ledmatrix, mode_bright_display);
            UpdatePowerBar(sample_POut, ledmatrix, mode_bright_display);
        }
        //t2 = micros();
        //Serial5.printf(" Output: %d us \t", t2 - t1);
        //======================================================= ACTUALIZACION DEL DIAGNOSTICO ===========================================================//
        //t1 = micros();
        LogDiagnosticData(theory_Vout, C_RAM_THEORY_VOLTAGE);
        LogDiagnosticData(sample_VOut, C_RAM_INSTANT_VOLTAGE);
        LogDiagnosticData(sample_IOut, C_RAM_INSTANT_CURRENT);
        LogDiagnosticData(sample_POut, C_RAM_INSTANT_POWER);
        LogDiagnosticData(capacity, C_RAM_CAPACITY);
        // t2 = micros();
        //Serial5.printf(" Logging: %d us \t", t2 - t1);
        //============================================================ TIMER LOGEO EEPROM  =============================================================//
        if (timer_sec_count.poll(1000) != C_TIMER_NOT_EXPIRED)
        {
            cont_sec++;
            if (cont_sec == C_EEPROM_UPDATE_TIME * 60)
            {
                //t1 = micros();

                if (flag_eeprom_init_fail == true)
                {
                    while (!Init_diagnostic_elements())
                        ;
                    flag_eeprom_init_fail = false;
                }
                else
                {
                    Serial5.println("Updateando EEPROM..");
                    IncrementDiagnosticData(1, C_RAM_WORK_TIME);
                    Serial5.println("Battery..");
                    if (!UpdateEepromBatery())
                    {
                        Serial5.println("Fail Update.");
                        flag_eeprom_upgrade_fail = true;
                    }
                    Serial5.println("Chasis...");
                    SaveEepromChasis();
                    Serial5.println("EEPROM Actualizada.");
                }
                cont_sec = 0;
                //t2 = micros();
                //Serial5.printf(" EEPROM: %d us \t", t2 - t1);
            }
            if ((cont_sec == 60) && (flag_eeprom_upgrade_fail))
            {
                if (!UpdateEepromBatery())
                {
                    Serial5.println("Fail Update.");
                    flag_eeprom_upgrade_fail = true;
                }
                else
                {
                    flag_eeprom_upgrade_fail = false;
                }
            }
        }

        //====================================================== LECTURA DE EVENTOS DE LA BOTONERA ========================================================//
        //t1 = micros();
        button_event = ReadDirPad();
        //t2 = micros();
        //Serial5.printf(" Buttons: %d us \t", t2 - t1);
        //Serial5.println();

        //====================================================== Control del tiempo del ciclo de control ========================================================//

        t2 = micros();
        t2_state = micros();
        while ((t2 - t1) < C_PROGRAM_CYCLE_PERIOD)
        {
            t2 = micros();
        }
        // Serial5.printf(" Ciclo de programa: %d / %d  us \t", t2_state - t1, t2 - t1);
        // Serial5.println();
    }
}

void loop()
{
}

void IrqCenterButtonHandler()
{
    flag_irq_center_button = true;
}
void Irq_BATTLOW__Handler()
{
    digitalWrite(C_PIN_OP_SWITCH, LOW);
    flag_error = true;
    error = C_ERROR_INPUT_UNDERVOLTAGE;
}
void Irq_USB_Handler()
{
    flag_usb_change = true;
}

/**
 * @brief 
 * 
 * @param pin_battery 
 * @param lowbattery 
 * @return int16_t 
 */
int16_t CapacityCheck(uint16_t pin_battery, bool *lowbattery)
{
    int16_t percent = 0;
    bool flag_low_batt_display = false;
    bool flag_low_batt_sound = false;
    int32_t batt_voltage = 0;
    int16_t sample = 0;
    analogReadCorrection(12, 2055);
    analogReadResolution(12);

    for (int i = 0; i < 8; i++)
    {
        sample += analogRead(pin_battery);
    }

    batt_voltage = sample / 8 * 3000 / 4096 * 167 / 100;

    percent = constrain(((batt_voltage - 3500) * 100 / 6 + 00), 0, 100);

    if (percent <= C_LOW_BATTERY_LEVEL)
    {
        *lowbattery = true;
        while ((flag_low_batt_display == false) || (flag_low_batt_sound == false))
        {
            if ((DisplayLowBattery(ledmatrix) != C_DISPLAY_ST_BUSSY) && (flag_low_batt_display == false))
            {
                flag_low_batt_display = true;
            }
            if (flag_low_batt_sound == false)
            {
                playSound(C_SOUND_LOW_BATTERY);
                flag_low_batt_sound = true;
            }
        }
    }
    else
    {
        *lowbattery = false;
    }
    return percent;
}