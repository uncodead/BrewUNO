const ENDPOINT_ROOT = calculateEndpointRoot("/rest/");
const WS_ENDPOINT_ROOT = calculateWebSocketRoot("/ws/");

function calculateEndpointRoot(endpointPath) {
    const httpRoot = process.env.REACT_APP_ENDPOINT_ROOT;
    if (httpRoot) {
        return httpRoot + endpointPath;
    }
    const location = window.location;
    return location.protocol + "//" + location.host + endpointPath;
}

function calculateWebSocketRoot(webSocketPath) {
    const webSocketRoot = process.env.REACT_APP_WS_ENDPOINT_ROOT;
    if (webSocketRoot) {
        return webSocketRoot + webSocketPath;
    }
    const location = window.location;
    const webProtocol = location.protocol === "https:" ? "wss:" : "ws:";
    return webProtocol + "//" + location.host + webSocketPath;
}


export const NTP_STATUS_ENDPOINT = ENDPOINT_ROOT + "ntpStatus";
export const NTP_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "ntpSettings";
export const AP_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "apSettings";
export const AP_STATUS_ENDPOINT = ENDPOINT_ROOT + "apStatus";
export const SCAN_NETWORKS_ENDPOINT = ENDPOINT_ROOT + "scanNetworks";
export const LIST_NETWORKS_ENDPOINT = ENDPOINT_ROOT + "listNetworks";
export const WIFI_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "wifiSettings";
export const BREW_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "brewSettings";
export const WIFI_STATUS_ENDPOINT = ENDPOINT_ROOT + "wifiStatus";
export const OTA_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "otaSettings";
export const SAVE_MASH_SETTINGS_SERVICE_PATH = ENDPOINT_ROOT + "saveMashSettings"
export const GET_MASH_SETTINGS_SERVICE_PATH = ENDPOINT_ROOT + "getMashSettings"
export const SAVE_BOIL_SETTINGS_SERVICE_PATH = ENDPOINT_ROOT + "saveBoilSettings"
export const GET_BOIL_SETTINGS_SERVICE_PATH = ENDPOINT_ROOT + "getBoilSettings"
export const SAVE_COOLING_SETTINGS_SERVICE_PATH = ENDPOINT_ROOT + "saveCoolingSettings"
export const GET_COOLING_SETTINGS_SERVICE_PATH = ENDPOINT_ROOT + "getCoolingSettings"
export const GET_ACTIVE_STATUS = ENDPOINT_ROOT + "getactivestatus"
export const START_BREW = ENDPOINT_ROOT + "startbrew"
export const STOP_BREW = ENDPOINT_ROOT + "stopbrew"
export const NEXT_STEP_BREW = ENDPOINT_ROOT + "nextstepbrew"
export const UNLOCK_STEP_BREW = ENDPOINT_ROOT + "unlockbrew"
export const RESUME_BREW = ENDPOINT_ROOT + "resumebrew"
export const PAUSE_BREW = ENDPOINT_ROOT + "pausebrew"
export const CHANGE_BOIL_PERCENTAGE = ENDPOINT_ROOT + "changeboilpercentage"
export const START_BOIL = ENDPOINT_ROOT + "startboil"
export const START_BOIL_COUNTER = ENDPOINT_ROOT + "startboilcounter"
export const START_TUNING = ENDPOINT_ROOT + "starttuning"
export const BREW_ENDPOINT = ENDPOINT_ROOT + 'brew';
export const START_PUMP = ENDPOINT_ROOT + 'startpump';
export const STOP_PUMP = ENDPOINT_ROOT + 'stoppump';
export const GET_SENSORS = ENDPOINT_ROOT + 'getsensors';
export const START_ANTICAVITATION = ENDPOINT_ROOT + 'startanticavitation';
export const GET_LANGUAGES = ENDPOINT_ROOT + 'getlang';
export const GET_LOGS_TEMPERATURES = ENDPOINT_ROOT + 'getLogTemperature';
export const GET_STATUS_WS = WS_ENDPOINT_ROOT + 'brew/status';
export const ACTIVATE = ENDPOINT_ROOT + 'activate';
export const REACTIVATE = ENDPOINT_ROOT + 'reactivate';
export const BREWFATHERAPIURL = "https://brewuno.com/api/brewfather/v1/recipes"; 