#include <TemperatureService.h>

TemperatureService::TemperatureService(AsyncWebServer *server, FS *fs, DallasTemperature dallasTemperature, BrewSettingsService *brewSettingsService) : _server(server),
                                                                                                                                                        _fs(fs),
                                                                                                                                                        _dallasTemperature(dallasTemperature),
                                                                                                                                                        _brewSettingsService(brewSettingsService)
{
    _server->on(GET_SENSORS_SERVICE_PATH, HTTP_GET, std::bind(&TemperatureService::GetTemperatureAndAdress, this, std::placeholders::_1));
}

TemperatureService::~TemperatureService() {}

void TemperatureService::GetTemperatureAndAdress(AsyncWebServerRequest *request)
{
    request->send(200, APPLICATION_JSON_TYPE, GetSensorsJson());
}

String TemperatureService::GetFirstSensorAddress()
{
    DeviceAddress Thermometer;
    _dallasTemperature.getAddress(Thermometer, 0);
    return GetAddressToString(Thermometer);
}

String json = "";

String TemperatureService::GetSensorsJson()
{
    if (json == "")
        GetTemperatures();
    return json;
}

Temperatures TemperatureService::GetTemperatures()
{
    Temperatures temps;
    temps.Main = 0;
    temps.Sparge = 0;
    DeviceAddress Thermometer;
    _dallasTemperature.requestTemperatures();
    String _json = "{ \"sensors\": [ ";
    String addr = "";
    for (int i = 0; i < DeviceCount; i++)
    {
        _dallasTemperature.getAddress(Thermometer, i);
        float temp = _brewSettingsService->TempUnit == "C" ? _dallasTemperature.getTempC(Thermometer) : _dallasTemperature.getTempF(Thermometer);
        _json += "{ \"address\": \"" + GetAddressToString(Thermometer) + "\",\"value\": \"" + String(temp) + "\"}";
        if (i < DeviceCount - 1)
            _json += ',';
        addr = GetAddressToString(Thermometer);
        if (addr == _brewSettingsService->MainSensor)
            temps.Main = temp + _brewSettingsService->MainSensorOffset;
        if (addr == _brewSettingsService->SpargeSensor)
            temps.Sparge = temp + _brewSettingsService->SpargeSensorOffset;
        if (addr == _brewSettingsService->BoilSensor)
            temps.Boil = temp + _brewSettingsService->BoilSensorOffset;
        if (addr == _brewSettingsService->AuxOneSensor)
            temps.AuxOne = temp + _brewSettingsService->AuxSensorOneOffset;
        if (addr == _brewSettingsService->AuxTwoSensor)
            temps.AuxTwo = temp + _brewSettingsService->AuxSensorTwoOffset;
        if (addr == _brewSettingsService->AuxThreeSensor)
            temps.AuxThree = temp + _brewSettingsService->AuxSensorThreeOffset;
    }
    json = _json + "]}";
    return temps;
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