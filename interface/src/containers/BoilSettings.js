import React, { Component } from 'react';
import { Divider } from '@material-ui/core';
import SectionContent from '../components/SectionContent';
import BoilSettingsForm from '../forms/MashBoilSettingsForm';
import SortableList from '../components/SortableList';
import { withSnackbar } from 'notistack';
import { SAVE_BOIL_SETTINGS_SERVICE_PATH, GET_BOIL_SETTINGS_SERVICE_PATH } from '../constants/Endpoints';
import { ExecuteRestCall } from '../components/Utils';

class BoilSettings extends Component {
  constructor() {
    super()

    this.state = { items: [] }
    this.getBoilSettings();
    this.child = React.createRef();
  }

  getBoilSettings = () => {
    ExecuteRestCall(GET_BOIL_SETTINGS_SERVICE_PATH, 'GET', (json) => {
      if (json.st != undefined && json.st != null)
        this.setState({ items: json.st })
    }, this.setState({ items: [] }), this.props)
  }

  saveBoilSettings = () => {
    fetch(SAVE_BOIL_SETTINGS_SERVICE_PATH, {
      method: 'POST',
      body: JSON.stringify({ "st": this.state.items }),
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json',
      },
    }).then(response => {
      if (response.ok) {
        this.props.enqueueSnackbar("Boil settings saved.", { variant: 'info', autoHideDuration: 2000, });
        return;
      }
      response.text().then(function (data) {
        throw Error("Boil Setings service returned unexpected response code: " + response.status + " Message: " + data);
      }).catch(error => {
        this.props.enqueueSnackbar(error.message, { variant: 'error', autoHideDuration: 2000, });
        this.getBoilSettings();
      });
    });
  }

  itemAdded = (newelement) => {
    if (newelement.index !== null) {
      var array = this.state.items.slice();
      array[newelement.index] = newelement;
      this.setState({
        items: array
      }, this.saveBoilSettings);
    }
    else
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

  itemFormEdited = index => event => {
    var array = this.state.items.slice();
    this.child.current.handleNameChange({ target: { value: array[index]['n'] } })
    this.child.current.handleAmountChange({ target: { value: array[index]['a'] } })
    this.child.current.handleTimeChange({ target: { value: array[index]['tm'] } })
    this.child.current.handleIndexChange(index)
    window.scrollTo(0, 0)
  }

  orderArray = (array) => {
    return array.sort((a, b) => b.tm - a.tm);
  }

  render() {
    return (
      <SectionContent title="Boil Timing" selected={this.props.active_boil_step_index >= 0}>
        {!this.props.listOnly ?
          <BoilSettingsForm callbackItemAdded={this.itemAdded} boil={true} ref={this.child} /> : null}
        <Divider />
        <SortableList
          items={this.state.items}
          callbackItemDeleted={this.itemDeleted}
          callbackFormEdited={this.itemFormEdited}
          dragHandle={false}
          boil={true}
          brewDay={this.props.brewDay}
          selectedIndex={this.props.selectedIndex}
        />
      </SectionContent>
    )
  }
}

export default withSnackbar(BoilSettings);