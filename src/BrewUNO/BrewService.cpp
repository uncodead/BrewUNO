#include <BrewUNO/BrewService.h>

BrewService::BrewService(AsyncWebServer *server,
                         FS *fs,
                         MashService *mashService,
                         BoilService *boilService,
                         BrewSettingsService *brewSettingsService,
                         KettleHeaterService *kettleHeaterService,
                         ActiveStatus *activeStatus,
                         TemperatureService *temperatureService) : _server(server),
                                                                   _fs(fs),
                                                                   _mashService(mashService),
                                                                   _boilService(boilService),
                                                                   _brewSettingsService(brewSettingsService),
                                                                   _kettleHeaterService(kettleHeaterService),
                                                                   _activeStatus(activeStatus),
                                                                   _temperatureService(temperatureService)
{
    _server->on(START_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::startBrew, this, std::placeholders::_1));
    _server->on(STOP_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::stopBrew, this, std::placeholders::_1));
    _server->on(GET_ACTIVE_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&BrewService::getActiveStatus, this, std::placeholders::_1));
    _server->on(NEXT_STEP_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::nextStep, this, std::placeholders::_1));
    _server->on(RESUME_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::resumeBrew, this, std::placeholders::_1));
    _server->on(START_BOIL_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::startBoil, this, std::placeholders::_1));
    _server->on(START_TUNING_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::startTuning, this, std::placeholders::_1));

    // configure update settings handler
    _updateHandler.setUri(CHANGE_BOIL_PERCENTAGE_SERVICE_PATH);
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(1024);
    _updateHandler.onRequest(std::bind(&BrewService::changeBoilPercentage, this, std::placeholders::_1, std::placeholders::_2));
    _server->addHandler(&_updateHandler);
}

BrewService::~BrewService() {}

void BrewService::startBrew(AsyncWebServerRequest *request)
{
    if (timeStatus() != timeSet)
    {
        request->send(500, APPLICATION_JSON_TYPE, NPT_JSON_ERROR_MESSAGE);
        return;
    }

    _activeStatus->TimeNow = now();
    _activeStatus->ActiveStep = mash;
    _activeStatus->BrewStarted = true;
    _activeStatus->ActiveMashStepIndex = 0;
    _activeStatus->ActiveBoilStepIndex = "";
    _activeStatus->BoilTime = _brewSettingsService->BoilTime * 60;
    _activeStatus->BoilTargetTemperature = _brewSettingsService->BoilTemperature;
    _activeStatus->BoilPowerPercentage = _brewSettingsService->BoilPowerPercentage;
    _activeStatus->SaveActiveStatus();
    _kettleHeaterService->SetTunings(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    //_kettleHeaterService->SetSampleTime(_brewSettingsService->SampleTime * 1000);
    _mashService->LoadMashSettings();
    _boilService->LoadBoilSettings();

    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::resumeBrew(AsyncWebServerRequest *request)
{
    if (timeStatus() != timeSet)
    {
        request->send(500, APPLICATION_JSON_TYPE, NPT_JSON_ERROR_MESSAGE);
        return;
    }

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

    _kettleHeaterService->SetTunings(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    //_kettleHeaterService->SetSampleTime(_brewSettingsService->SampleTime * 1000);

    _mashService->LoadMashSettings();
    _boilService->LoadBoilSettings();

    Pump().TurnPump(_activeStatus->Recirculation);

    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::stopBrew(AsyncWebServerRequest *request)
{
    _activeStatus->SaveActiveStatus(0, 0, 0, 0, -1, "", 0, 0, none, false);
    //_kettleHeaterService->DisablePID();
    Pump().TurnPumpOff();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::nextStep(AsyncWebServerRequest *request)
{
    if (timeStatus() != timeSet)
    {
        request->send(500, APPLICATION_JSON_TYPE, NPT_JSON_ERROR_MESSAGE);
        return;
    }

    _activeStatus->EndTime = now();
    _activeStatus->SaveActiveStatus();
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
}

void BrewService::startBoil(AsyncWebServerRequest *request)
{
    if (timeStatus() != timeSet)
    {
        request->send(500, APPLICATION_JSON_TYPE, NPT_JSON_ERROR_MESSAGE);
        return;
    }

    _activeStatus->TimeNow = now();
    _activeStatus->ActiveStep = boil;
    _activeStatus->BrewStarted = true;
    _activeStatus->ActiveBoilStepIndex = "";
    _activeStatus->BoilTime = _brewSettingsService->BoilTime * 60;
    _activeStatus->BoilTargetTemperature = _brewSettingsService->BoilTemperature;
    _activeStatus->BoilPowerPercentage = _brewSettingsService->BoilPowerPercentage;
    _activeStatus->TargetTemperature = _brewSettingsService->BoilTemperature;
    _activeStatus->SaveActiveStatus();

    _kettleHeaterService->SetTunings(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    //_kettleHeaterService->SetSampleTime(_brewSettingsService->SampleTime * 1000);

    _boilService->LoadBoilSettings();

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

void BrewService::startTuning(AsyncWebServerRequest *request)
{
    _kettleHeaterService->StartAutoTune();
    _activeStatus->PIDTuning = true;
    request->send(200, APPLICATION_JSON_TYPE, _activeStatus->GetJson());
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
    _activeStatus->SaveActiveStatus();
    //_kettleHeaterService->DisablePID();
}

void BrewService::loop()
{
    timeStatus_t status = timeStatus();
    if (!_activeStatus->BrewStarted && status != timeSet)
        return;

    _activeStatus->SetTemperature(_temperatureService->GetTemperature());
    _mashService->loop(_activeStatus);
    _boilService->loop(_activeStatus);
    _kettleHeaterService->Compute();
    _activeStatus->TimeNow = now();
    _activeStatus->SaveActiveStatusLoop();
    //delay(_brewSettingsService->SampleTime * 1000);
}