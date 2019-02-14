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
#include <AsyncJson.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <enum.h>

#define ACTIVE_STATUS_FILE "/config/activeStatus.json"

class ActiveStatus
{
public:
  ActiveStatus(FS *f);
  ~ActiveStatus();

  int ActiveStep;
  boolean BrewStarted;

  String ActiveBoilStepIndex;
  int ActiveMashStepIndex;
  int BoilTime;
  float BoilTargetTemperature;

  float TargetTemperature;
  float Temperature;

  time_t EndTime;
  time_t StartTime;
  time_t TimeNow;

  String Temperatures;

  void LoadActiveStatusSettings();
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
  void LogTemperature(float temperature);
  JsonObject *GetJson();

private:
  FS *_fs;

  JsonObject *_activeStatus;
};
#endif