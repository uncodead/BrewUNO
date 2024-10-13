#include <ActiveStatus.h>

bool logStarted = false;
time_t lastWrite = now();
time_t lastWriteWebSocket = now();
static const char ACTIVE_STATUS[] PROGMEM = "{\"as\":{{ActiveStep}},\"acsi\":{{ActiveCoolingStepIndex}},\"amsi\":{{ActiveMashStepIndex}},\"masn\":\"{{ActiveMashStepName}}\",\"casn\":\"{{ActiveCoolingStepName}}\",\"acssn\":\"{{ActiveCoolingStepSufixName}}\",\"amssn\":\"{{ActiveMashStepSufixName}}\",\"absi\":\"{{ActiveBoilStepIndex}}\",\"absn\":\"{{ActiveBoilStepName}}\",\"ctt\":{{CoolingTargetTemperature}},\"btt\":{{BoilTargetTemperature}},\"ttp\":{{TargetTemperature}},\"st\":{{StartTime}},\"et\":{{EndTime}},\"tn\":{{TimeNow}},\"bs\":{{BrewStarted}},\"tp\":{{Temperature}},\"stp\":{{SpargeTemperature}},\"ct\":{{BoilTemperature}},\"btp\":{{BoilTemperature}},\"atp\":{{AuxOneTemperature}},\"attp\":{{AuxTwoTemperature}},\"atttp\":{{AuxThreeTemperature}},\"axs\": \"{{AuxOneSensor}}\",\"axss\": \"{{AuxTwoSensor}}\",\"axsss\": \"{{AuxThreeSensor}}\",\"pp\":{{PWMPercentage}},\"spp\":{{SpargePWMPercentage}},\"cppt\":{{CoolingPWMPercentage}},\"bppt\":{{BoilPWMPercentage}},\"es\":{{EnableSparge}},\"eb\":{{EnableBoilKettle}},\"stt\":{{SpargeTargetTemperature}},\"sl\":{{StepLocked}},\"po\":{{PumpOn}},\"pir\":{{PumpIsResting}},\"tns\":{{TimeNotSet}},\"tu\": \"{{TempUnit}}\",\"v\": \"{{Version}}\",\"c\": \"{{Count}}\",\"bpp\":{{BoilPowerPercentage}},\"bfid\":\"{{BrewfatherId}}\",\"bfkey\":\"{{BrewfatherKey}}\"  }";

ActiveStatus::ActiveStatus(AsyncWebServer *server, FS *fs) : _fs(fs),
                                                             _webSocketStatus("/ws/brew/status")
{
    ActiveCoolingStepIndex = -1;
    ActiveMashStepIndex = -1;
    server->addHandler(&_webSocketStatus);
    server->on("/rest/getLogTemperature", HTTP_GET, std::bind(&ActiveStatus::GetLogTemperature, this, std::placeholders::_1));
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
                ActiveMashStepSufixName = _activeStatus["active_cooling_step_sufix_name"] | "";
                ActiveCoolingStepIndex = _activeStatus["active_cooling_step_index"];
                ActiveCoolingStepName = _activeStatus["active_cooling_step_name"] | "";
                ActiveCoolingStepSufixName = _activeStatus["active_mash_step_sufix_name"] | "";
                ActiveBoilStepIndex = _activeStatus["active_boil_step_index"] | "";
                ActiveBoilStepName = _activeStatus["active_boil_step_name"] | "";
                BoilTime = _activeStatus["boil_time"];
                BoilTargetTemperature = _activeStatus["boil_target_temperature"];
                CoolingTargetTemperature = _activeStatus["cooling_target_temperature"];
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

String ActiveStatus::GetJson()
{
    String active_status = FPSTR(ACTIVE_STATUS);
    active_status.replace("{{ActiveStep}}", String(ActiveStep));
    active_status.replace("{{ActiveMashStepIndex}}", String(ActiveMashStepIndex));
    active_status.replace("{{ActiveMashStepName}}", String(ActiveMashStepName));
    active_status.replace("{{ActiveMashStepSufixName}}", String(ActiveMashStepSufixName));
    active_status.replace("{{ActiveCoolingStepIndex}}", String(ActiveCoolingStepIndex));
    active_status.replace("{{ActiveCoolingStepName}}", String(ActiveCoolingStepName));
    active_status.replace("{{ActiveCoolingStepSufixName}}", String(ActiveCoolingStepSufixName));
    active_status.replace("{{ActiveBoilStepIndex}}", String(ActiveBoilStepIndex));
    active_status.replace("{{ActiveBoilStepName}}", String(ActiveBoilStepName));
    active_status.replace("{{BoilTargetTemperature}}", String(BoilTargetTemperature));
    active_status.replace("{{TargetTemperature}}", String(TargetTemperature));
    active_status.replace("{{StartTime}}", String(StartTime));
    active_status.replace("{{EndTime}}", String(EndTime));
    active_status.replace("{{TimeNow}}", String(TimeNow));
    active_status.replace("{{BrewStarted}}", String(BrewStarted));
    active_status.replace("{{Temperature}}", String(Temperature));
    active_status.replace("{{SpargeTemperature}}", String(SpargeTemperature));
    active_status.replace("{{BoilTemperature}}", String(BoilTemperature));
    active_status.replace("{{CoolingTemperature}}", String(CoolingTemperature));
    active_status.replace("{{CoolingTargetTemperature}}", String(CoolingTargetTemperature));
    active_status.replace("{{AuxOneTemperature}}", String(AuxOneTemperature));
    active_status.replace("{{AuxTwoTemperature}}", String(AuxTwoTemperature));
    active_status.replace("{{AuxThreeTemperature}}", String(AuxThreeTemperature));
    active_status.replace("{{AuxOneSensor}}", AuxOneSensor);
    active_status.replace("{{AuxTwoSensor}}", AuxTwoSensor);
    active_status.replace("{{AuxThreeSensor}}", AuxThreeSensor);
    active_status.replace("{{PWMPercentage}}", String(PWMPercentage, 2));
    active_status.replace("{{SpargePWMPercentage}}", String(SpargePWMPercentage, 2));
    active_status.replace("{{BoilPWMPercentage}}", String(BoilPWMPercentage, 2));
    active_status.replace("{{CoolingPWMPercentage}}", String(CoolingPWMPercentage, 2));
    active_status.replace("{{EnableSparge}}", String(EnableSparge));
    active_status.replace("{{EnableBoilKettle}}", String(EnableBoilKettle));
    active_status.replace("{{SpargeTargetTemperature}}", String(SpargeTargetTemperature));
    active_status.replace("{{StepLocked}}", String(StepLocked));
    active_status.replace("{{PumpOn}}", String(PumpOn));
    active_status.replace("{{PumpIsResting}}", String(PumpIsResting));
    active_status.replace("{{TimeNotSet}}", String(TimeNotSet));
    active_status.replace("{{TempUnit}}", TempUnit);
    active_status.replace("{{Version}}", String(Version));
    active_status.replace("{{Count}}", Count);
    active_status.replace("{{BoilPowerPercentage}}", String(BoilPowerPercentage));
    active_status.replace("{{BrewfatherId}}", BrewfatherId);
    active_status.replace("{{BrewfatherKey}}", BrewfatherKey);
    return active_status;
}

void ActiveStatus::RestartLogTemperature()
{
    SPIFFS.remove("/temps.json");
    logStarted = false;
}

void ActiveStatus::LogTemperature()
{
    File tempLog = SPIFFS.open("/temps.json", "a");
    if (logStarted)
        tempLog.print(",");

    logStarted = true;
    tempLog.print("{\"ttp\":");
    tempLog.print(TargetTemperature);
    tempLog.print(",\"tp\":");
    tempLog.print(Temperature);
    tempLog.print(",\"stt\":");
    tempLog.print(SpargeTargetTemperature);
    tempLog.print(",\"st\":");
    tempLog.print(SpargeTargetTemperature);
    tempLog.print(",\"time\":");
    tempLog.print(TimeNow);
    tempLog.print("}");
    tempLog.close();
}

void ActiveStatus::GetLogTemperature(AsyncWebServerRequest *request)
{
    request->send(SPIFFS, "/temps.json", "text/plain", false);
}

void ActiveStatus::SaveActiveStatusLoop()
{
    if ((now() - lastWriteWebSocket > 1))
    {
        lastWriteWebSocket = now();
        _webSocketStatus.textAll(GetJson());
    }

    if ((!BrewStarted) || (now() - lastWrite < 60))
        return;

    LogTemperature();

    TimeNow = now();

    SaveActiveStatus();
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
    ActiveCoolingStepIndex = -1;
    ActiveCoolingStepName = "";
    ActiveCoolingStepSufixName = "";
    ActiveBoilStepIndex = activeBoilStepIndex;
    ActiveBoilStepName = "";
    BoilTime = boilTime;
    BoilTargetTemperature = boilTargetTemperature;
    CoolingTargetTemperature = 0;
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
    StartBoilCounter = false;
    BoilPowerPercentage = 0;
    Count = "00:00:00";

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
    _activeStatus["active_cooling_step_index"] = ActiveCoolingStepIndex;
    _activeStatus["active_cooling_step_name"] = ActiveCoolingStepName;
    _activeStatus["active_cooling_step_sufix_name"] = ActiveCoolingStepSufixName;
    _activeStatus["active_boil_step_index"] = ActiveBoilStepIndex;
    _activeStatus["active_boil_step_name"] = ActiveBoilStepName;
    _activeStatus["boil_time"] = BoilTime;
    _activeStatus["boil_target_temperature"] = BoilTargetTemperature;
    _activeStatus["cooling_target_temperature"] = CoolingTargetTemperature;
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
    if (temps.Cooling >= 0)
        CoolingTemperature = temps.Cooling;
    if (temps.AuxOne >= 0)
        AuxOneTemperature = temps.AuxOne;
    if (temps.AuxTwo >= 0)
        AuxTwoTemperature = temps.AuxTwo;
    if (temps.AuxThree >= 0)
        AuxThreeTemperature = temps.AuxThree;
}