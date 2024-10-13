#include <MashService.h>

MashService::MashService(FS *fs, TemperatureService *temperatureService, Pump *pump) : _fs(fs),
                                                                                       _temperatureService(temperatureService),
                                                                                       _pump(pump)
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

time_t lastBeep = now();
void MashService::loop(ActiveStatus *activeStatus)
{
    if (!activeStatus->BrewStarted || activeStatus->ActiveStep != mash)
        return;

    JsonArray steps = _mashSettings["st"].as<JsonArray>();

    if (activeStatus->TargetTemperature == 0)
    {
        activeStatus->TargetTemperature = steps[0]["t"];
        activeStatus->ActiveMashStepIndex = 0;
        activeStatus->HeaterOn = ((int)steps[0]["ho"]) == 1;
        activeStatus->FullPower = ((int)steps[0]["fp"]) == 1;
        activeStatus->ActiveMashStepName = steps[0]["n"].as<String>();
        activeStatus->ActiveMashStepSufixName = getMashName(activeStatus, steps[0]);
        activeStatus->Recirculation = ((int)steps[0]["r"]) == 1;
        if (activeStatus->Recirculation)
            _pump->TurnPumpOn();
        else
            _pump->TurnPumpOff();

        // brew was stopped during anti cavitatiton
        if (!activeStatus->BrewStarted || activeStatus->ActiveStep != mash)
            return;
    }

    time_t timeNow = now();
    if (activeStatus->EndTime > 0 && timeNow > activeStatus->EndTime)
    {
        activeStatus->StepLocked = activeStatus->StepLock;
        if (activeStatus->StepLock)
        {
            _pump->CheckRest();
            if (timeNow - lastBeep > 15)
            {
                Buzzer().Ring(1, 1000);
                lastBeep = now();
            }
            return;
        }

        unsigned int nextMashStep = activeStatus->ActiveMashStepIndex + 1;
        if (steps.size() > nextMashStep)
            NextStep(activeStatus, steps, timeNow, nextMashStep);
        else
            BoilTime(activeStatus);
    }
    else
    {
        if (activeStatus->StartTime != 0)
            _pump->CheckRest();

        if (activeStatus->Temperature >= (activeStatus->TargetTemperature) && activeStatus->StartTime == 0)
            StepStarted(activeStatus, steps, timeNow);
    }
}

void MashService::NextStep(ActiveStatus *activeStatus, JsonArray steps, time_t timeNow, int nextMashStep)
{
    JsonObject step = steps[nextMashStep];
    activeStatus->ActiveMashStepIndex = nextMashStep;
    activeStatus->ActiveMashStepName = step["n"].as<String>();
    activeStatus->ActiveMashStepSufixName = getMashName(activeStatus, step);
    activeStatus->StartTime = 0;
    activeStatus->EndTime = 0;
    activeStatus->TargetTemperature = step["t"];
    activeStatus->Recirculation = ((int)step["r"]) == 1;
    activeStatus->HeaterOn = ((int)step["ho"]) == 1;
    activeStatus->FullPower = ((int)step["fp"]) == 1;
    activeStatus->StepLock = ((int)step["sl"]) == 1;
    Buzzer().Ring(1, 2000);
    if (!activeStatus->HeaterOn)
        StepStarted(activeStatus, steps, timeNow);
    else
        _pump->TurnPumpOn();

    activeStatus->SaveActiveStatus();
}

void MashService::BoilTime(ActiveStatus *activeStatus)
{
    Serial.println("Boil Time");
    activeStatus->ActiveStep = boil;
    activeStatus->StartTime = 0;
    activeStatus->EndTime = 0;
    activeStatus->ActiveMashStepIndex = -1;
    activeStatus->ActiveMashStepName = "";
    activeStatus->ActiveMashStepSufixName = "";
    activeStatus->TargetTemperature = activeStatus->BoilTargetTemperature;
    activeStatus->Recirculation = false;
    activeStatus->HeaterOn = true;
    Buzzer().Ring(2, 2000);
    _pump->TurnPumpOff();
    activeStatus->SaveActiveStatus();
}

void MashService::StepStarted(ActiveStatus *activeStatus, JsonArray steps, time_t timeNow)
{
    Serial.println("Step Started");
    JsonObject step = steps[activeStatus->ActiveMashStepIndex];
    activeStatus->StartTime = timeNow;
    activeStatus->EndTime = timeNow + (int(step["tm"]) * 60);
    activeStatus->HeaterOn = ((int)step["ho"]) == 1;
    activeStatus->StepLock = ((int)step["sl"]) == 1;
    activeStatus->ActiveMashStepName = step["n"].as<String>();
    activeStatus->ActiveMashStepSufixName = getMashName(activeStatus, step);

    Serial.print("Start time: ");
    Serial.println(activeStatus->StartTime);
    Serial.print("End Time: ");
    Serial.println(activeStatus->EndTime);
    Buzzer().Ring(1, 2000);

    activeStatus->Recirculation = ((int)step["r"]) == 1;
    if (activeStatus->Recirculation)
        _pump->TurnPumpOn();
    else
        _pump->TurnPumpOff();
    activeStatus->SaveActiveStatus();
}

String MashService::getMashName(ActiveStatus *activeStatus, JsonObject step)
{
    return step["tm"].as<String>() + "'@" +
           step["t"].as<String>() + activeStatus->TempUnit;
}