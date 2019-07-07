#ifndef DisplayService_h
#define DisplayService_h

#include <ArduinoJson.h>
#include <TimeLib.h>
#include <BrewUNO/enum.h>
#include <BrewUNO/ActiveStatus.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiStatus.h>
#include <APStatus.h>

class DisplayService
{
public:
  DisplayService(ActiveStatus *activeStatus, WiFiStatus *wifiStatus, LiquidCrystal_I2C *lcd);

  ~DisplayService();

  void loop();
  void begin();

private:
  LiquidCrystal_I2C *_lcd;
  WiFiStatus *_wifiStatus;
  ActiveStatus *_activeStatus;
};
#endif