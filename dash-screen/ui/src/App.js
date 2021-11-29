import React, { Component }       from 'react';
import Dashboard                  from './dashboard/Dashboard';

import {
  HashRouter as Router,
  Route
} from 'react-router-dom'

import './App.scss';

class App extends Component {

  render() {
    return (
      <div className="app__main-app-container" id="app-container">
        <Router>
          <div className="app__main-conent-container">
            <Route path="/" component={() => <Dashboard />}/>
          </div>
        </Router>
      </div>
    );
  }
}

export default App;