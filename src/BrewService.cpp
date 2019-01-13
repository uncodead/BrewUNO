#include <BrewService.h>

DynamicJsonBuffer jsonBuffer;

BrewService::BrewService(AsyncWebServer *server, FS *fs) : _server(server), _fs(fs)
{
    _activeStep = none;
    _startTime = 0;
    _endTime = 0;

    //temp
    _temperature = 30;
    _server->on("/rest/temp", HTTP_GET, std::bind(&BrewService::temp, this, std::placeholders::_1));
}

BrewService::~BrewService() {}

void BrewService::temp(AsyncWebServerRequest *request)
{
    _temperature += 0.5;
    _brewStarted = true;
    request->send(200, "text/plain charset=utf-8", String(_temperature));
}

void BrewService::startBrew(AsyncWebServerRequest *request)
{
    /*
    Load mash
    Load Boil
    _brewstarted = true
    _activeStep = mash;
    _startTime = 0;
    _endTime = 0;
    return json {mash,boil}
    */
}

void BrewService::getActiveStep(AsyncWebServerRequest *request)
{
    /*
    _activeStep:
    _activeMashStepIndex:
    _boilStepIndex:
    */
}

void BrewService::LoadBoilSettings()
{
    _boilSettings = &LoadSettings(BOIL_SETTINGS_FILE);
}

void BrewService::LoadMashSettings()
{
    _mashSettings = &LoadSettings(MASH_SETTINGS_FILE);
    JsonObject &step = _mashSettings->get<JsonArray>("steps")[0];
    _targetTemperature = step["temperature"];
}

JsonObject &BrewService::LoadSettings(String settingsFile)
{
    File configFile = _fs->open(settingsFile, "r");
    JsonObject *root = &(jsonBuffer.parseObject(configFile));
    configFile.close();
    return *root;
}

float BrewService::getTemperature()
{
    return _temperature;
}

void BrewService::loop()
{
    time_t timeNow = now();
    timeStatus_t status = timeStatus();
    if (status != 2)
    {
        return;
    }

    if (_activeStep == 2)
    {
        _activeStep = mash;
    }

    loopMash(timeNow);
    loopBoil(timeNow);

    delay(30000);
}

void BrewService::loopMash(time_t timeNow)
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
            _targetTemperature = 95; //check
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
}

void BrewService::loopBoil(time_t timeNow)
{
    if (!_brewStarted || _activeStep != 1)
    {
        return;
    }

    if (_startTime == 0 && getTemperature() >= _targetTemperature) //get from settings
    {
        LoadBoilSettings();
        _startTime = timeNow;
        _endTime = _startTime + 300; //get from settings im seconds
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
}

void BrewService::SetBoiIndexStep(time_t moment)
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