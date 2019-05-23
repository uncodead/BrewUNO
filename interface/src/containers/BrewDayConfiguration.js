import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import SectionContent from '../components/SectionContent';
import Brew from './Brew'

class BrewConfiguration extends Component {
  constructor() {
    super();
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <MenuAppBar sectionTitle="Brew">
        <Brew />
      </MenuAppBar>
    )
  }
}

export default BrewConfiguration
