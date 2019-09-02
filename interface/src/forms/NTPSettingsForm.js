import React from 'react';
import PropTypes from 'prop-types';

import { withStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import LinearProgress from '@material-ui/core/LinearProgress';
import { TextValidator, ValidatorForm } from 'react-material-ui-form-validator';
import Typography from '@material-ui/core/Typography';

import isIP from '../validators/isIP';
import isHostname from '../validators/isHostname';
import or from '../validators/or';

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
  button: {
    marginRight: theme.spacing.unit * 2,
    marginTop: theme.spacing.unit * 2,
  }
});

class NTPSettingsForm extends React.Component {

  componentWillMount() {
    ValidatorForm.addValidationRule('isIPOrHostname', or(isIP, isHostname));
  }

  render() {
    const { classes, ntpSettingsFetched, ntpSettings, errorMessage, handleValueChange, onSubmit, onReset } = this.props;
    return (
      <div>
        {
          !ntpSettingsFetched ?
            <div className={classes.loadingSettings}>
              <LinearProgress className={classes.loadingSettingsDetails} />
              <Typography variant="display1" className={classes.loadingSettingsDetails}>
                <IntText text="Loading" />...
           </Typography>
            </div>
            : ntpSettings ?
              <ValidatorForm onSubmit={onSubmit}>
                <TextValidator
                  validators={['required', 'isIPOrHostname']}
                  errorMessages={[<IntText text="NTPSettings.ServerRequired" />, <IntText text="NTPSettings.NotValidIPAddress" />]}
                  name="server"
                  label={<IntText text="Server" />}
                  className={classes.textField}
                  value={ntpSettings.server}
                  onChange={handleValueChange('server')}
                  margin="normal"
                />
                <TextValidator
                  validators={['required', 'isNumber', 'minNumber:60', 'maxNumber:86400']}
                  errorMessages={[<IntText text="NTPSettings.IntervalRequired" />, <IntText text="NTPSettings.IntervalMustBeNumber" />, <IntText text="NTPSettings.MustBeLeast60" />, <IntText text="NTPSettings.MustNotBeMoreThan86400" />]}
                  name="interval"
                  label={<IntText text="NTPSettings.IntervalSeconds" />}
                  className={classes.textField}
                  value={ntpSettings.interval}
                  type="number"
                  onChange={handleValueChange('interval')}
                  margin="normal"
                />
                <Button variant="raised" color="secondary" className={classes.button} type="submit">
                  <IntText text="Save" />
                </Button>
                <Button variant="raised" color="secondary" className={classes.button} onClick={onReset}>
                  <IntText text="Reset" />
                </Button>
              </ValidatorForm>
              :
              <div className={classes.loadingSettings}>
                <Typography variant="display1" className={classes.loadingSettingsDetails}>
                  {errorMessage}
                </Typography>
                <Button variant="raised" color="secondary" className={classes.button} onClick={onReset}>
                  <IntText text="Reset" />
                </Button>
              </div>
        }
      </div>
    );
  }
}

NTPSettingsForm.propTypes = {
  classes: PropTypes.object.isRequired,
  ntpSettingsFetched: PropTypes.bool.isRequired,
  ntpSettings: PropTypes.object,
  errorMessage: PropTypes.string,
  onSubmit: PropTypes.func.isRequired,
  onReset: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
};

export default withStyles(styles)(NTPSettingsForm);
