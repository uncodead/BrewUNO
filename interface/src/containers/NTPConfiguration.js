import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import NTPSettings from './NTPSettings';
import NTPStatus from './NTPStatus';
import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';
import IntText from '../components/IntText'
import { PageView, initGA, Event } from '../components/Tracking'

class NTPConfiguration extends Component {

  constructor(props) {
    super(props);
    this.state = {
        selectedTab: "ntpStatus"
    };
  }

  componentDidMount() {
    initGA('UA-149477072-2');
    PageView();
  }

  handleTabChange = (event, selectedTab) => {
    this.setState({ selectedTab });
  };

  render() {
    const { selectedTab } = this.state;
    return (
        <MenuAppBar sectionTitle={<IntText text="NTPSettings.NTPConfiguration" />}>
        <Tabs value={selectedTab} onChange={this.handleTabChange} indicatorColor="secondary" textColor="contrastText" fullWidth scrollable>
           <Tab value="ntpStatus" label={<IntText text="NTPSettings.NTPStatus" />} />
           <Tab value="ntpSettings" label={<IntText text="NTPSettings.Settings" />} />
         </Tabs>
         {selectedTab === "ntpStatus" && <NTPStatus />}
         {selectedTab === "ntpSettings" && <NTPSettings />}
        </MenuAppBar>
    )
  }
}

export default NTPConfiguration
