#include <BrewUNO/Pump.h>

time_t lastPumpStarted;
time_t lastPumpRest;

bool isResting = false;
bool recirculationOn = false;

Pump::Pump(BrewSettingsService *brewSettingsService) : _brewSettingsService(brewSettingsService)
{
}

void Pump::TurnPumpOn()
{
    TurnPump(true);
    recirculationOn = true;
    lastPumpStarted = now();
}
void Pump::TurnPumpOff()
{
    TurnPump(false);
    recirculationOn = false;
    lastPumpStarted = 0;
}

void Pump::TurnPump(bool on)
{
    if (on)
        digitalWrite(PUMP_BUS, LOW);
    else
        digitalWrite(PUMP_BUS, HIGH);

    Serial.println("Recirculation: " + String(on));
}

void Pump::CheckRest()
{
    if (recirculationOn)
    {
        time_t timeNow = now();
        if (!isResting && timeNow - lastPumpStarted >= _brewSettingsService->PumpRestInterval)
        {
            TurnPump(false);
            isResting = true;
            lastPumpStarted = 0;
            lastPumpRest = timeNow;
        }

        if (isResting && timeNow - lastPumpRest >= _brewSettingsService->PumpRestTime)
        {
            TurnPump(true);
            isResting = false;
            lastPumpRest = 0;
            lastPumpStarted = timeNow;
        }
    }
}

void Pump::AntiCavitation()
{
    for (byte i = 1; i < 6; i++)
    {
        TurnPump(true);
        delay(750 + i * 250);
        TurnPump(false);
        delay(350);
    }
}