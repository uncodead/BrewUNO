import React, { Component } from 'react';
import { Divider } from '@material-ui/core';
import SectionContent from '../components/SectionContent';
import MashSettingsForm from '../forms/MashBoilSettingsForm';
import SortableList from '../components/SortableList';
import { SAVE_MASH_SETUP_SERVICE_PATH }  from  '../constants/Endpoints';

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

    fetch(SAVE_MASH_SETUP_SERVICE_PATH, {method:'POST'}).then(response => {
      if (response.status === 200) {
        return;
      }
      throw Error("Mash Setings service returned unexpected response code: " + response.status);
    }).catch(error => {
        console.log(error)
    });
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
    return (
      <SectionContent title="Mash Settings">
        <MashSettingsForm callbackItemAdded={this.itemAdded} />
        <Divider />
        <SortableList 
          items={this.state.items} 
          callbackItemsSorted={this.itemsSorted} 
          callbackItemDeleted={this.itemDeleted}
          dragHandle={true}
          boil={false}
        />
      </SectionContent>
    )
  }
}

export default MashSettings;
