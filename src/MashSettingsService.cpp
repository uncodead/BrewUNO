#include <MashSettingsService.h>

MashSettingsService::MashSettingsService(AsyncWebServer *server) : _server(server) {
    _server->on(SAVE_MASH_SETUP_SERVICE_PATH, HTTP_POST, std::bind(&MashSettingsService::save, this, std::placeholders::_1));
}

void MashSettingsService::save(AsyncWebServerRequest *request) {
    AsyncJsonResponse * response = new AsyncJsonResponse();
    JsonObject& root = response->getRoot();
    root["status"] = "ok";
    response->setLength();
    request->send(response);
}