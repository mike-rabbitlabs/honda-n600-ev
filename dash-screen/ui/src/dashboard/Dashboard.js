import React, { Component }       from 'react';
import io                         from 'socket.io-client';

import "./Dashboard.scss";

const INITIAL_BATTERY_STATE = [
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
]

class Dashboard extends Component {

  constructor(props) {
    super(props);

    this.state            = {
      bms_state: INITIAL_BATTERY_STATE
    }
  }

  componentDidMount() {
    this.socket = io();

    this.socket.on('healthcheck', () => {
      console.log('healthcheck');
    })

    this.socket.on('bms-reading', data => {
      this.setState({ bms_state: data })
    })
  }

  calcSumVoltate() {
    let isComplete = true;
    let total = 0;
    for (let i = 0; i < this.state.bms_state.length; i++) {
      if (this.state.bms_state[i] === 0) {
        return "- V";
      }
      total += this.state.bms_state[i];
    }
    return `${total.toFixed(2)}V`
  }

  testRender() {
    return <div>DashboardComponent works!</div>
  }

  renderBatteries() {
    let batteries = [];
    for (let i = 0; i < this.state.bms_state.length; i++) {
      batteries.push(<div className="battery__container">
        <div className="progress" style={{ width: `${(this.state.bms_state[i]/4.2)*100}%`}}></div>
        <div className="title">{ this.state.bms_state[i].toFixed(2) }</div>
      </div>)
    }
    return batteries;
  }

  render() {
    return (
      <div className="dashboard__container">
          <div className="bms-container">
            <div className="title-bar">
              <div>BMS</div>
              <div>{ this.calcSumVoltate() }</div>
            </div>
            <div className="bms-body">
              {this.renderBatteries() }
            </div>
          </div>
      </div>
    );
  }
}

export default Dashboard;