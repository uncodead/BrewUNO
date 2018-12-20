import React, { Component } from 'react';
import FormGroup from '@material-ui/core/FormGroup';
import InputAdornment from '@material-ui/core/InputAdornment';
import Button from '@material-ui/core/Button';
import TextField from '@material-ui/core/TextField';

class BrewSettingsForm extends Component {
  constructor() {
    super()

    this.state = {
      boilPercent: 90,
      kP: 100,
      kI: 100,
      kD: 100
    }
  }
  render() {
    return (
      <FormGroup>
        <TextField required label="Boil %" type="number" margin="normal" fullWidth
          InputProps={{ endAdornment: <InputAdornment position="start">%</InputAdornment> }}
          value={this.state.boilPercent} onChange={this.handeTimeChange}
        />
        <TextField required label="kP" type="number" margin="normal" fullWidth
          value={this.state.kP} onChange={this.handeTimeChange}
        />
        <TextField required label="kI" type="number" margin="normal" fullWidth
          value={this.state.kI} onChange={this.handeTimeChange}
        />
        <TextField required label="kD" type="number" margin="normal" fullWidth
          value={this.state.kD} onChange={this.handeTimeChange}
        />
        <Button variant="contained" color="primary" onClick={this.addItem}>Save</Button>
      </FormGroup>
    )
  }
}

export default BrewSettingsForm;