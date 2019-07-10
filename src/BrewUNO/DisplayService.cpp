#include <BrewUNO/DisplayService.h>

byte apmode[] = {
    //icon for wifi ap
    B10101,
    B10101,
    B10101,
    B01110,
    B00100,
    B00100,
    B00100,
    B00100,
};

byte stmode[] = //icon for wifi station
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

byte gpump[] = //new icon for pumpgota
    {
        B00100,
        B00100,
        B01110,
        B01110,
        B11111,
        B11101,
        B11011,
        B01110,

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

byte gwm[] = // WM
    {
        B11111,
        B01000,
        B00100,
        B01000,
        B11111,
        B00000,
        B11111,
        B00110,
};

byte gpw[] = //PW
    {
        B00110,
        B11111,
        B00000,
        B11100,
        B10100,
        B10100,
        B11111,
        B00000,
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
  _lcd->createChar(1, apmode);
  _lcd->createChar(2, stmode);
  _lcd->createChar(3, gpump);
  _lcd->createChar(4, pheater);
  _lcd->createChar(5, sheater);
  _lcd->createChar(6, gcelsius);
  _lcd->createChar(7, gwm);
  _lcd->createChar(8, gpw);
}

time_t lastUpdate = now();

void DisplayService::loop()
{

  if (now() - lastUpdate > 1)
  {
    lastUpdate = now();

    //TITLE
    _lcd->home();
    _lcd->print("BrewUNO 0.6       ");

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
      _lcd->write(1);
    }

    //PRIMARY HEATER
    _lcd->setCursor(0, 1);
    _lcd->write(4);
    _lcd->setCursor(2, 1);
    _lcd->print(_activeStatus->Temperature);
    _lcd->setCursor(7, 1);
    _lcd->print(">");

    //SECONDARY HEATER
    _lcd->setCursor(0, 2);
    _lcd->write(5);
    _lcd->setCursor(2, 2);
    _lcd->print(_activeStatus->SpargeTemperature);
    _lcd->setCursor(7, 2);
    _lcd->print(">");

    //BOIL WHERE
    if (_activeStatus->BrewStarted)
    {
      //@START PROCESS

      //SETPOINT PRIMARY
      _lcd->setCursor(8, 1);
      if (_activeStatus->TargetTemperature > 99)
      {
        _lcd->print("1h");
      }
      else
      {
        _lcd->print(_activeStatus->TargetTemperature);
      }
      _lcd->setCursor(10, 1);
      _lcd->write(6);
      _lcd->setCursor(11, 1);
      _lcd->print("  ");

      //PWM PRIMARY
      _lcd->setCursor(13, 1);
      _lcd->write(7);
      _lcd->setCursor(14, 1);
      _lcd->print(" ");
      if (_activeStatus->PWMPercentage > 0)
      {
        if (_activeStatus->PWMPercentage > 99)
        {
          _lcd->setCursor(14, 1);
          _lcd->print("100");
        }
        else
        {
          _lcd->print(_activeStatus->PWMPercentage);
        }
      }
      else
      {
        _lcd->print("00");
      }
      _lcd->setCursor(17, 1);
      _lcd->print("%  ");

      //SETPOINT SECONDARY
      _lcd->setCursor(8, 2);
      if (_activeStatus->SpargeTargetTemperature > 99)
      {
        _lcd->print("1h");
      }
      else
      {
        _lcd->print(_activeStatus->SpargeTargetTemperature);
      }
      _lcd->setCursor(10, 2);
      _lcd->write(6);
      _lcd->setCursor(11, 2);
      _lcd->print("  ");

      //PWM SECONDARY
      _lcd->setCursor(13, 2);
      _lcd->write(8);
      _lcd->setCursor(14, 2);
      _lcd->print(" ");
      if (_activeStatus->SpargePWMPercentage > 0)
      {
        if (_activeStatus->SpargePWMPercentage > 99)
        {
          _lcd->setCursor(14, 2);
          _lcd->print("100");
        }
        else
        {
          _lcd->print(_activeStatus->SpargePWMPercentage);
        }
      }
      else
      {
        _lcd->print("00");
      }
      _lcd->setCursor(17, 2);
      _lcd->print("%  ");

      // ACTUAL STATUS INFO
      _lcd->setCursor(0, 3);
      _lcd->print("                    ");
      _lcd->setCursor(0, 3);
      String MessageStats, MessageStats2;
      int MessageStatsSz;
      MessageStats = _activeStatus->ActiveMashStepName;
      MessageStatsSz = (MessageStats.length() - 3);
      MessageStats = MessageStats.substring(0, MessageStatsSz);
      MessageStats2 = MessageStats;
      MessageStats2.replace(" @ ", "@");
      _lcd->print(MessageStats2.substring(0, 14));
      _lcd->setCursor(15, 3);
      _lcd->print("00:00");
    }
    else
    {
      //LINE 1 INFOS
      //SP CLEAR
      _lcd->setCursor(8, 1);
      _lcd->print("00"); //SP TEMP
      _lcd->write(6);    //°C ICON
      _lcd->setCursor(11, 1);
      _lcd->print("  ");
      //PWM CLEAR
      _lcd->setCursor(13, 1);
      _lcd->write(7); //PWM ICON
      _lcd->print(" 00");
      _lcd->setCursor(17, 1);
      _lcd->print("%  ");

      //LINE 2 INFOS
      //SP CLEAR
      _lcd->setCursor(8, 2);
      _lcd->print("00"); //SP TEMP
      _lcd->write(6);    //°C ICON
      _lcd->setCursor(11, 2);
      _lcd->print("  ");
      //PWM CLEAR
      _lcd->setCursor(13, 2);
      _lcd->write(8); //PWM ICON
      _lcd->print(" 00");
      _lcd->setCursor(17, 2);
      _lcd->print("%  ");
      _lcd->setCursor(0, 3);
      _lcd->print("                    ");
    }

    if (_activeStatus->ActiveStep == boil)
    {
      _lcd->setCursor(7, 1);
      _lcd->print(">");
      if (_activeStatus->BoilPowerPercentage > 99)
      {
        _lcd->print("1h");
      }
      else
      {
        _lcd->print(_activeStatus->BoilTargetTemperature);
      }
      _lcd->setCursor(0, 3);
      _lcd->print("To Boil Step   ");
      _lcd->setCursor(15, 3);
      _lcd->print("00:00");
    }

    if (_activeStatus->PumpOn == 0)
    {
      _lcd->setCursor(19, 1);
      _lcd->print("P");
    }
    else
    {
      _lcd->setCursor(19, 1);
      _lcd->write(3);
    }

    _lcd->setCursor(0, 1);
  }
}