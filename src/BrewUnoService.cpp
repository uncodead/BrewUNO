#include <BrewUnoService.h>

BrewUnoService::BrewUnoService(AsyncWebServer *server,
                               FS *fs,
                               MashService *mashService,
                               BoilService *boilService,
                               CoolingService *coolingService,
                               BrewSettingsService *brewSettingsService,
                               MashKettleHeaterService *mashKettleHeaterService,
                               SpargeKettleHeaterService *spargeKettleHeaterService,
                               BoilKettleHeaterService *boilKettleHeaterService,
                               CoolingKettleHeaterService *coolingKettleHeaterService,
                               ActiveStatus *activeStatus,
                               TemperatureService *temperatureService,
                               Pump *pump,
                               Lcd *lcd) : BrewCommands(server,
                                                        fs,
                                                        mashService,
                                                        boilService,
                                                        coolingService,
                                                        brewSettingsService,
                                                        mashKettleHeaterService,
                                                        spargeKettleHeaterService,
                                                        boilKettleHeaterService,
                                                        coolingKettleHeaterService,
                                                        activeStatus,
                                                        temperatureService,
                                                        pump,
                                                        lcd)
{
    _server->on(START_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewUnoService::startBrewHttp, this, std::placeholders::_1));
    _server->on(STOP_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewUnoService::stopBrewHttp, this, std::placeholders::_1));
    _server->on(GET_ACTIVE_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&BrewUnoService::getActiveStatus, this, std::placeholders::_1));
    _server->on(NEXT_STEP_SERVICE_PATH, HTTP_POST, std::bind(&BrewUnoService::nextStepHttp, this, std::placeholders::_1));
    _server->on(PAUSE_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewUnoService::pauseBrewHttp, this, std::placeholders::_1));
    _server->on(RESUME_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewUnoService::resumeBrewHttp, this, std::placeholders::_1));
    _server->on(UNLOCK_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewUnoService::unLockBrewHttp, this, std::placeholders::_1));
    _server->on(START_BOIL_SERVICE_PATH, HTTP_POST, std::bind(&BrewUnoService::startBoilHttp, this, std::placeholders::_1));
    _server->on(START_BOIL_COUNTER_SERVICE_PATH, HTTP_POST, std::bind(&BrewUnoService::startBoilCounterHttp, this, std::placeholders::_1));
    _server->on(START_ANTICAVITATION_SERVICE_PATH, HTTP_POST, std::bind(&BrewUnoService::startAnticavitationHttp, this, std::placeholders::_1));
    _updateHandler.setUri(CHANGE_BOIL_PERCENTAGE_SERVICE_PATH);
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(1024);
    _updateHandler.onRequest(std::bind(&BrewUnoService::changeBoilPercentageHttp, this, std::placeholders::_1, std::placeholders::_2));
    _server->addHandler(&_updateHandler);
}

BrewUnoService::~BrewUnoService() {}

void BrewUnoService::startBrewHttp(AsyncWebServerRequest *request)
{
    start();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewUnoService::pauseBrewHttp(AsyncWebServerRequest *request)
{
    pause();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewUnoService::resumeBrewHttp(AsyncWebServerRequest *request)
{
    resume();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewUnoService::unLockBrewHttp(AsyncWebServerRequest *request)
{
    unLock();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewUnoService::stopBrewHttp(AsyncWebServerRequest *request)
{
    stop();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewUnoService::nextStepHttp(AsyncWebServerRequest *request)
{
    nextStep();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewUnoService::startBoilCounterHttp(AsyncWebServerRequest *request)
{
    startBoilCounter();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewUnoService::startBoilHttp(AsyncWebServerRequest *request)
{
    startBoil();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewUnoService::startAnticavitationHttp(AsyncWebServerRequest *request)
{
    startAnticavitation();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewUnoService::changeBoilPercentageHttp(AsyncWebServerRequest *request, JsonDocument &json)
{
    if (json.is<JsonObject>())
    {
        changeBoilPercentage(json["boil_power_percentage"]);
        request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
    }
    else
        request->send(500, APPLICATION_JSON_TYPE, INVALID_JSON_ERROR);
}

void BrewUnoService::getActiveStatus(AsyncWebServerRequest *request)
{
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewUnoService::begin()
{
    _mashService->LoadMashSettings();
    _boilService->LoadBoilSettings();
    _coolingService->LoadCoolingSettings();
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
void BrewUnoService::loop()
{
    _lcd->update();
    if (now() - lastReadTemperature > 1)
    {
        Temperatures temps = _temperatureService->GetTemperatures();
        _activeStatus->SetTemperature(temps);
        _activeStatus->MainSensor = _brewSettingsService->MainSensor;
        _activeStatus->SpargeSensor = _brewSettingsService->SpargeSensor;
        _activeStatus->BoilSensor = _brewSettingsService->BoilSensor;
        _activeStatus->AuxOneSensor = _brewSettingsService->AuxOneSensor;
        _activeStatus->AuxTwoSensor = _brewSettingsService->AuxTwoSensor;
        _activeStatus->AuxThreeSensor = _brewSettingsService->AuxThreeSensor;
        _activeStatus->TempUnit = _brewSettingsService->TempUnit;
        lastReadTemperature = now();
    }
    _mashService->loop(_activeStatus);
    _boilService->loop(_activeStatus);
    _coolingService->loop(_activeStatus);
    HeaterCompute();
    LoadSettingsToStatus();
    _pump->antiCavitation();
    _activeStatus->SaveActiveStatusLoop();
}

void BrewUnoService::HeaterCompute()
{
    HeaterServiceStatus mashStatus = _mashKettleHeaterService->Compute(_activeStatus->Temperature, _activeStatus->TargetTemperature, _brewSettingsService->MashHeaterPercentage);
    HeaterServiceStatus spargeStatus = _spargeKettleHeaterService->Compute(_activeStatus->SpargeTemperature, _brewSettingsService->SpargeTemperature, _brewSettingsService->SpargePowerPercentage);
    HeaterServiceStatus boilStatus = _boilKettleHeaterService->Compute(_activeStatus->BoilTemperature, _brewSettingsService->BoilTemperature, _brewSettingsService->BoilPowerPercentage);
    HeaterServiceStatus coolingStatus = _coolingKettleHeaterService->Compute(_activeStatus->CoolingTemperature, _activeStatus->CoolingTargetTemperature, _brewSettingsService->MashHeaterPercentage);
    _activeStatus->PWM = mashStatus.PWM;
    _activeStatus->PWMPercentage = mashStatus.PWMPercentage;
    _activeStatus->SpargePWM = spargeStatus.PWM;
    _activeStatus->SpargePWMPercentage = spargeStatus.PWMPercentage;
    _activeStatus->BoilPWM = boilStatus.PWM;
    _activeStatus->BoilPWMPercentage = boilStatus.PWMPercentage;
    _activeStatus->CoolingPWM = coolingStatus.PWM;
    _activeStatus->CoolingPWMPercentage = coolingStatus.PWMPercentage;
    _activeStatus->PIDActing = mashStatus.PIDActing;
    _activeStatus->BoilPowerPercentage = _brewSettingsService->BoilPowerPercentage;
}

void BrewUnoService::LoadSettingsToStatus()
{
    _activeStatus->BrewfatherId = _brewSettingsService->BrewfatherId;
    _activeStatus->BrewfatherKey = _brewSettingsService->BrewfatherKey;
}