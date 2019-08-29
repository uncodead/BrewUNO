import React, { Component } from 'react';
import { Divider } from '@material-ui/core';
import SectionContent from '../components/SectionContent';
import MashSettingsForm from '../forms/MashBoilSettingsForm';
import SortableList from '../components/SortableList';
import { withSnackbar } from 'notistack';
import { SAVE_MASH_SETTINGS_SERVICE_PATH, GET_MASH_SETTINGS_SERVICE_PATH } from '../constants/Endpoints';
import { ExecuteRestCall } from '../components/Utils';

class MashSettings extends Component {
  constructor(props) {
    super(props)
    this.state = { items: [] }
    this.getMashSettings();
    this.child = React.createRef();
  }

  getMashSettings = () => {
    ExecuteRestCall(GET_MASH_SETTINGS_SERVICE_PATH, 'GET', (json) => {
      if (json.st != undefined && json.st != null)
        this.setState({ items: json.st })
    }, this.setState({ items: [] }), this.props)
  }

  saveMashSettings = () => {
    fetch(SAVE_MASH_SETTINGS_SERVICE_PATH, {
      method: 'POST',
      body: JSON.stringify({ "st": this.state.items }),
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json',
      },
    }).then(response => {
      if (response.ok) {
        this.props.enqueueSnackbar("Mash settings saved.", { variant: 'info', autoHideDuration: 500, });
        return;
      }
      response.text().then(function (data) {
        throw Error("Mash Setings service returned unexpected response code: " + response.status + " Message: " + data);
      }).catch(error => {
        this.props.enqueueSnackbar(error.message, { variant: 'error', autoHideDuration: 2000, });
        this.getMashSettings();
      });
    });
  }

  itemAdded = (newelement) => {
    if (newelement.index !== null) {
      var array = this.state.items.slice();
      array[newelement.index] = newelement;
      this.setState({
        items: array
      }, this.saveMashSettings);
    }
    else
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

  itemEdited = (index, name, value, textValue) => {
    var array = this.state.items.slice();
    array[index][name] = value;
    this.setState({
      items: array
    }, this.saveMashSettings);
  }

  itemFormEdited = index => event => {
    var array = this.state.items.slice();
    this.child.current.handleNameChange({ target: { value: array[index]['n'] } })
    this.child.current.handleTemperatureChange({ target: { value: array[index]['t'] } })
    this.child.current.handleTimeChange({ target: { value: array[index]['tm'] } })
    this.child.current.handleRecirculationChange(array[index]['r'])
    this.child.current.handleStepLock(array[index]['sl'])
    this.child.current.handleHeaterOn(array[index]['ho'])
    this.child.current.handleFullPower(array[index]['fp'])
    this.child.current.handleIndexChange(index)
    window.scrollTo(0, 0)
  }

  render() {
    return (
      <SectionContent title="Mash Settings" selected={this.props.selectedIndex >= 0}>
        {!this.props.listOnly ?
          <MashSettingsForm callbackItemAdded={this.itemAdded} ref={this.child} />
          : null}
        <Divider />
        <SortableList
          items={this.state.items}
          callbackItemsSorted={this.itemsSorted}
          callbackItemDeleted={this.itemDeleted}
          callbackItemEdited={this.itemEdited}
          callbackFormEdited={this.itemFormEdited}
          editItem
          dragHandle={!this.props.brewDay}
          boil={false}
          brewDay={this.props.brewDay}
          selectedIndex={this.props.selectedIndex}
        />
      </SectionContent>
    )
  }
}

export default withSnackbar(MashSettings);
