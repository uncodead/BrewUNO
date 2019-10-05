import React, { Component } from 'react';

import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';

import MenuAppBar from '../components/MenuAppBar';
import WiFiNetworkScanner from './WiFiNetworkScanner';
import WiFiSettings from './WiFiSettings';
import WiFiStatus from './WiFiStatus';
import { PageView, initGA, Event } from '../components/Tracking'
import IntText from '../components/IntText'

class WiFiConfiguration extends Component {
  constructor(props) {
    super(props);
    this.state = {
      selectedTab: "wifiStatus",
      selectedNetwork: null
    };
    this.selectNetwork = this.selectNetwork.bind(this);
    this.deselectNetwork = this.deselectNetwork.bind(this);
  }

  componentDidMount() {
    initGA('UA-149477072-2');
    PageView();
  }

  // TODO - slightly inapproperate use of callback ref possibly.
  selectNetwork(network) {
    this.setState({ selectedTab: "wifiSettings", selectedNetwork: network });
  }

  // deselects the network after the settings component mounts.
  deselectNetwork(network) {
    this.setState({ selectedNetwork: null });
  }

  handleTabChange = (event, selectedTab) => {
    this.setState({ selectedTab });
  };

  render() {
    const { selectedTab } = this.state;
    return (
      <MenuAppBar sectionTitle={<IntText text="WiFiSettings.WiFiConfiguration" />}>
        <Tabs value={selectedTab} onChange={this.handleTabChange} indicatorColor="secondary" textColor="contrastText" fullWidth scrollable>
          <Tab value="wifiStatus" label={<IntText text="WiFiSettings.WiFiStatus" />} />
          <Tab value="networkScanner" label={<IntText text="WiFiSettings.NetworkScanner" />} />
          <Tab value="wifiSettings" label={<IntText text="WiFiSettings.Settings" />} />
        </Tabs>
        {selectedTab === "wifiStatus" && <WiFiStatus fullDetails={true} />}
        {selectedTab === "networkScanner" && <WiFiNetworkScanner selectNetwork={this.selectNetwork} />}
        {selectedTab === "wifiSettings" && <WiFiSettings deselectNetwork={this.deselectNetwork} selectedNetwork={this.state.selectedNetwork} />}
      </MenuAppBar>
    )
  }
}

export default WiFiConfiguration;
