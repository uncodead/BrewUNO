#ifndef HeaterService_h
#define HeaterService_h

#include <TemperatureService.h>
#include <ActiveStatus.h>
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include <enum.h>
#include <BrewSettingsService.h>

struct HeaterServiceStatus
{
  double PWM;
  double PWMPercentage;
  boolean PIDActing;
};

class HeaterService
{
public:
  HeaterService(TemperatureService *temperatureService,
                ActiveStatus *activeStatus,
                BrewSettingsService *brewSettingsService) : _temperatureService(temperatureService),
                                                            _activeStatus(activeStatus),
                                                            _brewSettingsService(brewSettingsService)

  {
  }


  void WriteTimeProportionalPWM(uint8_t pin, int pwm)
  {
    unsigned long now = millis();

    if (now - _windowStartTime >= _windowSize)
    {
      _windowStartTime += _windowSize;
    }

    unsigned long onTime = (pwm * _windowSize) / 1023;

    if ((now - _windowStartTime) < onTime)
      digitalWrite(pin, InvertedPWM() ? LOW : HIGH);
    else
      digitalWrite(pin, InvertedPWM() ? HIGH : LOW);
  }

HeaterServiceStatus Compute(double input, double target, double heaterPercentage)
{
  HeaterServiceStatus status = {0, 0, false};

  uint8_t heaterBus = GetBus();

  SetUP();

  if (StopCompute())
  {
    TurnOff();
    return status;
  }

  SetPidParameters(input, target);

  if (_activeStatus->PIDSettingsUpdated)
  {
    _activeStatus->PIDSettingsUpdated = false;

    StartPID(
      _brewSettingsService->KP,
      _brewSettingsService->KI,
      _brewSettingsService->KD
    );

    Serial.println(
      "BrewSettings Updated: " +
      String(_brewSettingsService->KP) + "/" +
      String(_brewSettingsService->KI) + "/" +
      String(_brewSettingsService->KD)
    );

    return status;
  }

  if (_activeStatus->ActiveStep == boil)
  {
    status.PWM = (1023 * _brewSettingsService->BoilPowerPercentage) / 100;
    status.PWMPercentage = (status.PWM * 100) / 1023;

    WriteTimeProportionalPWM(heaterBus, status.PWM);

    return status;
  }

  if (_activeStatus->FullPower)
    heaterPercentage = 100;

  if (GetPidSetPoint() - GetPidInput() > _brewSettingsService->PIDStart)
  {
    status.PWM = (1023 * heaterPercentage) / 100;
    status.PWMPercentage = (status.PWM * 100) / 1023;
    status.PIDActing = false;

    WriteTimeProportionalPWM(heaterBus, status.PWM);

    return status;
  }

  // proteção contra overshoot
  if (GetPidInput() > GetPidSetPoint() + 0.1)
  {
    status.PWM = 0;
    status.PWMPercentage = 0;
    status.PIDActing = false;

    WriteTimeProportionalPWM(heaterBus, 0);

    StartPID(
      _brewSettingsService->KP,
      _brewSettingsService->KI,
      _brewSettingsService->KD
    );

    return status;
  }

  // cálculo PID
  PidCompute();

  int maxPWM = (1023 * heaterPercentage) / 100;

  status.PWM = GetPidOutput() > maxPWM ? maxPWM : GetPidOutput();
  status.PWMPercentage = (status.PWM * 100) / 1023;

  WriteTimeProportionalPWM(heaterBus, status.PWM);

  status.PIDActing = status.PWM > 0;

  return status;
}

protected:
  virtual void SetUP();
  virtual boolean StopCompute();
  virtual void StartPID(double kp, double ki, double kd);
  virtual void PidCompute();
  virtual double GetPidOutput();
  virtual double GetPidInput();
  virtual double GetPidSetPoint();
  virtual uint8_t GetBus();
  virtual void TurnOff();
  virtual bool InvertedPWM();
  virtual void SetPidParameters(double input, double setpoint);

  TemperatureService *_temperatureService;
  ActiveStatus *_activeStatus;
  BrewSettingsService *_brewSettingsService;
  PID *_kettlePID;
  unsigned long _windowStartTime = 0;
  const unsigned long _windowSize = 10000; // 10 segundos

};
#endif
