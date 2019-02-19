#include <ActiveStatus.h>

ActiveStatus::ActiveStatus(FS *fs) : _fs(fs)
{
}

ActiveStatus::~ActiveStatus() {}

String ActiveStatus::GetJson()
{
    File configFile = _fs->open(ACTIVE_STATUS_FILE, "r");
    String data;
    if (configFile && configFile.size())
    {
        int i;
        for (i   = 0; i < configFile.size(); i++) //Read upto complete file size
        {
            data += ((char)configFile.read());
        }
        configFile.close(); //Close file
    }

    return data;
}

void ActiveStatus::LogTemperature(float current, float target)
{
    if (Temperatures == "")
    {
        Temperatures = "{c:" + String(current) + ",t:" + String(target) + "}";
    }
    else
    {
        Temperatures = Temperatures + ',' + "{c:" + current + ",t:" + target + "}";
    }
    if (Temperatures.length() >= 179)
    {
        Temperatures.remove(0, 18);
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
    object["time_now"] = TimeNow;

    object["brew_started"] = BrewStarted;

    object["temperature"] = Temperature;
    object["temperatures"] = Temperatures;

    File configFile = _fs->open(ACTIVE_STATUS_FILE, "w");
    if (configFile)
    {
        object.printTo(configFile);
    }
    configFile.close();
}