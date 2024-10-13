import React, { Component } from 'react';
import { SortableContainer, SortableElement, arrayMove, SortableHandle } from 'react-sortable-hoc';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';
import DeleteIcon from '@material-ui/icons/Delete';
import EditIcon from '@material-ui/icons/Edit';
import Checkbox from '@material-ui/core/Checkbox';
import IconButton from '@material-ui/core/IconButton';
import DragIndicatorIcon from '@material-ui/icons/DragIndicator'
import Typography from '@material-ui/core/Typography'
import FormControlLabel from '@material-ui/core/FormControlLabel';
import Divider from '@material-ui/core/Divider';
import IntText from '../components/IntText'

class SortableList extends Component {
  constructor(props) {
    super(props)
  }

  onSortEnd = ({ oldIndex, newIndex }) => {
    this.props.callbackItemsSorted(arrayMove(this.props.items, oldIndex, newIndex))
  };

  deleteItem = (index) => {
    this.props.callbackItemDeleted(index)
  }

  getItemText = (item) => {
    if (this.props.boil) {
      var name = ''
      if (item.a !== undefined && item.a > 0)
        name += item.a + 'g@'
      return name + item.tm + '\'';
    }
    else if (this.props.cooling) {
      var name = ''
      if (item.a !== undefined && item.a > 0)
        name += item.a + 'g@'
      name += item.tm + '\'@' + item.t + '°'
      return name
    }
    else
      return item.tm + '\'@' + item.t + '°'
  }


  render() {
    const DragHandle = SortableHandle(() => <DragIndicatorIcon />);
    const SortableItem = SortableElement(({ value, itemIndex }) =>
      <List component="nav">
        <ListItem
          selected={
            !this.props.boil && this.props.selectedIndex === itemIndex ||
              this.props.boil && this.props.selectedIndex && this.props.selectedIndex.includes(itemIndex) ? true : false
          }
        >
          {this.props.dragHandle ? <DragHandle /> : null}
          <ListItemText
            onDoubleClick={this.props.callbackFormEdited(itemIndex)}
            primary={value.n}
            secondary={<Typography>{this.getItemText(value)}
              {this.props.brewDay && this.props.mash  && value.r ? <IntText text="Recirculation" spaceBefore={true} spaceAfter={true} /> : null}
              {this.props.brewDay && (this.props.mash || this.props.cooling) && value.ho ? <IntText text="Heater" spaceBefore={true} spaceAfter={true} /> : null}
              {this.props.brewDay && this.props.mash && value.fp ? <IntText text="FullPower" spaceBefore={true} spaceAfter={true} /> : null}
              {this.props.brewDay && this.props.mash && value.sl ? <IntText text="StepLockON" spaceBefore={true} spaceAfter={true} /> : null}
            </Typography>}
          />
          {!this.props.brewDay ?
            <IconButton aria-label="Edit"
              onClick={this.props.callbackFormEdited(itemIndex)}>
              <EditIcon fontSize="default" />
            </IconButton>
            : null}
          {!this.props.brewDay ?
            <IconButton aria-label="Delete"
              onClick={() => this.deleteItem(itemIndex)}>
              <DeleteIcon fontSize="default" />
            </IconButton>
            : null}
        </ListItem>
        {!this.props.brewDay ?
          <ListItem>
            {this.props.mash ?
              <FormControlLabel
                control={
                  <Checkbox
                    checked={value.r}
                    onChange={() => { this.props.callbackItemEdited(itemIndex, 'r', !value.r) }}
                  />
                }
                label={<IntText text="Pump" />}
              /> : null}
            {this.props.mash || this.props.cooling ?
              <FormControlLabel
                control={
                  <Checkbox
                    checked={value.ho}
                    onChange={() => { this.props.callbackItemEdited(itemIndex, 'ho', !value.ho) }}
                  />
                }
                label={<IntText text="Heater" />}
              /> : null}

          </ListItem>
          : null}
        {!this.props.brewDay ?
          <ListItem color="black">
            {this.props.mash ?
              <FormControlLabel
                control={
                  <Checkbox
                    checked={value.fp}
                    onChange={() => { this.props.callbackItemEdited(itemIndex, 'fp', !value.fp) }}
                  />
                }
                label={<IntText text="FullPower" />}
              /> : null}
            {this.props.mash ?
              <FormControlLabel
                control={
                  <Checkbox
                    checked={value.sl}
                    onChange={() => { this.props.callbackItemEdited(itemIndex, 'sl', !value.sl) }}
                  />
                }
                label={<IntText text="StepLockON" />}
              /> : null}
          </ListItem>
          : null}
        <Divider />
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