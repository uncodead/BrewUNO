#include <BoilService.h>

DynamicJsonBuffer jsonBufferBoil;

BoilService::BoilService(FS *fs, TemperatureService *temperatureService) : _fs(fs),
                                                                           _temperatureService(temperatureService)
{
    _startTime = 0;
    _endTime = 0;
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

void BoilService::SetTemperature(float temperature)
{
    _targetTemperature = temperature;
}

void BoilService::SetTime(int time)
{
    _boilTime = time * 60;
}

float BoilService::getTemperature()
{
    return _temperatureService->GetTemperature();
}

String BoilService::GetBoilStepIndex()
{
    return _boilStepIndex;
}

void BoilService::loop(time_t timeNow, boolean &_brewStarted, StepType &_activeStep, float &_setPoint)
{
    if (!_brewStarted || _activeStep != 1)
    {
        return;
    }

    if (_startTime == 0 && getTemperature() >= _targetTemperature)
    {
        _startTime = timeNow;
        _endTime = _startTime + _boilTime; //get from settings im seconds
        Serial.println("Boil started");
        Serial.println(_startTime);
        Serial.println(_endTime);
    }
    if (_endTime > 0 && timeNow > _endTime)
    {
        Serial.println("Boil ended");
        _startTime = 0;
        _endTime = 0;
        _activeStep = none;
        _brewStarted = false;
        return;
    }

    time_t moment = _endTime - timeNow;
    SetBoiIndexStep(moment / 60);
    _setPoint = _targetTemperature;
}

void BoilService::SetBoiIndexStep(time_t moment)
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
    if (currentStep != "" && currentStep != _boilStepIndex)
    {
        _boilStepIndex = currentStep;
        Serial.println(currentStep);
        Serial.println(_boilStepIndex);
        Serial.println("buzzer... ");
    }
}