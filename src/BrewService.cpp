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
    _activeStatus->SaveActiveStatus(0, 0, 0, 0, -1, "", 0, 0, none, false);
    _server->on(START_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::startBrew, this, std::placeholders::_1));
    _server->on(STOP_BREW_SERVICE_PATH, HTTP_POST, std::bind(&BrewService::stopBrew, this, std::placeholders::_1));
    _server->on(GET_ACTIVE_STATUS_SERVICE_PATH, HTTP_GET, std::bind(&BrewService::getActiveStatus, this, std::placeholders::_1));
}

BrewService::~BrewService() {}

void BrewService::startBrew(AsyncWebServerRequest *request)
{
    String json = "";
    _mashService->LoadMashSettings();
    _boilService->LoadBoilSettings();

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

    _activeStatus->GetJson()->printTo(json);
    request->send(200, "application/json", json);
}

void BrewService::stopBrew(AsyncWebServerRequest *request)
{
    String json = "";
    _activeStatus->SaveActiveStatus(0, 0, 0, 0, -1, "", 0, 0, none, false);
    _activeStatus->GetJson()->printTo(json);
    request->send(200, "application/json", json);
}

void BrewService::getActiveStatus(AsyncWebServerRequest *request)
{
    String json = "";
    _activeStatus->GetJson()->printTo(json);
    request->send(200, "application/json", json);
}

void BrewService::loop()
{
    timeStatus_t status = timeStatus();
    if (status != timeSet) return;

    _mashService->loop(_activeStatus);
    _boilService->loop(_activeStatus);

    _kettleHeaterService->Compute(_activeStatus);

    _activeStatus->SaveActiveStatus();

    delay(30000);
}