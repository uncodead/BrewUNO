import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import MashSettings from './MashSettings';
import BoilSettings from './BoilSettings';
import BrewSettings from './BrewSettings'
import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';
import IntText from '../components/IntText'
import { PageView, initGA, Event } from '../components/Tracking'

class BrewConfiguration extends Component {
  constructor() {
    super();
    this.state = {
      selectedTab: 'MashSettings'
    }
  }

  componentDidMount() {
    initGA('UA-149477072-2');
    PageView();
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
          <Tab value="BrewSettings" label={<IntText text="BrewSettings.Settings" />} />
        </Tabs>
        {selectedTab === "MashSettings" && <MashSettings />}
        {selectedTab === "BoilSettings" && <BoilSettings />}
        {selectedTab === "BrewSettings" && <BrewSettings />}
      </MenuAppBar>
    )
  }
}

export default BrewConfiguration
