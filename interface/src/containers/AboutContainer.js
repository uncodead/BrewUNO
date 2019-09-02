import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import About from './About'

import IntText from '../components/IntText'

class AboutContainer extends Component {
  constructor() {
    super();
  }

  render() {
    const { data, fetched, errorMessage } = this.props;
    return (
      <MenuAppBar sectionTitle={<IntText text="About" />}>
        <About />
      </MenuAppBar>
    )
  }
}

export default AboutContainer
