#include <SystemStatus.h>

SystemStatus::SystemStatus(AsyncWebServer *server) : _server(server)
{
  _server->on(SYSTEM_STATUS_SERVICE_PATH, HTTP_GET,
              std::bind(&SystemStatus::systemStatus, this, std::placeholders::_1));
}

void SystemStatus::systemStatus(AsyncWebServerRequest *request)
{
  AsyncJsonResponse *response = new AsyncJsonResponse(MAX_ESP_STATUS_SIZE);
  JsonObject root = response->getRoot();
#if defined(ESP8266)
  root["esp_platform"] = "esp8266";
#elif defined(ESP_PLATFORM)
  root["esp_platform"] = "esp32";
#endif
  root["cpu_freq_mhz"] = ESP.getCpuFreqMHz();
  root["free_heap"] = ESP.getFreeHeap();
  root["sketch_size"] = ESP.getSketchSize();
  root["free_sketch_space"] = ESP.getFreeSketchSpace();
  root["sdk_version"] = ESP.getSdkVersion();
  root["mac"] = WiFi.macAddress();
  root["flash_chip_size"] = ESP.getFlashChipSize();
  root["flash_chip_speed"] = ESP.getFlashChipSpeed();
  root["chip_id"] = ESP.getChipId();
  root["flash_chip_id"] = ESP.getFlashChipId();

  response->setLength();
  request->send(response);
}
