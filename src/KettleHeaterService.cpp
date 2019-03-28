#include <KettleHeaterService.h>

double KettleSetpoint, KettleInput, KettleOutput;
PID kettlePID = PID(&KettleInput, &KettleOutput, &KettleSetpoint, 2, 5, 1, DIRECT);

KettleHeaterService::KettleHeaterService(TemperatureService *temperatureService) : _temperatureService(temperatureService)
{
}

KettleHeaterService::~KettleHeaterService() {}

void KettleHeaterService::SetSampleTime(int sampleTime)
{
  kettlePID.SetSampleTime(sampleTime);
}

void KettleHeaterService::SetTunings(double kp, double ki, double kd)
{
  kettlePID.SetTunings(kp, ki, kd);
}

void KettleHeaterService::RestartPID()
{
  kettlePID.SetMode(AUTOMATIC);

  kettlePID.SetOutputLimits(0.0, 1.0); 
  kettlePID.SetOutputLimits(-1.0, 0.0);
  kettlePID.SetOutputLimits(0, 1023);

  kettlePID.SetMode(MANUAL);
}

void KettleHeaterService::DisablePID()
{
  RestartPID();
  analogWrite(HEATER_BUS, 0);
}

void KettleHeaterService::EnablePID()
{
  kettlePID.SetMode(AUTOMATIC);
}

void KettleHeaterService::SetRampPowerPercentage(double percent)
{
  _rampPowerPercentage = percent;
}

void KettleHeaterService::Compute(ActiveStatus *activeStatus)
{
  if (!activeStatus->BrewStarted || activeStatus->ActiveStep == none)
  {
    DisablePID();
    return;
  }

  EnablePID();
  kettlePID.SetOutputLimits(0, 1023);
  KettleSetpoint = activeStatus->TargetTemperature;
  KettleInput = activeStatus->Temperature;

  activeStatus->LogTemperature(KettleInput, KettleSetpoint);

  kettlePID.Compute();

  if (!activeStatus->TotalHeaterPower && activeStatus->StartTime <= 0) {
    KettleOutput = (KettleOutput * _rampPowerPercentage) / 100;
  }

  activeStatus->PWM = KettleOutput;
  Serial.println(KettleOutput);

  analogWrite(HEATER_BUS, KettleOutput);
}