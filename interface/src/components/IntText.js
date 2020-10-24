import React, { Component } from 'react';
import T from 'i18n-react';
import en from "../language/en.json"
import de from "../language/de.json"
import ptBR from "../language/pt-BR.json"
import ruRU from "../language/ru-RU.json"
import esES from "../language/es-ES.json"

class IntText extends Component {
  constructor(props) {
    super(props)
  }

  SetText(lang) {
    switch (lang) {
      case "pt-BR":
        T.setTexts(ptBR)
        break
      case "de":
        T.setTexts(de);
        break
      case "en":
        T.setTexts(en);
        break
      case "ru-RU":
        T.setTexts(ruRU);
        break
      case "es-ES":
        T.setTexts(esES);
        break
      default:
        T.setTexts(en)
        break
    }
    this.forceUpdate()
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