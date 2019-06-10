#include <BrewUNO/TemperatureService.h>

TemperatureService::TemperatureService(AsyncWebServer *server, FS *fs, DallasTemperature dallasTemperature) : _server(server), _fs(fs), _dallasTemperature(dallasTemperature)
{
    _dallasTemperature.begin();
    _server->on(GET_SENSORS_SERVICE_PATH, HTTP_GET, std::bind(&TemperatureService::GetTemperatureAndAdress, this, std::placeholders::_1));
}

TemperatureService::~TemperatureService() {}

void TemperatureService::GetTemperatureAndAdress(AsyncWebServerRequest *request)
{
    request->send(200, APPLICATION_JSON_TYPE, GetSensorsJson());
}

int TemperatureService::GetSensorsCount()
{
    return _dallasTemperature.getDeviceCount();
}

String TemperatureService::GetFirstSensorAddress()
{
    DeviceAddress Thermometer;
    _dallasTemperature.getAddress(Thermometer, 0);
    return GetAddressToString(Thermometer);
}

String TemperatureService::GetSensorsJson()
{
    DeviceAddress Thermometer;
    _dallasTemperature.requestTemperatures();
    int deviceCount = _dallasTemperature.getDeviceCount();
    Serial.println("Sensor count: " + String(deviceCount));
    String json = "{ \"sensors\": [ ";
    for (int i = 0; i < deviceCount; i++)
    {
        _dallasTemperature.getAddress(Thermometer, i);
        json += "{ \"address\": \"" + GetAddressToString(Thermometer) + "\",";
        json += "\"value\": \"" + String(_dallasTemperature.getTempC(Thermometer)) +  "\"},";
    }
    json += "]}";
    return json;
}

String TemperatureService::GetAddressToString(DeviceAddress deviceAddress)
{
    String str = "";
    for (uint8_t i = 0; i < 8; i++)
    {
        if (deviceAddress[i] < 16)
            str += String(0, HEX);
        str += String(deviceAddress[i], HEX);
    }
    return str;
}

float TemperatureService::GetTemperature(String sensorAddress)
{
    float tempC = 0;
    DeviceAddress Thermometer;
    _dallasTemperature.requestTemperatures();
    int deviceCount = _dallasTemperature.getDeviceCount();
    for (int i = 0; i < deviceCount; i++)
    {
        _dallasTemperature.getAddress(Thermometer, i);
        if (GetAddressToString(Thermometer) == sensorAddress)
        {
            tempC = _dallasTemperature.getTempC(Thermometer);
        }
    }
    return tempC;
}