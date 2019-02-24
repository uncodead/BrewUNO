#include <BrewService.h>

BrewService::BrewService(AsyncWebServer *server,
                         FS *fs,
                         MashService *mashService,
                         BoilService *boilService,
                         BrewSettingsService *brewSettingsService,
                         KettleHeaterService *kettleHeaterService,
                         ActiveStatus *activeStatus) : _server(server),
                                                       _fs(fs),
                                                       _mashService(mashService),
                                                       _boilService(boilService),
                                                       _brewSettingsService(brewSettingsService),
                                                       _kettleHeaterService(kettleHeaterService),
                                                       _activeStatus(activeStatus)

{
    _server->on(START_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::startBrew, this, std::placeholders::_1));
    _server->on(STOP_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::stopBrew, this, std::placeholders::_1));
    _server->on(GET_ACTIVE_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&BrewService::getActiveStatus, this, std::placeholders::_1));
    _server->on(NEXT_STEP_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::nextStep, this, std::placeholders::_1));
    _server->on(RESUME_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::resumeBrew, this, std::placeholders::_1));
}

BrewService::~BrewService() {}

void BrewService::startBrew(AsyncWebServerRequest *request)
{
    _activeStatus->TimeNow = now();
    _activeStatus->BoilTime = _brewSettingsService->BoilTime * 60;
    _activeStatus->BoilTargetTemperature = _brewSettingsService->BoilTemperature;
    _activeStatus->ActiveStep = mash;
    _activeStatus->BrewStarted = true;
    _activeStatus->ActiveMashStepIndex = 0;
    _activeStatus->ActiveBoilStepIndex = "";
    _activeStatus->SaveActiveStatus();

    _kettleHeaterService->SetTunings(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _kettleHeaterService->SetBoilPercent(_brewSettingsService->BoilPercent);

    request->send(200, "application/json", _activeStatus->GetJson());
}

void BrewService::stopBrew(AsyncWebServerRequest *request)
{
    _activeStatus->SaveActiveStatus(0, 0, 0, 0, -1, "", 0, 0, none, false);
    _kettleHeaterService->SetMode(MANUAL);
    
    String json = _activeStatus->GetJson();
    request->send(200, "application/json", json);
}

void BrewService::resumeBrew(AsyncWebServerRequest *request)
{
    timeStatus_t status = timeStatus();
    if (status != timeSet)
    {
        request->send(200, "application/json ", _activeStatus->GetJson());
        return;
    }

    _activeStatus->BrewStarted = true;
    if (_activeStatus->StartTime > 0 && _activeStatus->EndTime > 0)
    {
        int timeTotal = _activeStatus->EndTime - _activeStatus->StartTime;
        int timeSpent = _activeStatus->TimeNow - _activeStatus->StartTime;
        int timeLeft = timeTotal - timeSpent;
        _activeStatus->EndTime = now() + timeLeft;
    }
    _activeStatus->SaveActiveStatus();

    _kettleHeaterService->SetTunings(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _kettleHeaterService->SetBoilPercent(_brewSettingsService->BoilPercent);

    request->send(200, "application/json ", _activeStatus->GetJson());
}

void BrewService::nextStep(AsyncWebServerRequest *request)
{
    timeStatus_t status = timeStatus();
    if (status != timeSet)
    {
        request->send(200, "application/json ", _activeStatus->GetJson());
        return;
    }
    _activeStatus->EndTime = now();
    _activeStatus->SaveActiveStatus();
    request->send(200, "application/json ", _activeStatus->GetJson());
}

void BrewService::getActiveStatus(AsyncWebServerRequest *request)
{
    request->send(200, "application/json ", _activeStatus->GetJson());
}

void BrewService::begin()
{
    _mashService->LoadMashSettings();
    _boilService->LoadBoilSettings();

    _activeStatus->LoadActiveStatusSettings();
    _activeStatus->BrewStarted = false;
    _activeStatus->SaveActiveStatus();
}

void BrewService::loop()
{
    timeStatus_t status = timeStatus();
    if (status != timeSet)
        return;

    _mashService->loop(_activeStatus);
    _boilService->loop(_activeStatus);

    _kettleHeaterService->Compute(_activeStatus);

    _activeStatus->SaveActiveStatus();

    delay(5000);
}