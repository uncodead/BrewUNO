#ifndef BrewService_h
#define BrewService_h

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <IPAddress.h>
#include <AsyncJsonWebHandler.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <enum.h>
#include <MashService.h>
#include <BoilService.h>
#include <MashKettleHeaterService.h>
#include <SpargeKettleHeaterService.h>
#include <BrewSettingsService.h>
#include <ActiveStatus.h>
#include <Pump.h>
#include <BoilKettleHeaterService.h>
#include <Lcd.h>

#define START_BREW_SERVICE_PATH "/rest/startbrew"
#define STOP_BREW_SERVICE_PATH "/rest/stopbrew"
#define NEXT_STEP_SERVICE_PATH "/rest/nextstepbrew"
#define PAUSE_BREW_SERVICE_PATH "/rest/pausebrew"
#define RESUME_BREW_SERVICE_PATH "/rest/resumebrew"
#define UNLOCK_BREW_SERVICE_PATH "/rest/unlockbrew"
#define GET_ACTIVE_STATUS_SERVICE_PATH "/rest/getactivestatus"
#define CHANGE_BOIL_PERCENTAGE_SERVICE_PATH "/rest/changeboilpercentage"
#define START_BOIL_SERVICE_PATH "/rest/startboil"
#define START_BOIL_COUNTER_SERVICE_PATH "/rest/startboilcounter"
#define START_TUNING_SERVICE_PATH "/rest/starttuning"
#define START_ANTICAVITATION_SERVICE_PATH "/rest/startanticavitation"

#define APPLICATION_JSON_TYPE "application/json"
#define NPT_JSON_ERROR_MESSAGE "{ \"error\": true, \"message\":\"NTP server not reachable\"}"
#define INVALID_JSON_ERROR "Error validating json."

class BrewService
{
public:
  BrewService(AsyncWebServer *server,
              FS *fs,
              MashService *mashService,
              BoilService *boilService,
              BrewSettingsService *brewSettingsService,
              MashKettleHeaterService *kettleHeaterService,
              SpargeKettleHeaterService *spargeKettleHeaterService,
              BoilKettleHeaterService *boilKettleHeaterService,
              ActiveStatus *activeStatus,
              TemperatureService *temperatureService,
              Pump *pump,
              Lcd * lcd);

  ~BrewService();

  void loop();
  void begin();

  void startBrew();
  void stopBrew();
  void nextStep();
  void resumeBrew();
  void unLockBrew();
  void startBoil();
  void startBoilCounter();
  void pauseBrew();

private:
  FS *_fs;
  AsyncWebServer *_server;
  BoilService *_boilService;
  MashService *_mashService;
  BrewSettingsService *_brewSettingsService;
  MashKettleHeaterService *_mashKettleHeaterService;
  SpargeKettleHeaterService *_spargeKettleHeaterService;
  BoilKettleHeaterService *_boilKettleHeaterService;
  TemperatureService *_temperatureService;
  Pump *_pump;
  ActiveStatus *_activeStatus;
  Lcd *_lcd;

  AsyncJsonWebHandler _updateHandler;

  void getActiveStatus(AsyncWebServerRequest *request);
  void startBrewHttp(AsyncWebServerRequest *request);
  void stopBrewHttp(AsyncWebServerRequest *request);
  void nextStepHttp(AsyncWebServerRequest *request);
  void resumeBrewHttp(AsyncWebServerRequest *request);
  void unLockBrewHttp(AsyncWebServerRequest *request);
  void startBoilHttp(AsyncWebServerRequest *request);
  void startBoilCounterHttp(AsyncWebServerRequest *request);
  void pauseBrewHttp(AsyncWebServerRequest *request);
  void startAnticavitation(AsyncWebServerRequest *request);
  void changeBoilPercentage(AsyncWebServerRequest *request, JsonDocument &json);

  void HeaterCompute();
};
#endif