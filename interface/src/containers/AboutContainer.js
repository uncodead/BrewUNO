import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import About from './About'

class AboutContainer extends Component {
  constructor() {
    super();
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <MenuAppBar sectionTitle="About">
        <About />
      </MenuAppBar>
    )
  }
}

export default AboutContainer
