import React, { Component } from 'react';
import { restComponent } from '../components/RestComponent';
import { withStyles } from '@material-ui/core/styles';

import SectionContent from '../components/SectionContent';

import Grid from '@material-ui/core/Grid';
import TextField from '@material-ui/core/TextField';
import InputAdornment from '@material-ui/core/InputAdornment';
import Button from '@material-ui/core/Button';
import AddIcon from '@material-ui/icons/Add';

import MashStepComponent from '../components/MashStepComponent'


const styles = theme => ({
  container: {
    display: 'flex',
    flexWrap: 'wrap',
  },
  textField: {
    marginLeft: theme.spacing.unit,
    marginRight: theme.spacing.unit,
    width: 200,
  },
  button: {
    margin: theme.spacing.unit + 15,
  },
});

class BrewSettings extends Component {
  state = {

  };

  handleChange = name => event => {
    this.setState({
      [name]: event.target.value,
    });
  };
  render() {
    const { classes } = this.props;

    return (
      <SectionContent title="Brew Settings">
        <Grid item xs={6}>
          <form className={classes.container} noValidate autoComplete="off">
            <TextField
              id="step"
              required
              label="Step"
              className={classes.textField}
              margin="normal"
            />
            <TextField
              id="temperature"
              required
              label="Temperature"
              className={classes.textField}
              InputProps={{
                endAdornment: <InputAdornment position="start">ºC</InputAdornment>,
              }}
              margin="normal"
            />
            <TextField
              required
              id="standard-required"
              label="Time"
              className={classes.textField}
              margin="normal"
              InputProps={{
                endAdornment: <InputAdornment position="start">min</InputAdornment>,
              }}
            />
            <Button mini variant="fab" color="primary" className={classes.button}>
              <AddIcon />
            </Button>
          </form>
        </Grid>
        <Grid item xs={6}>
        <MashStepComponent />
        </Grid>
      </SectionContent>
    )
  }
}

export default withStyles(styles)(BrewSettings);
