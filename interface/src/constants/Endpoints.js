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

export const GET_ACTIVE_STATUS = ENDPOINT_ROOT + "getactivestatus"
export const START_BREW = ENDPOINT_ROOT + "startbrew"
export const STOP_BREW = ENDPOINT_ROOT + "stopbrew"
export const NEXT_STEP_BREW = ENDPOINT_ROOT + "nextstepbrew"
export const UNLOCK_STEP_BREW = ENDPOINT_ROOT + "unlockbrew"
export const RESUME_BREW = ENDPOINT_ROOT + "resumebrew"
export const CHANGE_BOIL_PERCENTAGE = ENDPOINT_ROOT + "changeboilpercentage"
export const START_BOIL = ENDPOINT_ROOT + "startboil"
export const START_TUNING = ENDPOINT_ROOT + "starttuning"
export const BREW_ENDPOINT = ENDPOINT_ROOT + 'brew';
export const START_PUMP = ENDPOINT_ROOT + 'startpump';
export const STOP_PUMP = ENDPOINT_ROOT + 'stoppump';
export const GET_SENSORS = ENDPOINT_ROOT + 'getsensors';

export const ExecuteRestCall = (url, method, callback, callbackError, props) => {
  fetch(url, {
    method: method,
    headers: {
      'Accept': 'application/json',
      'Content-Type': 'application/json',
    },
  }).then(response => {
    if (response.ok) {
      response.json()
        .catch(() => {
          if (props != undefined && props.enqueueSnackbar != undefined)
            props.enqueueSnackbar("Invalid json result", { variant: 'error', autoHideDuration: 2000, });
        })
        .then(json => callback(json));
      return;
    }
    if (props != undefined && props.enqueueSnackbar != undefined)
      response.json().then(json => props.enqueueSnackbar(json.message, { variant: 'info', autoHideDuration: 2000, }));
  }).catch(error => {
    if (props != undefined && props.enqueueSnackbar != undefined)
      props.enqueueSnackbar("Problem getting resource: " + error.message, { variant: 'error', autoHideDuration: 2000, });
    if (callbackError) {
      callbackError()
    }
  });
}