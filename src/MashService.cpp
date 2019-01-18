#include <MashService.h>

DynamicJsonBuffer jsonBufferMash;

MashService::MashService(FS *fs, TemperatureService *temperatureService) : _fs(fs),
                                                                           _temperatureService(temperatureService)
{
    _startTime = 0;
    _endTime = 0;
}

MashService::~MashService() {}

void MashService::LoadMashSettings()
{
    _mashSettings = &LoadSettings(MASH_SETTINGS_FILE);
    JsonObject &step = _mashSettings->get<JsonArray>("steps")[0];
    _targetTemperature = step["temperature"];
}

JsonObject &MashService::LoadSettings(String settingsFile)
{
    File configFile = _fs->open(settingsFile, "r");
    JsonObject *root = &(jsonBufferMash.parseObject(configFile));
    configFile.close();
    return *root;
}

float MashService::getTemperature()
{
    return _temperatureService->GetTemperature();
}

int MashService::GetActiveStep()
{
    return _activeMashStepIndex;
}

void MashService::loop(time_t timeNow, boolean &_brewStarted, StepType &_activeStep, float &_setPoint)
{
    if (!_brewStarted || _activeStep != 0)
    {
        return;
    }

    if (_mashSettings == NULL)
    {
        Serial.println("Load Mash Settings");
        LoadMashSettings();
    }

    if (_endTime > 0 && timeNow > _endTime)
    {
        Serial.println("Step over");
        unsigned int nextMashStep = _activeMashStepIndex + 1;
        if (_mashSettings->get<JsonArray>("steps").size() > nextMashStep)
        {
            JsonObject &step = _mashSettings->get<JsonArray>("steps")[nextMashStep];
            _activeMashStepIndex = nextMashStep;
            _startTime = 0;
            _endTime = 0;
            _targetTemperature = step["temperature"];
            Serial.print("Next step name: ");
            Serial.printf(step["name"]);
            Serial.println("");
            Serial.println("Next step temperature: ");
            Serial.printf(step["temperature"]);
        }
        else
        {
            Serial.println("Boil Time");
            _activeStep = boil;
            _startTime = 0;
            _endTime = 0;
        }
    }
    else
    {
        Serial.print("Temperature: ");
        Serial.println(getTemperature());
        Serial.print("Target: ");
        Serial.println(_targetTemperature);
        if (_startTime == 0 && (getTemperature() >= (_targetTemperature - 0.2)))
        {
            Serial.println("Step Started");
            JsonObject &step = _mashSettings->get<JsonArray>("steps")[_activeMashStepIndex];
            _startTime = timeNow;
            _endTime = timeNow + (int(step["time"]) * 60);

            Serial.print("Start time: ");
            Serial.println(_startTime);
            Serial.print("End Time: ");
            Serial.println(_endTime);
            // recirculation
        }
    }

    _setPoint = _targetTemperature;
}
