/**
 * @file B2.ino
 * @author Javi (Javier@musotoku.com)
 * @brief 
 *      V1: Fork de la version B1_V11-RC1. *      
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
const uint16_t C_PIN_ENABLE_LDO_VCC_2 = 1;
//const uint16_t C_PIN_FLAG_CHARG = 5;
const uint16_t C_PIN_OP_SWITCH = 13;
//const uint16_t C_PIN_USB_SEL = 7;
//const uint16_t C_PIN_TEST_MODE = 9;
//const uint16_t C_PIN_SDA_2 = 10;
const uint16_t C_PIN_EN_DCDC = 11;
//const uint16_t C_PIN_SCL_2 = 13;

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
const int32_t C_TIME_IDLE_30_SEG = 30000;  // 3 s
const int32_t C_TIME_INIT_SCREEN = 1000;   // 1 s

const uint32_t C_WATCH_DOG_TIME_MS = 1000;

const String C_DIAGNOSTIC_PASSWORD = "FPO";

uint8_t C_TXT_SLEEP = 2;
uint8_t C_TXT_ASK_OFF = 2;

const bool C_USB_CONNECTED = true;
const bool C_USB_DISCONNECTED = false;

const uint8_t C_NUMB_PRINTS_STATIC_DATA = 5;

const uint8_t C_LOW_BATTERY_LEVEL = 15;  // Porcentaje de bateria  partir del cual no se puede asegurar un correcto funcionamiento.
const uint8_t C_EMPTY_BATTERY_LEVEL = 5; // Porcentaje de bateria  partir del cual no se permite encender la bateria.

const uint16_t C_IDLE_TIMER_COUNT = 15 * 2; // 15 Minutos para que se apage la bateria por inactividad.
                                            // NOTE: El contador es de 30seg, por eso se multiplica por 2-

const uint16_t C_EEPROM_UPDATE_TIME = 15; //min

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

//--------------------------------------- Timers variables-------------------------------------
MilliTimer timer_encendido;

dcdc_controler DCDC(C_PIN_EN_DCDC);
MilliTimer protection_event_delay;
MilliTimer timer_diagnostic_querist;
MilliTimer timer_sec_count;
MilliTimer timer_print_diagnostic;
MilliTimer timer_display_capacity;
MilliTimer timer_recover_voltage;
MilliTimer timer_wait_sleep;
MilliTimer timer_irq_button_center;
MilliTimer timer_idle;
MilliTimer timer_display_error;
MilliTimer timer_info_error;
MilliTimer debug_serial_timer;
MilliTimer timer_retry_upgrade_eeprom;
MilliTimer timer_init_screen;
MilliTimer timer_end_screen;
MilliTimer timer_refresh_screen;
MilliTimer timer_log_sec;
MilliTimer timer_active;

//--------------------------------------- States variables-------------------------------------
int16_t sw_status = C_SW_ST_SLEEP;
int16_t sound = C_SOUND_MUTE;
int16_t button_event = C_NONE_EVENT;
bool usb_status = C_USB_DISCONNECTED;
bool sw_output = C_OFF, hw_output = C_OFF, user_output = C_OFF;
bool output_mode = C_NON_BOOST_MODE;
bool switch_on = true;

//--------------------------------------- Counters variables-------------------------------------
int32_t cont_sec = 0;
int16_t consmptn_event_protection_counter = 0;
int16_t OP_event_protection_counter = 0;
int16_t UV_event_protection_counter = 0;
uint16_t click_events = 0;
int16_t cont_idle_timer = 0;
uint32_t cont_log_sec = 0;
uint32_t cont_active = 0;
//--------------------------------------- Diagnostics variables-------------------------------------
uint16_t sample_IOut = 0;
uint16_t sample_VOut = 0;
uint16_t sample_POut = 0;
uint16_t sample_VIN = 0;
uint16_t theory_Vout = 50;
uint16_t capacity = 0;
uint8_t counter_prints_static_data = 0;
uint8_t afk_counter = 0;
String data;
bool enable_starting_text = true;
bool enable_starting_sound = false;
bool enable_ending_text = true;
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
bool flag_empty_battery = false;

bool reset_text = true;

bool protection_event_delay_flag = false;
bool mask_protection_state = C_MASK_DEACTIVATE;
volatile bool flag_error = false;

bool display_status = C_DISPLAY_ST_NOT_BUSSY;
bool display_error_status = C_DISPLAY_ST_NOT_BUSSY;
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
    Serial5.begin(57600);
    Serial5.println("START!\n\r");
    //------------------------- INIT PIN-------------------------------
    pinMode(C_PIN_BUTT_CENTER, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_UP, INPUT_PULLUP);
    pinMode(C_PIN_BUTT_DOWN, INPUT_PULLUP);
    //pinMode(C_PIN_FLAG_CHARG, INPUT_PULLUP);
    //pinMode(C_PIN_TEST_MODE, INPUT_PULLUP);
    pinMode(C_PIN_OP_SWITCH, OUTPUT);
    pinMode(C_PIN_ENABLE_LDO_VCC_2, OUTPUT);

    //pinMode(C_PIN_SCL_2, OUTPUT);
    //pinMode(C_PIN_SDA_2, OUTPUT);
    //pinMode(C_PIN_USB_SEL, OUTPUT);

    //------------------------ INITIALITATION PERIFERICOS ----------------------------
    Wire.begin();

    digitalWrite(C_PIN_OP_SWITCH, HIGH);        // Interruptor Salida
    digitalWrite(C_PIN_ENABLE_LDO_VCC_2, HIGH); // Encendido del DCDC
    InitBuzzer(C_MODE_DEFAULT);                 // Inicializacion del Buzzer
    initDisplay();                              // Inicializacion de la pantalla
    //DisplayLogo();
    if (!Init_local_eeprom()) // Incializacion EEPROM
    {
        flag_eeprom_init_fail = true;
    }

    //------------------------ CALCULO INICIAL CAPACIDAD ----------------------------
    capacity = CapacityCheck(C_PIN_V_IN, &flag_low_battery, &flag_empty_battery);
    for (int i = 0; i <= capacity; i += 1)
    {
        DisplayCap(i);
        delay(200 / i);
    }
    //------------------------ SETEO DEL VOLTAJE ANTERIOR----------------------------
    theory_Vout = ReadDiagnosticData(C_THEORY_VOLTAGE);
    if (theory_Vout == 0)
    {
        LogDiagnosticData(50, C_THEORY_VOLTAGE);
        theory_Vout = 50;
    }

    playSound(C_SOUND_UP);
    delay(1000);
    SwitchScreenOff();

    /*===============================================================================================================================================*/
    //                                                                 CONTROL LOOP
    /*===============================================================================================================================================*/
    while (1)
    {
        t1 = micros(); // Control del periodo del ciclo de trabajpo.

        //============================================================== SENSADO ==================================================================//

        under_voltage_protection.threshold = (theory_Vout - 20) * 100;                          // Actualizacion de la proteccion de undervoltage
        sample_IOut = boost_check.getSample(C_PIN_I_OUT) * 3000 / 4096 * 100 / 259;             // Lectura de la Corriente de Salida
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
            if (enable_starting_sound == false)
            {
                flag_sound_init = true;
            }
            else if (flag_sound_init == false)
            {
                playSound(C_SOUND_START);
                flag_sound_init = true;
            }
            //---------------- TEXTO DE ENCENDIDO --------------------//
            if (enable_starting_text == false)
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
            if ((flag_msg_init == true) && (flag_display_capacity == false) && (flag_sound_init == true))
            {
                //t1_cap = micros();
                flag_display_capacity = true;
                capacity = CapacityCheck(C_PIN_V_IN, &flag_low_battery, &flag_empty_battery);
                DisplayCap(capacity); // Actualizacion de la pantalla con la Capacidad
                timer_display_capacity.set(1000);
            }
            else if (flag_display_capacity == true) // Mientras este presionado el boton central no se dejara de mostrar la capacidad.
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

                    DCDC.SetVoltage(50, C_NON_BOOST_MODE);
                    digitalWrite(C_PIN_OP_SWITCH, LOW);
                    delay(50);
                    int tiempo_arrancado = 200; // ms
                    int tiempo_bajada = 60;     // ms
                    int steps_subida = 10;
                    int steps_bajada = 10;
                    for (int i = 0; i < steps_subida; i++)
                    {
                        DCDC.SetVoltage((120 - 50) / steps_subida * i + 50, C_NON_BOOST_MODE);
                        delay(tiempo_arrancado / steps_subida);
                    }
                    for (int i = steps_bajada; i >= 0; i--)
                    {
                        DCDC.SetVoltage((120 - theory_Vout) / steps_bajada * i + theory_Vout, C_NON_BOOST_MODE);
                        delay(tiempo_bajada / steps_bajada);
                    }

                    DCDC.SetVoltage(theory_Vout, C_NON_BOOST_MODE);
                    arrancado = true;
                }
                else if (arrancado)
                {
                    //------------ TIMER ACTIVE -------------//
                    if (timer_active.poll(1000) != C_TIMER_NOT_EXPIRED)
                    {
                        cont_active++;
                        if (cont_active == 60)
                        {
                            PostMortemLog(sample_POut, capacity, theory_Vout, 0);
                            cont_active = 0;
                        }
                        sample_VIN = analogRead(C_PIN_V_IN) * 3000 / 4096 * 250 / 150;
                        Serial5.printf("Vin:%d\n\r", sample_VIN);
                        if (sample_VIN <= 3300)
                        {
                            DisplayLowBattery();
                            playSound(C_SOUND_LOW_BATTERY);
                        }
                    }

                    //------------ PROTECCIONES--------------//
                    if (mask_protection_state == C_MASK_DEACTIVATE)
                    {

                        /*
                        //      Consumption Monitor     //
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
                                protection_event_delay.set(100);
                                protection_event_delay_flag = true;
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
                        */
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
                        capacity = CapacityCheck(C_PIN_V_IN, &flag_low_battery, &flag_empty_battery);
                        if (flag_low_battery == true)
                        {
                            DisplayCap(capacity);
                            delay(750);
                            trigger_Display_volt = true;
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
            if (button_event == C_NONE_EVENT)
            {
                if (timer_refresh_screen.poll() != C_TIMER_NOT_EXPIRED)
                {
                    trigger_Display_volt = true;
                    click_events = 0;
                }
            }
            else
            {
                sound = C_SOUND_MUTE;

                if (button_event == C_LP_CENTER)
                {
                    Serial5.printf("Longpress CENTER");
                    click_events++;
                    timer_refresh_screen.set(1000);
                    if (click_events == 1)
                    {
                        if (flag_stop == true)
                        {
                            DisplayCap(capacity);
                        }
                        else
                        {
                            click_events--;
                        }
                    }
                    else if ((click_events >= 2) && (click_events < 5))
                    {
                        OLED_display.clearDisplay();
                        OLED_display.setCursor(16, 0);
                        OLED_display.setTextSize(2);
                        OLED_display.print("OFF");
                        OLED_display.setCursor(0, 16);
                        OLED_display.setTextSize(2);
                        switch (click_events)
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
                        playSound(C_SOUND_UP);
                    }
                    else if (click_events == 5)
                    {
                        flag_sleep = true;
                        click_events = 0;
                    }
                }
                else
                {
                    //----- Subida de voltaje --------//
                    if (button_event == C_LP_UP)
                    {
                        Serial5.printf("Longpress UP");
                        if (theory_Vout < MAX_VOLTAGE)
                        {
                            theory_Vout += 10;
                            sound = C_SOUND_UP;
                            trigger_Display_volt = true;
                        }
                    }
                    else if (button_event == C_CLICK_UP)
                    {
                        Serial5.printf("Click UP");
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
                        Serial5.printf("Longpress DOWN");
                        if (theory_Vout > MIN_VOLTAGE)
                        {
                            theory_Vout -= 10;
                            sound = C_SOUND_DOWN;
                            trigger_Display_volt = true;
                        }
                    }
                    else if (button_event == C_CLICK_DOWN)
                    {
                        Serial5.printf("Click DOWN");
                        if (theory_Vout > MIN_VOLTAGE)
                        {
                            theory_Vout -= 1;
                            sound = C_SOUND_DOWN;
                            trigger_Display_volt = true;
                        }
                    }
                }

                theory_Vout = constrain(theory_Vout, MIN_VOLTAGE, MAX_VOLTAGE);
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

            //------- Actualizacion de la barra de potencia ----------//
            sample_POut += 10;
            if (sample_POut == MAX_POWER_DISPLAYED)
            {
                sample_POut = 0;
            }

            UpdatePowerBar(sample_POut);
        }
        /*________________________________________________________________ SLEEP ____________________________________________________________________*/
        else if (sw_status == C_SW_ST_SLEEP)
        {
            if (switch_on)
            {
                flag_msg_sleep = true;
                switch_on = false;
            }
            //----------- MENSAJE DE APAGADO --------------
            if (enable_ending_text == false)
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
                    SaveEeprom();                                                                       // Salvado en EEPROM
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
                    OLED_display.clearDisplay();
                    OLED_display.setCursor(0, 8);
                    OLED_display.setTextSize(2);
                    OLED_display.print("OvPw");
                    OLED_display.display();
                    PostMortemLog(sample_POut, capacity, theory_Vout, C_ERROR_OVERPOWER);
                    break;
                case C_ERROR_OVERCURRENT:
                    OLED_display.clearDisplay();
                    OLED_display.setCursor(0, 8);
                    OLED_display.setTextSize(2);
                    OLED_display.print("OvCu");
                    OLED_display.display();
                    PostMortemLog(sample_POut, capacity, theory_Vout, C_ERROR_OVERCURRENT);
                    break;
                case C_ERROR_OUPUT_UNDERVOLTAGE:
                    OLED_display.clearDisplay();
                    OLED_display.setCursor(0, 8);
                    OLED_display.setTextSize(2);
                    OLED_display.print("UnVo");
                    OLED_display.display();
                    PostMortemLog(sample_POut, capacity, theory_Vout, C_ERROR_OUPUT_UNDERVOLTAGE);
                    break;
                case C_ERROR_INPUT_UNDERVOLTAGE:
                    OLED_display.clearDisplay();
                    OLED_display.setCursor(0, 8);
                    OLED_display.setTextSize(2);
                    OLED_display.print("UnVi");
                    OLED_display.display();
                    PostMortemLog(sample_POut, capacity, theory_Vout, C_ERROR_INPUT_UNDERVOLTAGE);
                    break;
                default:
                    OLED_display.clearDisplay();
                    OLED_display.setCursor(0, 8);
                    OLED_display.setTextSize(2);
                    OLED_display.print("ERROR");
                    OLED_display.display();
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
            if (flag_work == true)
            {
                /* New State */
                sw_status = C_SW_ST_STOP;

                /* Output */
                sw_output = C_OFF;
                user_output = C_OFF;

                /* Clear Flags */
                flag_msg_init = false;
                flag_work = false;
                flag_sound_init = false;
                flag_display_capacity = false;

                /* Change-State Effects */
                Serial5.printf("Change TO STOP\n\r");
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
                user_output = C_OFF;
                sw_output = C_OFF;

                /* Clear Flags */
                flag_low_battery = false;

                /* Change-State Effects */
                Serial5.printf("Change TO STOP\n\r");
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
                sw_output = C_OFF;

                /* Clear Flags */
                flag_low_battery = false;

                /* Change-State Effects */
                Serial5.printf("Change TO ERROR\n\r");
                display_error_status = C_DISPLAY_ST_BUSSY;
                timer_display_error.set(500);
                SwitchScreenOff();
                PowerBar(0);
                playSound(C_SOUND_ERROR);
                switch (error)
                {
                case C_ERROR_OVERPOWER:
                    Serial5.printf("OP ERROR\n\r");
                    break;
                case C_ERROR_OVERCURRENT:
                    Serial5.printf("OC ERROR\n\r");
                    break;
                case C_ERROR_OUPUT_UNDERVOLTAGE:
                    Serial5.printf("OUV ERROR\n\r");
                    break;
                case C_ERROR_INPUT_UNDERVOLTAGE:
                    Serial5.printf("IUV ERROR\n\r");
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
                    sw_output = C_ON;
                    user_output = C_ON; // Activacion de la salida.
                    /* Clear Flags */
                    flag_stop = false;
                    /* Change-State Effects */
                    Serial5.printf("Change TO RUN\n\r");
                    playSound(C_SOUND_ON);
                }
            }
            //----------- TO SLEEP -----------
            if (flag_sleep == true)
            {
                /* New State */
                sw_status = C_SW_ST_SLEEP;

                /* Output */
                sw_output = C_OFF;

                /* Clear Flags */
                flag_msg_sleep = false;
                flag_sound_end = false;
                flag_sleep = false;
                go_sleep = true;

                /* Change-State Effects */
                Serial5.printf("Change TO SLEEP\n\r");
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
                sw_output = C_OFF;

                /* Clear Flags */
                flag_initialize = false;
                flag_waiting = C_TIMER_IDLE;
                flag_msg_sleep = false;
                flag_sound_end = false;

                /* Change-State Effects */
                Serial5.printf("Change TO START\n\r");
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
                sw_output = C_OFF;
                user_output = C_OFF;

                /* Clear Flags */

                /* Change-State Effects */
                Serial5.printf("Change TO STOP\n\r");
                timer_idle.set(30000);
                trigger_Display_volt = true;
                timer_recover_voltage.set(1000);
            }
        }
        //=============================================================================================================================================
        //                                                              END STATE MANAGEMENT
        //=============================================================================================================================================

        //============================================================== OUTPUT MANAGEMENT ===========================================================//
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
            theory_Vout = constrain(theory_Vout, MIN_VOLTAGE, MAX_VOLTAGE);
            DCDC.SetVoltage(theory_Vout, output_mode);
            digitalWrite(C_PIN_OP_SWITCH, LOW);
            LedWork(C_ON);
        }
        else if (hw_output == C_OFF)
        {
            digitalWrite(C_PIN_OP_SWITCH, HIGH);
            consmptn_event_protection_counter = 0;
            OP_event_protection_counter = 0;
            UV_event_protection_counter = 0;
            LedWork(C_OFF);
        }
        //======================================================= ACTUALIZACION DEL DIAGNOSTICO ===========================================================//
        LogDiagnosticData(theory_Vout, C_THEORY_VOLTAGE);
        //============================================================ TIMER LOGEO EEPROM  =============================================================//
        if (timer_sec_count.poll(1000) != C_TIMER_NOT_EXPIRED)
        {
            cont_sec++;
            if (cont_sec == C_EEPROM_UPDATE_TIME * 60)
            {
                if (flag_eeprom_init_fail == true)
                {
                    Serial5.println("Inicializando EEPROM..");
                    //while (!Init_diagnostic_elements())
                    //  ;
                    flag_eeprom_init_fail = false;
                }
                else
                {
                    Serial5.println("Updateando EEPROM..");
                    IncrementDiagnosticData(C_WORK_TIME);
                    SaveEeprom();
                    Serial5.println("EEPROM Actualizada.");
                }
                cont_sec = 0;
            }
        }

        //====================================================== LECTURA DE EVENTOS DE LA BOTONERA ========================================================//
        button_event = ReadDirPad();

        //====================================================== Control del tiempo del ciclo de control ========================================================//
        t2 = micros();
        t2_state = micros();
        if (timer_log_sec.poll(1000) != C_TIMER_NOT_EXPIRED)
        {
            cont_log_sec++;

            Serial5.printf("%d\n\r", cont_log_sec);
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

    for (int i = 0; i < 8; i++)
    {
        sample += analogRead(pin_battery);
    }

    batt_voltage = sample / 8 * 3000 / 4096 * 250 / 150;

    percent = constrain(((batt_voltage - 3300) * 100 / 800), 0, 100);

    if (percent <= C_EMPTY_BATTERY_LEVEL)
    {
        *empty_batt = true;
        *lowbattery = true;
        playSound(C_SOUND_DEATH_BATTERY);
        DisplayNoBattery();
    }
    else if (percent <= C_LOW_BATTERY_LEVEL)
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