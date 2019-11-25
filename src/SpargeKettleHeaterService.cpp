#include <SpargeKettleHeaterService.h>

double _spargeKettleSetpoint, _spargeKettleInput, _spargeKettleOutput;
PID _spargeKettlePID = PID(&_spargeKettleInput, &_spargeKettleOutput, &_spargeKettleSetpoint, 1, 1, 1, P_ON_M, DIRECT);

SpargeKettleHeaterService::SpargeKettleHeaterService(TemperatureService *temperatureService,
                                                     ActiveStatus *activeStatus,
                                                     BrewSettingsService *brewSettingsService) : HeaterService(temperatureService,
                                                                                                               activeStatus,
                                                                                                               brewSettingsService)
{
}

double SpargeKettleHeaterService::GetPidOutput()
{
  return _spargeKettleOutput;
}

double SpargeKettleHeaterService::GetPidInput()
{
  return _spargeKettleInput;
}

double SpargeKettleHeaterService::GetPidSetPoint()
{
  return _spargeKettleSetpoint;
}

uint8_t SpargeKettleHeaterService::GetBus()
{
  return SPARGE_HEATER_BUS;
}

void SpargeKettleHeaterService::PidCompute()
{
  _spargeKettlePID.Compute();
}

void SpargeKettleHeaterService::StartPID(double kp, double ki, double kd)
{
  _spargeKettlePID.SetOutputLimits(0.0, 1.0);  // Forces minimum up to 0.0
  _spargeKettlePID.SetOutputLimits(-1.0, 0.0); // Forces maximum down to 0.0
  _spargeKettlePID.SetTunings(kp, ki, kd, P_ON_M);
  _spargeKettlePID.SetOutputLimits(0, 1023);
  _spargeKettlePID.SetMode(AUTOMATIC);
}

void SpargeKettleHeaterService::SetPidParameters(double input, double setpoint)
{
  _spargeKettleInput = input;
  _spargeKettleSetpoint = setpoint;
}

void SpargeKettleHeaterService::SetUP()
{
  _activeStatus->EnableSparge = _brewSettingsService->EnableSparge;
  _activeStatus->SpargeTargetTemperature = _brewSettingsService->SpargeTemperature;
}

bool SpargeKettleHeaterService::InvertedPWM()
{
  return false;
}

boolean SpargeKettleHeaterService::StopCompute()
{
  return !_activeStatus->BrewStarted || !_activeStatus->EnableSparge || _activeStatus->ActiveStep != mash || _activeStatus->PWM > 100;
}

void SpargeKettleHeaterService::TurnOff()
{
  analogWrite(GetBus(), 0);
}