import React, { Component } from 'react';
import SectionContent from '../components/SectionContent';
import MashSettings from './MashSettings';
import BoilSettings from './BoilSettings';
import Button from '@material-ui/core/Button';
import { withStyles } from '@material-ui/core/styles';
import { withNotifier } from '../components/SnackbarNotification';
import ConfirmDialog from '../components/ConfirmDialog';
import {
  START_PUMP, STOP_PUMP,
  GET_ACTIVE_STATUS, START_BREW, UNLOCK_STEP_BREW,
  NEXT_STEP_BREW, STOP_BREW, RESUME_BREW,
  ExecuteRestCall, CHANGE_BOIL_PERCENTAGE,
  START_BOIL, START_TUNING
} from '../constants/Endpoints';
import { getDateTime, pad } from '../components/Utils';
import ResponsiveContainer from 'recharts/lib/component/ResponsiveContainer';
import LineChart from 'recharts/lib/chart/LineChart';
import Line from 'recharts/lib/cartesian/Line';
import XAxis from 'recharts/lib/cartesian/XAxis';
import YAxis from 'recharts/lib/cartesian/YAxis';
import CartesianGrid from 'recharts/lib/cartesian/CartesianGrid';
import Tooltip from 'recharts/lib/component/Tooltip';
import Typography from '@material-ui/core/Typography';
import Slider from '@material-ui/lab/Slider';
import Divider from '@material-ui/core/Divider';
import Card from '@material-ui/core/Card';
import CardContent from '@material-ui/core/CardContent';
import Grid from '@material-ui/core/Grid';
import Paper from '@material-ui/core/Paper';
import BrewStatusGadget from '../components/BrewStatusGadget'
import { withSnackbar } from 'notistack';

const styles = theme => ({
  button: {
    margin: theme.spacing.unit,
  },
  input: {
    display: 'none',
  },
  gadgetCard: {
    background: "#303030",
  },
  slider: {
    padding: '22px 0px',
  },
  sliderThumb: {
    background: '#fff',
  },
  trackBefore: {
    background: '#536dfe',
  },
  trackAfter: {
    background: '#fff',
  },
  brewSettingsCard: {
    background: "#303030",
  },
  chartCard: {
    background: "#262626",
  },
});


let interval, timerProgress;

class Brew extends Component {
  constructor(props) {
    super(props)
    this.getStatus();
    this.state = {
      status: { temperature: '-' },
      data: [],
      progressCompleted: 0,
      confirmDialogOpen: false,
      boilPower: 0,
      activeMashStepName: "",
      activeBoilStepName: "",
      statusInitialized: false
    }
    interval = setInterval(() => {
      this.getStatus();
    }, 2000);
    timerProgress = setInterval(() => {
      this.brewProgress();
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
        data: [...splice_data, { name: time, Target: this.state.status.target_temperature, Current: this.state.status.temperature }],
      })

      if (this.state.boilPower == 0)
        this.setState({
          boilPower: this.state.status.boil_power_percentage
        })

      const action = (key) => (
        <Button color="secondary" onClick={() => { this.props.closeSnackbar(key) }}>
          {'Dismiss'}
        </Button>
      );

      if (this.state.statusInitialized) {
        if (this.getActiveStep() == "Mash" && this.state.status.active_mash_step_name !== "" && this.state.activeMashStepName !== this.state.status.active_mash_step_name) {
          this.state.activeMashStepName = this.state.status.active_mash_step_name;
          this.props.enqueueSnackbar("Mash Step: " + this.state.activeMashStepName, {
            persist: true,
            action,
          });
        }
        if (this.getActiveStep() == "Boil" && this.state.status.active_boil_step_name !== "" && this.state.activeBoilStepName !== this.state.status.active_boil_step_name) {
          this.state.activeBoilStepName = this.state.status.active_boil_step_name;
          this.props.enqueueSnackbar("Boil Step: " + this.state.activeBoilStepName, {
            persist: true,
            action,
          });
        }
      }
      this.setState({ statusInitialized: true })
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
    if (this.state.status.brew_started !== 1 || this.state.status.end_time <= 0) {
      this.setState({
        countdown: '00:00:00',
        progressCompleted: 0
      })
      return;
    }
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
      hours %= 24; minutes %= 60; seconds %= 60;
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

  actionBrew = (message, url, callback) => {
    if (message !== '') {
      this.setState({
        confirmDialogOpen: true,
        confirmDialogMessage: message,
        confirmAction: (confirm) => {
          if (confirm) {
            ExecuteRestCall(url, 'POST', (json) => { this.setState({ status: json }) }, null, this.props)
            if (callback) { callback() }
          }
          this.setState({ confirmDialogOpen: false })
        }
      });
    }
    else {
      ExecuteRestCall(url, 'POST', (json) => { this.setState({ status: json }) }, null, this.props)
      if (callback) { callback() }
    }
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
      <SectionContent title="">
        {this.state.status.active_step === 0 && this.state.status.brew_started === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => {
              this.actionBrew('Do you want start brew? Check if you\'ve secure water volume at kettle.', START_BREW,
                () => { this.props.raiseNotification('Anti Cavitation test started.') })
            }}>Start</Button> : null}
        {this.state.status.active_step > 0 && this.state.status.brew_started === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => {
              this.actionBrew('Do you want resume brew? Check if you\'ve secure water volume at kettle.', RESUME_BREW,
                () => { this.props.raiseNotification('Anti Cavitation test started.') })
            }}>Resume</Button> : null}
        {this.state.status.active_step === 1 && this.state.status.brew_started === 1 && this.state.status.step_locked === 1 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew('Do you want unlock the current step?', UNLOCK_STEP_BREW) }}>Unlock Step</Button> : null}
        {this.state.status.active_step > 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew('Do you want stop brew?', STOP_BREW, () => { this.setState({ data: [] }) }) }}>Stop</Button> : null}
        {this.state.status.active_step === 1 && this.state.status.brew_started === 1 && this.state.status.pid_tuning === 0 && this.state.status.step_locked === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew('Do you want skip the current step?', NEXT_STEP_BREW) }}>Next Step</Button> : null}
        {this.state.status.active_step === 1 && this.state.status.brew_started === 1 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.startTuning() }}> {this.state.status.pid_tuning === 1 ? "Tuning..." : "PID Tune"}</Button> : null}
        {this.state.status.active_step === 0 && this.state.status.brew_started === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew('Do you want start boil?', START_BOIL) }}>Boil</Button> : null}
        <Button variant="contained" color="secondary" className={classes.button}
          onClick={() => { this.actionBrew('', this.state.status.pump_on ? STOP_PUMP : START_PUMP) }}>
          Pump</Button>

        <Divider />
        <Card className={classes.gadgetCard}>
          <CardContent>
            <BrewStatusGadget
              Temperature={this.state.status.temperature}
              TargetTemperature={this.state.status.target_temperature}
              BoilTemperature={this.state.status.boil_target_temperature}
              PWM={this.state.status.pwm}
              Progress={this.state.progressCompleted}
              ActiveStep={this.getActiveStep()}
              BoilTime={this.state.status.boil_time}
              StartTime={this.state.status.start_time > 0 ? getDateTime(this.state.status.start_time).toLocaleTimeString() : null}
              EndTime={this.state.status.end_time > 0 ? getDateTime(this.state.status.end_time).toLocaleTimeString() : null}
              CountDown={this.state.countdown}
              PumpOn={this.state.status.pump_on}
            />
          </CardContent>
        </Card>
        <Divider />
        {this.state.status.active_step === 2 && this.state.status.brew_started === 1 ?
          <Grid item xs={12}>
            <Grid container justify="center" spacing={16}></Grid>
            <Grid item>
              <Card className={this.props.className}>
                <CardContent>
                  <Typography color="textSecondary" variant="subtitle1" gutterBottom>Boil Power {this.state.boilPower}%</Typography>
                  <Slider
                    classes={{
                      container: classes.slider, thumb: classes.sliderThumb, trackBefore: classes.trackBefore,
                      trackAfter: classes.trackAfter,
                    }}
                    step={1}
                    value={this.state.boilPower}
                    onChange={this.handleChangeBoilPowerPercentage}
                    onDragEnd={this.handleSaveChangeBoilPowerPercentage}
                  />
                </CardContent>
              </Card>
            </Grid>
          </Grid>
          : null}
        <Card className={classes.chartCard}>
          <CardContent>
            <ResponsiveContainer width="90%" height={320} >
              <LineChart data={this.state.data} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
                <XAxis dataKey="name" tick={{ fill: '#707070', fontSize: "12px", fontFamily: "Montserrat", }} />
                <YAxis yAxisId="left" tick={{ fill: '#707070', fontSize: "12px", fontFamily: "Montserrat", }} />
                <CartesianGrid vertical={false} strokeDasharray="3 3" />
                <Tooltip />
                <Line type="monotone" yAxisId="left" dataKey="Target" stroke="#f9a825" dot={null} />
                <Line type="monotone" yAxisId="left" dataKey="Current" stroke="#c62828" dot={null} activeDot={{ r: 10 }} />
              </LineChart>
            </ResponsiveContainer>
          </CardContent>
        </Card>
        <Paper className={classes.brewSettingsCard}>
          <Grid container justify="center">
            <Grid item>
              <Grid container >
                <Grid item>
                  <MashSettings listOnly={true} brewDay={true} selectedIndex={this.state.status.active_mash_step_index} />
                </Grid>
                <Grid item>
                  <BoilSettings listOnly={true} brewDay={true} selectedIndex={this.state.status.active_boil_step_index} />
                </Grid>
              </Grid>
            </Grid>
          </Grid>
        </Paper>
        <ConfirmDialog
          confirmAction={this.state.confirmAction}
          confirmDialogOpen={this.state.confirmDialogOpen}
          confirmDialogMessage={this.state.confirmDialogMessage}
        />

      </SectionContent >
    )
  }
}

export default withSnackbar(withNotifier(withStyles(styles)(Brew)));