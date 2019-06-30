#include <BrewUNO/DisplayService.h>

DisplayService::DisplayService(ActiveStatus *activeStatus, LiquidCrystal_I2C *lcd) : _activeStatus(activeStatus), _lcd(lcd)
{
}

DisplayService::~DisplayService() {}

void DisplayService::begin()
{
  _lcd->init();
  _lcd->backlight();
}

time_t lastUpdate = now();
void DisplayService::loop()
{
  if (now() - lastUpdate > 1)
  {
    lastUpdate = now();

    _lcd->home();
    _lcd->print("BrewUNO");
    _lcd->setCursor(0, 1);
    _lcd->print("Temperature: ");
    _lcd->setCursor(12, 1);
    _lcd->print(_activeStatus->Temperature);
  }
}