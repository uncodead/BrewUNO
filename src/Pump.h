#ifndef Pump_h
#define Pump_h

#include <Arduino.h>
#include <TimeLib.h>
#include <BrewSettingsService.h>
#include <ActiveStatus.h>
#include <Buzzer.h>

#define APPLICATION_JSON_TYPE "application/json"
#define START_PUMP_SERVIVE_PATH "/rest/startpump"
#define STOP_PUMP_SERVIVE_PATH "/rest/stoppump"

class Pump
{
public:
  Pump(AsyncWebServer *server, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService);

  AsyncWebServer *_server;
  BrewSettingsService *_brewSettingsService;
  ActiveStatus *_activeStatus;

  void TurnPumpOn();
  void TurnPumpOff();
  void CheckRest();
  void antiCavitation();
  void startPumpHttpService(AsyncWebServerRequest *request);
  void stopPumpHttpService(AsyncWebServerRequest *request);
  
  void TurnPump(bool on);
};

#endif
