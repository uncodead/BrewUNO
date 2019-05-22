import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import MashSettings from './MashSettings';
import BoilSettings from './BoilSettings';
import BrewSettings from './BrewSettings'
import Brew from './Brew'
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
        <Tabs value={selectedTab} onChange={this.handleTabChange} indicatorColor="secondary" textColor="secondary" fullWidth scrollable>
          <Tab value="MashSettings" label="Mash" />
          <Tab value="BoilSettings" label="Boil" />
          <Tab value="BrewSettings" label="Settings" />
          <Tab value="Brew" label="Brew" />
        </Tabs>
        {selectedTab === "MashSettings" && <MashSettings />}
        {selectedTab === "BoilSettings" && <BoilSettings />}
        {selectedTab === "BrewSettings" && <BrewSettings />}
        {selectedTab === "Brew" && <Brew />}
      </MenuAppBar>
    )
  }
}

export default BrewConfiguration
