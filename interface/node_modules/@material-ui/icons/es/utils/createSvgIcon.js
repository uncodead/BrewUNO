import React from 'react';
import pure from 'recompose/pure';
import SvgIcon from '@material-ui/core/SvgIcon';

function createSvgIcon(path, displayName) {
  let Icon = props => React.createElement(SvgIcon, props, path);

  Icon.displayName = displayName;
  Icon = pure(Icon);
  Icon.muiName = 'SvgIcon';
  return Icon;
}

;
export default createSvgIcon;