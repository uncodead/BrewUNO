import React, { Component } from 'react';
import { Divider } from '@material-ui/core';
import SectionContent from '../components/SectionContent';
import CoolingSettingsForm from '../forms/MashBoilSettingsForm';
import ImportConfig from './ImportConfig'
import SortableList from '../components/SortableList';
import { withSnackbar } from 'notistack';
import { SAVE_COOLING_SETTINGS_SERVICE_PATH, GET_COOLING_SETTINGS_SERVICE_PATH } from '../constants/Endpoints';
import { ExecuteRestCall } from '../components/Utils';
import IntText from '../components/IntText'
import Cookies from 'js-cookie';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';

class CoolingSettings extends Component {
  constructor(props) {
    super(props)
    this.state = { items: [] }
    this.child = React.createRef();
  }

  componentDidMount() {
    if (Cookies.get('coolingSettings') === undefined) {
      setTimeout(function () {
        this.getCoolingSettings();
      }.bind(this), 2000)
    }
    else {
      this.getCoolingSettings();
    }
    
    
  }

  getCoolingSettings = () => {
    if (Cookies.get('coolingSettings') === undefined) {
      ExecuteRestCall(GET_COOLING_SETTINGS_SERVICE_PATH, 'GET', (json) => {
        if (json !== undefined && json.st !== undefined && json.st !== null) {
          this.setState({ items: json.st })
          Cookies.set('coolingSettings', json.st)
        }
      }, this.setState({ items: [] }), this.props)
    }
    else {
      var json = JSON.parse(Cookies.get('coolingSettings'))
      this.setState({ items: json })
    }
  }

  saveCoolingSettings = () => {
    Cookies.set('coolingSettings', this.state.items)
    fetch(SAVE_COOLING_SETTINGS_SERVICE_PATH, {
      method: 'POST',
      body: JSON.stringify({ "st": this.state.items }),
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json',
      },
    }).then(response => {
      if (response.ok) {
        this.props.enqueueSnackbar(<IntText text="Cooling.SavedAlert" />, { variant: 'info', autoHideDuration: 500, });
        return;
      }
      response.text().then(function (data) {
        throw Error(<IntText text="Cooling.ErrorAlert" /> + response.status + " - " + data);
      }).catch(error => {
        this.props.enqueueSnackbar(error.message, { variant: 'error', autoHideDuration: 2000, });
        this.getCoolingSettings();
      });
    });
  }

  onImport = (items) => {
    this.setState({
      items: items
    }, this.saveCoolingSettings)
  }

  itemAdded = (newelement) => {
    if (newelement.index !== null) {
      var array = this.state.items.slice();
      array[newelement.index] = newelement;
      this.setState({
        items: array
      }, this.saveCoolingSettings);
    }
    else
      this.setState({
        items: [...this.state.items, newelement]
      }, this.saveCoolingSettings)
  }

  itemDeleted = (index) => {
    var array = [...this.state.items];
    array.splice(index, 1);

    this.setState({
      items: array
    }, this.saveCoolingSettings);
  }

  itemEdited = (index, name, value, textValue) => {
    var array = this.state.items.slice();
    array[index][name] = value;
    this.setState({
      items: array
    }, this.saveCoolingSettings);
  }

  itemFormEdited = index => event => {
    var array = this.state.items.slice();
    this.child.current.handleNameChange({ target: { value: array[index]['n'] } })
    this.child.current.handleTemperatureChange({ target: { value: array[index]['t'] } })
    this.child.current.handleTimeChange({ target: { value: array[index]['tm'] } })
    this.child.current.handleAmountChange({ target: { value: array[index]['a'] } })
    this.child.current.handleHeaterOn(null, array[index]['ho'])
    this.child.current.handleIndexChange(index)
    window.scrollTo(0, 0)
  }

  render() {
    return (
      <SectionContent title={<IntText text="CoolingSettings.Settings" />} selected={this.props.selectedIndex >= 0}>
        {!this.props.listOnly ?
          <CoolingSettingsForm cooling={true} callbackItemAdded={this.itemAdded} ref={this.child} />
          : null}
        <Divider />
        <SortableList
          items={this.state.items}
          callbackItemDeleted={this.itemDeleted}
          callbackFormEdited={this.itemFormEdited}
          callbackItemEdited={this.itemEdited}
          dragHandle={false}
          cooling={true}
          brewDay={this.props.brewDay}
          selectedIndex={this.props.selectedIndex}
        />
        {!this.props.brewDay ?
          <ImportConfig name='cooling' items={this.state.items} onImport={this.onImport} />
          : null}
      </SectionContent>
    )
  }
}

export default withSnackbar(CoolingSettings);
