#ifndef ActiveStatus_h
#define ActiveStatus_h

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncArduinoJson6.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <BrewUNO/enum.h>

#define MAX_ACTIVESTATUS_SIZE 2024
#define ACTIVE_STATUS_FILE "/config/activeStatus.json"

class ActiveStatus
{
public:
  ActiveStatus(FS *f);
  ~ActiveStatus();

  int ActiveStep;
  int LastActiveStep;
  boolean BrewStarted;

  String ActiveBoilStepIndex;
  String ActiveBoilStepName;

  int ActiveMashStepIndex;
  String ActiveMashStepName;
  String ActiveMashStepSufixName;
  
  int BoilTime;
  double PWM;
  double PWMPercentage;
  double SpargePWM;
  double SpargePWMPercentage;
  boolean PIDActing;

  double BoilTargetTemperature;
  double BoilPowerPercentage;

  double TargetTemperature;
  double Temperature;
  double SpargeTemperature;
  String Temperatures;

  bool EnableSparge;
  double SpargeTargetTemperature;

  String MainSensor;
  String SpargeSensor;

  time_t EndTime;
  time_t StartTime;
  time_t TimeNow;

  boolean Recirculation;
  boolean PIDTuning;
  boolean PumpOn;
  boolean PumpIsResting;
  boolean PIDSettingsUpdated;

  boolean HeaterOn;
  boolean FullPower;
  boolean StepLock;
  boolean StepLocked;

  boolean LoadActiveStatusSettings();
  void SaveActiveStatus(time_t StartTime,
                        time_t EndTime,
                        time_t TimeNow,
                        float TargetTemperature,
                        int ActiveMashStepIndex,
                        String ActiveBoilStepIndex,
                        int boilTime,
                        float boilTargetTemperature,
                        int ActiveStep,
                        boolean brewStarted);
  void SaveActiveStatus();
  void SaveActiveStatusLoop();
  void SetTemperature(float);
  void SetSpargeTemperature(float);
  void SetJsonTemperatures(String json);
  String GetJson();

private:
  FS *_fs;
};
#endif