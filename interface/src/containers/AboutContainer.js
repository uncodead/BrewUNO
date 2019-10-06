import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import About from './About'
import { PageView, initGA, Event } from '../components/Tracking'
import IntText from '../components/IntText'

class AboutContainer extends Component {
  constructor() {
    super();
  }

  componentDidMount() {
    initGA('UA-149477072-2');
    PageView();
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
