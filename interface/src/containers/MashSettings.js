import React, { Component } from 'react';
import { Divider } from '@material-ui/core';
import SectionContent from '../components/SectionContent';
import MashSettingsForm from '../forms/MashBoilSettingsForm';
import SortableList from '../components/SortableList';
import { withSnackbar } from 'notistack';
import { SAVE_MASH_SETTINGS_SERVICE_PATH, GET_MASH_SETTINGS_SERVICE_PATH } from '../constants/Endpoints';
import { ExecuteRestCall } from '../components/Utils';
import IntText from '../components/IntText'
import Cookies from 'js-cookie';
import LinearProgress from '@material-ui/core/LinearProgress';
import Typography from '@material-ui/core/Typography';

class MashSettings extends Component {
  constructor(props) {
    super(props)
    this.state = { items: [] }
    this.child = React.createRef();
  }

  componentDidMount() {
    if (Cookies.get('mashSettings') === undefined) {
      setTimeout(function () {
        this.getMashSettings();
      }.bind(this), 2000)
    }
    else {
      this.getMashSettings();
    }
  }

  getMashSettings = () => {
    if (Cookies.get('mashSettings') === undefined) {
      ExecuteRestCall(GET_MASH_SETTINGS_SERVICE_PATH, 'GET', (json) => {
        if (json.st !== undefined && json.st !== null) {
          this.setState({ items: json.st })
          Cookies.set('mashSettings', json.st)
        }
      }, this.setState({ items: [] }), this.props)
    }
    else {
      var json = JSON.parse(Cookies.get('mashSettings'))
      this.setState({ items: json })
    }
  }

  saveMashSettings = () => {
    Cookies.set('mashSettings', this.state.items)
    fetch(SAVE_MASH_SETTINGS_SERVICE_PATH, {
      method: 'POST',
      body: JSON.stringify({ "st": this.state.items }),
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json',
      },
    }).then(response => {
      if (response.ok) {
        this.props.enqueueSnackbar(<IntText text="Mash.SavedAlert" />, { variant: 'info', autoHideDuration: 500, });
        return;
      }
      response.text().then(function (data) {
        throw Error(<IntText text="Mash.ErrorAlert" /> + response.status + " - " + data);
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
    this.child.current.handleRecirculationChange(null, array[index]['r'])
    this.child.current.handleStepLock(null, array[index]['sl'])
    this.child.current.handleHeaterOn(null, array[index]['ho'])
    this.child.current.handleFullPower(null, array[index]['fp'])
    this.child.current.handleIndexChange(index)
    window.scrollTo(0, 0)
  }

  render() {
    return (
      <SectionContent title={<IntText text="MashSettings.Settings" />} selected={this.props.selectedIndex >= 0}>
        {!this.props.listOnly ?
          <MashSettingsForm callbackItemAdded={this.itemAdded} ref={this.child} />
          : null}
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
