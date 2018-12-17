import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import BrewSettings from './BrewSettings';
import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

class BrewConfiguration extends Component {
  constructor() {
    super();
    this.state = {
      selectedTab: 'BrewSettings'
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
          <Tab value="BrewSettings" label="Brew Settings" />
          <Tab value="Configuration" label="Brew Configuration" />
        </Tabs>
        {selectedTab === "BrewSettings" && <BrewSettings />}
        {selectedTab === "Configuration" && <div />}
      </MenuAppBar>
    )
  }
}

export default BrewConfiguration
