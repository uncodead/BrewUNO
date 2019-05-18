#include <BrewUNO/MashService.h>

MashService::MashService(FS *fs, TemperatureService *temperatureService) : _fs(fs),
                                                                           _temperatureService(temperatureService)
{
}

MashService::~MashService() {}

DynamicJsonDocument jsonDocumentMash = DynamicJsonDocument(MAX_ACTIVESTATUS_SIZE);
JsonObject _mashSettings;

void MashService::LoadMashSettings()
{
    File configFile = _fs->open(MASH_SETTINGS_FILE, "r");
    if (configFile &&
        configFile.size() <= MAX_ACTIVESTATUS_SIZE &&
        deserializeJson(jsonDocumentMash, configFile) == DeserializationError::Ok && jsonDocumentMash.is<JsonObject>())
        _mashSettings = jsonDocumentMash.as<JsonObject>();
    configFile.close();
}

void MashService::loop(ActiveStatus *activeStatus)
{
    if (!activeStatus->BrewStarted || activeStatus->ActiveStep != mash)
        return;

    time_t timeNow = now();
    JsonArray steps = _mashSettings["steps"].as<JsonArray>();
    if (activeStatus->TargetTemperature == 0)
    {
        JsonObject step = steps[0];
        activeStatus->TargetTemperature = step["temperature"];
    }

    if (activeStatus->EndTime > 0 && timeNow > activeStatus->EndTime)
    {
        Serial.println("Step over, next step: ");
        unsigned int nextMashStep = activeStatus->ActiveMashStepIndex + 1;
        if (steps.size() > nextMashStep)
        {
            JsonObject step = steps[nextMashStep];
            activeStatus->ActiveMashStepIndex = nextMashStep;
            activeStatus->StartTime = 0;
            activeStatus->EndTime = 0;
            activeStatus->TargetTemperature = step["temperature"];
            activeStatus->Recirculation = ((int)step["recirculation"]) == 1;
            Buzzer().Ring();
        }
        else
        {
            Serial.println("Boil Time");
            activeStatus->ActiveStep = boil;
            activeStatus->StartTime = 0;
            activeStatus->EndTime = 0;
            activeStatus->ActiveMashStepIndex = -1;
            activeStatus->TargetTemperature = activeStatus->BoilTargetTemperature;
            activeStatus->Recirculation = false;
            activeStatus->Recirculation = false;
            Buzzer().Ring();
            Pump().TurnPumpOff();
        }
    }
    else
    {
        Serial.print("Temperature: ");
        Serial.println(activeStatus->Temperature);
        Serial.print("Target: ");
        Serial.println(activeStatus->TargetTemperature);

        if (activeStatus->StartTime == 0)
        {
            // Recirculation while brew not started
            Pump().TurnPumpOn();
        }

        // toddo: colocar em configuracao
        if (activeStatus->Temperature >= (activeStatus->TargetTemperature - 0.2) && activeStatus->StartTime == 0)
        {
            Serial.println("Step Started");
            JsonObject step = steps[activeStatus->ActiveMashStepIndex];
            activeStatus->StartTime = timeNow;
            activeStatus->EndTime = timeNow + (int(step["time"]) * 60);

            Serial.print("Start time: ");
            Serial.println(activeStatus->StartTime);
            Serial.print("End Time: ");
            Serial.println(activeStatus->EndTime);
            Buzzer().Ring();
            activeStatus->Recirculation = ((int)step["recirculation"]) == 1;
            Pump().TurnPump(activeStatus->Recirculation);
        }
    }
}