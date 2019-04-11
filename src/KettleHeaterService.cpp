#include <KettleHeaterService.h>

boolean heatOff = true;

double KettleSetpoint, KettleInput, KettleOutput;
PID kettlePID = PID(&KettleInput, &KettleOutput, &KettleSetpoint, 1, 1, 1, DIRECT);
double _kp, _ki, _kd;

KettleHeaterService::KettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus) : _temperatureService(temperatureService),
                                                                                                               _activeStatus(activeStatus)
{
}

KettleHeaterService::~KettleHeaterService() {}

void KettleHeaterService::SetSampleTime(int sampleTime)
{
  kettlePID.SetSampleTime(sampleTime);
  kettlePID.SetOutputLimits(0, 1023);
}

void KettleHeaterService::SetTunings(double kp, double ki, double kd)
{
  // save tunings
  _kp = kp;
  _ki = ki;
  _kd = kd;
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
  heatOff = true;
  RestartPID();
  analogWrite(HEATER_BUS, 0);
}

void KettleHeaterService::EnablePID()
{
  kettlePID.SetMode(AUTOMATIC);
}

void KettleHeaterService::Compute()
{
  if (!_activeStatus->BrewStarted || _activeStatus->ActiveStep == none)
  {
    DisablePID();
    return;
  }

  if (heatOff)
  {
    Serial.print("Heat Off, turn on... ");
    EnablePID();
    heatOff = false;
  }

  if (_activeStatus->StartTime <= 0)
  {
    // restore tunings
    kettlePID.SetTunings(_kp, _ki, _kd);
    Serial.println("PID original");
  }

  if (_activeStatus->RestartPID)
  {
    RestartPID();
    heatOff = true;
    _activeStatus->RestartPID = false;
    // set agressives tunings
    kettlePID.SetTunings(100, 100, 100);
    Serial.println("PID Agressive");
  }

  KettleInput = _activeStatus->Temperature;
  KettleSetpoint = _activeStatus->TargetTemperature;

  kettlePID.Compute();
  _activeStatus->PWM = KettleOutput;
  Serial.print("PWM: ");
  Serial.println(_activeStatus->PWM);

  Serial.print("KPID: ");
  Serial.println(kettlePID.GetKp() + ' ' + kettlePID.GetKi() + ' ' + kettlePID.GetKd());

  if (_activeStatus->ActiveStep == boil)
  {
    analogWrite(HEATER_BUS, (1023 * _activeStatus->BoilPowerPercentage) / 100);
    return;
  }

  analogWrite(HEATER_BUS, KettleOutput);
}