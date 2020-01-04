#include <BoilKettleHeaterService.h>

double _boilKettleSetpoint, _boilKettleInput, _boilKettleOutput;
PID _boilKettlePID = PID(&_boilKettleInput, &_boilKettleOutput, &_boilKettleSetpoint, 1, 1, 1, P_ON_M, DIRECT);

BoilKettleHeaterService::BoilKettleHeaterService(TemperatureService *temperatureService,
                                                 ActiveStatus *activeStatus,
                                                 BrewSettingsService *brewSettingsService) : HeaterService(temperatureService,
                                                                                                           activeStatus,
                                                                                                           brewSettingsService)
{
}

double BoilKettleHeaterService::GetPidOutput()
{
  return _boilKettleOutput;
}

double BoilKettleHeaterService::GetPidInput()
{
  return _boilKettleInput;
}

double BoilKettleHeaterService::GetPidSetPoint()
{
  return _boilKettleSetpoint;
}

uint8_t BoilKettleHeaterService::GetBus()
{
  if (_brewSettingsService->EnableBoilKettle)
    return BOIL_HEATER_BUS;
  return HEATER_BUS;
}

void BoilKettleHeaterService::PidCompute()
{
  _boilKettlePID.Compute();
}

PID &BoilKettleHeaterService::GetPid()
{
  return _boilKettlePID;
}

void BoilKettleHeaterService::StartPID(double kp, double ki, double kd)
{
  _boilKettlePID.SetOutputLimits(0.0, 1.0);  // Forces minimum up to 0.0
  _boilKettlePID.SetOutputLimits(-1.0, 0.0); // Forces maximum down to 0.0
  _boilKettlePID.SetTunings(kp, ki, kd, P_ON_M);
  _boilKettlePID.SetOutputLimits(0, 1023);
  _boilKettlePID.SetMode(AUTOMATIC);
}

void BoilKettleHeaterService::SetPidParameters(double input, double setpoint)
{
  _boilKettleInput = input;
  _boilKettleSetpoint = setpoint;
}

void BoilKettleHeaterService::SetUP()
{
  _activeStatus->EnableBoilKettle = _brewSettingsService->EnableBoilKettle;
  _activeStatus->BoilTargetTemperature = _brewSettingsService->BoilTemperature;
  if (!_activeStatus->EnableBoilKettle)
    _activeStatus->BoilTemperature = _activeStatus->Temperature;
}

bool BoilKettleHeaterService::InvertedPWM()
{
  return _brewSettingsService->EnableBoilKettle;
}

boolean BoilKettleHeaterService::StopCompute()
{
  return !_activeStatus->BrewStarted || _activeStatus->ActiveStep != boil;
}

void BoilKettleHeaterService::TurnOff()
{
  if (!_activeStatus->BrewStarted || _activeStatus->EnableBoilKettle)
  {
    analogWrite(BOIL_HEATER_BUS, 1023);
    digitalWrite(BOIL_HEATER_BUS, HIGH);
  }
}