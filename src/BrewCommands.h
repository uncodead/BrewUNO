#ifndef BrewCommands_h
#define BrewCommands_h

#include <TemperatureService.h>
#include <ActiveStatus.h>
#include <PID_v1.h>

#include <MashService.h>
#include <BoilService.h>
#include <CoolingService.h>
#include <MashKettleHeaterService.h>
#include <SpargeKettleHeaterService.h>
#include <CoolingKettleHeaterService.h>
#include <BrewSettingsService.h>
#include <ActiveStatus.h>
#include <Pump.h>
#include <BoilKettleHeaterService.h>

class BrewCommands
{
public:
  BrewCommands(AsyncWebServer *server,
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
               Lcd *lcd) : _server(server),
                           _fs(fs),
                           _mashService(mashService),
                           _boilService(boilService),
                           _coolingService(coolingService),
                           _brewSettingsService(brewSettingsService),
                           _mashKettleHeaterService(mashKettleHeaterService),
                           _spargeKettleHeaterService(spargeKettleHeaterService),
                           _boilKettleHeaterService(boilKettleHeaterService),
                           _coolingKettleHeaterService(coolingKettleHeaterService),
                           _activeStatus(activeStatus),
                           _temperatureService(temperatureService),
                           _pump(pump),
                           _lcd(lcd)

  {
  }

public:
  FS *_fs;
  AsyncWebServer *_server;
  BoilService *_boilService;
  MashService *_mashService;
  CoolingService *_coolingService;
  BrewSettingsService *_brewSettingsService;
  MashKettleHeaterService *_mashKettleHeaterService;
  SpargeKettleHeaterService *_spargeKettleHeaterService;
  BoilKettleHeaterService *_boilKettleHeaterService;
  CoolingKettleHeaterService *_coolingKettleHeaterService;
  TemperatureService *_temperatureService;
  Pump *_pump;
  ActiveStatus *_activeStatus;
  Lcd *_lcd;

  void start()
  {
    _activeStatus->TimeNow = now();
    _activeStatus->ActiveStep = mash;
    _activeStatus->BrewStarted = true;
    _activeStatus->ActiveMashStepIndex = 0;
    _activeStatus->ActiveBoilStepIndex = "";
    _activeStatus->BoilTime = _brewSettingsService->BoilTime * 60;
    _activeStatus->BoilTargetTemperature = _brewSettingsService->BoilTemperature;
    _activeStatus->SaveActiveStatus();
    _activeStatus->RestartLogTemperature();
    _mashKettleHeaterService->StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _spargeKettleHeaterService->StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    _boilKettleHeaterService->StartPID(100, 100, 100);
    _mashService->LoadMashSettings();
    _boilService->LoadBoilSettings();
    _coolingService->LoadCoolingSettings();
  }

  void pause()
  {
    _activeStatus->BrewStarted = false;
    _pump->TurnPumpOff();
    _activeStatus->SaveActiveStatus();
  }

  void resume()
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
    _coolingService->LoadCoolingSettings();
    if (_activeStatus->Recirculation || _activeStatus->StartTime <= 0)
    {
      if (_activeStatus->ActiveStep == mash)
        _pump->TurnPumpOn();
    }
    else
      _pump->TurnPumpOff();
  }

  void unLock()
  {
    _activeStatus->StepLock = false;
    _activeStatus->SaveActiveStatus();
  }

  void stop()
  {
    _activeStatus->SaveActiveStatus(0, 0, 0, 0, -1, "", 0, 0, none, false);
    _activeStatus->RestartLogTemperature();
    _pump->TurnPumpOff();
    _mashKettleHeaterService->Compute(_activeStatus->Temperature, _activeStatus->TargetTemperature, _brewSettingsService->MashHeaterPercentage);
    _spargeKettleHeaterService->Compute(_activeStatus->SpargeTemperature, _brewSettingsService->SpargeTemperature, _brewSettingsService->SpargePowerPercentage);
    _boilKettleHeaterService->Compute(_activeStatus->BoilTemperature, _brewSettingsService->BoilTemperature, _brewSettingsService->BoilPowerPercentage);
  }

  void nextStep()
  {
    _activeStatus->StepLock = false;
    _activeStatus->EndTime = now();
    _activeStatus->SaveActiveStatus();
  }

  void startBoilCounter()
  {
    _activeStatus->StartBoilCounter = true;
  }

  void startBoil()
  {
    _activeStatus->TimeNow = now();
    _activeStatus->ActiveStep = boil;
    _activeStatus->BrewStarted = true;
    _activeStatus->HeaterOn = true;
    _activeStatus->ActiveBoilStepIndex = "";
    _activeStatus->BoilTime = _brewSettingsService->BoilTime * 60;
    _activeStatus->BoilTargetTemperature = _brewSettingsService->BoilTemperature;
    _activeStatus->StartTime = 0;
    _activeStatus->EndTime = 0;
    _activeStatus->ActiveBoilStepName = "Heating to Boil";
    _activeStatus->SaveActiveStatus();
    _boilKettleHeaterService->StartPID(100, 100, 100);
    _boilService->LoadBoilSettings();
    _coolingService->LoadCoolingSettings();
    _pump->TurnPumpOff();
  }

  void startAnticavitation()
  {
    if (!_activeStatus->BrewStarted)
    {
      _activeStatus->LastActiveStep = _activeStatus->ActiveStep;
      _activeStatus->ActiveStep = anticavitation;
    }
  }

  void changeBoilPercentage(double percentage)
  {
    _brewSettingsService->BoilPowerPercentage = percentage;
    _activeStatus->SaveActiveStatus();
  }
};
#endif