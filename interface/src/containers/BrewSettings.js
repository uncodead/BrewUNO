import React, { Component } from 'react';
import { restComponent } from '../components/RestComponent';
import { withStyles } from '@material-ui/core/styles';
import SectionContent from '../components/SectionContent';
import Grid from '@material-ui/core/Grid';
import TextField from '@material-ui/core/TextField';
import InputAdornment from '@material-ui/core/InputAdornment';
import Button from '@material-ui/core/Button';
import Fab from '@material-ui/core/Fab';
import AddIcon from '@material-ui/icons/Add';
import MashStepComponent from '../components/MashStepComponent'
import { SortableContainer, SortableElement, arrayMove } from 'react-sortable-hoc';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import ListItemText from '@material-ui/core/ListItemText';
import { Divider } from '@material-ui/core';
import Switch from '@material-ui/core/Switch';
import DeleteIcon from '@material-ui/icons/Delete';
import Checkbox from '@material-ui/core/Checkbox';
import FormGroup from '@material-ui/core/FormGroup';
import FormControlLabel from '@material-ui/core/FormControlLabel';

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
});

class BrewSettings extends Component {
  state = {
    items: [
      { index: 0, name: 'Mash In', temperature: 65, time: 5, reciculation: false },
      { index: 1, name: 'Protein Stop', temperature: 68, time: 60, reciculation: true },
      { index: 1, name: 'Sacarification', temperature: 68, time: 60, reciculation: true },
      { index: 1, name: 'Mash out', temperature: 68, time: 60, reciculation: true },
    ],
  };

  onSortEnd = ({ oldIndex, newIndex }) => {
    this.setState({
      items: arrayMove(this.state.items, oldIndex, newIndex),
    });
    console.log(this.state.items)
  };

  render() {
    const { classes } = this.props;

    const SortableItem = SortableElement(({ value }) =>
      <ListItem>
        <ListItemText className={classes.textField} primary={value.name} />
        <ListItemText primary={value.temperature} />
        <ListItemText primary={value.time} />
        <Switch checked={value.reciculation} />
        <DeleteIcon />
      </ListItem>
    );

    const SortableList = SortableContainer(({ items }) => {
      return (
        <List>
          {items.map((value, index) => (
            <SortableItem key={`item-${index}`} index={index} value={value} />
          ))}
        </List>
      );
    });

    return (
      <SectionContent title="Brew Settings">
        <FormGroup>
          <TextField id="step" required label="Step" margin="normal" fullWidth />
          <TextField id="temperature" required label="Temperature" type="number" margin="normal" fullWidth
            InputProps={{ endAdornment: <InputAdornment position="start">ºC</InputAdornment> }}
          />
          <TextField required id="standard-required" label="Time" type="number" margin="normal" fullWidth
            InputProps={{ endAdornment: <InputAdornment position="start">min</InputAdornment> }}
          />
          <FormControlLabel control={<Switch />} label="Recirculation" />
          <Button variant="contained" color="primary">Add</Button>
        </FormGroup>
        <Divider />
        <SortableList items={this.state.items} onSortEnd={this.onSortEnd} />
      </SectionContent>
    )
  }
}

export default withStyles(styles)(BrewSettings);
