import React, { Component } from 'react';
import T from 'i18n-react'

class IntText extends Component {
  constructor(props) {
    super(props)
  }

  render() {
    return (
      <T.span text={this.props.text} />
    )
  }
}

export default IntText;