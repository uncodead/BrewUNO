export const getDateTime = (seconds) => {
  var date = new Date(0);
  date.setUTCSeconds(seconds);
  return date;
}

export const pad = (n, width, z) => {
  z = z || '0';
  n = n + '';
  return n.length >= width ? n : new Array(width - n.length + 1).join(z) + n;
}

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