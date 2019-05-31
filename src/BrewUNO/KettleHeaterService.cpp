#include <BrewUNO/KettleHeaterService.h>

double KettleSetpoint, KettleInput, KettleOutput;
PID _kettlePID = PID(&KettleInput, &KettleOutput, &KettleSetpoint, 1, 1, 1, P_ON_M, DIRECT);

PID_ATune aTune(&KettleInput, &KettleOutput);
double aTuneStep = 1023, aTuneNoise = 1, aTuneStartValue = 0;
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

void KettleHeaterService::StartPID(double kp, double ki, double kd)
{
  _kettlePID.SetOutputLimits(0.0, 1.0);  // Forces minimum up to 0.0
  _kettlePID.SetOutputLimits(-1.0, 0.0); // Forces maximum down to 0.0
  _kettlePID.SetTunings(kp, ki, kd, P_ON_M);
  _kettlePID.SetOutputLimits(0, 1023);
  _kettlePID.SetMode(AUTOMATIC);
}

void KettleHeaterService::StartAutoTune()
{
  KettleOutput = aTuneStartValue;
  aTune.SetNoiseBand(aTuneNoise);
  aTune.SetOutputStep(aTuneStep);
  aTune.SetLookbackSec((int)aTuneLookBack);
  aTune.SetControlType(1);
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

  if (!_activeStatus->PIDTuning && KettleSetpoint - KettleInput > _brewSettingsService->PIDStart)
  {
    _activeStatus->PWM = ((1023 * _brewSettingsService->MashHeaterPercentage) / 100);
    analogWrite(HEATER_BUS, _activeStatus->PWM);
    return;
  }

  if (KettleInput >= KettleSetpoint + 0.3) {
    _activeStatus->PWM = 0;
    analogWrite(HEATER_BUS, _activeStatus->PWM);
    StartPID(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD);
    return;
  }

  if (_activeStatus->PIDTuning)
  {
    Serial.println("tuning..");
    if (aTune.Runtime() != 0)
    {
      _activeStatus->PIDTuning = false;
      _activeStatus->BrewStarted = false;
      _activeStatus->StartTime = 0;
      _activeStatus->EndTime = 0;
      _brewSettingsService->KP = aTune.GetKp();
      _brewSettingsService->KI = aTune.GetKi();
      _brewSettingsService->KD = aTune.GetKd();
      _brewSettingsService->writeToFS();
      _kettlePID.SetMode(ATuneModeRemember);
      _kettlePID.SetTunings(_brewSettingsService->KP, _brewSettingsService->KI, _brewSettingsService->KD, P_ON_M);
    }
  }
  else
    _kettlePID.Compute();

  Serial.print("OUTPUT: ");
  Serial.println(KettleOutput);

  _activeStatus->PWM = KettleOutput;
  analogWrite(HEATER_BUS, KettleOutput);
}