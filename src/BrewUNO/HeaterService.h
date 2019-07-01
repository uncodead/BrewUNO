#ifndef HeaterService_h
#define HeaterService_h

#include <BrewUNO/TemperatureService.h>
#include <BrewUNO/ActiveStatus.h>
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include <BrewUNO/enum.h>
#include <BrewUNO/BrewSettingsService.h>

struct HeaterServiceStatus
{
  int PWM;
  boolean PIDActing;
};

class HeaterService
{
public:
  HeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService, PID *kettlePID, int heaterBus) : _temperatureService(temperatureService),
                                                                                                                                                               _activeStatus(activeStatus),
                                                                                                                                                               _brewSettingsService(brewSettingsService),
                                                                                                                                                               _kettlePID(kettlePID)

  {
    _heaterBus = heaterBus;
  }

  void StartPID(double kp, double ki, double kd)
  {
    _kettlePID->SetOutputLimits(0.0, 1.0);  // Forces minimum up to 0.0
    _kettlePID->SetOutputLimits(-1.0, 0.0); // Forces maximum down to 0.0
    _kettlePID->SetTunings(kp, ki, kd, P_ON_M);
    _kettlePID->SetOutputLimits(0, 1023);
    _kettlePID->SetMode(AUTOMATIC);
  }

  HeaterServiceStatus Compute(double input, double target, double heaterPercentage)
  {
    HeaterServiceStatus status;
    if (StopCompute())
    {
      status.PIDActing = false;
      status.PWM = 0;
      analogWrite(_heaterBus, 0);
      return status;
    }

    _kettleInput = input;
    _kettleSetpoint = target;

    if (_activeStatus->PIDSettingsUpdated)
    {
      _activeStatus->PIDSettingsUpdated = false;
      StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
      Serial.println("BrewSettings Updated: " + String(_brewSettingsService->KP) + "/" + String(_brewSettingsService->KI) + "/" + String(_brewSettingsService->KD));
      return status;
    }

    if (_activeStatus->ActiveStep == boil)
    {
      status.PIDActing = false;
      status.PWM = ((1023 * _activeStatus->BoilPowerPercentage) / 100);
      analogWrite(_heaterBus, status.PWM);
      return status;
    }

    if (_kettleSetpoint - _kettleInput > _brewSettingsService->PIDStart)
    {
      status.PIDActing = false;
      status.PWM = ((1023 * heaterPercentage) / 100);
      analogWrite(_heaterBus, status.PWM);
      return status;
    }

    // to prevent pid overshoot
    if (_kettleInput > _kettleSetpoint + 0.1)
    {
      status.PWM = 0;
      status.PIDActing = false;
      analogWrite(_heaterBus, _activeStatus->PWM);
      StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
      return status;
    }

    _kettlePID->Compute();

    int maxPWM = ((1023 * heaterPercentage) / 100);
    status.PWM = _kettleOutput > maxPWM ? maxPWM : _kettleOutput;

    analogWrite(_heaterBus, status.PWM);

    status.PIDActing = status.PWM > 0;
    return status;
  }

protected:
  virtual bool StopCompute();
  TemperatureService *_temperatureService;
  ActiveStatus *_activeStatus;
  BrewSettingsService *_brewSettingsService;
  PID *_kettlePID;
  int _heaterBus;
  double _kettleSetpoint, _kettleInput, _kettleOutput;
};
#endif