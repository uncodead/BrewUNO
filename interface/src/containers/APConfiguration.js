import React, { Component } from 'react';
import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';
import MenuAppBar from '../components/MenuAppBar';
import APSettings from './APSettings';
import APStatus from './APStatus';
import { PageView, initGA } from '../components/Tracking'
import IntText from "../components/IntText";

class APConfiguration extends Component {

  constructor(props) {
    super(props);
    this.state = {
        selectedTab: "apStatus",
        selectedNetwork: null
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
      <MenuAppBar sectionTitle="AP Configuration">
        <Tabs value={selectedTab} onChange={this.handleTabChange} indicatorColor="secondary" textColor="contrastText" fullWidth scrollable>
           <Tab value="apStatus" label={<IntText text="APSettings.APStatus" />} />
           <Tab value="apSettings" label={<IntText text="APSettings.Settings" />} />
         </Tabs>
         {selectedTab === "apStatus" && <APStatus fullDetails={true} />}
         {selectedTab === "apSettings" && <APSettings />}
      </MenuAppBar>
    )
  }
}

export default APConfiguration;
