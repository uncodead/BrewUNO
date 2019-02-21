import React, { Component } from 'react';
import SectionContent from '../components/SectionContent';
import MashSettings from './MashSettings';
import BoilSettings from './BoilSettings';
import Button from '@material-ui/core/Button';
import { withStyles } from '@material-ui/core/styles';
import { withNotifier } from '../components/SnackbarNotification';
import { GET_ACTIVE_STATUS, START_BREW, NEXT_STEP_BREW, STOP_BREW, ExecuteRestCall } from '../constants/Endpoints';

import ResponsiveContainer from 'recharts/lib/component/ResponsiveContainer';
import LineChart from 'recharts/lib/chart/LineChart';
import Line from 'recharts/lib/cartesian/Line';
import XAxis from 'recharts/lib/cartesian/XAxis';
import YAxis from 'recharts/lib/cartesian/YAxis';
import CartesianGrid from 'recharts/lib/cartesian/CartesianGrid';
import Tooltip from 'recharts/lib/component/Tooltip';
import Legend from 'recharts/lib/component/Legend';

import Dialog from '@material-ui/core/Dialog';
import DialogActions from '@material-ui/core/DialogActions';
import DialogContent from '@material-ui/core/DialogContent';
import DialogContentText from '@material-ui/core/DialogContentText';
import DialogTitle from '@material-ui/core/DialogTitle';

import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemIcon from '@material-ui/core/ListItemIcon';
import ListItemText from '@material-ui/core/ListItemText';

const styles = theme => ({
  button: {
    margin: theme.spacing.unit,
  },
  input: {
    display: 'none',
  },
});

let interval, timer;

class Brew extends Component {

  constructor(props) {
    super(props)
    this.getStatus();
    this.state = {
      status: {},
      data: [],
      fetched: false,
      confirmDialogOpen: false,
    }

    interval = setInterval(() => {
      this.getStatus();
    }, 5000);

    timer = setInterval(() => {
      this.timeBetweenDates();
    }, 1000);
  }

  updateStatus() {
    if (this.state.status.active_step > 0) {
      if (!this.state.fetched) {
        var json = eval("[" + this.state.status.temperatures + "]")
        var firstValues = [];
        json.forEach(element => {
          firstValues.push({ name: '', Target: element.t, Current: element.c })
        });
        this.setState({
          data: firstValues,
          fetched: true
        })
      }
      else {
        if (this.state.data.length >= 100) {
          var array = this.state.data
          array.splice(0, 1);
          this.setState({ data: array });
        }
        this.setState({
          data: [...this.state.data, { name: '', Target: this.state.status.target_temperature, Current: this.state.status.temperature }]
        })
      }
    }
  }

  getStatus() {
    ExecuteRestCall(GET_ACTIVE_STATUS, 'GET', (json) => { this.setState({ status: json }, this.updateStatus) }, null, this.props)
  }

  confirmHandleClose = (confirm) => {
    if (confirm === true)
      this.state.confirmAction();

    this.setState({
      confirmDialogOpen: false
    });
  };

  componentWillUnmount() {
    clearInterval(interval);
  }

  getActiveStep() {
    switch (this.state.status.active_step) {
      case 1:
        return "Mash";
      case 2:
        return "Boil"
      default:
        return "stopped"
    }
  }

  getDateTime(seconds) {
    var date = new Date(0);
    date.setUTCSeconds(seconds);
    return date;
  }

  timeBetweenDates() {
    debugger
    var dateEntered = this.getDateTime(this.state.status.end_time);
    var now = new Date();
    var difference = dateEntered.getTime() - now.getTime();

    if (difference <= 0) {
      this.setState({
        countdown: '00:00:00'
      })
    } else {
      var seconds = Math.floor(difference / 1000);
      var minutes = Math.floor(seconds / 60);
      var hours = Math.floor(minutes / 60);
      var days = Math.floor(hours / 24);

      hours %= 24;
      minutes %= 60;
      seconds %= 60;

      this.setState({
        countdown: this.pad(hours, 2) + ':' + this.pad(minutes, 2) + ':' + this.pad(seconds, 2)
      })
    }
  }

  pad(n, width, z) {
    z = z || '0';
    n = n + '';
    return n.length >= width ? n : new Array(width - n.length + 1).join(z) + n;
  }

  render() {
    const { classes } = this.props;
    return (
      <SectionContent title="Brew">
        <Button variant="contained" color="primary" className={classes.button}
          disabled={this.state.status.active_step != 0}
          onClick={() => {
            this.setState({
              confirmDialogOpen: true,
              confirmDialogMessage: 'Do you want to start brew?',
              confirmAction: () => {
                ExecuteRestCall(START_BREW, 'POST', (json) => { this.setState({ status: json }) }, this.props)
                this.setState({ data: [] })
              }
            });
          }
          }
        >
          Start
      </Button>
        <Button variant="contained" color="secondary" className={classes.button}
          disabled={this.state.status.active_step <= 0}
          onClick={() => {
            this.setState({
              confirmDialogOpen: true,
              confirmDialogMessage: 'Do you want to stop brew?',
              confirmAction: () => {
                ExecuteRestCall(STOP_BREW, 'POST', (json) => { this.setState({ status: json }) }, this.props)
                this.setState({ data: [] })
              }
            });
          }
          }
        >
          Stop
      </Button>
        <Button variant="contained" color="secondary" className={classes.button}
          disabled={this.state.status.active_step != 1}
          onClick={() => {
            this.setState({
              confirmDialogOpen: true,
              confirmDialogMessage: 'Do you want to skip the current step?',
              confirmAction: () => { ExecuteRestCall(NEXT_STEP_BREW, 'POST', (json) => { this.setState({ status: json }) }, this.props) }
            });
          }
          }
        >
          Next Step
      </Button>
        <Dialog
          open={this.state.confirmDialogOpen}
          onClose={this.confirmHandleClose}
          aria-labelledby="alert-dialog-title"
          aria-describedby="alert-dialog-description"
        >
          <DialogTitle id="alert-dialog-title">{"Confirmation Dialog"}</DialogTitle>
          <DialogContent>
            <DialogContentText id="alert-dialog-description">
              {this.state.confirmDialogMessage}
            </DialogContentText>
          </DialogContent>
          <DialogActions>
            <Button onClick={() => { this.confirmHandleClose(false) }} color="primary">
              No
            </Button>
            <Button onClick={() => { this.confirmHandleClose(true) }} color="primary" autoFocus>
              Yes
            </Button>
          </DialogActions>
        </Dialog>

        <ResponsiveContainer width="100%" height={320} >
          <LineChart data={this.state.data} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
            <XAxis dataKey="name" />
            <YAxis />
            <CartesianGrid vertical={false} strokeDasharray="3 3" />
            <Tooltip />
            <Legend />
            <Line type="monotone" dataKey="Target" stroke="#82ca9d" dot={null} />
            <Line type="monotone" dataKey="Current" stroke="#8884d8" dot={null} activeDot={{ r: 10 }} />
          </LineChart>
        </ResponsiveContainer>

        <SectionContent title="Status">
          <List component="nav">
            <ListItem button>
              <ListItemText primary="Active Step" secondary={this.getActiveStep()} />
            </ListItem>
            <ListItem button>
              <ListItemText primary="Boil time" secondary={this.state.status.boil_time} />
            </ListItem>
            <ListItem button>
              <ListItemText primary="Start of step" secondary={
                this.state.status.start_time > 0
                  ? this.getDateTime(this.state.status.start_time).toLocaleString()
                  : null
              } />
            </ListItem>
            <ListItem button>
              <ListItemText primary="End of step" secondary={
                this.state.status.end_time > 0
                  ? this.getDateTime(this.state.status.end_time).toLocaleString()
                  : null
              } />
            </ListItem>
            <ListItem button>
              <ListItemText primary="Countdown" secondary={this.state.countdown} />
            </ListItem>
          </List>
        </SectionContent>

        <MashSettings listOnly={true} brewDay={true} selectedIndex={this.state.status.active_mash_step_index} />
        <BoilSettings listOnly={true} brewDay={true} selectedIndex={this.state.status.active_boil_step_index} />
      </SectionContent>
    )
  }
}

export default withNotifier(withStyles(styles)(Brew));