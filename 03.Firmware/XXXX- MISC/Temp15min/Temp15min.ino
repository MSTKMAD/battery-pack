// Temp15min.ino
// Apaga un pin cada 15 minutos y lo vuelve a activar tras 5 segundos.
// Cambia CONTROL_PIN y niveles si tu circuito usa active LOW.

const int CONTROL_PIN = 3;          // pin a controlar (cambiar si hace falta)
const uint8_t ACTIVE_LEVEL = LOW;  // nivel cuando "activo" (por ejemplo HIGH)
const uint8_t INACTIVE_LEVEL = HIGH; // nivel cuando "apagado"  (por ejemplo LOW)

const unsigned long ACTIVE_MS = 15UL * 60UL * 1000UL; // 15 minutos en ms
const unsigned long OFF_MS = 5UL * 1000UL;            // 5 segundos en ms

enum State
{
    ACTIVE,
    OFF_PERIOD
};
State state = ACTIVE;
unsigned long stateStart = 0;

void setup()
{
    pinMode(CONTROL_PIN, OUTPUT);
    digitalWrite(CONTROL_PIN, ACTIVE_LEVEL); // iniciar activo
    state = ACTIVE;
    stateStart = millis();
}

void loop()
{
    unsigned long now = millis();
    if (state == ACTIVE)
    {
        if (now - stateStart >= ACTIVE_MS)
        {
            // pasar a periodo apagado
            digitalWrite(CONTROL_PIN, INACTIVE_LEVEL);
            state = OFF_PERIOD;
            stateStart = now;
        }
    }
    else
    { // OFF_PERIOD
        if (now - stateStart >= OFF_MS)
        {
            // volver a activar y reiniciar conteo de 15 minutos
            digitalWrite(CONTROL_PIN, ACTIVE_LEVEL);
            state = ACTIVE;
            stateStart = now;
        }
    }
}