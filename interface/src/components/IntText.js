import React, { Component } from 'react';
import T from 'i18n-react'

class IntText extends Component {
  constructor(props) {
    super(props)
  }

  render() {
    return (
      <span>
        {this.props.spaceBefore ? ' ' : null}
        <T.span text={this.props.text} />
        {this.props.spaceAfter ? ' ' : null}
      </span>
    )
  }
}

export default IntText;