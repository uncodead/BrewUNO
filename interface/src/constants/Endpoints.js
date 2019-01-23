const ENDPOINT_ROOT = process.env.REACT_APP_ENDPOINT_ROOT;

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
export const GET_ACTIVE_STATUS = ENDPOINT_ROOT + "getActiveStatus"
export const START_BREW = ENDPOINT_ROOT + "startBrew"
export const STOP_BREW = ENDPOINT_ROOT + "stopBrew"
export const NEXT_STEP_BREW = ENDPOINT_ROOT + "nextStepBrew"

export const BREW_ENDPOINT = ENDPOINT_ROOT + 'brew';


export const ExecuteRestCall = (url, method, callback, callbackError, props) => {
  fetch(url, {
    method: method,
    headers: {
      'Accept': 'application/json',
      'Content-Type': 'application/json',
    },
  }).then(response => {
    if (response.ok) {
      response.json().then(json => {
        callback(json)
      });
      return;
    }
    throw Error(response.status);
  }).catch(error => {
    props.raiseNotification("Problem getting resource: " + error.message);
    if (callbackError){
      callbackError
    }
  });
}