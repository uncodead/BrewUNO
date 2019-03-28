#include <MashService.h>

DynamicJsonBuffer jsonBufferMash;

MashService::MashService(FS *fs, TemperatureService *temperatureService) : _fs(fs),
                                                                           _temperatureService(temperatureService)
{
}

MashService::~MashService() {}

void MashService::LoadMashSettings()
{
    _mashSettings = &LoadSettings(MASH_SETTINGS_FILE);
}

JsonObject &MashService::LoadSettings(String settingsFile)
{
    File configFile = _fs->open(settingsFile, "r");
    JsonObject *root = &(jsonBufferMash.parseObject(configFile));
    configFile.close();
    return *root;
}

void MashService::loop(ActiveStatus *activeStatus)
{
    if (!activeStatus->BrewStarted || activeStatus->ActiveStep != mash)
    {
        return;
    }

    time_t timeNow = now();

    if (activeStatus->TargetTemperature == 0)
    {
        JsonObject &step = _mashSettings->get<JsonArray>("steps")[0];
        activeStatus->TargetTemperature = step["temperature"];
        activeStatus->TotalHeaterPower = step["totalHeaterPower"] == "true";
    }

    if (activeStatus->EndTime > 0 && timeNow > activeStatus->EndTime)
    {
        Serial.println("Step over, next step: ");
        unsigned int nextMashStep = activeStatus->ActiveMashStepIndex + 1;
        if (_mashSettings->get<JsonArray>("steps").size() > nextMashStep)
        {
            JsonObject &step = _mashSettings->get<JsonArray>("steps")[nextMashStep];
            activeStatus->ActiveMashStepIndex = nextMashStep;
            activeStatus->StartTime = 0;
            activeStatus->EndTime = 0;
            activeStatus->TargetTemperature = step["temperature"];
            activeStatus->Recirculation = step["recirculation"] == "true";
            activeStatus->TotalHeaterPower = step["totalHeaterPower"] == "true";
            Serial.print("Next step name: ");
            Serial.printf(step["name"]);
            Serial.println("");
            Serial.print("Next step temperature: ");
            Serial.printf(step["temperature"]);
            Serial.println("");
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
            activeStatus->TotalHeaterPower = false;
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

        if (activeStatus->StartTime == 0 && (activeStatus->Temperature >= (activeStatus->TargetTemperature - 0.2)))
        {
            Serial.println("Step Started");
            JsonObject &step = _mashSettings->get<JsonArray>("steps")[activeStatus->ActiveMashStepIndex];
            activeStatus->StartTime = timeNow;
            activeStatus->EndTime = timeNow + (int(step["time"]) * 60);

            Serial.print("Start time: ");
            Serial.println(activeStatus->StartTime);
            Serial.print("End Time: ");
            Serial.println(activeStatus->EndTime);
            Buzzer().Ring();
            activeStatus->Recirculation = step["recirculation"] == "true";
            activeStatus->TotalHeaterPower = step["totalHeaterPower"] == "true";
            Pump().TurnPump(activeStatus->Recirculation);
        }
    }
}