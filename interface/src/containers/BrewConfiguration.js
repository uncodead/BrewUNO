import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import MashSettings from './MashSettings';
import BoilSettings from './BoilSettings';
import BrewSettings from './BrewSettings'
import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

import IntText from '../components/IntText'

class BrewConfiguration extends Component {
  constructor() {
    super();
    this.state = {
      selectedTab: 'MashSettings'
    }
  }

  handleTabChange = (event, selectedTab) => {
    this.setState({
      selectedTab: selectedTab
    })
  }

  render() {
    const { selectedTab } = this.state
    return (
      <MenuAppBar sectionTitle="Brew Settings">
        <Tabs value={selectedTab} onChange={this.handleTabChange} indicatorColor="secondary" textColor="contrastText" fullWidth scrollable>
          <Tab value="MashSettings" label={<IntText text="Mash" />} />
          <Tab value="BoilSettings" label={<IntText text="Boil" />} />
          <Tab value="BrewSettings" label={<IntText text="Settings" />} />
        </Tabs>
        {selectedTab === "MashSettings" && <MashSettings />}
        {selectedTab === "BoilSettings" && <BoilSettings />}
        {selectedTab === "BrewSettings" && <BrewSettings />}
      </MenuAppBar>
    )
  }
}

export default BrewConfiguration
