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
#include <enum.h>

#define PGM_P const char *
#define PGM_VOID_P const void *
#define PSTR(s) (__extension__({static const char __c[] PROGMEM = (s); &__c[0]; }))

#define MAX_ACTIVESTATUS_SIZE 2024
#define ACTIVE_STATUS_FILE "/config/activeStatus.json"
#define APPLICATION_JSON_TYPE "application/json"

class ActiveStatus
{
public:
  ActiveStatus(AsyncWebServer *server, FS *f);
  ~ActiveStatus();

  int ActiveStep;
  int LastActiveStep;
  boolean BrewStarted;

  String BrewfatherId;
  String BrewfatherKey;

  String ActiveBoilStepIndex;
  String ActiveBoilStepName;

  int ActiveMashStepIndex;
  String ActiveMashStepName;
  String ActiveMashStepSufixName;

  int ActiveCoolingStepIndex;
  String ActiveCoolingStepName;
  String ActiveCoolingStepSufixName;

  int BoilTime;
  double PWM;
  double PWMPercentage;
  double SpargePWM;
  double SpargePWMPercentage;
  double BoilPWM;
  double BoilPWMPercentage;
  double CoolingPWM;
  double CoolingPWMPercentage;
  boolean PIDActing;
  boolean StartBoilCounter;

  double BoilTargetTemperature;
  double BoilPowerPercentage;

  double CoolingTargetTemperature;

  double TargetTemperature;
  double Temperature;
  double BoilTemperature;
  double CoolingTemperature;
  double SpargeTemperature;
  double AuxOneTemperature;
  double AuxTwoTemperature;
  double AuxThreeTemperature;

  bool EnableSparge;
  bool EnableBoilKettle;
  double SpargeTargetTemperature;

  String MainSensor;
  String SpargeSensor;
  String BoilSensor;
  String AuxOneSensor;
  String AuxTwoSensor;
  String AuxThreeSensor;

  String Count;

  time_t EndTime;
  time_t StartTime;
  time_t TimeNow;

  boolean Recirculation;
  boolean PIDTuning;
  boolean PumpOn;
  boolean PumpIsResting;
  boolean PIDSettingsUpdated;
  boolean TimeNotSet;

  String TempUnit;

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
  void SetTemperature(Temperatures temps);
  String GetJson();
  void TimeNotSetted();
  void TimeSetted();
  void RestartLogTemperature();

private:
  FS *_fs;

  void onWSEventStatus(AsyncWebSocket *server,
                       AsyncWebSocketClient *client,
                       AwsEventType type,
                       void *arg,
                       uint8_t *data,
                       size_t len);
  void LogTemperature();
  void GetLogTemperature(AsyncWebServerRequest *request);

protected:
  AsyncWebSocket _webSocketStatus;
};
#endif