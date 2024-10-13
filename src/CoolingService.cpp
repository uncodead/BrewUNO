#include <CoolingService.h>

CoolingService::CoolingService(FS *fs, TemperatureService *temperatureService) : _fs(fs),
                                                                                 _temperatureService(temperatureService)
{
}

CoolingService::~CoolingService() {}

DynamicJsonDocument jsonDocumentCooling = DynamicJsonDocument(MAX_ACTIVESTATUS_SIZE);
JsonObject _coolingSettings;

void CoolingService::LoadCoolingSettings()
{
    File configFile = _fs->open(COOLING_SETTINGS_FILE, "r");
    if (configFile &&
        configFile.size() <= MAX_ACTIVESTATUS_SIZE &&
        deserializeJson(jsonDocumentCooling, configFile) == DeserializationError::Ok && jsonDocumentCooling.is<JsonObject>())
        _coolingSettings = jsonDocumentCooling.as<JsonObject>();
    configFile.close();
}

time_t lastBeepCooling = now();
void CoolingService::loop(ActiveStatus *activeStatus)
{
    if (!activeStatus->BrewStarted || activeStatus->ActiveStep != cooling)
        return;

    JsonArray steps = _coolingSettings["st"].as<JsonArray>();

    if (steps.size() == 0)
    {
        activeStatus->SaveActiveStatus(0, 0, 0, 0, -1, "", 0, 0, none, false);
        return;
    }

    if (activeStatus->CoolingTargetTemperature == 0 && activeStatus->ActiveCoolingStepIndex <= 0)
    {
        activeStatus->CoolingTargetTemperature = steps[0]["t"];
        activeStatus->ActiveCoolingStepIndex = 0;
        activeStatus->ActiveCoolingStepName = steps[0]["n"].as<String>();
        activeStatus->HeaterOn = ((int)steps[0]["ho"]) == 1;
        activeStatus->ActiveCoolingStepSufixName = getCoolingName(activeStatus, steps[0]);
        if (activeStatus->CoolingTargetTemperature <= 0)
        {
            activeStatus->HeaterOn = false;
        }
    }

    if (activeStatus->CoolingTargetTemperature > 0)
        activeStatus->TargetTemperature = activeStatus->CoolingTargetTemperature;

    time_t timeNow = now();
    if (activeStatus->EndTime > 0 && timeNow > activeStatus->EndTime)
    {
        unsigned int nextCoolingStep = activeStatus->ActiveCoolingStepIndex + 1;
        if (steps.size() > nextCoolingStep)
            NextStep(activeStatus, steps, timeNow, nextCoolingStep);
        else
            activeStatus->SaveActiveStatus(0, 0, 0, 0, -1, "", 0, 0, none, false);
    }
    else
    {
        if ((activeStatus->CoolingTemperature <= activeStatus->CoolingTargetTemperature || activeStatus->CoolingTargetTemperature <= 0) && activeStatus->StartTime == 0)
            StepStarted(activeStatus, steps, timeNow);
    }
}

void CoolingService::NextStep(ActiveStatus *activeStatus, JsonArray steps, time_t timeNow, int nextCoolingStep)
{
    JsonObject step = steps[nextCoolingStep];
    activeStatus->ActiveCoolingStepIndex = nextCoolingStep;
    activeStatus->ActiveCoolingStepName = step["n"].as<String>();
    activeStatus->ActiveCoolingStepSufixName = getCoolingName(activeStatus, step);
    activeStatus->StartTime = 0;
    activeStatus->EndTime = 0;
    activeStatus->CoolingTargetTemperature = step["t"];
    activeStatus->HeaterOn = ((int)step["ho"]) == 1;
    if (activeStatus->CoolingTargetTemperature <= 0)
        activeStatus->HeaterOn = false;
    Buzzer().Ring(1, 2000);
    activeStatus->SaveActiveStatus();
}

void CoolingService::StepStarted(ActiveStatus *activeStatus, JsonArray steps, time_t timeNow)
{
    Serial.println("Step Started");
    JsonObject step = steps[activeStatus->ActiveCoolingStepIndex];
    activeStatus->StartTime = timeNow;
    activeStatus->EndTime = timeNow + (int(step["tm"]) * 60);
    activeStatus->ActiveCoolingStepName = step["n"].as<String>();
    activeStatus->ActiveCoolingStepSufixName = getCoolingName(activeStatus, step);
    activeStatus->HeaterOn = ((int)step["ho"]) == 1;

    Serial.print("Start time: ");
    Serial.println(activeStatus->StartTime);
    Serial.print("End Time: ");
    Serial.println(activeStatus->EndTime);
    Buzzer().Ring(1, 2000);

    activeStatus->SaveActiveStatus();
}

String CoolingService::getCoolingName(ActiveStatus *activeStatus, JsonObject step)
{
    return step["tm"].as<String>() + "'@" +
           step["t"].as<String>() + activeStatus->TempUnit;
}