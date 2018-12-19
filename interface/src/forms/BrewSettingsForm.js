import React, { Component } from 'react';
import FormGroup from '@material-ui/core/FormGroup';
import FormControlLabel from '@material-ui/core/FormControlLabel';
import Button from '@material-ui/core/Button';
import InputAdornment from '@material-ui/core/InputAdornment';
import Switch from '@material-ui/core/Switch';
import TextField from '@material-ui/core/TextField';

class BrewSettingsForm extends Component {
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
  }

  handleNameChange = (e) => {
    this.setState({ name: e.target.value })
  }

  handleTemperatureChange = (e) => {
    this.setState({ temperature: e.target.value })
  }

  handeTimeChange = (e) => {
    this.setState({ time: parseInt(e.target.value) })
  }

  handleRecirculationChange = (e, checked) => {
    this.setState({ recirculation: checked })
  }

  render() {
    return (
      <form>
        <FormGroup>
          <TextField required label="Name" margin="normal" fullWidth
            value={this.state.name} onChange={this.handleNameChange} />
          {
            !this.props.boil ?
              <TextField required label="Temperature" type="number" margin="normal" fullWidth
                InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
                value={this.state.temprerature} onChange={this.handleTemperatureChange}
              />
              : null
          }
          <TextField required label="Time" type="number" margin="normal" fullWidth
            InputProps={{ endAdornment: <InputAdornment position="start">min</InputAdornment> }}
            value={this.state.time} onChange={this.handeTimeChange}
          />
          {
            !this.props.boil ?
              <FormControlLabel control={<Switch ref="recirculation" checked={this.state.recirculation} onChange={this.handleRecirculationChange} />} label="Recirculation" />
              : null
          }
          <Button variant="contained" color="primary" onClick={this.addItem}>Add</Button>
        </FormGroup>
      </form>
    )
  }
}

export default (BrewSettingsForm);