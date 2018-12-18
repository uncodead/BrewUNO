import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import MashSettings from './MashSettings';
import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

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
        <Tabs value={selectedTab} onChange={this.handleTabChange} indicatorColor="primary" textColor="primary" fullWidth scrollable>
          <Tab value="MashSettings" label="Mash Settings" />
          <Tab value="BoilSettings" label="Boil Settings" />
          <Tab value="Configuration" label="Brew Configuration" />
          <Tab value="Brew" label="Brew" />
        </Tabs>
        {selectedTab === "MashSettings" && <MashSettings />}
        {selectedTab === "Configuration" && <div />}
      </MenuAppBar>
    )
  }
}

export default BrewConfiguration
