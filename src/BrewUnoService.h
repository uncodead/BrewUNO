#ifndef BrewUnoService_h
#define BrewUnoService_h

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
#include <CoolingService.h>
#include <MashKettleHeaterService.h>
#include <SpargeKettleHeaterService.h>
#include <BrewSettingsService.h>
#include <ActiveStatus.h>
#include <Pump.h>
#include <BoilKettleHeaterService.h>
#include <Lcd.h>
#include <BrewCommands.h>

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

class BrewUnoService : public BrewCommands
{
public:
  BrewUnoService(AsyncWebServer *server,
              FS *fs,
              MashService *mashService,
              BoilService *boilService,
              CoolingService *coolingService,
              BrewSettingsService *brewSettingsService,
              MashKettleHeaterService *kettleHeaterService,
              SpargeKettleHeaterService *spargeKettleHeaterService,
              BoilKettleHeaterService *boilKettleHeaterService,
              CoolingKettleHeaterService *coolingKettleHeaterService,
              ActiveStatus *activeStatus,
              TemperatureService *temperatureService,
              Pump *pump,
              Lcd * lcd);

  ~BrewUnoService();

  void loop();
  void begin();

private:
  AsyncJsonWebHandler _updateHandler;

  void onWSEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
  void getActiveStatus(AsyncWebServerRequest *request);
  void startBrewHttp(AsyncWebServerRequest *request);
  void stopBrewHttp(AsyncWebServerRequest *request);
  void nextStepHttp(AsyncWebServerRequest *request);
  void resumeBrewHttp(AsyncWebServerRequest *request);
  void unLockBrewHttp(AsyncWebServerRequest *request);
  void startBoilHttp(AsyncWebServerRequest *request);
  void startBoilCounterHttp(AsyncWebServerRequest *request);
  void pauseBrewHttp(AsyncWebServerRequest *request);
  void startAnticavitationHttp(AsyncWebServerRequest *request);
  void changeBoilPercentageHttp(AsyncWebServerRequest *request, JsonDocument &json);

  void HeaterCompute();
  void LoadSettingsToStatus();
};
#endif