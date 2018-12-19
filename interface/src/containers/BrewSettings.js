import React, { Component } from 'react';
import { Divider } from '@material-ui/core';
import SectionContent from '../components/SectionContent';
import BrewSettingsForm from '../forms/BrewSettingsForm';

class BrewSettings extends Component {
  constructor() {
    super()
  }

  render() {
    return (
      <SectionContent title="Brew Settings">
        <BrewSettingsForm />
      </SectionContent>
    )
  }
}

export default BrewSettings;
