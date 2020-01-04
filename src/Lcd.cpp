#include <Lcd.h>

String blankline = "\x20"
                   " \040 \x20"
                   " \040 \x20"
                   " \040 \x20"
                   " \040 \x20"
                   " \040 ";
byte apmode[] = {B01010, B00100, B01010, B00100, B00100, B00100, B01110, B11111};
byte stmode[] = {B01110, B10001, B00100, B01010, B00000, B00100, B00000, B00000};
byte gpump[] = {B00100, B00100, B01110, B01110, B11111, B11101, B11011, B01110};
byte pheater[] = {B10100, B11100, B10100, B00010, B00110, B00010, B00010, B00111};
byte sheater[] = {B10100, B11100, B10100, B00111, B00001, B00111, B00100, B00111};
byte gcelsius[] = {B01000, B10100, B01000, B00110, B01001, B01000, B01001, B00110};
byte gwm[] = {B11111, B01000, B00100, B01000, B11111, B00000, B11111, B00110};
byte gpw[] = {B00110, B11111, B00000, B11100, B10100, B10100, B11111, B00000};

Lcd::Lcd(ActiveStatus *activeStatus, WiFiStatus *wifiStatus, LiquidCrystal_I2C *lcd) : _activeStatus(activeStatus),
                                                                                       _wifiStatus(wifiStatus),
                                                                                       _lcd(lcd)
{
    Wire.begin();
}

Lcd::~Lcd() {}

void Lcd::autoScan()
{
    byte error, address;
    Serial.println("Scanning I2C bus...");
    for (address = 1; address < 127; address++)
    {
        if (address == PCF8574_ADDRESS)
        {
            Serial.print("Found not display addr: ");
            Serial.println(address, HEX);
            continue;
        }
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0)
        {
            Serial.print("Found: ");
            Serial.println(address, HEX);
            _lcd->updateAddress(address);
            break;
        }
    }
}

void Lcd::begin()
{
    autoScan();
    _lcd->init();
    _lcd->backlight();
    _lcd->createChar(apmode_icon, apmode);
    _lcd->createChar(stmode_icon, stmode);
    _lcd->createChar(gpump_icon, gpump);
    _lcd->createChar(pheater_icon, pheater);
    _lcd->createChar(sheater_icon, sheater);
    _lcd->createChar(gcelsius_icon, gcelsius);
    _lcd->createChar(gwm_icon, gwm);
    _lcd->createChar(gpw_icon, gpw);
}

time_t lastUpdate = now();
void Lcd::update()
{
    if (now() - lastUpdate > 1)
    {
        lastUpdate = now();
        printHead();
        printMashBody();
        printBoilBody();
        printSpargeBody();
        printFooter();
    }
}

void Lcd::printMashBody()
{
    if (_activeStatus->ActiveStep == mash || _activeStatus->ActiveStep == none)
    {
        BodyLine line;
        line.line = 1;
        line.heatIcon = pheater_icon;
        line.pwmIcon = gwm_icon;
        line.temperature = _activeStatus->Temperature;
        line.targetTemperature = _activeStatus->TargetTemperature;
        line.pwm = _activeStatus->PWMPercentage;
        line.showPump = true;
        line.sparge = false;
        printBody(line);
    }
}

void Lcd::printBoilBody()
{
    if (_activeStatus->ActiveStep == boil)
    {
        BodyLine line;
        line.line = 1;
        line.heatIcon = pheater_icon;
        line.pwmIcon = gwm_icon;
        line.temperature = _activeStatus->BoilTemperature;
        line.targetTemperature = _activeStatus->BoilTargetTemperature;
        line.pwm = _activeStatus->BoilPWMPercentage;
        line.showPump = true;
        line.sparge = false;
        printBody(line);
    }
}

void Lcd::printSpargeBody()
{
    BodyLine line;
    line.line = 2;
    line.heatIcon = sheater_icon;
    line.pwmIcon = gpw_icon;
    line.temperature = _activeStatus->SpargeTemperature;
    line.targetTemperature = _activeStatus->SpargeTargetTemperature;
    line.pwm = _activeStatus->SpargePWMPercentage;
    line.showPump = false;
    line.sparge = true;
    printBody(line);
}

void Lcd::printHead()
{
    _lcd->setCursor(0, 0);
    wl_status_t status = WiFi.status();
    WiFiMode_t currentWiFiMode = WiFi.getMode();
    if (status == WL_CONNECTED)
        _lcd->write(stmode_icon);
    else if (currentWiFiMode == WIFI_AP || currentWiFiMode == WIFI_AP_STA)
        _lcd->write(apmode_icon);
    if (_activeStatus->BrewStarted && !_activeStatus->StepLocked)
    {
        _activeStatus->Count = GetCount(true);
        _lcd->print("\x20"
                    "B\162e\x77"
                    "U\116O\x20"
                    " \040" +
                    _activeStatus->Count);
    }
    else if (_activeStatus->StepLocked)
    {
        _activeStatus->Count = GetCount(false);
        _lcd->print("\x20"
                    "B\162e\x77"
                    "U\116O\x20"
                    " " +
                    _activeStatus->Count + "\x4C"
                                           "");
    }
    else
        _lcd->print("\x20"
                    "B\162e\x77"
                    "U\116O\x20"
                    " \166" +
                    String(Version) + "\x20"
                                      " ");
}

void Lcd::printBody(BodyLine line)
{
    _lcd->setCursor(0, line.line);
    _lcd->write(line.heatIcon);
    if (line.sparge && !_activeStatus->EnableSparge)
        _lcd->print("\x20"
                    "0\060.\x30"
                    "0\0760\x30"
                    "");
    else
        _lcd->print("\x20"
                    "" +
                    getTemperature(line.temperature, false) +
                    "\x3E"
                    "" +
                    (_activeStatus->BrewStarted ? getTemperature(line.targetTemperature, true) : "\x30"
                                                                                                 "0"));

    _lcd->setCursor(10, line.line);
    if (_activeStatus->TempUnit == "\x43"
                                   "")
        _lcd->write(gcelsius_icon);
    else
        _lcd->print("\x46"
                    "");
    printPWM(line);
    printPump(line);
}

void Lcd::printPump(BodyLine line)
{
    _lcd->setCursor(19, 1);
    if (line.showPump && _activeStatus->PumpOn)
        _lcd->write(gpump_icon);
    else if (line.showPump)
        _lcd->print("\x50"
                    "");
}

void Lcd::printPWM(BodyLine line)
{
    _lcd->setCursor(13, line.line);
    _lcd->write(line.pwmIcon);
    _lcd->setCursor(14, line.line);
    if (line.pwm <= 0)
        _lcd->print("\x20"
                    " \060%");
    else if (line.pwm < 10)
        _lcd->print("\x20"
                    " " +
                    String(line.pwm).substring(0, 1) +
                    "\x25"
                    "");
    else if (line.pwm <= 99)
        _lcd->print("\x20"
                    "" +
                    String(line.pwm).substring(0, 2) +
                    "\x25"
                    "");
    else
        _lcd->print("\x31"
                    "0\060%");
}

void Lcd::printFooter()
{
    _lcd->setCursor(0, 3);
    
    if (!_activeStatus->BrewStarted)
        printIpFooter();
    else if (_activeStatus->ActiveStep == mash)
        printMashFooter();
    else if (_activeStatus->ActiveStep == boil && _activeStatus->ActiveBoilStepName != "")
        printBoilFooter();
    else
        _lcd->print(blankline);
}

void Lcd::printBoilFooter()
{
    String boil = _activeStatus->ActiveBoilStepName.substring(0, 20);
    _lcd->print(boil);
    RemoveLastChars(boil.length());
}

void Lcd::printMashFooter()
{
    String step = _activeStatus->ActiveMashStepName.substring(0, 12) + " " +
                  _activeStatus->ActiveMashStepSufixName.substring(0, 7);
    _lcd->print(step.substring(0, 20));
    RemoveLastChars(step.length());
}

void Lcd::printIpFooter()
{
    String ip = "";
    wl_status_t status = WiFi.status();
    WiFiMode_t currentWiFiMode = WiFi.getMode();
    if (status == WL_CONNECTED)
        ip = "\x49"
             "P\072 " +
             WiFi.localIP().toString();
    else if (currentWiFiMode == WIFI_AP || currentWiFiMode == WIFI_AP_STA)
        ip = "\x49"
             "P\072 " +
             WiFi.softAPIP().toString();
    _lcd->print(ip);
    RemoveLastChars(ip.length());
}

String Lcd::getTemperature(double temperature, bool target)
{
    if (target)
        if (temperature <= 99)
            return String(temperature).substring(0, 2);
        else
            return "\x31"
                   "H";
    else if (temperature <= 0)
        return "\x30"
               "0\0560\x30"
               "";
    else if (temperature >= 100)
        return "\x31"
               "0\060.\x30"
               "";
    else
        return String(temperature);
}

void Lcd::RemoveLastChars(int length)
{
    for (int i = 0; i < 20 - length; i++)
        _lcd->print(" ");
}

String Lcd::GetCount(bool down)
{
    int difference = down ? _activeStatus->EndTime - now() : now() - _activeStatus->EndTime;
    if (difference <= 0 && down)
        return "\x30"
               "0\0720\x30"
               ":\0600";
    int seconds = floor(difference);
    int minutes = floor(seconds / 60);
    int hours = floor(minutes / 60);
    minutes %= 60;
    seconds %= 60;
    char buffer[16];
    sprintf(buffer, "%02u:%02u:%02u", hours, minutes, seconds);
    return buffer;
}