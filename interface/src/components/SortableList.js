import React, { Component } from 'react';
import { SortableContainer, SortableElement, arrayMove, SortableHandle } from 'react-sortable-hoc';
import { withStyles } from '@material-ui/core/styles';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';
import ListItemSecondaryAction from '@material-ui/core/ListItemSecondaryAction';
import DeleteIcon from '@material-ui/icons/Delete';
import Switch from '@material-ui/core/Switch';
import IconButton from '@material-ui/core/IconButton';
import DragIndicatorIcon from '@material-ui/icons/DragIndicator'
import Typography from '@material-ui/core/Typography'
import Button from "@material-ui/core/Button";

class OrderedList extends Component {
  constructor(props) {
    super(props)
  }

  onSortEnd = ({ oldIndex, newIndex }) => {
    this.props.callbackItemsSorted(arrayMove(this.props.items, oldIndex, newIndex))
  };

  deleteItem = (index) => {
    this.props.callbackItemDeleted(index)
  }

  render() {
    const { classes } = this.props;
    const DragHandle = SortableHandle(() => <DragIndicatorIcon />);
    const SortableItem = SortableElement(({ value, itemIndex }) =>
      <ListItem>
        <DragHandle />
        <ListItemText
          primary={value.name}
          secondary={<Typography>{value.time} mins at {value.temperature} ºC {itemIndex}</Typography>}
        />
        <Switch checked={value.recirculation} />
        <div onClick={() => this.deleteItem(itemIndex)}>
          <IconButton aria-label="Delete">
            <DeleteIcon fontSize="madium" />
          </IconButton>
        </div>
      </ListItem>
    );

    const SortableList = SortableContainer(({ items }) => {
      return (
        <List>
          {items.map((value, index) => (
            <SortableItem key={`item-${index}`} index={index} value={value} itemIndex={index}/>
          ))}
        </List>
      );
    });

    return <SortableList items={this.props.items} onSortEnd={this.onSortEnd} useDragHandle={true} />
  }
}

export default (OrderedList);