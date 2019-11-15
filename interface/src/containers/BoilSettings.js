import React, { Component } from 'react';
import { Divider } from '@material-ui/core';
import SectionContent from '../components/SectionContent';
import BoilSettingsForm from '../forms/MashBoilSettingsForm';
import SortableList from '../components/SortableList';
import { withSnackbar } from 'notistack';
import { SAVE_BOIL_SETTINGS_SERVICE_PATH, GET_BOIL_SETTINGS_SERVICE_PATH } from '../constants/Endpoints';
import { ExecuteRestCall } from '../components/Utils';
import IntText from '../components/IntText'
import Cookies from 'js-cookie';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';

class BoilSettings extends Component {
  constructor() {
    super()
    this.state = { items: [] }
    this.child = React.createRef();
  }

  componentDidMount() {
    if (Cookies.get('boilSettings') === undefined) {
      setTimeout(function () {
        this.getBoilSettings();
      }.bind(this), 3000)
    }
    else {
      this.getBoilSettings();
    }
  }

  getBoilSettings = () => {
    if (Cookies.get('boilSettings') === undefined) {
      ExecuteRestCall(GET_BOIL_SETTINGS_SERVICE_PATH, 'GET', (json) => {
        if (json.st != undefined && json.st != null) {
          this.setState({ items: json.st })
          Cookies.set('boilSettings', json.st)
        }
      }, this.setState({ items: [] }), this.props)
    }
    else {
      var json = JSON.parse(Cookies.get('boilSettings'))
      this.setState({ items: json })
    }
  }

  saveBoilSettings = () => {
    Cookies.set('boilSettings', this.state.items)
    fetch(SAVE_BOIL_SETTINGS_SERVICE_PATH, {
      method: 'POST',
      body: JSON.stringify({ "st": this.state.items }),
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json',
      },
    }).then(response => {
      if (response.ok) {
        this.props.enqueueSnackbar(<IntText text="BoilSettings.SavedAlert" />, { variant: 'info', autoHideDuration: 2000, });
        return;
      }
      response.text().then(function (data) {
        throw Error(<IntText text="BoilSettings.ErrorAlert" /> + response.status + " - " + data);
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
      <SectionContent title={<IntText text="BoilSettings.Settings" />} selected={this.props.active_boil_step_index >= 0}>
        {!this.props.listOnly ?
          <BoilSettingsForm callbackItemAdded={this.itemAdded} boil={true} ref={this.child} /> : null}
        <Divider />
        {
          this.state.items === null || this.state.items === undefined || this.state.items.length <= 0 ?
            <div>
              <LinearProgress />
              <Typography variant="display1">{<IntText text="Loading" />}...</Typography>
            </div>
            : null
        }
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