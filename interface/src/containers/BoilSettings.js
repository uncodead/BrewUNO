import React, { Component } from 'react';
import { Divider } from '@material-ui/core';
import SectionContent from '../components/SectionContent';
import BoilSettingsForm from '../forms/MashBoilSettingsForm';
import SortableList from '../components/SortableList';

class BoilSettings extends Component {
  constructor() {
    super()

    this.state = {
      items:  this.orderArray([
        { name: 'Amarillo', time: 60 },
        { name: 'Galaxy', time: 30 },
        { name: 'Cascade', time: 20 },
        { name: 'Citra', time: 20 },
        { name: 'Whirfloc', time: 10 },
      ]),
    }
  }

  itemAdded = (newelement) => {
    this.setState({
      items: this.orderArray([...this.state.items, newelement])
    })
  }

  itemDeleted = (index) => {
    var array = [...this.state.items];
    array.splice(index, 1);

    this.setState({
      items: this.orderArray(array)
    });
  }

  orderArray = (array) => {
    return array.sort((a, b) => b.time - a.time);
  }

  render() {
    return (
      <SectionContent title="Boil Settings">
        <BoilSettingsForm callbackItemAdded={this.itemAdded} boil={true} />
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

export default BoilSettings;