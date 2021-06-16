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
    minWidth: '95%'
  },
  formControlTypography: {
    minWidth: '100%'
  }
})
var PaperStyle = {
  background: '#313131'
}

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
                  this.child.current.SetText(brewSettings.lg)
                  onSubmit()
                }} ref="BrewSettingsForm" className={classes.root}>
                <IntText ref={this.child} />
                <Grid container spacing={16}>

                  <Grid item xs={6}>
                    <Paper className={classes.root} style={PaperStyle}>
                      <Typography className={classes.formControl} color="textSecondary"><IntText text="Language" /></Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.lg}
                        onChange={handleValueChange("lg")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        <MenuItem value={'en'}><IntText text="English" /></MenuItem>
                        <MenuItem value={'de'}><IntText text="German" /></MenuItem>
                        <MenuItem value={'pt-BR'}><IntText text="Portuguese" /></MenuItem>
                        <MenuItem value={'ru-RU'}><IntText text="Russian" /></MenuItem>
                        <MenuItem value={'es-ES'}><IntText text="Spanish" /></MenuItem>
                      </Select>
                      <Typography className={classes.formControl} color="textSecondary"><IntText text="TemperatureUnit" /></Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.tu}
                        onChange={handleValueChange("tu")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        <MenuItem value={'C'}><IntText text="Celsius" /></MenuItem>
                        <MenuItem value={'F'}><IntText text="Fahrenheit" /></MenuItem>
                      </Select>
                    </Paper>
                  </Grid>

                  <Grid item xs={6}>
                    <Paper className={classes.root} style={PaperStyle}>
                      <Typography className={classes.formControl} color="textSecondary"><IntText text="MainSensor" /></Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.ms}
                        onChange={handleValueChange("ms")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        {this.props.sensors.map(value => (
                          <MenuItem value={value.address}>{value.address} - {value.value}</MenuItem>
                        ))}
                      </Select>
                      <TextValidator className={classes.formControl}
                        name="mso"
                        validators={['required', 'isFloat']}
                        label={<IntText text="BrewSettings.OffSetCalibration" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start"></InputAdornment> }}
                        value={brewSettings.mso}
                        onChange={handleFloatValueChange("mso")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                    </Paper>
                  </Grid>

                  <Grid item xs={6}>
                    <Paper className={classes.root} style={PaperStyle}>
                      <Typography className={classes.formControl} color="textSecondary"><IntText text="BrewSettings.MashBoilPWM" /></Typography>
                      <TextValidator className={classes.formControl}
                        name="mhp"
                        validators={['required']}
                        label={<IntText text="BrewSettings.MashHeaterPercentage" />}
                        type="number"
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">%</InputAdornment> }}
                        value={brewSettings.mhp}
                        onChange={handleValueChange("mhp")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="bt"
                        validators={['required']}
                        label={<IntText text="BrewSettings.BoilTime" />}
                        type="number"
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">min</InputAdornment> }}
                        value={brewSettings.bt}
                        onChange={handleValueChange("bt")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="bpp"
                        validators={['required']}
                        label={<IntText text="BrewSettings.BoilPowerPercentage" />}
                        type="number"
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">%</InputAdornment> }}
                        value={brewSettings.bpp}
                        onChange={handleValueChange("bpp")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="btemp"
                        validators={['required', 'isFloat']}
                        label={<IntText text="BrewSettings.BoilTemperature" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start"></InputAdornment> }}
                        value={brewSettings.btemp}
                        onChange={handleValueChange("btemp")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                    </Paper>
                  </Grid>

                  <Grid item xs={6}>
                    <Paper className={classes.root} style={PaperStyle}>
                      <Typography className={classes.formControl} color="textSecondary"><IntText text="SpargeSensor" /></Typography>
                      <div style={{ marginTop: 0, marginLeft: 20, padding: 0 }}>
                        <FormControlLabel
                          control={
                            <Switch
                              checked={brewSettings.es}
                              value={'es'}
                              onChange={handleCheckboxChange('es')}
                              color="secondary"
                              margin
                            />
                          }
                          label={<IntText text="BrewSettings.EnableSparge" />}
                        />
                      </div>
                      <Select className={classes.formControl}
                        value={brewSettings.ss}
                        onChange={handleValueChange("ss")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        <option value=''></option>
                        {this.props.sensors.map(value => (
                          <MenuItem value={value.address}>{value.address} - {value.value}</MenuItem>
                        ))}
                      </Select>
                      <TextValidator className={classes.formControl}
                        name="sso"
                        validators={['required', 'isFloat']}
                        label={<IntText text="BrewSettings.OffSetCalibration" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start"></InputAdornment> }}
                        value={brewSettings.sso}
                        onChange={handleFloatValueChange("sso")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="spp"
                        validators={['required']}
                        label={<IntText text="BrewSettings.SpargePowerPercentage" />}
                        type="number"
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">%</InputAdornment> }}
                        value={brewSettings.spp}
                        onChange={handleValueChange("spp")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="st"
                        validators={['required', 'isFloat']}
                        label={<IntText text="BrewSettings.SpargeTemperature" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start"></InputAdornment> }}
                        value={brewSettings.st}
                        onChange={handleValueChange("st")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                    </Paper>
                  </Grid>

                  <Grid item xs={6}>
                    <Paper className={classes.root} style={PaperStyle}>
                      <Typography className={classes.formControl} color="textSecondary"><IntText text="BoilSensor" /></Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.bs}
                        onChange={handleValueChange("bs")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        <option value=''></option>
                        {this.props.sensors.map(value => (
                          <MenuItem value={value.address}>{value.address} - {value.value}</MenuItem>
                        ))}
                      </Select>
                      <TextValidator className={classes.formControl}
                        name="bso"
                        validators={['required', 'isFloat']}
                        label={<IntText text="BrewSettings.OffSetCalibration" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start"></InputAdornment> }}
                        value={brewSettings.bso}
                        onChange={handleFloatValueChange("bso")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <Typography className={classes.formControl}><IntText text="BrewSettings.BoilAttention" /></Typography>
                      <div style={{ marginTop: 0, marginLeft: 20, padding: 0 }}>
                        <FormControlLabel
                          control={
                            <Switch
                              checked={brewSettings.ebk}
                              value={'ebk'}
                              onChange={handleCheckboxChange('ebk')}
                              color="secondary"
                              margin
                            />
                          }
                          label={<IntText text="BrewSettings.EnableBoilKettle" />}
                        />
                      </div>
                    </Paper>
                  </Grid>

                  <Grid item xs={6}>
                    <Paper className={classes.root} style={PaperStyle}>
                      <Typography className={classes.formControl} color="textSecondary">{<IntText text="BrewSettings.PumpCycle" />}</Typography>
                      <div style={{ marginTop: 0, marginLeft: 20, padding: 0 }}>
                        <FormControlLabel
                          control={
                            <Switch
                              checked={brewSettings.ip}
                              value={'ip'}
                              onChange={handleCheckboxChange('ip')}
                              color="secondary"
                              margin
                            />
                          }
                          label={<IntText text="BrewSettings.InvertPump" />}
                        />
                      </div>
                      <Typography className={classes.formControl}><IntText text="BrewSettings.RestartAttention" /></Typography>
                      <TextValidator className={classes.formControl}
                        name="pri"
                        validators={['required']}
                        label={<IntText text="BrewSettings.PumpRestInterval" />}
                        type="number"
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">sec</InputAdornment> }}
                        value={brewSettings.pri}
                        onChange={handleValueChange("pri")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <TextValidator className={classes.formControl}
                        name="prt"
                        validators={['required']}
                        label={<IntText text="BrewSettings.PumpRestTime" />}
                        type="number"
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start">sec</InputAdornment> }}
                        value={brewSettings.prt}
                        onChange={handleValueChange("prt")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                    </Paper>
                  </Grid>

                  <Grid item xs={6}>
                    <Paper className={classes.root} style={PaperStyle}>
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
                        name="ps"
                        validators={['required', 'isFloat']}
                        label={<IntText text="BrewSettings.PIDStartAt" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start"></InputAdornment> }}
                        value={brewSettings.ps}
                        onChange={handleValueChange("ps")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <Typography className={classes.formControl} color="textSecondary"><IntText text="BrewSettings.PCFAddress" /></Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.pa}
                        onChange={handleValueChange("pa")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        <MenuItem value={0}><IntText text="0x20" /></MenuItem>
                        <MenuItem value={1}><IntText text="0x21" /></MenuItem>
                        <MenuItem value={2}><IntText text="0x22" /></MenuItem>
                        <MenuItem value={3}><IntText text="0x23" /></MenuItem>
                        <MenuItem value={4}><IntText text="0x24" /></MenuItem>
                        <MenuItem value={5}><IntText text="0x25" /></MenuItem>
                        <MenuItem value={6}><IntText text="0x26" /></MenuItem>
                        <MenuItem value={7}><IntText text="0x27" /></MenuItem>
                        <MenuItem value={8}><IntText text="0x38" /></MenuItem>
                      </Select>
                      <Typography className={classes.formControl}><IntText text="BrewSettings.RestartAttention" /></Typography>
                    </Paper>
                  </Grid>

                  <Grid item xs={6}>
                    <Paper className={classes.root} style={PaperStyle}>
                      <Typography className={classes.formControl} color="textSecondary">{<IntText text="BrewSettings.AuxiliarySensor1" />}</Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.aso}
                        onChange={handleValueChange("aso")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        <option value=''></option>
                        {this.props.sensors.map(value => (
                          <MenuItem value={value.address}>{value.address} - {value.value}</MenuItem>
                        ))}
                      </Select>
                      <TextValidator className={classes.formControl}
                        name="asoo"
                        validators={['isFloat']}
                        label={<IntText text="BrewSettings.OffSetCalibration" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start"></InputAdornment> }}
                        value={brewSettings.asoo}
                        onChange={handleFloatValueChange("asoo")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <Typography className={classes.formControl} color="textSecondary">{<IntText text="BrewSettings.AuxiliarySensor2" />}</Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.ast}
                        onChange={handleValueChange("ast")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        <option value=''></option>
                        {this.props.sensors.map(value => (
                          <MenuItem value={value.address}>{value.address} - {value.value}</MenuItem>
                        ))}
                      </Select>
                      <TextValidator className={classes.formControl}
                        name="auxSensorTwoOffset"
                        validators={['isFloat']}
                        label={<IntText text="BrewSettings.OffSetCalibration" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start"></InputAdornment> }}
                        value={brewSettings.asto}
                        onChange={handleFloatValueChange("asto")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                      <Typography className={classes.formControl} color="textSecondary">{<IntText text="BrewSettings.AuxiliarySensor3" />}</Typography>
                      <Select className={classes.formControl}
                        value={brewSettings.asth}
                        onChange={handleValueChange("asth")}
                        fullWidth
                        inputProps={{ required: true }}
                      >
                        <option value=''></option>
                        {this.props.sensors.map(value => (
                          <MenuItem value={value.address}>{value.address} - {value.value}</MenuItem>
                        ))}
                      </Select>
                      <TextValidator className={classes.formControl}
                        name="astho"
                        validators={['isFloat']}
                        label={<IntText text="BrewSettings.OffSetCalibration" />}
                        fullWidth
                        InputProps={{ endAdornment: <InputAdornment position="start"></InputAdornment> }}
                        value={brewSettings.astho}
                        onChange={handleFloatValueChange("astho")}
                        errorMessages={[<IntText text="FieldRequired" />]}
                      />
                    </Paper>
                  </Grid>

                </Grid>

                <div style={{ marginTop: 20, }}>
                  <Button variant="raised" fullWidth color="secondary" type="submit">
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