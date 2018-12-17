import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import BrewSettings from './BrewSettings';

class BrewConfiguration extends Component {
  render() {
    return (
        <MenuAppBar sectionTitle="Brew Settings">
          <BrewSettings />
        </MenuAppBar>
    )
  }
}

export default BrewConfiguration
