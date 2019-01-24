#include <BoilService.h>

DynamicJsonBuffer jsonBufferBoil;

BoilService::BoilService(FS *fs, TemperatureService *temperatureService) : _fs(fs),
                                                                           _temperatureService(temperatureService)
{
}

BoilService::~BoilService() {}

void BoilService::LoadBoilSettings()
{
    _boilSettings = &LoadSettings(BOIL_SETTINGS_FILE);
}

JsonObject &BoilService::LoadSettings(String settingsFile)
{
    File configFile = _fs->open(settingsFile, "r");
    JsonObject *root = &(jsonBufferBoil.parseObject(configFile));
    configFile.close();
    return *root;
}

void BoilService::loop(ActiveStatus *activeStatus)
{
    if (!activeStatus->BrewStarted || activeStatus->ActiveStep != boil)
    {
        return;
    }

    time_t timeNow = now();

    if (activeStatus->StartTime == 0 && _temperatureService->GetTemperature() >= activeStatus->BoilTargetTemperature)
    {
        activeStatus->StartTime = timeNow;
        activeStatus->EndTime = activeStatus->StartTime + activeStatus->BoilTime; //get from settings im seconds
        Serial.println("Boil started");
        Serial.println(activeStatus->StartTime);
        Serial.println(activeStatus->EndTime);
        Serial.println("buzzer...  D0");
    }
    if (activeStatus->EndTime > 0 && timeNow > activeStatus->EndTime)
    {
        Serial.println("Boil ended");
        activeStatus->StartTime = 0;
        activeStatus->EndTime = 0;
        activeStatus->ActiveStep = none;
        activeStatus->BrewStarted = false;
        Serial.println("buzzer...  D0");
        return;
    }

    time_t moment = activeStatus->EndTime - timeNow;
    SetBoiIndexStep(activeStatus, moment / 60);
}

void BoilService::SetBoiIndexStep(ActiveStatus *activeStatus, time_t moment)
{
    int index = 0;
    String currentStep = "";
    JsonArray &steps = _boilSettings->get<JsonArray>("steps");
    for (auto step : steps)
    {
        if (step["time"] == moment)
        {
            currentStep = currentStep == "" ? String(index) : currentStep + "," + String(index);
        }
        index += 1;
    }
    if (currentStep != "" && currentStep != activeStatus->ActiveBoilStepIndex)
    {
        activeStatus->ActiveBoilStepIndex = currentStep;
        Serial.println(currentStep);
        Serial.println(activeStatus->ActiveBoilStepIndex);
        Serial.println("buzzer...  D0");
    }
}