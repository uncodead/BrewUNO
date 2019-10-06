import React, { Component } from 'react';

import { OTA_SETTINGS_ENDPOINT }  from  '../constants/Endpoints';
import {restComponent} from '../components/RestComponent';
import SectionContent from '../components/SectionContent';
import OTASettingsForm from '../forms/OTASettingsForm';

import IntText from '../components/IntText'

class OTASettings extends Component {

  componentDidMount() {
      this.props.loadData();
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <SectionContent title={<IntText text="OTASettings.Settings" />}>
      	<OTASettingsForm
          otaSettings={data}
          otaSettingsFetched={fetched}
          errorMessage={errorMessage}
          onSubmit={this.props.saveData}
          onReset={this.props.loadData}
          handleValueChange={this.props.handleValueChange}
          handleCheckboxChange={this.props.handleCheckboxChange}
        />
      </SectionContent>
    )
  }

}

export default restComponent(OTA_SETTINGS_ENDPOINT, OTASettings);
