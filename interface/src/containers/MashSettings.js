import React, { Component } from 'react';
import { Divider } from '@material-ui/core';
import SectionContent from '../components/SectionContent';
import MashSettingsForm from '../forms/MashBoilSettingsForm';
import SortableList from '../components/SortableList';
import { withNotifier } from '../components/SnackbarNotification';
import { SAVE_MASH_SETTINGS_SERVICE_PATH, GET_MASH_SETTINGS_SERVICE_PATH, ExecuteRestCall } from '../constants/Endpoints';

class MashSettings extends Component {
  constructor(props) {
    super(props)
    this.state = { items: [] }
    this.getMashSettings();
  }

  getMashSettings = () => {
    ExecuteRestCall(GET_MASH_SETTINGS_SERVICE_PATH, 'GET', (json) => { this.setState({ items: json.steps }) }, this.setState({ items: [] }), this.props)
  }

  saveMashSettings = () => {
    fetch(SAVE_MASH_SETTINGS_SERVICE_PATH, {
      method: 'POST',
      body: JSON.stringify({ "steps": this.state.items }),
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json',
      },
    }).then(response => {
      if (response.ok) {
        this.props.raiseNotification("Mash settings saved.");
        return;
      }
      response.text().then(function (data) {
        throw Error("Mash Setings service returned unexpected response code: " + response.status + " Message: " + data);
      }).catch(error => {
        this.props.raiseNotification(error.message);
        this.getMashSettings();
      });
    });
  }

  itemAdded = (newelement) => {
    this.setState({
      items: [...this.state.items, newelement]
    }, this.saveMashSettings)
  }

  itemsSorted = (items) => {
    this.setState({
      items: items
    }, this.saveMashSettings)
  }

  itemDeleted = (index) => {
    var array = [...this.state.items];
    array.splice(index, 1);

    this.setState({
      items: array
    }, this.saveMashSettings);
  }

  render() {
    return (
      <SectionContent title="Mash Settings" selected={this.props.selectedIndex >= 0}>
        {!this.props.listOnly ? <MashSettingsForm callbackItemAdded={this.itemAdded} /> : null}
        <Divider />
        <SortableList
          items={this.state.items}
          callbackItemsSorted={this.itemsSorted}
          callbackItemDeleted={this.itemDeleted}
          dragHandle={!this.props.brewDay}
          boil={false}
          brewDay={this.props.brewDay}
          selectedIndex={this.props.selectedIndex}
        />
      </SectionContent>
    )
  }
}

export default withNotifier(MashSettings);
