#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#endif

#include <FS.h>
#include <SecuritySettingsService.h>
#include <WiFiSettingsService.h>
#include <APSettingsService.h>
#include <NTPSettingsService.h>
#include <OTASettingsService.h>
#include <AuthenticationService.h>
#include <WiFiScanner.h>
#include <WiFiStatus.h>
#include <NTPStatus.h>
#include <APStatus.h>
#include <SystemStatus.h>

#include <PID_v1.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

#include <BrewUNO/MashSettingsService.h>
#include <BrewUNO/BoilSettingsService.h>
#include <BrewUNO/BrewSettingsService.h>
#include <BrewUNO/BrewService.h>
#include <BrewUNO/MashService.h>
#include <BrewUNO/BoilService.h>
#include <BrewUNO/TemperatureService.h>
#include <BrewUNO/HeaterService.h>
#include <BrewUNO/MashKettleHeaterService.h>
#include <BrewUNO/SpargeKettleHeaterService.h>
#include <BrewUNO/ActiveStatus.h>
#include <BrewUNO/Buzzer.h>
#include <BrewUNO/Pump.h>
#include <BrewUNO/DisplayService.h>
#include <BrewUNO/InternationalizationService.h>

#define SERIAL_BAUD_RATE 115200

OneWire oneWire(TEMPERATURE_BUS);
DallasTemperature DS18B20(&oneWire);
int deviceCount = 0;

LiquidCrystal_I2C lcd(0x0, 20, 4);
AsyncWebServer server(80);

SecuritySettingsService securitySettingsService = SecuritySettingsService(&server, &SPIFFS);
WiFiSettingsService wifiSettingsService = WiFiSettingsService(&server, &SPIFFS, &securitySettingsService);
APSettingsService apSettingsService = APSettingsService(&server, &SPIFFS, &securitySettingsService);
NTPSettingsService ntpSettingsService = NTPSettingsService(&server, &SPIFFS, &securitySettingsService);
OTASettingsService otaSettingsService = OTASettingsService(&server, &SPIFFS, &securitySettingsService);
AuthenticationService authenticationService = AuthenticationService(&server, &securitySettingsService);

WiFiScanner wifiScanner = WiFiScanner(&server, &securitySettingsService);
WiFiStatus wifiStatus = WiFiStatus(&server, &securitySettingsService);
NTPStatus ntpStatus = NTPStatus(&server, &securitySettingsService);
APStatus apStatus = APStatus(&server, &securitySettingsService);
SystemStatus systemStatus = SystemStatus(&server, &securitySettingsService);

//brewUNO
ActiveStatus activeStatus = ActiveStatus(&SPIFFS);

BrewSettingsService brewSettingsService = BrewSettingsService(&server, &SPIFFS, &activeStatus);
TemperatureService temperatureService = TemperatureService(&server, &SPIFFS, DS18B20, &brewSettingsService);
MashSettingsService mashSettings = MashSettingsService(&server, &SPIFFS);
BoilSettingsService boilSettingsService = BoilSettingsService(&server, &SPIFFS, &brewSettingsService);

Pump pump = Pump(&server, &activeStatus, &brewSettingsService);
DisplayService display = DisplayService(&activeStatus, &wifiStatus, &lcd);
MashKettleHeaterService mashKettleHeaterService = MashKettleHeaterService(&temperatureService, &activeStatus, &brewSettingsService, HEATER_BUS);
SpargeKettleHeaterService spargeKettleHeaterService = SpargeKettleHeaterService(&temperatureService, &activeStatus, &brewSettingsService, SPARGE_HEATER_BUS);
MashService mashService = MashService(&SPIFFS, &temperatureService, &pump);
BoilService boilService = BoilService(&SPIFFS, &temperatureService, &brewSettingsService);
BrewService brewService = BrewService(&server, &SPIFFS, &mashService, &boilService, &brewSettingsService, &mashKettleHeaterService, &spargeKettleHeaterService, &activeStatus, &temperatureService, &pump);
InternationalizationService internationalizationService = InternationalizationService(&server, &SPIFFS, &brewSettingsService);

void setup()
{
  // Disable wifi config persistance and auto reconnect
  WiFi.persistent(false);
  WiFi.setAutoReconnect(false);

#if defined(ESP_PLATFORM)
  // Init the wifi driver on ESP32
  WiFi.mode(WIFI_MODE_MAX);
  WiFi.mode(WIFI_MODE_NULL);
#endif

  Serial.begin(SERIAL_BAUD_RATE);
  SPIFFS.begin();
  //SPIFFS.format();

  // Start security settings service first
  securitySettingsService.begin();

  // Start services
  ntpSettingsService.begin();
  otaSettingsService.begin();
  apSettingsService.begin();
  wifiSettingsService.begin();

  // Serving static resources from /www/
  server.serveStatic("/js/", SPIFFS, "/www/js/");
  server.serveStatic("/css/", SPIFFS, "/www/css/");
  server.serveStatic("/fonts/", SPIFFS, "/www/fonts/");
  server.serveStatic("/app/", SPIFFS, "/www/app/");
  server.serveStatic("/favicon.ico", SPIFFS, "/www/favicon.ico");

  // Serving all other get requests with "/www/index.htm"
  // OPTIONS get a straight up 200 response
  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_GET)
      request->send(SPIFFS, "/www/index.html");
    else if (request->method() == HTTP_OPTIONS)
      request->send(200);
    else
      request->send(404);
  });

// Disable CORS if required
#if defined(ENABLE_CORS)
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", CORS_ORIGIN);
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Accept, Content-Type, Authorization");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Credentials", "true");
#endif

  server.begin();

  //BrewUNO
  pinMode(PUMP_BUS, OUTPUT);
  pinMode(BUZZER_BUS, OUTPUT);
  digitalWrite(BUZZER_BUS, LOW);
  pinMode(HEATER_BUS, OUTPUT);
  pinMode(SPARGE_HEATER_BUS, OUTPUT);

  pump.TurnPumpOff();
  DS18B20.begin();
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  deviceCount = DS18B20.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
  temperatureService.DeviceCount = deviceCount;
  brewSettingsService.begin();
  brewService.begin();
  display.begin();
}

void loop()
{
  wifiSettingsService.loop();
  apSettingsService.loop();
  ntpSettingsService.loop();
  otaSettingsService.loop();
  brewService.loop();
  display.loop();
}
