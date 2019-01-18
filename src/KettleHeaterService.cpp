#include <KettleHeaterService.h>

#define HEATER_BUS 3

double KettleSetpoint, KettleInput, KettleOutput;
PID kettlePID = PID(&KettleInput, &KettleOutput, &KettleSetpoint, 2, 5, 1, DIRECT);

KettleHeaterService::KettleHeaterService(TemperatureService *temperatureService) : _temperatureService(temperatureService)
{
  KettleSetpoint = 0;
}

KettleHeaterService::~KettleHeaterService() {}

void KettleHeaterService::SetK(double kp, double ki, double kd)
{
  kettlePID.SetTunings(kp, ki, kd);
}

void KettleHeaterService::SetBoilPercent(double percent)
{
  _boilPercent = percent;
}

void KettleHeaterService::SetSetpoint(double setpoint)
{
  KettleSetpoint = setpoint;
}

void KettleHeaterService::Compute(boolean boil)
{
  KettleInput = _temperatureService->GetTemperature();
  kettlePID.Compute();

  if (KettleOutput < 0)
  {
    KettleOutput = 0;
  }
  if (KettleOutput > 1023)
  {
    KettleOutput = 1023;
  }

  if (boil)
  {
    KettleOutput = (KettleOutput * _boilPercent) / 100;
  }

  analogWrite(HEATER_BUS, KettleOutput);
}