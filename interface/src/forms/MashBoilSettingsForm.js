import React, { Component } from 'react';
import FormGroup from '@material-ui/core/FormGroup';
import FormControlLabel from '@material-ui/core/FormControlLabel';
import Button from '@material-ui/core/Button';
import InputAdornment from '@material-ui/core/InputAdornment';
import Switch from '@material-ui/core/Switch';
import TextField from '@material-ui/core/TextField';
import { ValidatorForm, TextValidator } from 'react-material-ui-form-validator';
import { Divider } from '@material-ui/core';


class MashBoilSettingsForm extends Component {
  constructor(props) {
    super(props)

    this.state = {
      name: '',
      temperature: '',
      time: '',
      recirculation: false
    }
  }
  addItem = (event) => {
    this.props.callbackItemAdded(this.state)
    this.setState({name:'',  temperature:'', time:'', recirculation:''})
  }

  handleNameChange = (e) => {
    this.setState({ name: e.target.value })
  }

  handleTemperatureChange = (e) => {
    this.setState({ temperature: e.target.value })
  }

  handeTimeChange = (e) => {
    this.setState({ time: e.target.value })
  }

  handleRecirculationChange = (e, checked) => {
    this.setState({ recirculation: checked })
  }

  render() {
    return (
      <ValidatorForm ref="form" onSubmit={this.addItem} onError={error => console.log(error)}>
        <TextValidator 
          label="Name" 
          fullWidth
          name="name"
          value={this.state.name} 
          onChange={this.handleNameChange} 
          validators={['required']} 
          errorMessages={['this field is required']} />
        {
          !this.props.boil ?
            <TextValidator 
              name="temperature" 
              validators={['required']}  
              label="Temperature" 
              type="number" 
              fullWidth
              InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
              value={this.state.temperature} 
              onChange={this.handleTemperatureChange}
              errorMessages={['this field is required']}
            />
            : null
        }
        <TextValidator 
          name="time" 
          validators={['required']} 
          label="Time" 
          type="number"
          fullWidth
          InputProps={{ endAdornment: <InputAdornment position="start">min</InputAdornment> }}
          value={this.state.time} 
          onChange={this.handeTimeChange}
          errorMessages={['this field is required']}
        />
        {
          !this.props.boil ?
            <FormControlLabel control={<Switch ref="recirculation" checked={this.state.recirculation} onChange={this.handleRecirculationChange} />} label="Recirculation" />
            : null
        }
        <Divider/>
        <Button type="submit" variant="contained" fullWidth color="primary">Add</Button>
      </ValidatorForm>
    )
  }
}

export default (MashBoilSettingsForm);