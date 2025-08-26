#include <CoolingKettleHeaterService.h>

double _coolingKettleSetpoint, _coolingKettleInput, _coolingKettleOutput;
PID _coolingKettlePID = PID(&_coolingKettleInput, &_coolingKettleOutput, &_coolingKettleSetpoint, 1, 1, 1, P_ON_M, DIRECT);

CoolingKettleHeaterService::CoolingKettleHeaterService(TemperatureService *temperatureService,
                                                       ActiveStatus *activeStatus,
                                                       BrewSettingsService *brewSettingsService) : HeaterService(temperatureService,
                                                                                                                 activeStatus,
                                                                                                                 brewSettingsService)
{
}

double CoolingKettleHeaterService::GetPidOutput()
{
  return _coolingKettleOutput;
}

double CoolingKettleHeaterService::GetPidInput()
{
  return _coolingKettleInput;
}

double CoolingKettleHeaterService::GetPidSetPoint()
{
  return _coolingKettleSetpoint;
}

uint8_t CoolingKettleHeaterService::GetBus()
{
  if (_brewSettingsService->EnableBoilKettle)
    return BOIL_HEATER_BUS;
  return HEATER_BUS;
}

void CoolingKettleHeaterService::PidCompute()
{
  _coolingKettlePID.Compute();
}

PID &CoolingKettleHeaterService::GetPid()
{
  return _coolingKettlePID;
}

void CoolingKettleHeaterService::StartPID(double kp, double ki, double kd)
{
  _coolingKettlePID.SetOutputLimits(0.0, 1.0);  // Forces minimum up to 0.0
  _coolingKettlePID.SetOutputLimits(-1.0, 0.0); // Forces maximum down to 0.0
  _coolingKettlePID.SetTunings(kp, ki, kd, P_ON_M);
  _coolingKettlePID.SetOutputLimits(0, 1023);
  _coolingKettlePID.SetMode(AUTOMATIC);
}

void CoolingKettleHeaterService::SetPidParameters(double input, double setpoint)
{
  _coolingKettleInput = input;
  _coolingKettleSetpoint = setpoint;
}

void CoolingKettleHeaterService::SetUP()
{
  _activeStatus->EnableBoilKettle = _brewSettingsService->EnableBoilKettle;
}

bool CoolingKettleHeaterService::InvertedPWM()
{
  return _brewSettingsService->EnableBoilKettle;
}

boolean CoolingKettleHeaterService::StopCompute()
{
  return !_activeStatus->BrewStarted ||
         _activeStatus->ActiveStep != cooling ||
         !_activeStatus->HeaterOn;
}

void CoolingKettleHeaterService::TurnOff()
{
  if (!_activeStatus->BrewStarted ||
      (_activeStatus->ActiveStep == cooling && !_activeStatus->HeaterOn))
    analogWrite(GetBus(), 0);
}