import React, { Component } from 'react';

import { OTA_SETTINGS_ENDPOINT }  from  '../constants/Endpoints';
import {restComponent} from '../components/RestComponent';
import SectionContent from '../components/SectionContent';


class OTASettings extends Component {

  componentDidMount() {
      this.props.loadData();
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <SectionContent title="Brew Settings">
      	
      </SectionContent>
    )
  }

}

export default restComponent(OTA_SETTINGS_ENDPOINT, OTASettings);
