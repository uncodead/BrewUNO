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