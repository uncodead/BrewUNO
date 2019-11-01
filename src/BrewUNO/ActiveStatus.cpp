#include <BrewUNO/ActiveStatus.h>

ActiveStatus::ActiveStatus(FS *fs) : _fs(fs)
{
}

ActiveStatus::~ActiveStatus() {}

boolean ActiveStatus::LoadActiveStatusSettings()
{
    File configFile = _fs->open(ACTIVE_STATUS_FILE, "r");
    if (configFile)
    {
        size_t size = configFile.size();
        if (size <= MAX_ACTIVESTATUS_SIZE)
        {
            DynamicJsonDocument _activeStatusJsonDocument = DynamicJsonDocument(MAX_ACTIVESTATUS_SIZE);
            DeserializationError error = deserializeJson(_activeStatusJsonDocument, configFile);
            if (error == DeserializationError::Ok && _activeStatusJsonDocument.is<JsonObject>())
            {
                JsonObject _activeStatus = _activeStatusJsonDocument.as<JsonObject>();
                ActiveStep = _activeStatus["active_step"];
                ActiveMashStepIndex = _activeStatus["active_mash_step_index"];
                ActiveMashStepName = _activeStatus["active_mash_step_name"] | "";
                ActiveMashStepSufixName = _activeStatus["active_mash_step_sufix_name"] | "";
                ActiveBoilStepIndex = _activeStatus["active_boil_step_index"] | "";
                ActiveBoilStepName = _activeStatus["active_boil_step_name"] | "";
                BoilTime = _activeStatus["boil_time"];
                BoilTargetTemperature = _activeStatus["boil_target_temperature"];
                TargetTemperature = _activeStatus["target_temperature"];
                EndTime = _activeStatus["end_time"];
                StartTime = _activeStatus["start_time"];
                TimeNow = _activeStatus["time_now"];
                BrewStarted = _activeStatus["brew_started"];
                PWM = _activeStatus["pwm"];
                SpargePWM = _activeStatus["sparge_pwm"];
                Recirculation = _activeStatus["recirculation"];
                HeaterOn = _activeStatus["heater_on"];
                StepLock = _activeStatus["step_lock"];
                BoilPowerPercentage = _activeStatus["boil_power_percentage"];
                PIDTuning = _activeStatus["pid_tuning"];
                TimeNotSet = _activeStatus["time_not_set"];
                Count = _activeStatus["count"] | "";
                TempUnit = _activeStatus["temp_unit"] | "";
                configFile.close();
            }
        }
        configFile.close();
    }
    return true;
}

void ActiveStatus::TimeNotSetted()
{
    TimeNotSet = true;
    Serial.println("Time NOT setted");
}

void ActiveStatus::TimeSetted()
{
    TimeNotSet = false;
    Serial.println("Time setted");
}

static const char ACTIVE_STATUS[] PROGMEM = "{\"active_step\":{{ActiveStep}},\"active_mash_step_index\":{{ActiveMashStepIndex}},\"active_mash_step_name\":\"{{ActiveMashStepName}}\",\"active_mash_step_sufix_name\":\"{{ActiveMashStepSufixName}}\",\"active_boil_step_index\":\"{{ActiveBoilStepIndex}}\",\"active_boil_step_name\":\"{{ActiveBoilStepName}}\",\"boil_time\":{{BoilTime}},\"boil_target_temperature\":{{BoilTargetTemperature}},\"target_temperature\":{{TargetTemperature}},\"start_time\":{{StartTime}},\"end_time\":{{EndTime}},\"time_now\":{{TimeNow}},\"brew_started\":{{BrewStarted}},\"temperature\":{{Temperature}},\"sparge_temperature\":{{SpargeTemperature}},\"boil_temperature\":{{BoilTemperature}},\"auxone_temperature\":{{AuxOneTemperature}},\"auxtwo_temperature\":{{AuxTwoTemperature}},\"auxthree_temperature\":{{AuxThreeTemperature}},\"main_sensor\": \"{{MainSensor}}\",\"sparge_sensor\": \"{{SpargeSensor}}\",\"boil_sensor\": \"{{BoilSensor}}\",\"auxone_sensor\": \"{{AuxOneSensor}}\",\"auxtwo_sensor\": \"{{AuxTwoSensor}}\",\"auxthree_sensor\": \"{{AuxThreeSensor}}\",\"pwm\":{{PWM}},\"pwm_percentage\":{{PWMPercentage}},\"sparge_pwm\":{{SpargePWM}},\"sparge_pwm_percentage\":{{SpargePWMPercentage}},\"boil_pwm\":{{BoilPWM}},\"boil_pwm_percentage\":{{BoilPWMPercentage}},\"enable_sparge\":{{EnableSparge}},\"enable_boilkettle\":{{EnableBoilKettle}},\"sparge_target_temperature\":{{SpargeTargetTemperature}},\"recirculation\":{{Recirculation}},\"heater_on\":{{HeaterOn}},\"step_lock\":{{StepLock}},\"step_locked\":{{StepLocked}},\"pid_tuning\":{{PIDTuning}},\"pump_on\":{{PumpOn}},\"pump_is_resting\":{{PumpIsResting}},\"time_not_set\":{{TimeNotSet}},\"temp_unit\": \"{{TempUnit}}\",\"version\": \"{{Version}}\",\"count\": \"{{Count}}\",\"boil_power_percentage\":{{BoilPowerPercentage}} }";

String ActiveStatus::GetJson()
{
    String active_status = FPSTR(ACTIVE_STATUS);
    active_status.replace("{{ActiveStep}}", String(ActiveStep));
    active_status.replace("{{ActiveMashStepIndex}}", String(ActiveMashStepIndex));
    active_status.replace("{{ActiveMashStepName}}", String(ActiveMashStepName));
    active_status.replace("{{ActiveMashStepSufixName}}", String(ActiveMashStepSufixName));
    active_status.replace("{{ActiveBoilStepIndex}}", String(ActiveBoilStepIndex));
    active_status.replace("{{ActiveBoilStepName}}", String(ActiveBoilStepName));
    active_status.replace("{{BoilTime}}", String(BoilTime));
    active_status.replace("{{BoilTargetTemperature}}", String(BoilTargetTemperature));
    active_status.replace("{{TargetTemperature}}", String(TargetTemperature));
    active_status.replace("{{StartTime}}", String(StartTime));
    active_status.replace("{{EndTime}}", String(EndTime));
    active_status.replace("{{TimeNow}}", String(TimeNow));
    active_status.replace("{{BrewStarted}}", String(BrewStarted));
    active_status.replace("{{Temperature}}", String(Temperature));
    active_status.replace("{{SpargeTemperature}}", String(SpargeTemperature));
    active_status.replace("{{BoilTemperature}}", String(BoilTemperature));
    active_status.replace("{{AuxOneTemperature}}", String(AuxOneTemperature));
    active_status.replace("{{AuxTwoTemperature}}", String(AuxTwoTemperature));
    active_status.replace("{{AuxThreeTemperature}}", String(AuxThreeTemperature));
    active_status.replace("{{MainSensor}}", MainSensor);
    active_status.replace("{{SpargeSensor}}", SpargeSensor);
    active_status.replace("{{BoilSensor}}", BoilSensor);
    active_status.replace("{{AuxOneSensor}}", AuxOneSensor);
    active_status.replace("{{AuxTwoSensor}}", AuxTwoSensor);
    active_status.replace("{{AuxThreeSensor}}", AuxThreeSensor);
    active_status.replace("{{PWM}}", String(PWM, 2));
    active_status.replace("{{PWMPercentage}}", String(PWMPercentage, 2));
    active_status.replace("{{SpargePWM}}", String(SpargePWM, 2));
    active_status.replace("{{SpargePWMPercentage}}", String(SpargePWMPercentage, 2));
    active_status.replace("{{BoilPWM}}", String(BoilPWM, 2));
    active_status.replace("{{BoilPWMPercentage}}", String(BoilPWMPercentage, 2));
    active_status.replace("{{EnableSparge}}", String(EnableSparge));
    active_status.replace("{{EnableBoilKettle}}", String(EnableBoilKettle));
    active_status.replace("{{SpargeTargetTemperature}}", String(SpargeTargetTemperature));
    active_status.replace("{{Recirculation}}", String(Recirculation));
    active_status.replace("{{HeaterOn}}", String(HeaterOn));
    active_status.replace("{{StepLock}}", String(StepLock));
    active_status.replace("{{StepLocked}}", String(StepLocked));
    active_status.replace("{{PIDTuning}}", String(PIDTuning));
    active_status.replace("{{PumpOn}}", String(PumpOn));
    active_status.replace("{{PumpIsResting}}", String(PumpIsResting));
    active_status.replace("{{TimeNotSet}}", String(TimeNotSet));
    active_status.replace("{{TempUnit}}", TempUnit);
    active_status.replace("{{Version}}", String(Version));
    active_status.replace("{{Count}}", Count);
    active_status.replace("{{BoilPowerPercentage}}", String(BoilPowerPercentage));
    return active_status;
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
    ActiveMashStepName = "";
    ActiveMashStepSufixName = "";
    ActiveBoilStepIndex = activeBoilStepIndex;
    ActiveBoilStepName = "";
    BoilTime = boilTime;
    BoilTargetTemperature = boilTargetTemperature;
    TargetTemperature = targetTemperature;
    StartTime = startTime;
    EndTime = endTime;
    TimeNow = timeNow;
    BrewStarted = brewStarted;
    PWM = 0;
    PWMPercentage = 0;
    SpargePWM = 0;
    SpargePWMPercentage = 0;
    BoilPWM = 0;
    BoilPWMPercentage = 0;
    Recirculation = false;
    HeaterOn = true;
    StepLock = false;
    StepLocked = false;
    PIDTuning = false;
    BoilPowerPercentage = 0;
    Count = "00:00:00";

    SaveActiveStatus();
}

time_t lastWrite = now();

void ActiveStatus::SaveActiveStatusLoop()
{
    if ((!BrewStarted) || (now() - lastWrite < 60))
        return;

    SaveActiveStatus();
}

void ActiveStatus::SaveActiveStatus()
{
    lastWrite = now();

    DynamicJsonDocument _activeStatusJsonDocument = DynamicJsonDocument(MAX_ACTIVESTATUS_SIZE);
    JsonObject _activeStatus = _activeStatusJsonDocument.to<JsonObject>();

    _activeStatus["active_step"] = ActiveStep;
    _activeStatus["active_mash_step_index"] = ActiveMashStepIndex;
    _activeStatus["active_mash_step_name"] = ActiveMashStepName;
    _activeStatus["active_mash_step_sufix_name"] = ActiveMashStepSufixName;
    _activeStatus["active_boil_step_index"] = ActiveBoilStepIndex;
    _activeStatus["active_boil_step_name"] = ActiveBoilStepName;
    _activeStatus["boil_time"] = BoilTime;
    _activeStatus["boil_target_temperature"] = BoilTargetTemperature;
    _activeStatus["target_temperature"] = TargetTemperature;
    _activeStatus["start_time"] = StartTime;
    _activeStatus["end_time"] = EndTime;
    _activeStatus["time_now"] = TimeNow;
    _activeStatus["brew_started"] = BrewStarted;
    _activeStatus["temperature"] = Temperature;
    _activeStatus["pwm"] = PWM;
    _activeStatus["sparge_pwm"] = SpargePWM;
    _activeStatus["recirculation"] = Recirculation;
    _activeStatus["heater_on"] = HeaterOn;
    _activeStatus["step_lock"] = StepLock;
    _activeStatus["boil_power_percentage"] = BoilPowerPercentage;
    _activeStatus["pid_tuning"] = PIDTuning;
    _activeStatus["temp_unit"] = TempUnit;
    _activeStatus["time_not_set"] = TimeNotSet;
    _activeStatus["count"] = Count;

    File configFile = _fs->open(ACTIVE_STATUS_FILE, "w");
    if (configFile)
        serializeJson(_activeStatus, configFile);
    else
    {
        Serial.println("Error mounting the file system");
        return;
    }

    configFile.close();
}

void ActiveStatus::SetTemperature(Temperatures temps)
{
    if (temps.Main >= 0)
        Temperature = temps.Main;
    if (temps.Sparge >= 0)
        SpargeTemperature = temps.Sparge;
    if (temps.Boil >= 0)
        BoilTemperature = temps.Boil;
    if (temps.AuxOne >= 0)
        AuxOneTemperature = temps.AuxOne;
    if (temps.AuxTwo >= 0)
        AuxTwoTemperature = temps.AuxTwo;
    if (temps.AuxThree >= 0)
        AuxThreeTemperature = temps.AuxThree;
}