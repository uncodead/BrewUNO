import React, { Component } from 'react';
import { withStyles } from '@material-ui/core/styles';


const styles = theme => ({
  root: {
    display: 'flex',
    flexWrap: 'wrap',
  },
  margin: {
    margin: theme.spacing.unit,
  },
  textField: {
    flexBasis: 200,
  },
});

class MashStepComponent extends Component {
  render() {
    return (
      <div>ola</div>
    )
  }
}

export default withStyles(styles)(MashStepComponent);