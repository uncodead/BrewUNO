import React, { Component } from 'react';
import PropTypes from 'prop-types';
import FormGroup from '@material-ui/core/FormGroup';
import InputAdornment from '@material-ui/core/InputAdornment';
import Button from '@material-ui/core/Button';
import Typography from '@material-ui/core/Typography';
import LinearProgress from '@material-ui/core/LinearProgress';
import { ValidatorForm, TextValidator } from 'react-material-ui-form-validator';

class BrewSettingsForm extends Component {
  render() {
    const { classes, formRef, brewSettingsFetched, brewSettings, errorMessage, handleValueChange, onSubmit, onReset } = this.props;

    return (
      <div>
        {
          !this.props.brewSettingsFetched ?
            <div>
              <LinearProgress />
              <Typography>
                Loading...
              </Typography>
            </div>
            : brewSettings ?
              <ValidatorForm onSubmit={onSubmit} ref="BrewSettingsForm">
                <TextValidator
                  name="boilTemperature"
                  validators={['required']}
                  label="Boil temperature"
                  type="number"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                  value={brewSettings.boilTemperature}
                  onChange={handleValueChange("boilTemperature")}
                  errorMessages={['this field is required']}
                />
                <TextValidator
                  name="boilPowerPercentage"
                  validators={['required']}
                  label="Boil Power %"
                  type="number"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">%</InputAdornment> }}
                  value={brewSettings.boilPowerPercentage}
                  onChange={handleValueChange("boilPowerPercentage")}
                  errorMessages={['this field is required']}
                />
                <TextValidator
                  name="boilTime"
                  validators={['required']}
                  label="Boil time"
                  type="number"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">min</InputAdornment> }}
                  value={brewSettings.boilTime}
                  onChange={handleValueChange("boilTime")}
                  errorMessages={['this field is required']}
                />
                <TextValidator
                  name="sampleTime"
                  validators={['required']}
                  label="Sample Time PID"
                  type="number"
                  fullWidth
                  InputProps={{ endAdornment: <InputAdornment position="start">sec</InputAdornment> }}
                  value={brewSettings.sampleTime}
                  onChange={handleValueChange("sampleTime")}
                  errorMessages={['this field is required']}
                />
                <TextValidator
                  name="kp"
                  label="kP"
                  type="number"
                  validators={['required']}
                  fullWidth
                  value={brewSettings.kP}
                  onChange={handleValueChange("kP")}
                  errorMessages={['this field is required']}
                />
                <TextValidator
                  name="ki"
                  label="kI"
                  type="number"
                  validators={['required']}
                  fullWidth
                  value={brewSettings.kI}
                  onChange={handleValueChange("kI")}
                  errorMessages={['this field is required']}
                />
                <TextValidator
                  name="kd"
                  label="kD"
                  type="number"
                  validators={['required']}
                  fullWidth
                  value={brewSettings.kD}
                  onChange={handleValueChange("kD")}
                  errorMessages={['this field is required']}
                />
                <Button variant="raised" color="primary" type="submit">
                  Save
              </Button>
              </ValidatorForm>
              :
              <div>
                <Typography>
                  {errorMessage}
                </Typography>
              </div>
        }
      </div>
    )
  }
}

BrewSettingsForm.propTypes = {
  brewSettingsFetched: PropTypes.bool.isRequired,
  brewSettings: PropTypes.object,
  errorMessage: PropTypes.string,
  onSubmit: PropTypes.func.isRequired,
  handleValueChange: PropTypes.func.isRequired,
};

export default BrewSettingsForm;