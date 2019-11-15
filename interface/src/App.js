import React, { Component } from 'react';
import AppRouting from './AppRouting';
import CssBaseline from '@material-ui/core/CssBaseline';
import { SnackbarProvider } from 'notistack';
import JssProvider from 'react-jss/lib/JssProvider';
import { create } from 'jss';
import {
  MuiThemeProvider,
  createMuiTheme,
  createGenerateClassName,
  jssPreset,
} from '@material-ui/core/styles';
import { ExecuteRestCall } from './components/Utils'
import { BREW_SETTINGS_ENDPOINT } from './constants/Endpoints';
import IntText from "./components/IntText"
import Cookies from 'js-cookie';

const theme = createMuiTheme({
  palette: {
    type: 'dark',
    primary: {
      main: '#313131',
      light: '#616161',
      dark: '#424242',
      contrastText: '#9E9E9E',
    },
    secondary: {
      light: '#616161',
      main: '#5c6bc0',
      // dark: will be calculated from palette.secondary.main,
      contrastText: '#fff',
    },
    overrides: {
      MuiSlider: {
        track: { backgroundColor: 'red' },
        thumb: { backgroundColor: 'red' },
      },
    }
  },
});

const styles = {
  success: { backgroundColor: 'purple' },
  error: { backgroundColor: 'blue' },
  warning: { backgroundColor: 'green' },
  info: { backgroundColor: 'yellow' },
};

// JSS instance
const jss = create(jssPreset());

// Class name generator.
const generateClassName = createGenerateClassName();


class App extends Component {
  constructor(props) {
    super(props)
    this.child = React.createRef();
    setTimeout(function () { //Start the timer
      ExecuteRestCall(BREW_SETTINGS_ENDPOINT, 'GET', json => {
        this.child.current.SetText(json.lg)
      })
    }.bind(this), 4000)
    Cookies.remove('mashSettings')
    Cookies.remove('boilSettings')
    Cookies.remove('status')
  }

  render() {
    return (
      <JssProvider jss={jss} generateClassName={generateClassName}>
        <MuiThemeProvider theme={theme}>
          <SnackbarProvider maxSnack={5} classes={{
            variantSuccess: styles.success,
            variantError: styles.error,
            variantWarning: styles.warning,
            variantInfo: styles.info
          }}>
            <IntText ref={this.child} />
            <CssBaseline />
            <AppRouting />
          </SnackbarProvider>
        </MuiThemeProvider>
      </JssProvider>
    )
  }
}

export default App
