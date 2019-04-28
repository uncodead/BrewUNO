#include <BrewUNO/KettleHeaterService.h>

int WindowSize = 5000;
unsigned long windowStartTime;
double KettleSetpoint, KettleInput, KettleOutput;
PID _kettlePID = PID(&KettleInput, &KettleOutput, &KettleSetpoint, 1, 1, 1, DIRECT);

PID_ATune aTune(&KettleInput, &KettleOutput);
double aTuneStep = 5000, aTuneNoise = 1, aTuneStartValue = 0;
unsigned int aTuneLookBack = 20;
byte ATuneModeRemember = 2;
double kp = 2, ki = 0.5, kd = 2;
boolean tuning = false;

KettleHeaterService::KettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService) : _temperatureService(temperatureService),
                                                                                                                                                         _activeStatus(activeStatus),
                                                                                                                                                         _brewSettingsService(brewSettingsService)
{
}

KettleHeaterService::~KettleHeaterService() {}

void KettleHeaterService::SetTunings(double kp, double ki, double kd)
{
  _kettlePID.SetTunings(kp, ki, kd);
  _kettlePID.SetOutputLimits(0, WindowSize);
  windowStartTime = millis();
  _kettlePID.SetMode(AUTOMATIC);
}

void KettleHeaterService::StartAutoTune()
{
  KettleOutput = aTuneStartValue;
  aTune.SetNoiseBand(aTuneNoise);
  aTune.SetOutputStep(aTuneStep);
  aTune.SetLookbackSec((int)aTuneLookBack);
  ATuneModeRemember = _kettlePID.GetMode();
}

void KettleHeaterService::Compute()
{
  if (!_activeStatus->BrewStarted || _activeStatus->ActiveStep == none)
  {
    analogWrite(HEATER_BUS, 0);
    return;
  }

  KettleInput = _activeStatus->Temperature;
  KettleSetpoint = _activeStatus->TargetTemperature;

  if (_activeStatus->ActiveStep == boil)
  {
    _activeStatus->PWM = ((1023 * _activeStatus->BoilPowerPercentage) / 100);
    analogWrite(HEATER_BUS, _activeStatus->PWM);
    return;
  }

  if (_activeStatus->PIDTuning)
  {
    Serial.println("tuning..");
    if (aTune.Runtime() != 0)
    {
      _activeStatus->PIDTuning = false;
      _activeStatus->BrewStarted = false;
      _brewSettingsService->KP = aTune.GetKp();
      _brewSettingsService->KI = aTune.GetKi();
      _brewSettingsService->KD = aTune.GetKd();
      _brewSettingsService->writeToFS();
      _kettlePID.SetMode(ATuneModeRemember);
      _kettlePID.SetTunings(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    }
  }
  else
    _kettlePID.Compute();

  Serial.print("OUTPUT: ");
  Serial.println(KettleOutput);

  unsigned long now = millis();
  if (now - windowStartTime > WindowSize)
  {
    windowStartTime += WindowSize;
    Serial.println("time to shift the Relay Window");
  }

  if (KettleOutput > now - windowStartTime)
  {
    digitalWrite(HEATER_BUS, HIGH);
    _activeStatus->PWM = 1023;
    Serial.println("on");
  }
  else
  {
    digitalWrite(HEATER_BUS, LOW);
    _activeStatus->PWM = 0;
    Serial.println("off");
  }
}