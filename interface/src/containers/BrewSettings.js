import React, { Component } from 'react';
import { Divider } from '@material-ui/core';
import SectionContent from '../components/SectionContent';
import BrewSettingsForm from '../forms/BrewSettingsForm';
import { restComponent } from '../components/RestComponent';
import {
  GET_SENSORS
} from '../constants/Endpoints';
import { ExecuteRestCall } from '../components/Utils';

import { BREW_SETTINGS_ENDPOINT } from '../constants/Endpoints';

class BrewSettings extends Component {
  constructor(props) {
    super(props)
    this.state = { sensors: [] }
    this.getSensors();
  }

  getSensors() {
    ExecuteRestCall(GET_SENSORS, 'GET', (json) => {
      this.setState({
        sensors: json.sensors
      })
    }, null, this.props)
  }

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    console.log(data)
    return (
      <SectionContent title="Brew Settings">
        <BrewSettingsForm
          brewSettings={data}
          brewSettingsFetched={fetched}
          errorMessage={errorMessage}
          onSubmit={this.props.saveData}
          handleValueChange={this.props.handleValueChange}
          sensors={this.state.sensors}
        />
      </SectionContent>
    )
  }
}

export default restComponent(BREW_SETTINGS_ENDPOINT, BrewSettings);
