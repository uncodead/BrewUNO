import React, { Component } from 'react';
import { withStyles } from '@material-ui/core/styles';
import { Divider } from '@material-ui/core';
import SectionContent from '../components/SectionContent';
import MashForm from '../forms/MashForm';
import SortableList from '../components/SortableList';

const styles = theme => ({
  container: {
    display: 'flex',
    flexWrap: 'wrap',
  },
});

class MashSettings extends Component {
  constructor() {
    super()

    this.state = {
      items: [
        { name: 'Mash In', temperature: 65, time: 5, recirculation: false },
        { name: 'Protein Stop', temperature: 68, time: 60, recirculation: true },
        { name: 'Sacarification', temperature: 68, time: 60, recirculation: true },
        { name: 'Mash out', temperature: 68, time: 60, recirculation: true },
      ],
    }
  }

  itemAdded = (newelement) => {
    this.setState({
      items: [...this.state.items, newelement]
    })
  }

  itemsSorted = (items) => {
    this.setState({
      items: items
    })
  }

  itemDeleted = (index) => {
    var array = [...this.state.items];
    array.splice(index, 1);

    this.setState({
      items: array
    });
  }

  render() {
    const { classes } = this.props;

    return (
      <SectionContent title="Mash Settings">
        <MashForm callbackItemAdded={this.itemAdded} />
        <Divider />
        <SortableList 
          items={this.state.items} 
          callbackItemsSorted={this.itemsSorted} 
          callbackItemDeleted={this.itemDeleted}
        />
      </SectionContent>
    )
  }
}

export default withStyles(styles)(MashSettings);
