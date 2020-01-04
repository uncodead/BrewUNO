#ifndef Lcd_h
#define Lcd_h

#include <ArduinoJson.h>
#include <TimeLib.h>
#include <enum.h>
#include <ActiveStatus.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiStatus.h>
#include <APStatus.h>

#define PCF8574_ADDRESS 0x20

#define apmode_icon 1
#define stmode_icon 2
#define gpump_icon 3
#define pheater_icon 4
#define sheater_icon 5
#define gcelsius_icon 6
#define gwm_icon 7
#define gpw_icon 8

struct LineBody
{
    int Line;
    byte HeatIcon;
    byte PwmIcon;
    double Temperatura;
    double TargetTemperature;
    double Pwm;
    bool ShowPump;
    bool Sparge;
};

class Lcd
{
public:
    Lcd(ActiveStatus *activeStatus, WiFiStatus *wifiStatus, LiquidCrystal_I2C *lcd);
    ~Lcd();
    void update();
    void begin();

private:
    LiquidCrystal_I2C *_lcd;
    WiFiStatus *_wifiStatus;
    ActiveStatus *_activeStatus;

    struct BodyLine
    {
        int line;
        byte heatIcon;
        byte pwmIcon;
        double temperature;
        double targetTemperature;
        double pwm;
        bool showPump;
        bool sparge;
    };

    void autoScan();
    void printHead();
    void printFooter();
    String GetCount(bool down);
    void RemoveLastChars(int length);
    String getTemperature(double targetTemperature, bool target);
    void printBody(BodyLine line);
    void printMashBody();
    void printBoilBody();
    void printSpargeBody();
    void printPWM(BodyLine line);
    void printPump(BodyLine line);
    void printIpFooter();
    void printMashFooter();
    void printBoilFooter();
};
#endif