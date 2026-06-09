import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import Brew from './Brew'
import { PageView, initGA } from '../components/Tracking'

class BrewConfiguration extends Component {

  componentDidMount() {
    initGA('UA-149477072-2');
    PageView();
  }

  render() {
    return (
      <MenuAppBar sectionTitle="Brew">
        <Brew />
      </MenuAppBar>
    )
  }
}

export default BrewConfiguration
