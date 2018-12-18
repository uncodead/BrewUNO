import React, { Component } from 'react';
import { withStyles } from '@material-ui/core/styles';
import SectionContent from '../components/SectionContent';
import { Divider } from '@material-ui/core';
import MashForm from '../forms/MashForm';
import OrderedList from '../components/OrderedList';

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
        { index: 0, name: 'Mash In', temperature: 65, time: 5, recirculation: false },
        { index: 1, name: 'Protein Stop', temperature: 68, time: 60, recirculation: true },
        { index: 1, name: 'Sacarification', temperature: 68, time: 60, recirculation: true },
        { index: 1, name: 'Mash out', temperature: 68, time: 60, recirculation: true },
      ],
    }
  }

  itemAdded = (newelement) => {
    this.setState({
      items: [...this.state.items, newelement]
    })

    console.log(this.state.items)
    this.forceUpdate();
  }

  render() {
    const { classes } = this.props;

    return (
      <SectionContent title="Mash Settings">
        <MashForm callbackItemAdded={this.itemAdded} />
        <Divider />
        <OrderedList items={this.state.items} />
      </SectionContent>
    )
  }
}

export default withStyles(styles)(MashSettings);
