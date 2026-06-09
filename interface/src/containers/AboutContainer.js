import React, { Component } from 'react';
import MenuAppBar from '../components/MenuAppBar';
import About from './About'
import { PageView, initGA } from '../components/Tracking'
import IntText from '../components/IntText'

class AboutContainer extends Component {

  componentDidMount() {
    initGA('UA-149477072-2');
    PageView();
  }

  render() {
    return (
      <MenuAppBar sectionTitle={<IntText text="About" />}>
        <About />
      </MenuAppBar>
    )
  }
}

export default AboutContainer
