#include <BrewUNO/BrewService.h>

BrewService::BrewService(AsyncWebServer *server,
                         FS *fs,
                         MashService *mashService,
                         BoilService *boilService,
                         BrewSettingsService *brewSettingsService,
                         MashKettleHeaterService *mashKettleHeaterService,
                         SpargeKettleHeaterService *spargeKettleHeaterService,
                         ActiveStatus *activeStatus,
                         TemperatureService *temperatureService,
                         Pump *pump) : _server(server),
                                       _fs(fs),
                                       _mashService(mashService),
                                       _boilService(boilService),
                                       _brewSettingsService(brewSettingsService),
                                       _mashKettleHeaterService(mashKettleHeaterService),
                                       _spargeKettleHeaterService(spargeKettleHeaterService),
                                       _activeStatus(activeStatus),
                                       _temperatureService(temperatureService),
                                       _pump(pump)
{
    _server->on(START_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::startBrew, this, std::placeholders::_1));
    _server->on(STOP_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::stopBrew, this, std::placeholders::_1));
    _server->on(GET_ACTIVE_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&BrewService::getActiveStatus, this, std::placeholders::_1));
    _server->on(NEXT_STEP_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::nextStep, this, std::placeholders::_1));
    _server->on(PAUSE_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::pauseBrew, this, std::placeholders::_1));
    _server->on(RESUME_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::resumeBrew, this, std::placeholders::_1));
    _server->on(UNLOCK_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::unLockBrew, this, std::placeholders::_1));
    _server->on(START_BOIL_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::startBoil, this, std::placeholders::_1));
    _updateHandler.setUri(CHANGE_BOIL_PERCENTAGE_SERVICE_PATH);
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(1024);
    _updateHandler.onRequest(std::bind(&BrewService::changeBoilPercentage, this, std::placeholders::_1, std::placeholders::_2));
    _server->addHandler(&_updateHandler);
}

BrewService::~BrewService() {}

void BrewService::startBrew(AsyncWebServerRequest *request)
{
    _activeStatus->TimeNow = now();
    _activeStatus->ActiveStep = mash;
    _activeStatus->BrewStarted = true;
    _activeStatus->ActiveMashStepIndex = 0;
    _activeStatus->ActiveBoilStepIndex = "";
    _activeStatus->BoilTime = _brewSettingsService->BoilTime * 60;
    _activeStatus->BoilTargetTemperature = _brewSettingsService->BoilTemperature;
    _activeStatus->BoilPowerPercentage = _brewSettingsService->BoilPowerPercentage;
    _activeStatus->SaveActiveStatus();
    _mashKettleHeaterService->StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _spargeKettleHeaterService->StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _mashService->LoadMashSettings();
    _boilService->LoadBoilSettings();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::unLockBrew(AsyncWebServerRequest *request)
{
    _activeStatus->StepLock = false;
    _activeStatus->SaveActiveStatus();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::stopBrew(AsyncWebServerRequest *request)
{
    _activeStatus->SaveActiveStatus(0, 0, 0, 0, -1, "", 0, 0, none, false);
    _pump->TurnPumpOff();
    _mashKettleHeaterService->Compute(_activeStatus->Temperature, _activeStatus->TargetTemperature, _brewSettingsService->MashHeaterPercentage);
    _spargeKettleHeaterService->Compute(_activeStatus->SpargeTemperature, _brewSettingsService->SpargeTemperature, _brewSettingsService->SpargePowerPercentage);
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::pauseBrew(AsyncWebServerRequest *request)
{
    _activeStatus->BrewStarted = false;
    _pump->TurnPumpOff();
    _activeStatus->SaveActiveStatus();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::resumeBrew(AsyncWebServerRequest *request)
{
    if (_activeStatus->StartTime > 0 && _activeStatus->EndTime > 0)
    {
        int timeTotal = _activeStatus->EndTime - _activeStatus->StartTime;
        int timeSpent = _activeStatus->TimeNow - _activeStatus->StartTime;
        int timeLeft = timeTotal - timeSpent;
        _activeStatus->EndTime = now() + timeLeft;
    }
    _activeStatus->BoilPowerPercentage = _brewSettingsService->BoilPowerPercentage;
    _activeStatus->BrewStarted = true;
    _activeStatus->SaveActiveStatus();
    _mashKettleHeaterService->StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _spargeKettleHeaterService->StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _mashService->LoadMashSettings();
    _boilService->LoadBoilSettings();
    if (_activeStatus->Recirculation || _activeStatus->StartTime <= 0)
        _pump->TurnPumpOn();
    else
        _pump->TurnPumpOff();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::nextStep(AsyncWebServerRequest *request)
{
    _activeStatus->StepLock = false;
    _activeStatus->EndTime = now();
    _activeStatus->SaveActiveStatus();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::startBoil(AsyncWebServerRequest *request)
{
    _activeStatus->TimeNow = now();
    _activeStatus->ActiveStep = boil;
    _activeStatus->BrewStarted = true;
    _activeStatus->HeaterOn = true;
    _activeStatus->ActiveBoilStepIndex = "";
    _activeStatus->BoilTime = _brewSettingsService->BoilTime * 60;
    _activeStatus->BoilTargetTemperature = _brewSettingsService->BoilTemperature;
    _activeStatus->BoilPowerPercentage = _brewSettingsService->BoilPowerPercentage;
    _activeStatus->TargetTemperature = _brewSettingsService->BoilTemperature;
    _activeStatus->SaveActiveStatus();
    _mashKettleHeaterService->StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _boilService->LoadBoilSettings();
    _pump->TurnPumpOff();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::changeBoilPercentage(AsyncWebServerRequest *request, JsonDocument &json)
{
    if (json.is<JsonObject>())
    {
        _activeStatus->BoilPowerPercentage = json["boil_power_percentage"];
        _activeStatus->SaveActiveStatus();
        request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
    }
    else
        request->send(500, APPLICATION_JSON_TYPE, INVALID_JSON_ERROR);
}

void BrewService::getActiveStatus(AsyncWebServerRequest *request)
{
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::begin()
{
    _mashService->LoadMashSettings();
    _boilService->LoadBoilSettings();
    _activeStatus->LoadActiveStatusSettings();
    _activeStatus->BrewStarted = false;
    if (_temperatureService->DeviceCount == 1)
    {
        _brewSettingsService->MainSensor = _temperatureService->GetFirstSensorAddress();
        _activeStatus->MainSensor = _brewSettingsService->MainSensor;
        _brewSettingsService->writeToFS();
    }
}

time_t lastReadTemperature = now();
void BrewService::loop()
{
    if (now() - lastReadTemperature > 1)
    {
        Temperatures temps = _temperatureService->GetTemperatures(_brewSettingsService->MainSensor, _brewSettingsService->SpargeSensor);
        _activeStatus->SetTemperature(temps.Main);
        _activeStatus->SetSpargeTemperature(temps.Sparge);
        _activeStatus->SetJsonTemperatures(temps.Json);
        _activeStatus->MainSensor = _brewSettingsService->MainSensor;
        _activeStatus->SpargeSensor = _brewSettingsService->SpargeSensor;
        lastReadTemperature = now();
    }

    _mashService->loop(_activeStatus);
    _boilService->loop(_activeStatus);
    HeaterCompute();
    if (_activeStatus->BrewStarted)
    {
        _activeStatus->TimeNow = now();
        _activeStatus->SaveActiveStatusLoop();
    }
}

void BrewService::HeaterCompute()
{
    HeaterServiceStatus mashStatus = _mashKettleHeaterService->Compute(_activeStatus->Temperature, _activeStatus->TargetTemperature, _brewSettingsService->MashHeaterPercentage);
    HeaterServiceStatus spargeStatus = _spargeKettleHeaterService->Compute(_activeStatus->SpargeTemperature, _brewSettingsService->SpargeTemperature, _brewSettingsService->SpargePowerPercentage);
    _activeStatus->PWM = mashStatus.PWM;
    _activeStatus->PWMPercentage = mashStatus.PWMPercentage;
    _activeStatus->SpargePWM = spargeStatus.PWM;
    _activeStatus->SpargePWMPercentage = spargeStatus.PWMPercentage;
    _activeStatus->PIDActing = mashStatus.PIDActing;
}