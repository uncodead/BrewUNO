import React from 'react';
import * as Highlight from '../constants/Highlight';
import IntText from '../components/IntText'

export const NTP_TIME_NOT_SET = 0;
export const NTP_TIME_NEEDS_SYNC = 1;
export const NTP_TIME_SET = 2;

export const isSynchronized = ntpStatus => ntpStatus && (ntpStatus.status === NTP_TIME_NEEDS_SYNC || ntpStatus.status === NTP_TIME_SET);

export const ntpStatusHighlight = ntpStatus => {
  switch (ntpStatus.status) {
    case NTP_TIME_SET:
      return Highlight.SUCCESS;
    case NTP_TIME_NEEDS_SYNC:
      return Highlight.WARN;
    case NTP_TIME_NOT_SET:
    default:
      return Highlight.ERROR;
  }
}

export const ntpStatus = ntpStatus => {
  switch (ntpStatus.status) {
    case NTP_TIME_SET:
      return <IntText text="Synchronized" />;
    case NTP_TIME_NEEDS_SYNC:
      return <IntText text="NTPSettings.SynchronizationRequired" />;
    case NTP_TIME_NOT_SET:
      return <IntText text="NTPSettings.TimeNotSet" />;
    default:
      return <IntText text="NTPSettings.Unknown" />;
  }
}
