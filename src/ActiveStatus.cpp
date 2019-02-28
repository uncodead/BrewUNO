#include <ActiveStatus.h>

ActiveStatus::ActiveStatus(FS *fs) : _fs(fs)
{
}

ActiveStatus::~ActiveStatus() {}

boolean ActiveStatus::LoadActiveStatusSettings()
{
    DynamicJsonBuffer jsonBufferActiveStatus;

    File configFile = _fs->open(ACTIVE_STATUS_FILE, "r");
    String data;
    if (configFile && configFile.size())
    {
        int i;
        for (i = 0; i < configFile.size(); i++)
        {
            data += ((char)configFile.read());
        }
        configFile.close();
    }

    JsonObject &_activeStatus = (jsonBufferActiveStatus.parseObject(data));
    Serial.println(_activeStatus.success());
    _activeStatus.prettyPrintTo(Serial);
    configFile.close();

    ActiveStep = _activeStatus.get<int>("active_step");
    ActiveMashStepIndex = _activeStatus.get<int>("active_mash_step_index");
    ActiveBoilStepIndex = _activeStatus.get<String>("active_boil_step_index");
    BoilTime = _activeStatus.get<time_t>("boil_time");
    BoilTargetTemperature = _activeStatus.get<float>("boil_target_temperature");
    TargetTemperature = _activeStatus.get<float>("target_temperature");
    EndTime = _activeStatus.get<time_t>("end_time");
    StartTime = _activeStatus.get<time_t>("start_time");
    TimeNow = _activeStatus.get<time_t>("time_now");
    BrewStarted = _activeStatus.get<boolean>("brew_started");
    Temperatures = _activeStatus.get<String>("temperatures");
    PWM = _activeStatus.get<int>("pwm");

    return _activeStatus.success();
}

String ActiveStatus::GetJson()
{
    Serial.println("GET Json:");
    String status = "{\"active_step\":" + String(ActiveStep) + "," +
                    "\"active_mash_step_index\":" + String(ActiveMashStepIndex) + "," +
                    "\"active_boil_step_index\":\"" + String(ActiveBoilStepIndex) + "\"" + "," +
                    "\"boil_time\":" + String(BoilTime) + "," +
                    "\"boil_target_temperature\":" + String(BoilTargetTemperature) + "," +
                    "\"target_temperature\":" + String(TargetTemperature) + "," +
                    "\"start_time\":" + String(StartTime) + "," +
                    "\"end_time\":" + String(EndTime) + "," +
                    "\"time_now\":" + String(TimeNow) + "," +
                    "\"brew_started\":" + String(BrewStarted) + "," +
                    "\"temperature\":" + String(Temperature) + "," +
                    "\"temperatures\":\"" + String(Temperatures) + "\"" + "," +
                    "\"pwm\":" + String(PWM) + "}";
    Serial.println(status);
    return status;
}

void ActiveStatus::LogTemperature(float current, float target)
{
    String strCurrent = String(current);
    String strTarget = String(target);
    if (current < 100)
    {
        strCurrent = " " + String(current);
    }
    if (target < 100)
    {
        strTarget = " " + String(target);
    }

    if (Temperatures == "")
    {
        Temperatures = "{c:" + strCurrent + ",t:" + strTarget + "}";
    }
    else
    {
        Temperatures = Temperatures + ',' + "{c:" + strCurrent + ",t:" + strTarget + "}";
    }
    if (Temperatures.length() >= 220)
    {
        Temperatures.remove(0, 20);
    }
}

void ActiveStatus::SaveActiveStatus(time_t startTime,
                                    time_t endTime,
                                    time_t timeNow,
                                    float targetTemperature,
                                    int activeMashStepIndex,
                                    String activeBoilStepIndex,
                                    int boilTime,
                                    float boilTargetTemperature,
                                    int activeStep,
                                    boolean brewStarted)
{
    ActiveStep = activeStep;

    ActiveMashStepIndex = activeMashStepIndex;

    ActiveBoilStepIndex = activeBoilStepIndex;
    BoilTime = boilTime;
    BoilTargetTemperature = boilTargetTemperature;
    TargetTemperature = targetTemperature;
    StartTime = startTime;
    EndTime = endTime;
    TimeNow = timeNow;
    BrewStarted = brewStarted;
    Temperature = 0;
    Temperatures = "";
    PWM = 0;

    SaveActiveStatus();
}

void ActiveStatus::SaveActiveStatus()
{
    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject &object = jsonBuffer.createObject();

    object["active_step"] = ActiveStep;
    object["active_mash_step_index"] = ActiveMashStepIndex;
    object["active_boil_step_index"] = ActiveBoilStepIndex;
    object["boil_time"] = BoilTime;
    object["boil_target_temperature"] = BoilTargetTemperature;
    object["target_temperature"] = TargetTemperature;
    object["start_time"] = StartTime;
    object["end_time"] = EndTime;
    object["time_now"] = now();
    object["brew_started"] = BrewStarted;
    object["temperature"] = Temperature;
    object["temperatures"] = Temperatures;
    object["pwm"] = PWM;

    File configFile = _fs->open(ACTIVE_STATUS_FILE, "w");
    if (configFile)
    {
        object.printTo(configFile);
    }
    configFile.close();
}