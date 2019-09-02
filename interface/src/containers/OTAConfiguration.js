import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import OTASettings from './OTASettings';

import IntText from '../components/IntText'

class OTAConfiguration extends Component {
  render() {
    return (
      <MenuAppBar sectionTitle={<IntText text="OTASettings.OTAConfiguration" />}>
        <OTASettings />
      </MenuAppBar>
    )
  }
}

export default OTAConfiguration
