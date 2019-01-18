#include <BrewService.h>

BrewService::BrewService(AsyncWebServer *server,
                         FS *fs,
                         MashService *mashService,
                         BoilService *boilService,
                         BrewSettingsService *brewSettingsService,
                         KettleHeaterService *kettleHeaterService) : _server(server),
                                                                     _fs(fs),
                                                                     _mashService(mashService),
                                                                     _boilService(boilService),
                                                                     _brewSettingsService(brewSettingsService),
                                                                     _kettleHeaterService(kettleHeaterService)

{
    _activeStep = none;
    _server->on(START_BREW_SERVICE_PATH, HTTP_GET, std::bind(&BrewService::startBrew, this, std::placeholders::_1));
    _server->on(GET_ACTIVE_STEP_SERVICE_PATH, HTTP_GET, std::bind(&BrewService::getActiveStep, this, std::placeholders::_1));
}

BrewService::~BrewService() {}

void BrewService::startBrew(AsyncWebServerRequest *request)
{
    _mashService->LoadMashSettings();
    _boilService->LoadBoilSettings(_brewSettingsService->BoilTemperature, _brewSettingsService->BoilTime);

    _brewStarted = true;
    _activeStep = mash;

    _kettleHeaterService->SetK(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _kettleHeaterService->SetBoilPercent(_brewSettingsService->BoilPercent);
    
    request->send(200, "application/json", "{ brew_started: true }");
}

void BrewService::getActiveStep(AsyncWebServerRequest *request)
{
    int mashStepIndex = _mashService->GetActiveStep();
    String boilStepIndex = _boilService->GetBoilStepIndex();
    request->send(200, "application/json", "{ active_step: " + String(_activeStep) + ", active_mash_step_index: " + mashStepIndex + ", boil_step_index:" + boilStepIndex + " }");
}

void BrewService::loop()
{
    time_t timeNow = now();
    timeStatus_t status = timeStatus();
    if (status != 2)
    {
        return;
    }

    _mashService->loop(timeNow, _brewStarted, _activeStep, _setPoint);
    _boilService->loop(timeNow, _brewStarted, _activeStep, _setPoint);

    _kettleHeaterService->SetSetpoint(_setPoint);
    _kettleHeaterService->Compute(_activeStep == 1);

    delay(30000);
}