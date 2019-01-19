#include <ActiveStatus.h>

DynamicJsonBuffer jsonBufferActiveStatus;

ActiveStatus::ActiveStatus(FS *fs) : _fs(fs)
{
}

ActiveStatus::~ActiveStatus() {}

JsonObject *ActiveStatus::GetJson()
{
    LoadActiveStatusSettings();
    return _activeStatus;
}

void ActiveStatus::LoadActiveStatusSettings()
{
    File configFile = _fs->open(ACTIVE_STATUS_FILE, "r");
    _activeStatus = &(jsonBufferActiveStatus.parseObject(configFile));
    configFile.close();

    ActiveStep = _activeStatus->get<int>("active_step");

    ActiveMashStepIndex = _activeStatus->get<int>("active_mash_step_index");

    ActiveBoilStepIndex = _activeStatus->get<String>("active_boil_step_index");
    BoilTime = _activeStatus->get<time_t>("boil_time");
    BoilTargetTemperature = _activeStatus->get<float>("boil_target_temperature");

    TargetTemperature = _activeStatus->get<float>("target_temperature");

    EndTime = _activeStatus->get<time_t>("start_time");
    StartTime = _activeStatus->get<time_t>("end_time");
    TimeNow = _activeStatus->get<time_t>("time_now");

    BrewStarted = _activeStatus->get<boolean>("brew_started");
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
    SaveActiveStatus();
}

void ActiveStatus::SaveActiveStatus()
{
    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject& object = jsonBuffer.createObject();

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

    File configFile = _fs->open(ACTIVE_STATUS_FILE, "w");
    object.printTo(configFile);
    configFile.close();
    //LoadActiveStatusSettings();
}