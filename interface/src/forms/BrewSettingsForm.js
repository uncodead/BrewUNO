import React, { Component } from 'react';
import { withStyles } from '@material-ui/core/styles';
import PropTypes from 'prop-types';
import InputAdornment from '@material-ui/core/InputAdornment';
import Button from '@material-ui/core/Button';
import Typography from '@material-ui/core/Typography';
import LinearProgress from '@material-ui/core/LinearProgress';
import { ValidatorForm, TextValidator } from 'react-material-ui-form-validator';
import MenuItem from '@material-ui/core/MenuItem';
import Select from '@material-ui/core/Select';
import FormControlLabel from '@material-ui/core/FormControlLabel';
import Switch from '@material-ui/core/Switch';
import { Grid, Paper } from '@material-ui/core';
import IntText from '../components/IntText'

const styles = theme => ({
  root: {
    display: 'flex',
    flexWrap: 'wrap',
    flexGrow: 1,
    width: '100%',
    maxWidth: '900px',
  },
  paper: {
    padding: theme.spacing.unit * 3,
    textAlign: 'left',
  },
  formControl: {
    margin: theme.spacing.unit,
    minWidth: 120,
  },
  formControlTypography: {
    minWidth: '100%'
  }
})

class BrewSettingsForm extends Component {
  constructor(props) {
    super(props)
    this.child = React.createRef();
  }

  render() {
    const { classes, brewSettings, errorMessage, handleValueChange, handleFloatValueChange, handleCheckboxChange, onSubmit } = this.props;

    return (
      <div>
        {
          !this.props.brewSettingsFetched ?
            <div>
              <LinearProgress />
              <Typography>
                <IntText text="Loading" />...
              </Typography>
            </div>
            : brewSettings ?
              <ValidatorForm onSubmit={
                () => {
                  this.child.current.SetText(brewSettings.language)
                  onSubmit()
                }} ref="BrewSettingsForm" className={classes.root}>
                <IntText ref={this.child} />
                <Grid container spacing={16}>

                  <Grid item xs={6}>
                    <Paper className={classes.root}>
                      <Typography className={classes.formControl} color="textSecondary"><IntText text="Language" /></Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.language}
                        onChange={handleValueChange("language")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        <MenuItem value={'en'}><IntText text="English" /></MenuItem>
                        <MenuItem value={'pt-BR'}><IntText text="Portuguese" /></MenuItem>
                        <MenuItem value={'ru-RU'}><IntText text="Russian" /></MenuItem>
                      </Select>
                      <Typography className={classes.formControl} color="textSecondary"><IntText text="TemperatureUnit" /></Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.tempUnit}
                        onChange={handleValueChange("tempUnit")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        <MenuItem value={'C'}><IntText text="Celsius" /></MenuItem>
                        <MenuItem value={'F'}><IntText text="Fahrenheit" /></MenuItem>
                      </Select>

                    </Paper>
                  </Grid>

                  <Grid item xs={6}>
                    <Paper className={classes.root}>
                      <Typography className={classes.formControl} color="textSecondary"><IntText text="MainSensor" /></Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.mainSensor}
                        onChange={handleValueChange("mainSensor")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        {this.props.sensors.map(value => (
                          <MenuItem value={value.address}>{value.address} - {value.value}ºC</MenuItem>
                        ))}
                      </Select>
                      <TextValidator className={classes.formControl}
                        name="MainSensorOffset"
                        validators={['required', 'isFloat']}
                        label={<IntText text="BrewSettings.OffSetCalibration" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                        value={brewSettings.mainSensorOffset}
                        onChange={handleFloatValueChange("mainSensorOffset")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                    </Paper>
                  </Grid>

                  <Grid item xs={6}>
                    <Paper className={classes.root}>
                      <Typography className={classes.formControl} color="textSecondary"><IntText text="BrewSettings.MashBoilPWM" /></Typography>
                      <TextValidator className={classes.formControl}
                        name="MashHeaterPercentage"
                        validators={['required']}
                        label={<IntText text="BrewSettings.MashHeaterPercentage" />}
                        type="number"
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">%</InputAdornment> }}
                        value={brewSettings.mashHeaterPercentage}
                        onChange={handleValueChange("mashHeaterPercentage")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="boilTime"
                        validators={['required']}
                        label={<IntText text="BrewSettings.BoilTime" />}
                        type="number"
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">min</InputAdornment> }}
                        value={brewSettings.boilTime}
                        onChange={handleValueChange("boilTime")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="boilTemperature"
                        validators={['required']}
                        label={<IntText text="BrewSettings.BoilTemperature" />}
                        type="number"
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                        value={brewSettings.boilTemperature}
                        onChange={handleValueChange("boilTemperature")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="boilPowerPercentage"
                        validators={['required']}
                        label={<IntText text="BrewSettings.BoilPowerPercentage" />}
                        type="number"
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">%</InputAdornment> }}
                        value={brewSettings.boilPowerPercentage}
                        onChange={handleValueChange("boilPowerPercentage")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                    </Paper>
                  </Grid>

                  <Grid item xs={6}>
                    <Paper className={classes.root}>
                      <Typography className={classes.formControl} color="textSecondary"><IntText text="SpargeSensor" /></Typography>
                      <div style={{ marginTop: 0, marginLeft: 20, padding: 0 }}>
                        <FormControlLabel
                          control={
                            <Switch
                              checked={brewSettings.enableSparge}
                              value={'enableSparge'}
                              onChange={handleCheckboxChange('enableSparge')}
                              color="secondary"
                              margin
                            />
                          }
                          label={<IntText text="BrewSettings.EnableSparge" />}
                        />
                      </div>
                      <Select className={classes.formControl}
                        value={brewSettings.spargeSensor}
                        onChange={handleValueChange("spargeSensor")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        <option value=''></option>
                        {this.props.sensors.map(value => (
                          <MenuItem value={value.address}>{value.address} - {value.value}ºC</MenuItem>
                        ))}
                      </Select>
                      <TextValidator className={classes.formControl}
                        name="SpargeSensorOffset"
                        validators={['required', 'isFloat']}
                        label={<IntText text="BrewSettings.OffSetCalibration" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                        value={brewSettings.spargeSensorOffset}
                        onChange={handleFloatValueChange("spargeSensorOffset")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="spargePowerPercentage"
                        validators={['required']}
                        label={<IntText text="BrewSettings.SpargePowerPercentage" />}
                        type="number"
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">%</InputAdornment> }}
                        value={brewSettings.spargePowerPercentage}
                        onChange={handleValueChange("spargePowerPercentage")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="spargeTemperature"
                        validators={['required', 'isFloat']}
                        label={<IntText text="BrewSettings.SpargeTemperature" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                        value={brewSettings.spargeTemperature}
                        onChange={handleValueChange("spargeTemperature")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                    </Paper>
                  </Grid>

                  <Grid item xs={6}>
                    <Paper className={classes.root}>
                      <Typography className={classes.formControl} color="textSecondary">PID</Typography>
                      <TextValidator className={classes.formControl}
                        name="kp"
                        label="kP"
                        validators={['required', 'isFloat']}
                        fullWidth
                        value={brewSettings.kP}
                        onChange={handleValueChange("kP")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="ki"
                        label="kI"
                        validators={['required', 'isFloat']}
                        fullWidth
                        value={brewSettings.kI}
                        onChange={handleValueChange("kI")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="kd"
                        label="kD"
                        validators={['required', 'isFloat']}
                        fullWidth
                        value={brewSettings.kD}
                        onChange={handleValueChange("kD")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="pidStart"
                        validators={['required', 'isFloat']}
                        label={<IntText text="BrewSettings.PIDStartAt" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                        value={brewSettings.pidStart}
                        onChange={handleValueChange("pidStart")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <Typography className={classes.formControl} color="textSecondary">{<IntText text="BrewSettings.PumpCycle" />}</Typography>
                      <TextValidator className={classes.formControl}
                        name="pumpRestInterval"
                        validators={['required']}
                        label={<IntText text="BrewSettings.PumpRestInterval" />}
                        type="number"
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">sec</InputAdornment> }}
                        value={brewSettings.pumpRestInterval}
                        onChange={handleValueChange("pumpRestInterval")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="pumpRestTime"
                        validators={['required']}
                        label={<IntText text="BrewSettings.PumpRestTime" />}
                        type="number"
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">sec</InputAdornment> }}
                        value={brewSettings.pumpRestTime}
                        onChange={handleValueChange("pumpRestTime")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                    </Paper>
                  </Grid>

                  <Grid item xs={6}>
                    <Paper className={classes.root}>
                      <Typography className={classes.formControl} color="textSecondary">{<IntText text="BrewSettings.AuxiliarySensor1" />}</Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.auxSensorOne}
                        onChange={handleValueChange("auxSensorOne")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        {this.props.sensors.map(value => (
                          <MenuItem value={value.address}>{value.address} - {value.value}ºC</MenuItem>
                        ))}
                      </Select>
                      <TextValidator className={classes.formControl}
                        name="auxSensorOneOffset"
                        validators={['isFloat']}
                        label={<IntText text="BrewSettings.OffSetCalibration" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                        value={brewSettings.auxSensorOneOffset}
                        onChange={handleFloatValueChange("auxSensorOneOffset")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <Typography className={classes.formControl} color="textSecondary">{<IntText text="BrewSettings.AuxiliarySensor2" />}</Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.auxSensorTwo}
                        onChange={handleValueChange("auxSensorTwo")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        {this.props.sensors.map(value => (
                          <MenuItem value={value.address}>{value.address} - {value.value}ºC</MenuItem>
                        ))}
                      </Select>
                      <TextValidator className={classes.formControl}
                        name="auxSensorTwoOffset"
                        validators={['isFloat']}
                        label={<IntText text="BrewSettings.OffSetCalibration" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                        value={brewSettings.auxSensorTwoOffset}
                        onChange={handleFloatValueChange("auxSensorTwoOffset")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <Typography className={classes.formControl} color="textSecondary">{<IntText text="BrewSettings.AuxiliarySensor3" />}</Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.auxSensorThree}
                        onChange={handleValueChange("auxSensorThree")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        {this.props.sensors.map(value => (
                          <MenuItem value={value.address}>{value.address} - {value.value}ºC</MenuItem>
                        ))}
                      </Select>
                      <TextValidator className={classes.formControl}
                        name="auxSensorThreeOffset"
                        validators={['isFloat']}
                        label={<IntText text="BrewSettings.OffSetCalibration" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                        value={brewSettings.auxSensorThreeOffset}
                        onChange={handleFloatValueChange("auxSensorThreeOffset")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                    </Paper>
                  </Grid>

                </Grid>

                <div style={{ marginTop: 20, marginLeft: 5, padding: 0 }}>
                  <Button variant="raised" color="secondary" type="submit" margin="20">
                    {<IntText text="Save" />}
                  </Button>
                </div>

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
  handleCheckboxChange: PropTypes.func.isRequired,
  handleFloatValueChange: PropTypes.func.isRequired,
};

export default withStyles(styles)(BrewSettingsForm);