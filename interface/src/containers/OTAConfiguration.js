import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import OTASettings from './OTASettings';
import { PageView, initGA, Event } from '../components/Tracking'
import IntText from '../components/IntText'

class OTAConfiguration extends Component {
  componentDidMount() {
    initGA('UA-149477072-2');
    PageView();
  }

  render() {
    return (
      <MenuAppBar sectionTitle={<IntText text="OTASettings.OTAConfiguration" />}>
        <OTASettings />
      </MenuAppBar>
    )
  }
}

export default OTAConfiguration
