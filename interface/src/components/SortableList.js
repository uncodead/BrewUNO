import React, { Component } from 'react';
import { SortableContainer, SortableElement, arrayMove, SortableHandle } from 'react-sortable-hoc';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';
import DeleteIcon from '@material-ui/icons/Delete';
import Checkbox from '@material-ui/core/Checkbox';
import IconButton from '@material-ui/core/IconButton';
import DragIndicatorIcon from '@material-ui/icons/DragIndicator'
import Typography from '@material-ui/core/Typography'
import FormControlLabel from '@material-ui/core/FormControlLabel';
import Divider from '@material-ui/core/Divider';

class SortableList extends Component {
  onSortEnd = ({ oldIndex, newIndex }) => {
    this.props.callbackItemsSorted(arrayMove(this.props.items, oldIndex, newIndex))
  };

  deleteItem = (index) => {
    this.props.callbackItemDeleted(index)
  }

  getItemText = (item) => {
    if (this.props.boil) {
      return item.a + 'g at ' + item.tm + ' min';
    }
    return item.tm + ' mins at ' + item.t + ' ºC'
  }

  render() {
    const DragHandle = SortableHandle(() => <DragIndicatorIcon />);
    const SortableItem = SortableElement(({ value, itemIndex }) =>
      <List component="nav">
        <ListItem
          selected={
            !this.props.boil && this.props.selectedIndex === itemIndex ||
            this.props.boil && this.props.selectedIndex && this.props.selectedIndex.includes(itemIndex)
          }
        >
          {this.props.dragHandle ? <DragHandle /> : null}
          <ListItemText
            primary={value.n}
            secondary={<Typography>{this.getItemText(value)}
              {this.props.brewDay && !this.props.boil && value.r ? ' (Recirculation) ' : null}
              {this.props.brewDay && !this.props.boil && value.ho ? ' (Heater) ' : null}
              {this.props.brewDay && !this.props.boil && value.fp ? ' (Full Power)' : null}
              {this.props.brewDay && !this.props.boil && value.sl ? ' (Step LOCK)' : null}
            </Typography>}
          />
          {!this.props.brewDay ?
            <IconButton aria-label="Delete"
              onClick={() => this.deleteItem(itemIndex)}>
              <DeleteIcon fontSize="default" />
            </IconButton>
            : null}
        </ListItem>
        {!this.props.brewDay ?
          <ListItem color="black">
            {!this.props.boil ?
              <FormControlLabel
                control={
                  <Checkbox
                    checked={value.r}
                    disabled
                  />
                }
                label="Pump"
              /> : null}
            {!this.props.boil ?
              <FormControlLabel
                control={
                  <Checkbox
                    checked={value.ho}
                    disabled
                  />
                }
                label="Heater"
              /> : null}
              {!this.props.boil ?
                <FormControlLabel
                  control={
                    <Checkbox
                      checked={value.fp}
                      disabled
                    />
                  }
                  label="Full Power"
                /> : null}
            {!this.props.boil ?
              <FormControlLabel
                control={
                  <Checkbox 
                    checked={value.sl}
                    disabled
                  />
                }
                label="Step LOCK"
              /> : null}
          </ListItem>
          : null}
        <Divider middle />
      </List>
    );

    const SortableList = SortableContainer(({ items }) => {
      return (
        <List>
          {items.map((value, index) => (
            <SortableItem key={`item-${index}`} index={index} value={value} itemIndex={index} />
          ))}
        </List>
      );
    });

    return <SortableList items={this.props.items} onSortEnd={this.onSortEnd} useDragHandle={true} />
  }
}

export default (SortableList);