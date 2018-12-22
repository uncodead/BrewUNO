import React, { Component } from 'react';
import FormGroup from '@material-ui/core/FormGroup';
import InputAdornment from '@material-ui/core/InputAdornment';
import Button from '@material-ui/core/Button';
import TextField from '@material-ui/core/TextField';
import { ValidatorForm, TextValidator } from 'react-material-ui-form-validator';

class BrewSettingsForm extends Component {
  constructor() {
    super()

    this.state = {
      boilPercent: 90,
      sampleTime: 1,
      kP: 100,
      kI: 100,
      kD: 100
    }
  }

  handleBoilPercent = (e) => {
    this.setState({ boilPercent: e.target.value })
  }

  handlesampleTime = (e) => {
    this.setState({ sampleTime: e.target.value })
  }

  handleKp = (e) => {
    this.setState({ kP: e.target.value })
  }

  handleKi = (e) => {
    this.setState({ kI: e.target.value })
  }

  handleKd = (e) => {
    this.setState({ kD: e.target.value })
  }

  render() {
    return (
      <ValidatorForm ref="form" onSubmit={this.saveSettings} onError={error => console.log(error)}>
        <TextValidator 
          name="boilPercent"
          validators={['required']}  
          label="Boil %" 
          type="number" 
          fullWidth
          InputProps={{ endAdornment: <InputAdornment position="start">%</InputAdornment> }}
          value={this.state.boilPercent} 
          onChange={this.handleBoilPercent}
          errorMessages={['this field is required']}
        />
        <TextValidator 
          name="sampleTime"
          validators={['required']}  
          label="Sample Time PID" 
          type="number" 
          fullWidth
          InputProps={{ endAdornment: <InputAdornment position="start">sec</InputAdornment> }}
          value={this.state.sampleTime} 
          onChange={this.handleSampleTime}
          errorMessages={['this field is required']}
        />
        <TextValidator 
          name="kp"
          label="kP"
          type="number"
          validators={['required']}  
          fullWidth
          value={this.state.kP} 
          onChange={this.handleKp}
          errorMessages={['this field is required']}
        />
        <TextValidator 
          name="ki"
          label="kI"
          type="number"
          validators={['required']}
          fullWidth
          value={this.state.kI}
          onChange={this.handleKi}
          errorMessages={['this field is required']}
        />
        <TextValidator 
          name="kd"
          label="kD"
          type="number"
          validators={['required']}
          fullWidth
          value={this.state.kD}
          onChange={this.handleKd}
          errorMessages={['this field is required']}
        />
        <Button type="submit" variant="contained" fullWidth color="primary" onClick={this.addItem}>Save</Button>
      </ValidatorForm>
    )
  }
}

export default BrewSettingsForm;