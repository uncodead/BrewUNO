import React, { Fragment } from 'react';
import PropTypes from 'prop-types';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';
import MenuItem from '@material-ui/core/MenuItem';

import { TextValidator, ValidatorForm, SelectValidator } from 'react-material-ui-form-validator';

import { isAPEnabled } from '../constants/WiFiAPModes';
import PasswordValidator from '../components/PasswordValidator';

import IntText from '../components/IntText'

const styles = theme => ({
  loadingSettings: {
    margin: theme.spacing.unit,
  },
  loadingSettingsDetails: {
    margin: theme.spacing.unit * 4,
    textAlign: "center"
  },
  textField: {
    width: "100%"
  },
  selectField: {
    width: "100%",
    marginTop: theme.spacing.unit * 2,
    marginBottom: theme.spacing.unit
  },
  button: {
    marginRight: theme.spacing.unit * 2,
    marginTop: theme.spacing.unit * 2,
  }
});

class APSettingsForm extends React.Component {

  render() {
    const { classes, apSettingsFetched, apSettings, errorMessage, handleValueChange, onSubmit, onReset } = this.props;
    return (
      <div>
        {
          !apSettingsFetched ?

            <div className={classes.loadingSettings}>
              <LinearProgress className={classes.loadingSettingsDetails} />
              <Typography variant="display1" className={classes.loadingSettingsDetails}>
                {<IntText text="Loading" />}...
           </Typography>
            </div>

            : apSettings ?

              <ValidatorForm onSubmit={onSubmit} ref="APSettingsForm">

                <SelectValidator name="provision_mode" label={<IntText text="APSettings.ProvideAccessPoint" />} value={apSettings.provision_mode} className={classes.selectField}
                  onChange={handleValueChange('provision_mode')}>
                  <MenuItem value={0}>{<IntText text="Always" />}</MenuItem>
                  <MenuItem value={1}>{<IntText text="APSettings.WhenWiFiDisconnected" />}</MenuItem>
                  <MenuItem value={2}>{<IntText text="Never" />}</MenuItem>
                </SelectValidator>

                {
                  isAPEnabled(apSettings.provision_mode) &&
                  <Fragment>
                    <TextValidator
                      validators={['required', 'matchRegexp:^.{0,32}$']}
                      errorMessages={[<IntText text="APSettings.AccessPointSSIDRequired" />, <IntText text="APSettings.AccessPointSSIDMustBe32" />]}
                      name="ssid"
                      label={<IntText text="APSettings.AccessPointSSID" />}
                      className={classes.textField}
                      value={apSettings.ssid}
                      onChange={handleValueChange('ssid')}
                      margin="normal"
                    />
                    <PasswordValidator
                      validators={['required', 'matchRegexp:^.{0,64}$']}
                      errorMessages={[<IntText text="APSettings.AccessPointPasswordRequired" />, <IntText text="APSettings.AccessPointPasswordMustBe64" />]}
                      name="password"
                      label={<IntText text="APSettings.AccessPointPassword" />}
                      className={classes.textField}
                      value={apSettings.password}
                      onChange={handleValueChange('password')}
                      margin="normal"
                    />
                  </Fragment>
                }

                <Button variant="raised" color="secondary" className={classes.button} type="submit">
                  {<IntText text="Save" />}
                </Button>
                <Button variant="raised" color="secondary" className={classes.button} onClick={onReset}>
                  {<IntText text="Reset" />}
                </Button>

              </ValidatorForm>

              :

              <div className={classes.loadingSettings}>
                <Typography variant="display1" className={classes.loadingSettingsDetails}>
                  {errorMessage}
                </Typography>
                <Button variant="raised" color="secondary" className={classes.button} onClick={onReset}>
                  {<IntText text="Reset" />}
                </Button>
              </div>
        }
      </div>
    );
  }
}

APSettingsForm.propTypes = {
  classes: PropTypes.object.isRequired,
  apSettingsFetched: PropTypes.bool.isRequired,
  apSettings: PropTypes.object,
  errorMessage: PropTypes.string,
  onSubmit: PropTypes.func.isRequired,
  onReset: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired
};

export default withStyles(styles)(APSettingsForm);
