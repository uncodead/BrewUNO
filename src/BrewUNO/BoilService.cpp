#include <BrewUNO/BoilService.h>

BoilService::BoilService(FS *fs, TemperatureService *temperatureService) : _fs(fs),
                                                                           _temperatureService(temperatureService)
{
}

BoilService::~BoilService() {}

void BoilService::LoadBoilSettings()
{
    _boilSettings = &LoadSettings(BOIL_SETTINGS_FILE);
}

JsonDocument &BoilService::LoadSettings(String settingsFile)
{
    File configFile = _fs->open(settingsFile, "r");
    if (configFile)
    {
        size_t size = configFile.size();
        if (size <= MAX_ACTIVESTATUS_SIZE)
        {
            DynamicJsonDocument jsonDocument = DynamicJsonDocument(MAX_ACTIVESTATUS_SIZE);
            deserializeJson(jsonDocument, configFile);
            return jsonDocument;
        }
    }
}

void BoilService::loop(ActiveStatus *activeStatus)
{
    if (!activeStatus->BrewStarted || activeStatus->ActiveStep != boil)
    {
        return;
    }

    time_t timeNow = now();

    if (activeStatus->StartTime == 0 && activeStatus->Temperature >= activeStatus->BoilTargetTemperature)
    {
        activeStatus->StartTime = timeNow;
        activeStatus->EndTime = activeStatus->StartTime + activeStatus->BoilTime;
        activeStatus->TargetTemperature = activeStatus->BoilTargetTemperature;
        Serial.println("Boil started");
        Serial.println(activeStatus->StartTime);
        Serial.println(activeStatus->EndTime);
        Buzzer().Ring();
    }
    if (activeStatus->EndTime > 0 && timeNow > activeStatus->EndTime)
    {
        Serial.println("Boil ended");
        Buzzer().Ring();
        activeStatus->SaveActiveStatus(0, 0, 0, 0, -1, "", 0, 0, none, false);
        return;
    }

    time_t moment = activeStatus->EndTime - timeNow;
    SetBoiIndexStep(activeStatus, moment / 60);
}

void BoilService::SetBoiIndexStep(ActiveStatus *activeStatus, time_t moment)
{
    int index = 0;
    String currentStep = "";
    JsonArray steps = _boilSettings->getMember("steps").as<JsonArray>();
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
        Buzzer().Ring();
    }
}