#include <BrewService.h>

BrewService::BrewService(AsyncWebServer *server,
                         FS *fs,
                         MashService *mashService,
                         BoilService *boilService,
                         BrewSettingsService *brewSettingsService,
                         MashKettleHeaterService *mashKettleHeaterService,
                         SpargeKettleHeaterService *spargeKettleHeaterService,
                         BoilKettleHeaterService *boilKettleHeaterService,
                         ActiveStatus *activeStatus,
                         TemperatureService *temperatureService,
                         Pump *pump,
                         Lcd *lcd) : _server(server),
                                     _fs(fs),
                                     _mashService(mashService),
                                     _boilService(boilService),
                                     _brewSettingsService(brewSettingsService),
                                     _mashKettleHeaterService(mashKettleHeaterService),
                                     _spargeKettleHeaterService(spargeKettleHeaterService),
                                     _boilKettleHeaterService(boilKettleHeaterService),
                                     _activeStatus(activeStatus),
                                     _temperatureService(temperatureService),
                                     _pump(pump),
                                     _lcd(lcd)
{
    _server->on(START_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::startBrewHttp, this, std::placeholders::_1));
    _server->on(STOP_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::stopBrewHttp, this, std::placeholders::_1));
    _server->on(GET_ACTIVE_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&BrewService::getActiveStatus, this, std::placeholders::_1));
    _server->on(NEXT_STEP_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::nextStepHttp, this, std::placeholders::_1));
    _server->on(PAUSE_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::pauseBrewHttp, this, std::placeholders::_1));
    _server->on(RESUME_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::resumeBrewHttp, this, std::placeholders::_1));
    _server->on(UNLOCK_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::unLockBrewHttp, this, std::placeholders::_1));
    _server->on(START_BOIL_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::startBoilHttp, this, std::placeholders::_1));
    _server->on(START_BOIL_COUNTER_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::startBoilCounterHttp, this, std::placeholders::_1));
    _server->on(START_ANTICAVITATION_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::startAnticavitation, this, std::placeholders::_1));
    _updateHandler.setUri(CHANGE_BOIL_PERCENTAGE_SERVICE_PATH);
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(1024);
    _updateHandler.onRequest(std::bind(&BrewService::changeBoilPercentage, this, std::placeholders::_1, std::placeholders::_2));
    _server->addHandler(&_updateHandler);
}

BrewService::~BrewService() {}

void BrewService::startBrewHttp(AsyncWebServerRequest *request)
{
    startBrew();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::startBrew()
{
    _activeStatus->TimeNow = now();
    _activeStatus->ActiveStep = mash;
    _activeStatus->BrewStarted = true;
    _activeStatus->ActiveMashStepIndex = 0;
    _activeStatus->ActiveBoilStepIndex = "";
    _activeStatus->BoilTime = _brewSettingsService->BoilTime * 60;
    _activeStatus->BoilTargetTemperature = _brewSettingsService->BoilTemperature;
    _activeStatus->SaveActiveStatus();
    _mashKettleHeaterService->StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _spargeKettleHeaterService->StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _boilKettleHeaterService->StartPID(100, 100, 100);
    _mashService->LoadMashSettings();
    _boilService->LoadBoilSettings();
}

void BrewService::pauseBrewHttp(AsyncWebServerRequest *request)
{
    pauseBrew();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::pauseBrew()
{
    _activeStatus->BrewStarted = false;
    _pump->TurnPumpOff();
    _activeStatus->SaveActiveStatus();
}

void BrewService::resumeBrewHttp(AsyncWebServerRequest *request)
{
    resumeBrew();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::resumeBrew()
{
    if (_activeStatus->StartTime > 0 && _activeStatus->EndTime > 0)
    {
        int timeTotal = _activeStatus->EndTime - _activeStatus->StartTime;
        int timeSpent = _activeStatus->TimeNow - _activeStatus->StartTime;
        int timeLeft = timeTotal - timeSpent;
        _activeStatus->EndTime = now() + timeLeft;
    }
    _activeStatus->BrewStarted = true;
    _activeStatus->SaveActiveStatus();
    _mashKettleHeaterService->StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _spargeKettleHeaterService->StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _boilKettleHeaterService->StartPID(100, 100, 100);
    _mashService->LoadMashSettings();
    _boilService->LoadBoilSettings();
    if (_activeStatus->Recirculation || _activeStatus->StartTime <= 0)
    {
        if (_activeStatus->ActiveStep == mash)
            _pump->TurnPumpOn();
    }
    else
        _pump->TurnPumpOff();
}

void BrewService::unLockBrewHttp(AsyncWebServerRequest *request)
{
    unLockBrew();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::unLockBrew()
{
    _activeStatus->StepLock = false;
    _activeStatus->SaveActiveStatus();
}

void BrewService::stopBrewHttp(AsyncWebServerRequest *request)
{
    stopBrew();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::stopBrew()
{
    _activeStatus->SaveActiveStatus(0, 0, 0, 0, -1, "", 0, 0, none, false);
    _pump->TurnPumpOff();
    _mashKettleHeaterService->Compute(_activeStatus->Temperature, _activeStatus->TargetTemperature, _brewSettingsService->MashHeaterPercentage);
    _spargeKettleHeaterService->Compute(_activeStatus->SpargeTemperature, _brewSettingsService->SpargeTemperature, _brewSettingsService->SpargePowerPercentage);
    _boilKettleHeaterService->Compute(_activeStatus->BoilTemperature, _brewSettingsService->BoilTemperature, _brewSettingsService->BoilPowerPercentage);
}

void BrewService::nextStepHttp(AsyncWebServerRequest *request)
{
    nextStep();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::nextStep()
{
    _activeStatus->StepLock = false;
    _activeStatus->EndTime = now();
    _activeStatus->SaveActiveStatus();
}

void BrewService::startBoilCounterHttp(AsyncWebServerRequest *request)
{
    startBoilCounter();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::startBoilHttp(AsyncWebServerRequest *request)
{
    startBoil();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::startBoilCounter()
{
    _activeStatus->StartBoilCounter = true;
}

void BrewService::startBoil()
{
    _activeStatus->TimeNow = now();
    _activeStatus->ActiveStep = boil;
    _activeStatus->BrewStarted = true;
    _activeStatus->HeaterOn = true;
    _activeStatus->ActiveBoilStepIndex = "";
    _activeStatus->BoilTime = _brewSettingsService->BoilTime * 60;
    _activeStatus->BoilTargetTemperature = _brewSettingsService->BoilTemperature;
    _activeStatus->SaveActiveStatus();
    _boilKettleHeaterService->StartPID(100, 100, 100);
    _boilService->LoadBoilSettings();
    _pump->TurnPumpOff();
}

void BrewService::startAnticavitation(AsyncWebServerRequest *request)
{
    if (!_activeStatus->BrewStarted)
    {
        _activeStatus->LastActiveStep = _activeStatus->ActiveStep;
        _activeStatus->ActiveStep = anticavitation;
    }
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::changeBoilPercentage(AsyncWebServerRequest *request, JsonDocument &json)
{
    if (json.is<JsonObject>())
    {
        _brewSettingsService->BoilPowerPercentage = json["boil_power_percentage"];
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
        _brewSettingsService->BoilSensor = _brewSettingsService->MainSensor;
        _activeStatus->MainSensor = _brewSettingsService->MainSensor;
        _activeStatus->BoilSensor = _brewSettingsService->BoilSensor;
    }
    if (_temperatureService->DeviceCount > 1 && _brewSettingsService->BoilSensor == "")
        _brewSettingsService->BoilSensor = _temperatureService->GetFirstSensorAddress();
}

time_t lastReadTemperature = now();
void BrewService::loop()
{
    _lcd->update();
    if (now() - lastReadTemperature > 1)
    {
        Temperatures temps = _temperatureService->GetTemperatures();
        _activeStatus->SetTemperature(temps);
        _activeStatus->MainSensor = _brewSettingsService->MainSensor;
        _activeStatus->SpargeSensor = _brewSettingsService->SpargeSensor;
        _activeStatus->BoilSensor = _brewSettingsService->EnableBoilKettle ? _brewSettingsService->BoilSensor : _activeStatus->MainSensor;
        _activeStatus->AuxOneSensor = _brewSettingsService->AuxOneSensor;
        _activeStatus->AuxTwoSensor = _brewSettingsService->AuxTwoSensor;
        _activeStatus->AuxThreeSensor = _brewSettingsService->AuxThreeSensor;
        _activeStatus->TempUnit = _brewSettingsService->TempUnit;
        lastReadTemperature = now();
    }
    _mashService->loop(_activeStatus);
    _boilService->loop(_activeStatus);
    HeaterCompute();
    _pump->antiCavitation();
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
    HeaterServiceStatus boilStatus = _boilKettleHeaterService->Compute(_activeStatus->BoilTemperature, _brewSettingsService->BoilTemperature, _brewSettingsService->BoilPowerPercentage);
    _activeStatus->PWM = mashStatus.PWM;
    _activeStatus->PWMPercentage = mashStatus.PWMPercentage;
    _activeStatus->SpargePWM = spargeStatus.PWM;
    _activeStatus->SpargePWMPercentage = spargeStatus.PWMPercentage;
    _activeStatus->BoilPWM = boilStatus.PWM;
    _activeStatus->BoilPWMPercentage = boilStatus.PWMPercentage;
    _activeStatus->PIDActing = mashStatus.PIDActing;
    _activeStatus->BoilPowerPercentage = _brewSettingsService->BoilPowerPercentage;
}