import React, { Component } from 'react';
import { Divider } from '@material-ui/core';
import SectionContent from '../components/SectionContent';
import BoilSettingsForm from '../forms/MashBoilSettingsForm';
import SortableList from '../components/SortableList';
import { withNotifier } from '../components/SnackbarNotification';  
import { SAVE_BOIL_SETTINGS_SERVICE_PATH, GET_BOIL_SETTINGS_SERVICE_PATH } from '../constants/Endpoints';

class BoilSettings extends Component {
  constructor() {
    super()

    this.state = { items: [] }
    this.getBoilSettings();
  }

  getBoilSettings = () => {
    fetch(GET_BOIL_SETTINGS_SERVICE_PATH, {
      method: 'GET',
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json',
      },
    }).then(response => {
      if (response.ok) {
        response.json().then(json => {
          this.setState({ items: json.steps })
        });
        return;
      }
      throw Error("Boil Setings service returned unexpected response code: " + response.status);
    }).catch(error => {
      this.props.raiseNotification("Problem getting Boil Settings: " + error.message);
      this.setState({ items: [] })
    });
  }

  saveBoilSettings = () => {
    fetch(SAVE_BOIL_SETTINGS_SERVICE_PATH, {
      method: 'POST',
      body: JSON.stringify({ "steps": this.state.items }),
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json',
      },
    }).then(response => {
      if (response.ok) {
        return;
      }
      throw Error("Boil Setings service returned unexpected response code: " + response.status);
    }).catch(error => {
      this.props.raiseNotification("Problem saving Boil Settings: " + error.message);
      this.getBoilSettings();
    });
  }

  itemAdded = (newelement) => {
    this.setState({
      items: this.orderArray([...this.state.items, newelement])
    }, this.saveBoilSettings)
  }

  itemDeleted = (index) => {
    var array = [...this.state.items];
    array.splice(index, 1);

    this.setState({
      items: this.orderArray(array)
    }, this.saveBoilSettings);
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

export default withNotifier(BoilSettings);