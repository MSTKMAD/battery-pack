/**
 * @file pid.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-03-10
 *
 * @copyright Copyright (c) 2025
 *
 */

const int r_motor = 16; // 9v
//const int r_motor = 8; // 6v
const float Kp = 1.5;  // 1
const float Ki = 0.03; // 0.03
const float Kd = 0.02; // 0.02
const int delta = 30;  // ms

float Vreal_target = 0;
float Vdcdc = 0;
float integral_error = 0;
int prev_increment = 0;
float u_control = 0;

int controlPID(int motor_dut_current, bool reset)
{
    float I, P, D = 0;
    float increment = 0;
    float error = 0;
    float current_actual = 0;
    float Vreal_motor = 0;

    if (reset)
    {
        u_control = Vdcdc;
    }

    current_actual = motor_dut_current;
    Serial.printf("Current:%0.1f -", current_actual);

    Vreal_motor = u_control / 10 - r_motor * current_actual / 1000;
    Serial.printf("V:%0.1f - ", Vreal_motor);
    Serial.printf("Vt:%0.1f - ", Vreal_target);

    // Error de Corriente
    error = Vreal_target - Vreal_motor;
    Serial.printf("Error:%0.2f - ", error);

    // Proporcional
    P = Kp * error;
    Serial.printf("P:%0.2f - ", P);

    // Integral
    integral_error += error;
    I = Ki * integral_error;
    Serial.printf("I:%0.2f - ", I);

    // Derivativo
    D = Kd * (error - prev_increment);
    Serial.printf("D:%0.2f - ", D);

    // Control
    u_control = u_control / 10 + P + I + D;
    Serial.printf("U:%0.2f \n", u_control);

    u_control = u_control * 10;
    prev_increment = error;

    return u_control;
}

void setTarget(float volt, float current)
{
    Vreal_target = volt / 10 - r_motor * current / 1000;
    Vdcdc = volt;
}