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
#include <WiFiSettingsService.h>
#include <WiFiStatus.h>
#include <WiFiScanner.h>
#include <APSettingsService.h>
#include <NTPSettingsService.h>
#include <NTPStatus.h>
#include <OTASettingsService.h>
#include <APStatus.h>

#include <PID_v1.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <BrewUNO/MashSettingsService.h>
#include <BrewUNO/BoilSettingsService.h>
#include <BrewUNO/BrewSettingsService.h>
#include <BrewUNO/BrewService.h>
#include <BrewUNO/MashService.h>
#include <BrewUNO/BoilService.h>
#include <BrewUNO/TemperatureService.h>
#include <BrewUNO/KettleHeaterService.h>
#include <BrewUNO/ActiveStatus.h>
#include <BrewUNO/Buzzer.h>
#include <BrewUNO/Pump.h>

#define SERIAL_BAUD_RATE 115200

OneWire oneWire(TEMPERATURE_BUS);
DallasTemperature DS18B20(&oneWire);

AsyncWebServer server(80);

WiFiSettingsService wifiSettingsService = WiFiSettingsService(&server, &SPIFFS);
APSettingsService apSettingsService = APSettingsService(&server, &SPIFFS);
NTPSettingsService ntpSettingsService = NTPSettingsService(&server, &SPIFFS);
OTASettingsService otaSettingsService = OTASettingsService(&server, &SPIFFS);

WiFiScanner wifiScanner = WiFiScanner(&server);
WiFiStatus wifiStatus = WiFiStatus(&server);
NTPStatus ntpStatus = NTPStatus(&server);
APStatus apStatus = APStatus(&server);

//brewUNO
TemperatureService temperatureService = TemperatureService(&server, &SPIFFS, DS18B20);
BrewSettingsService brewSettingsService = BrewSettingsService(&server, &SPIFFS);
MashSettingsService mashSettings = MashSettingsService(&server, &SPIFFS);
BoilSettingsService boilSettingsService = BoilSettingsService(&server, &SPIFFS, &brewSettingsService);

ActiveStatus activeStatus = ActiveStatus(&SPIFFS);
Pump pump = Pump(&server, &activeStatus, &brewSettingsService);
KettleHeaterService kettleHeaterService = KettleHeaterService(&temperatureService, &activeStatus, &brewSettingsService);
MashService mashService = MashService(&SPIFFS, &temperatureService, &pump);
BoilService boilService = BoilService(&SPIFFS, &temperatureService);
BrewService brewService = BrewService(&server, &SPIFFS, &mashService, &boilService, &brewSettingsService, &kettleHeaterService, &activeStatus, &temperatureService, &pump);

void setup()
{
  // Disable wifi config persistance
  WiFi.persistent(false);

  Serial.begin(SERIAL_BAUD_RATE);
  SPIFFS.begin();
  //SPIFFS.format();

  // start services
  ntpSettingsService.begin();
  otaSettingsService.begin();
  apSettingsService.begin();
  wifiSettingsService.begin();

  brewSettingsService.begin();
  brewService.begin();

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
    {
      request->send(SPIFFS, "/www/index.html");
    }
    else if (request->method() == HTTP_OPTIONS)
    {
      request->send(200);
    }
    else
    {
      request->send(404);
    }
  });

// Disable CORS if required
#if defined(ENABLE_CORS)
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
#endif

  server.begin();

  pinMode(PUMP_BUS, OUTPUT);
  pinMode(BUZZER_BUS, OUTPUT);
  digitalWrite(BUZZER_BUS, LOW);
  pinMode(HEATER_BUS, OUTPUT);
  pump.TurnPumpOff();
  DS18B20.begin();
}

void loop()
{
  apSettingsService.loop();
  ntpSettingsService.loop();
  otaSettingsService.loop();
  brewService.loop();
}
