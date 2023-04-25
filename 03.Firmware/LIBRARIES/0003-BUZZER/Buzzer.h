/**
 * @file Buzzer.h
 * @author Javi (Javier@musotoku.com)
 * @brief
 * @version 2
 * @date 2021-02-11
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "notes.h"
// #include <MilliTimer.h>

uint16_t C_PIN_BUZZER = 2;

const uint16_t C_SOUND_MUTE = 0;
const uint16_t C_SOUND_START = 1;
const uint16_t C_SOUND_END = 2;
const uint16_t C_SOUND_UP = 3;
const uint16_t C_SOUND_DOWN = 4;
const uint16_t C_SOUND_ON = 5;
const uint16_t C_SOUND_OFF = 6;
const uint16_t C_SOUND_CHARGE_IN = 7;
const uint16_t C_SOUND_CHARGE_OUT = 8;
const uint16_t C_SOUND_DEATH_BATTERY = 9;
const uint16_t C_SOUND_FULL_CHARGE = 10;
const uint16_t C_SOUND_LOW_BATTERY = 11;
const uint16_t C_SOUND_ERROR = 12;

// Cada una de las melodias esta formada por Nota+Duracion de Nota en ms + Tiempo en silencio antes de la siguiente nota.

const uint16_t C_NOTES_START_1[] = {NOTE_E6, 125, 5, NOTE_G6, 125, 5, NOTE_E7, 125, 5, NOTE_C7, 125, 5, NOTE_D7, 125, 5, NOTE_G7, 125, 5};
const uint16_t C_NOTES_END_1[] = {NOTE_G7, 125, 5, NOTE_D7, 125, 5, NOTE_C7, 125, 5, NOTE_E7, 125, 5, NOTE_G6, 125, 5, NOTE_E6, 125, 5};
const uint16_t C_NOTES_DOWN_1[] = {NOTE_D8, 10, 10, NOTE_A7, 10, 10};
const uint16_t C_NOTES_UP_1[] = {NOTE_B7, 10, 10, NOTE_E8, 10, 10};
const uint16_t C_NOTES_ON_1[] = {NOTE_B7, 100, 0, NOTE_E8, 200, 0};
const uint16_t C_NOTES_OFF_1[] = {NOTE_E8, 100, 0, NOTE_B7, 200, 0};
const uint16_t C_NOTES_CHARGE_IN[] = {NOTE_C5, 125, 10, NOTE_C7, 125, 10, NOTE_C6, 125, 10};
const uint16_t C_NOTES_CHARGE_OUT[] = {NOTE_C6, 125, 10, NOTE_C7, 125, 10, NOTE_C5, 125, 10};
const uint16_t C_NOTES_LOW_BATTERY[] = {NOTE_B7, 200, 10, NOTE_F7, 200, 10, NOTE_B7, 200, 10, NOTE_F7, 200, 10, NOTE_B7, 200, 10, NOTE_F7, 200, 10};
const uint16_t C_NOTES_FULL_CHARGE[] = {NOTE_DS5, 150, 10, NOTE_DS5, 150, 10, NOTE_DS5, 150, 10, NOTE_DS5, 400, 5, NOTE_B4, 400, 5, NOTE_CS5, 400, 5, NOTE_DS5, 150, 150, NOTE_CS5, 150, 5, NOTE_DS5, 500, 5};
const uint16_t C_NOTES_DEATHBATTERY[] = {NOTE_G5, 100, 10, NOTE_G5, 100, 10, NOTE_G5, 100, 10};
const uint16_t C_NOTES_ERROR[] = {NOTE_C8, 100, 50, NOTE_C8, 100, 50, NOTE_C8, 100, 50, NOTE_C8, 100, 50};

MilliTimer timer_buzzer;

const bool C_BUZZER_BUSSY = true;
const bool C_BUZZER_NOT_BUSSY = false;

uint16_t SOUND_START[100];
uint16_t size_sound_start;
uint16_t SOUND_END[100];
uint16_t size_sound_end;
uint16_t SOUND_DOWN[100];
uint16_t size_sound_down;
uint16_t SOUND_UP[100];
uint16_t size_sound_up;
uint16_t SOUND_ON[100];
uint16_t size_sound_on;
uint16_t SOUND_OFF[100];
uint16_t size_sound_off;
uint16_t SOUND_CHARGE_IN[100];
uint16_t size_sound_charge_in;
uint16_t SOUND_CHARGE_OUT[100];
uint16_t size_sound_charge_out;
uint16_t SOUND_DEATH_BATTERY[100];
uint16_t size_sound_death_battery;
uint16_t SOUND_FULL_CHARGE[100];
uint16_t size_sound_full_charge;
uint16_t SOUND_LOW_BATTERY[100];
uint16_t size_sound_low_battery;
uint16_t SOUND_ERROR[100];
uint16_t size_sound_error;

const uint16_t C_MODE_HACK_ALTERNATIVE = 0x10;
const uint16_t C_MODE_DEFAULT = 0x11;

/**
 * @brief Inicializacion del buzzer con la seleccion del sonido de cada accion.
 *
 * @param mode
 */
void InitBuzzer(uint16_t mode = C_MODE_DEFAULT, uint16_t pin_buzzer = C_PIN_BUZZER)
{
    C_PIN_BUZZER = pin_buzzer;
    pinMode(C_PIN_BUZZER, OUTPUT);

    switch (mode)
    {
    case C_MODE_DEFAULT:
        memcpy(SOUND_START, C_NOTES_START_1, sizeof(C_NOTES_START_1));
        size_sound_start = (sizeof(C_NOTES_START_1) / sizeof(C_NOTES_START_1[0])) / 3; // Caluclo de numero de notas: Tamaño del array/ (tamaño de una nota * 3) <- Se divide por 3 por Nota+Duracion+Espacio siguiente nota
        memcpy(SOUND_END, C_NOTES_END_1, sizeof(C_NOTES_END_1));
        size_sound_end = (sizeof(C_NOTES_END_1) / sizeof(C_NOTES_END_1[0])) / 3;
        memcpy(SOUND_UP, C_NOTES_UP_1, sizeof(C_NOTES_UP_1));
        size_sound_up = (sizeof(C_NOTES_UP_1) / sizeof(C_NOTES_UP_1[0])) / 3;
        memcpy(SOUND_DOWN, C_NOTES_DOWN_1, sizeof(C_NOTES_DOWN_1));
        size_sound_down = (sizeof(C_NOTES_DOWN_1) / sizeof(C_NOTES_DOWN_1[0])) / 3;
        memcpy(SOUND_ON, C_NOTES_ON_1, sizeof(C_NOTES_ON_1));
        size_sound_on = (sizeof(C_NOTES_ON_1) / sizeof(C_NOTES_ON_1[0])) / 3;
        memcpy(SOUND_OFF, C_NOTES_OFF_1, sizeof(C_NOTES_OFF_1));
        size_sound_off = (sizeof(C_NOTES_OFF_1) / sizeof(C_NOTES_OFF_1[0])) / 3;
        memcpy(SOUND_CHARGE_IN, C_NOTES_CHARGE_IN, sizeof(C_NOTES_CHARGE_IN));
        size_sound_charge_in = (sizeof(C_NOTES_CHARGE_IN) / sizeof(C_NOTES_CHARGE_IN[0])) / 3;
        memcpy(SOUND_CHARGE_OUT, C_NOTES_CHARGE_OUT, sizeof(C_NOTES_CHARGE_OUT));
        size_sound_charge_out = (sizeof(C_NOTES_CHARGE_OUT) / sizeof(C_NOTES_CHARGE_OUT[0])) / 3;
        memcpy(SOUND_DEATH_BATTERY, C_NOTES_DEATHBATTERY, sizeof(C_NOTES_DEATHBATTERY));
        size_sound_death_battery = (sizeof(C_NOTES_DEATHBATTERY) / sizeof(C_NOTES_DEATHBATTERY[0])) / 3;
        memcpy(SOUND_FULL_CHARGE, C_NOTES_FULL_CHARGE, sizeof(C_NOTES_FULL_CHARGE));
        size_sound_full_charge = (sizeof(C_NOTES_FULL_CHARGE) / sizeof(C_NOTES_FULL_CHARGE[0])) / 3;
        memcpy(SOUND_LOW_BATTERY, C_NOTES_LOW_BATTERY, sizeof(C_NOTES_LOW_BATTERY));
        size_sound_low_battery = 6;
        memcpy(SOUND_ERROR, C_NOTES_ERROR, sizeof(C_NOTES_ERROR));
        size_sound_error = (sizeof(C_NOTES_ERROR) / sizeof(C_NOTES_ERROR[0])) / 3;
        break;

    default:
        memcpy(SOUND_START, C_NOTES_START_1, sizeof(C_NOTES_START_1));
        size_sound_start = (sizeof(C_NOTES_START_1) / sizeof(C_NOTES_START_1[0])) / 3;
        memcpy(SOUND_END, C_NOTES_END_1, sizeof(C_NOTES_END_1));
        size_sound_end = (sizeof(C_NOTES_END_1) / sizeof(C_NOTES_END_1[0])) / 3;
        memcpy(SOUND_UP, C_NOTES_UP_1, sizeof(C_NOTES_UP_1));
        size_sound_up = (sizeof(C_NOTES_UP_1) / sizeof(C_NOTES_UP_1[0])) / 3;
        memcpy(SOUND_DOWN, C_NOTES_DOWN_1, sizeof(C_NOTES_DOWN_1));
        size_sound_down = (sizeof(C_NOTES_DOWN_1) / sizeof(C_NOTES_DOWN_1[0])) / 3;
        memcpy(SOUND_ON, C_NOTES_ON_1, sizeof(C_NOTES_ON_1));
        size_sound_on = (sizeof(C_NOTES_ON_1) / sizeof(C_NOTES_ON_1[0])) / 3;
        memcpy(SOUND_OFF, C_NOTES_OFF_1, sizeof(C_NOTES_OFF_1));
        size_sound_off = (sizeof(C_NOTES_OFF_1) / sizeof(C_NOTES_OFF_1[0])) / 3;
        memcpy(SOUND_CHARGE_IN, C_NOTES_CHARGE_IN, sizeof(C_NOTES_CHARGE_IN));
        size_sound_charge_in = (sizeof(C_NOTES_CHARGE_IN) / sizeof(C_NOTES_CHARGE_IN[0])) / 3;
        memcpy(SOUND_CHARGE_OUT, C_NOTES_CHARGE_OUT, sizeof(C_NOTES_CHARGE_OUT));
        size_sound_charge_out = (sizeof(C_NOTES_CHARGE_OUT) / sizeof(C_NOTES_CHARGE_OUT[0])) / 3;
        memcpy(SOUND_DEATH_BATTERY, C_NOTES_DEATHBATTERY, sizeof(C_NOTES_DEATHBATTERY));
        size_sound_death_battery = (sizeof(C_NOTES_DEATHBATTERY) / sizeof(C_NOTES_DEATHBATTERY[0])) / 3;
        memcpy(SOUND_FULL_CHARGE, C_NOTES_FULL_CHARGE, sizeof(C_NOTES_FULL_CHARGE));
        size_sound_full_charge = (sizeof(C_NOTES_FULL_CHARGE) / sizeof(C_NOTES_FULL_CHARGE[0])) / 3;
        memcpy(SOUND_LOW_BATTERY, C_NOTES_LOW_BATTERY, sizeof(C_NOTES_LOW_BATTERY));
        size_sound_low_battery = 6;
        memcpy(SOUND_ERROR, C_NOTES_ERROR, sizeof(C_NOTES_ERROR));
        size_sound_error = (sizeof(C_NOTES_ERROR) / sizeof(C_NOTES_ERROR[0])) / 3;
        break;
    }
}
/**
 * @brief Get the Sound object
 *
 * @param local
 * @param sound_id
 * @return uint16_t
 */
uint16_t getSound(uint16_t *local, uint16_t sound_id)
{
    if (sound_id == C_SOUND_START)
    {
        memcpy(local, SOUND_START, sizeof(SOUND_START));
        return (size_sound_start);
    }
    else if (sound_id == C_SOUND_END)
    {
        memcpy(local, SOUND_END, sizeof(SOUND_END));
        return (size_sound_end);
    }
    else if (sound_id == C_SOUND_UP)
    {
        memcpy(local, SOUND_UP, sizeof(SOUND_UP));
        return (size_sound_up);
    }
    else if (sound_id == C_SOUND_DOWN)
    {
        memcpy(local, SOUND_DOWN, sizeof(SOUND_DOWN));
        return (size_sound_down);
    }
    else if (sound_id == C_SOUND_ON)
    {
        memcpy(local, SOUND_ON, sizeof(SOUND_ON));
        return (size_sound_on);
    }
    else if (sound_id == C_SOUND_OFF)
    {
        memcpy(local, SOUND_OFF, sizeof(SOUND_OFF));
        return (size_sound_off);
    }
    else if (sound_id == C_SOUND_CHARGE_IN)
    {
        memcpy(local, SOUND_CHARGE_IN, sizeof(SOUND_CHARGE_IN));
        return (size_sound_charge_in);
    }

    else if (sound_id == C_SOUND_CHARGE_OUT)
    {
        memcpy(local, SOUND_CHARGE_OUT, sizeof(SOUND_CHARGE_OUT));
        return (size_sound_charge_out);
    }

    else if (sound_id == C_SOUND_DEATH_BATTERY)
    {
        memcpy(local, SOUND_DEATH_BATTERY, sizeof(SOUND_DEATH_BATTERY));
        return (size_sound_death_battery);
    }
    else if (sound_id == C_SOUND_FULL_CHARGE)
    {
        memcpy(local, SOUND_FULL_CHARGE, sizeof(SOUND_FULL_CHARGE));
        return (size_sound_full_charge);
    }

    else if (sound_id == C_SOUND_LOW_BATTERY)
    {
        memcpy(local, SOUND_LOW_BATTERY, sizeof(SOUND_LOW_BATTERY));
        return (size_sound_low_battery);
    }
    else if (sound_id == C_SOUND_ERROR)
    {
        memcpy(local, SOUND_ERROR, sizeof(SOUND_ERROR));
        return (size_sound_error);
    }
    return 0;
}
/**
 * @brief Funcion que reproduce los sonidos.
 *
 * @param num
 * @return true
 * @return false
 */
void playSound(int16_t num)
{
#ifdef SERIAL_DEBUG
    Serial5.printf("Sound %d\n", num);
#endif
    uint16_t index = 0;
    bool buzzer_state = C_BUZZER_BUSSY;
    uint16_t local_sound[100];
    uint16_t size_sound = 0;

    size_sound = getSound(local_sound, num);

    timer_buzzer.set(10);
    for (uint16_t i = 0; i / 3 < size_sound; i += 3)
    {
        while (timer_buzzer.poll() == C_TIMER_NOT_EXPIRED)
        {
            Watchdog.reset();
        };
        tone(C_PIN_BUZZER, local_sound[i], local_sound[i + 1]);
        timer_buzzer.set(local_sound[i + 1] + local_sound[i + 2]);
    }
}
