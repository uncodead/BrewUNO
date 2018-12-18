import React, { Component } from 'react';
import { SortableContainer, SortableElement, arrayMove } from 'react-sortable-hoc';
import { withStyles } from '@material-ui/core/styles';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';
import DeleteIcon from '@material-ui/icons/Delete';
import Switch from '@material-ui/core/Switch';

const styles = theme => ({
  textField: {
    marginLeft: theme.spacing.unit,
    marginRight: theme.spacing.unit,
    width: 200,
  },
});

class OrderedList extends Component {

  constructor(props) {
    super(props)
  }

  onSortEnd = ({ oldIndex, newIndex }) => {
    this.setState({
      items: arrayMove(this.props.items, oldIndex, newIndex),
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
        <Switch checked={value.recirculation} />
        <DeleteIcon />
      </ListItem>
    );

    const SortableList = SortableContainer(({ items }) => {
      return (
        <List>
          <ListItem>
            <ListItemText primary="Name" />
            <ListItemText primary="ºC" />
            <ListItemText primary="'" />
            <ListItemText primary="Recirculation" />
          </ListItem>
          {items.map((value, index) => (
            <SortableItem key={`item-${index}`} index={index} value={value} />
          ))}
        </List>
      );
    });

    return <SortableList items={this.props.items} onSortEnd={this.onSortEnd} />
  }
}

export default withStyles(styles)(OrderedList);