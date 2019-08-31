import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import Brew from './Brew'

class BrewConfiguration extends Component {
  constructor() {
    super();
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
