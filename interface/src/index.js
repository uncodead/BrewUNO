import React from 'react';
import { render } from 'react-dom';
import history from './history';
import { Router, Route, Redirect, Switch } from 'react-router';
import App from './App';
import T from 'i18n-react';
import { GET_LANGUAGES } from './constants/Endpoints';
import { ExecuteRestCall } from './components/Utils'

ExecuteRestCall(GET_LANGUAGES, 'GET', json => T.setTexts(json.ui))

render((
  <Router history={history}>
    <Switch>
      <Redirect exact from='/' to='/home' />
      <Route path="/" component={App} />
    </Switch>
  </Router>
), document.getElementById("root"))
