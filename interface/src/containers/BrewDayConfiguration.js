import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import Brew from './Brew'
import { PageView, initGA, Event } from '../components/Tracking'

class BrewConfiguration extends Component {
  constructor() {
    super();
  }

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
