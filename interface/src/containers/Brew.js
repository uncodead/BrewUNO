import React, { Component } from 'react';
import SectionContent from '../components/SectionContent';
import MashSettings from './MashSettings';
import BoilSettings from './BoilSettings';
import Button from '@material-ui/core/Button';
import { withStyles } from '@material-ui/core/styles';
import { withNotifier } from '../components/SnackbarNotification';
import LinearProgress from '@material-ui/core/LinearProgress';
import ConfirmDialog from '../components/ConfirmDialog';
import { GET_ACTIVE_STATUS, START_BREW, NEXT_STEP_BREW, STOP_BREW, RESUME_BREW, ExecuteRestCall, CHANGE_BOIL_PERCENTAGE, START_BOIL, START_TUNING }
  from '../constants/Endpoints';
import { getDateTime, pad } from '../components/Utils';
import ResponsiveContainer from 'recharts/lib/component/ResponsiveContainer';
import LineChart from 'recharts/lib/chart/LineChart';
import Line from 'recharts/lib/cartesian/Line';
import XAxis from 'recharts/lib/cartesian/XAxis';
import YAxis from 'recharts/lib/cartesian/YAxis';
import CartesianGrid from 'recharts/lib/cartesian/CartesianGrid';
import Tooltip from 'recharts/lib/component/Tooltip';
import Legend from 'recharts/lib/component/Legend';
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';
import Typography from '@material-ui/core/Typography';
import Slider from '@material-ui/lab/Slider';

const styles = theme => ({
  button: {
    margin: theme.spacing.unit,
  },
  input: {
    display: 'none',
  },
  root: {

  },
  slider: {
    padding: '22px 0px'
  },
});

let interval, timerProgress;

class Brew extends Component {
  constructor(props) {
    super(props)
    this.getStatus();
    this.state = {
      status: {},
      data: [],
      progressCompleted: 0,
      confirmDialogOpen: false,
      boilPower: 0
    }
    this.setStatusInterval();
    timerProgress = setInterval(() => {
      this.brewProgress();
    }, 1000);
  }

  setStatusInterval() {
    interval = setInterval(() => {
      this.getStatus();
    }, 1000);
  }

  updateStatus() {
    if (this.state.status.active_step > 0 && this.state.status.brew_started == 1) {
      var now = getDateTime(this.state.status.time_now);
      var time = now.getHours() + ":" + now.getMinutes() + ":" + now.getSeconds()
      var splice_data = this.state.data;
      if (splice_data.length >= 100)
        splice_data.splice(0, 1);
      this.setState({
        data: [...splice_data, { name: time, Target: this.state.status.target_temperature, Current: this.state.status.temperature, PWM: this.state.status.pwm / 100 }],
      })
      if (this.state.boilPower == 0) {
        this.setState({
          boilPower: this.state.status.boil_power_percentage
        })
      }
    }
  }

  getStatus() {
    ExecuteRestCall(GET_ACTIVE_STATUS, 'GET', (json) => {
      if (json != null && json != undefined && json != '')
        this.setState({ status: json }, this.updateStatus)
    }, null, this.props)
  }

  getActiveStep() {
    switch (this.state.status.active_step) {
      case 1:
        return "Mash"
      case 2:
        return "Boil"
      default:
        return "Stopped"
    }
  }

  brewProgress() {
    if (this.state.status.brew_started != 1)
      return;

    var dateEntered = getDateTime(this.state.status.end_time);
    var now = new Date();
    var difference = dateEntered.getTime() - now.getTime();

    if (difference <= 0) {
      this.setState({
        countdown: '00:00:00',
        progressCompleted: 100
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
        countdown: pad(hours, 2) + ':' + pad(minutes, 2) + ':' + pad(seconds, 2),
        progressCompleted: Math.round(((now - getDateTime(this.state.status.start_time)) / (getDateTime(this.state.status.end_time) - getDateTime(this.state.status.start_time))) * 100)
      })
    }
  }

  handleChangeBoilPowerPercentage = (event, value) => {
    this.setState({ boilPower: value });
  }

  handleSaveChangeBoilPowerPercentage = () => {
    fetch(CHANGE_BOIL_PERCENTAGE, {
      method: 'POST',
      body: JSON.stringify({ "boil_power_percentage": this.state.boilPower }),
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json',
      },
    }).then(response => {
      if (response.ok) {
        this.props.raiseNotification("Boiling power configured.");
        return;
      }
      response.text().then(function (data) {
        throw Error(data);
      }).catch(error => {
        this.props.raiseNotification(error.message);
      });
    });
  };

  actionBrew = (message, url) => {
    this.setState({
      confirmDialogOpen: true,
      confirmDialogMessage: message,
      confirmAction: (confirm) => {
        if (confirm)
          ExecuteRestCall(url, 'POST', (json) => { this.setState({ status: json }) }, null, this.props)
        this.setState({ confirmDialogOpen: false })
      }
    });
  }

  startTuning = () => {
    if (this.state.status.pid_tuning === 0) {
      this.actionBrew('Do you want start PID tune?', START_TUNING)
    }
  }

  componentWillUnmount() {
    clearInterval(interval);
  }

  render() {
    const { classes } = this.props;
    return (
      <SectionContent title="Brew">

        {this.state.status.active_step === 0 && this.state.status.brew_started === 0 ?
          <Button variant="contained" color="primary" className={classes.button}
            onClick={() => { this.actionBrew('Do you want start brew?', START_BREW) }}>Start</Button> : null}
        {this.state.status.active_step > 0 && this.state.status.brew_started === 0 ?
          <Button variant="contained" color="primary" className={classes.button}
            onClick={() => { this.actionBrew('Do you want resume brew?', RESUME_BREW) }}>Resume</Button> : null}
        {this.state.status.active_step > 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew('Do you want stop brew?', STOP_BREW) }}>Stop</Button> : null}
        {this.state.status.active_step === 1 && this.state.status.brew_started === 1 && this.state.status.pid_tuning === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew('Do you want skip the current step?', NEXT_STEP_BREW) }}>Next Step</Button> : null}
        {this.state.status.active_step === 1 && this.state.status.brew_started === 1 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.startTuning() }}> {this.state.status.pid_tuning === 1 ? "Tuning..." : "PID Tune"}</Button> : null}
        {this.state.status.active_step === 0 && this.state.status.brew_started === 0 ?
          <Button variant="contained" color="primary" className={classes.button}
            onClick={() => { this.actionBrew('Do you want start boil?', START_BOIL) }}>Boil</Button> : null}

        <ResponsiveContainer width="100%" height={320} >
          <LineChart data={this.state.data} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
            <XAxis dataKey="name" />
            <YAxis yAxisId="left" />
            <YAxis yAxisId="right" orientation="right" />
            <CartesianGrid vertical={false} strokeDasharray="3 3" />
            <Tooltip />
            <Legend />
            <Line type="monotone" yAxisId="left" dataKey="Target" stroke="#82ca9d" dot={null} />
            <Line type="monotone" yAxisId="left" dataKey="Current" stroke="#8884d8" dot={null} activeDot={{ r: 10 }} />
            <Line type="monotone" yAxisId="left" dataKey="PWM" stroke="#FF0000" dot={null} />
          </LineChart>
        </ResponsiveContainer>

        <SectionContent title="Progress">
          <LinearProgress variant="determinate" value={this.state.progressCompleted} />
        </SectionContent>

        <SectionContent title="Boiling power %">
          <div className={classes.root}>
            <Slider
              classes={{ container: classes.slider }}
              value={this.state.boilPower}
              onChange={this.handleChangeBoilPowerPercentage}
              onDragEnd={this.handleSaveChangeBoilPowerPercentage}
            />
          </div>
          <Typography id="label">{this.state.boilPower}%</Typography>
        </SectionContent>

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
                  ? getDateTime(this.state.status.start_time).toLocaleString()
                  : null
              } />
            </ListItem>
            <ListItem button>
              <ListItemText primary="End of step" secondary={
                this.state.status.end_time > 0
                  ? getDateTime(this.state.status.end_time).toLocaleString()
                  : null
              } />
            </ListItem>
            <ListItem button>
              <ListItemText primary="Countdown" secondary={this.state.countdown} />
            </ListItem>
          </List>
        </SectionContent>

        <ConfirmDialog
          confirmAction={this.state.confirmAction}
          confirmDialogOpen={this.state.confirmDialogOpen}
          confirmDialogMessage={this.state.confirmDialogMessage}
        />

        <MashSettings listOnly={true} brewDay={true} selectedIndex={this.state.status.active_mash_step_index} />
        <BoilSettings listOnly={true} brewDay={true} selectedIndex={this.state.status.active_boil_step_index} />
      </SectionContent >
    )
  }
}

export default withNotifier(withStyles(styles)(Brew));