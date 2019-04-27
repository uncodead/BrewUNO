#include <BrewUno/Pump.h>

void Pump::TurnPumpOn()
{
    TurnPump(true);
}
void Pump::TurnPumpOff()
{
    TurnPump(false);
}

void Pump::TurnPump(bool on)
{
    if (on)
    {
        digitalWrite(PUMP_BUS, LOW);
        Serial.println("Recirculation on");
    }
    else
    {
        digitalWrite(PUMP_BUS, HIGH);
        Serial.println("Recirculation off");
    }
}