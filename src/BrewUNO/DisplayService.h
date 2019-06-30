#ifndef DisplayService_h
#define DisplayService_h

#include <ArduinoJson.h>
#include <TimeLib.h>
#include <BrewUNO/enum.h>
#include <BrewUNO/ActiveStatus.h>
#include <LiquidCrystal_I2C.h>

class DisplayService
{
public:
  DisplayService(ActiveStatus *activeStatus, LiquidCrystal_I2C *lcd);

  ~DisplayService();

  void loop();
  void begin();

private:
  LiquidCrystal_I2C *_lcd;
  ActiveStatus *_activeStatus;
};
#endif