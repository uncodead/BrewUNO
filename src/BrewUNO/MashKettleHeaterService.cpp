#include <BrewUNO/MashKettleHeaterService.h>

double _mashKettleSetpoint, _mashKettleInput, _mashKettleOutput;
PID _mashKettlePID = PID(&_mashKettleInput, &_mashKettleOutput, &_mashKettleSetpoint, 1, 1, 1, P_ON_M, DIRECT);

MashKettleHeaterService::MashKettleHeaterService(TemperatureService *temperatureService,
                                                 ActiveStatus *activeStatus,
                                                 BrewSettingsService *brewSettingsService,
                                                 int heaterBus) : HeaterService(temperatureService,
                                                                                activeStatus, brewSettingsService, heaterBus)
{
}

double MashKettleHeaterService::GetPidOutput()
{
  return _mashKettleOutput;
}

double MashKettleHeaterService::GetPidInput()
{
  return _mashKettleInput;
}

double MashKettleHeaterService::GetPidSetPoint()
{
  return _mashKettleSetpoint;
}

void MashKettleHeaterService::PidCompute()
{
  _mashKettlePID.Compute();
}

PID &MashKettleHeaterService::GetPid()
{
  return _mashKettlePID;
}

void MashKettleHeaterService::StartPID(double kp, double ki, double kd)
{
  _mashKettlePID.SetOutputLimits(0.0, 1.0);  // Forces minimum up to 0.0
  _mashKettlePID.SetOutputLimits(-1.0, 0.0); // Forces maximum down to 0.0
  _mashKettlePID.SetTunings(kp, ki, kd, P_ON_M);
  _mashKettlePID.SetOutputLimits(0, 1023);
  _mashKettlePID.SetMode(AUTOMATIC);
}

void MashKettleHeaterService::SetPidParameters(double input, double setpoint)
{
  _mashKettleInput = input;
  _mashKettleSetpoint = setpoint;
}

boolean MashKettleHeaterService::StopCompute()
{
  return !_activeStatus->BrewStarted ||
         _activeStatus->ActiveStep == none ||
         _activeStatus->ActiveStep == anticavitation ||
         _activeStatus->PumpIsResting ||
         !_activeStatus->HeaterOn;
}