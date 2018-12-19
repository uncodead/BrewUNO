import React, { Component } from 'react';
import { withStyles } from '@material-ui/core/styles';
import { Divider } from '@material-ui/core';
import SectionContent from '../components/SectionContent';
import BrewSettingsForm from '../forms/BrewSettingsForm';
import SortableList from '../components/SortableList';

const styles = theme => ({
  container: {
    display: 'flex',
    flexWrap: 'wrap',
  },
});

class BoilSettings extends Component {
  constructor() {
    super()

    this.state = {
      items: [
        { name: 'Amarillo', time: 60 },
        { name: 'Galaxy', time: 30 },
        { name: 'Cascade', time: 20 },
        { name: 'Citra', time: 20 },
        { name: 'Whirfloc', time: 10 },
      ],
    }
  }

  itemAdded = (newelement) => {
    this.setState({
      items: [...this.state.items, newelement]
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
      <SectionContent title="Boil Settings">
        <BrewSettingsForm callbackItemAdded={this.itemAdded} boil={true} />
        <Divider />
        <SortableList 
          items={this.state.items}
          callbackItemDeleted={this.itemDeleted}
          dragHandle={false}
          boil={true}
        />
      </SectionContent>
    )
  }
}

export default withStyles(styles)(BoilSettings);