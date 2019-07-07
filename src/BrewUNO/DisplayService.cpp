#include <BrewUNO/DisplayService.h>

byte termometer[] = //icon for termometer
    {
        B00100,
        B01110,
        B01010,
        B01010,
        B01010,
        B11111,
        B11111,
        B01110,
};

byte gwifist[] = //Wifi Station
    {
        B01110,
        B10001,
        B00100,
        B01010,
        B00000,
        B00100,
        B00000,
        B00000,
};

byte apmode[] = {
    B10101,
    B10101,
    B10101,
    B01110,
    B00100,
    B00100,
    B00100,
    B00100,
};

byte spoint[] = //icon for setpoint
    {
        B11100,
        B10000,
        B11100,
        B00111,
        B11101,
        B00111,
        B00100,
        B00100,
};

byte pheater[] = //icon for primary heater
    {
        B11100,
        B10100,
        B11100,
        B10101,
        B10101,
        B00111,
        B00101,
        B00101,
};

byte sheater[] = //icon for secondary heater
    {
        B11100,
        B10000,
        B11100,
        B00101,
        B11101,
        B00111,
        B00101,
        B00101,
};

byte gcelsius[] = //graphic °C
    {
        B01000,
        B10100,
        B01000,
        B00110,
        B01001,
        B01000,
        B01001,
        B00110,
};

byte gpwm[] = //graphic pwm
    {
        B11111,
        B01100,
        B11111,
        B00110,
        B11111,
        B11000,
        B10100,
        B11111,
};

byte ppwm[] = //graphic pwm flame
    {
        B00100,
        B00110,
        B01101,
        B01001,
        B10010,
        B10110,
        B01100,
        B00100,
};

DisplayService::DisplayService(ActiveStatus *activeStatus, WiFiStatus *wifiStatus, LiquidCrystal_I2C *lcd) : _activeStatus(activeStatus),
                                                                                                             _wifiStatus(wifiStatus),
                                                                                                             _lcd(lcd)
{
}

DisplayService::~DisplayService() {}

void DisplayService::begin()
{
  _lcd->init();
  _lcd->backlight();
  _lcd->createChar(1, termometer);
  _lcd->createChar(2, gwifist);
  _lcd->createChar(9, apmode);
  _lcd->createChar(3, spoint);
  _lcd->createChar(4, pheater);
  _lcd->createChar(5, sheater);
  _lcd->createChar(6, gcelsius);
  _lcd->createChar(7, gpwm);
  _lcd->createChar(8, ppwm);
}

time_t lastUpdate = now();

void DisplayService::loop()
{

  if (now() - lastUpdate > 1)
  {
    lastUpdate = now();

    //TITLE
    _lcd->home();
    _lcd->print("   BrewUNO 0.0.6   ");

    //WIFI ST9 AP10
    wl_status_t status = WiFi.status();
    WiFiMode_t currentWiFiMode = WiFi.getMode();
    _lcd->setCursor(19, 0);
    if (status == WL_CONNECTED)
    {
      _lcd->write(2);
    }
    else if (currentWiFiMode == WIFI_AP || currentWiFiMode == WIFI_AP_STA)
    {
      _lcd->write(9);
    }

    //PRIMARY HEATER
    _lcd->setCursor(0, 1);
    _lcd->write(4);
    _lcd->setCursor(2, 1);
    _lcd->print(_activeStatus->Temperature);
    _lcd->setCursor(7, 1);
    _lcd->write(6);
    _lcd->print(" > ");

    //SETPOINT
    _lcd->setCursor(11, 1);
    _lcd->write(3);
    if (_activeStatus->TargetTemperature > 0)
    {
      _lcd->setCursor(13, 1);
      _lcd->print(_activeStatus->TargetTemperature);
      _lcd->write(6);
    }

    //SPARGE/SECONDARY HEATER
    _lcd->setCursor(0, 2);
    _lcd->write(5);
    if (_activeStatus->SpargeTemperature > 0)
    {
      _lcd->setCursor(2, 2);
      _lcd->print(_activeStatus->SpargeTemperature);
      _lcd->setCursor(7, 2);
      _lcd->write(6);
    }

    //PWM POWER
    _lcd->setCursor(13, 2);
    _lcd->print("    ");
    if (_activeStatus->PWM > 0)
    {
      _lcd->setCursor(11, 2);
      _lcd->write(7);
      _lcd->setCursor(13, 2);
      _lcd->print(_activeStatus->PWMPercentage);
      _lcd->setCursor(19, 2);
      _lcd->print("%");
    }
    else if (_activeStatus->SpargePWM > 0)
    {
      _lcd->setCursor(11, 2);
      _lcd->write(8);
      _lcd->setCursor(13, 2);
      _lcd->print(_activeStatus->SpargePWMPercentage);
      _lcd->setCursor(19, 2);
      _lcd->print("%");
    }

    _lcd->setCursor(0, 3);
    _lcd->print(_activeStatus->ActiveMashStepName.substring(0, 19));

    if (_activeStatus->PumpOn == 0)
    {
      _lcd->setCursor(8, 2);
      _lcd->print(" | ");
    }
    else
    {
      _lcd->setCursor(8, 2);
      _lcd->print(" p ");
    }

    if (!_activeStatus->BrewStarted)
    {
      //SP CLEAR
      _lcd->setCursor(13, 1);
      _lcd->print("00.00");

      //PWM CLEAR
      _lcd->setCursor(11, 2);
      _lcd->write(7);

      _lcd->setCursor(2, 2);
      _lcd->print("00.00");

      _lcd->setCursor(13, 2);
      _lcd->print("00.00");

      _lcd->setCursor(0, 3);
      _lcd->print("                    ");
    }
  }
}