import React, { Component } from 'react';
import SectionContent from '../components/SectionContent';
import MashSettings from './MashSettings';
import BoilSettings from './BoilSettings';
import Button from '@material-ui/core/Button';
import { withStyles } from '@material-ui/core/styles';
import ConfirmDialog from '../components/ConfirmDialog';
import {
  START_PUMP, STOP_PUMP,
  GET_ACTIVE_STATUS, START_BREW, UNLOCK_STEP_BREW,
  NEXT_STEP_BREW, STOP_BREW, RESUME_BREW,
  CHANGE_BOIL_PERCENTAGE,
  START_BOIL, START_TUNING, PAUSE_BREW
} from '../constants/Endpoints';
import { getDateTime, ExecuteRestCall } from '../components/Utils';
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
import PauseCircleFilled from '@material-ui/icons/PauseCircleFilled';
import PlayCircleFilledWhite from '@material-ui/icons/PlayCircleFilledWhite';
import Cancel from '@material-ui/icons/Cancel';
import Menu from '@material-ui/core/Menu';
import MenuItem from '@material-ui/core/MenuItem';
import PopupState, { bindTrigger, bindMenu } from 'material-ui-popup-state';
import IconButton from '@material-ui/core/IconButton';
import ArrowDropDown from '@material-ui/icons/ArrowDropDown';
import { withSnackbar } from 'notistack';

const styles = theme => ({
  button: {
    margin: theme.spacing.unit,
  },
  button_pump: {
    marginRight: theme.spacing.unit,
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
  pumpColor1: {
    color: "#447bd6",
  },
  pumpColor2: {
    color: "#5c94f2",
  },
});


let interval;

class Brew extends Component {
  constructor(props) {
    super(props)
    this.getStatus();
    this.state = {
      status: { temperature: '-' },
      data: [],
      confirmDialogOpen: false,
      boilPower: 0,
      activeStepName: "",
      statusInitialized: false,
      copyDialogMessage: false
    }
    interval = setInterval(() => {
      this.getStatus();
    }, 2000);
  }

  updateStatus() {
    if (this.state.status.active_step > 0 && this.state.status.brew_started == 1) {
      var now = getDateTime(this.state.status.time_now);
      var time = now.getHours() + ":" + now.getMinutes() + ":" + now.getSeconds()
      var splice_data = this.state.data;

      if (splice_data.length >= 200)
        splice_data.splice(0, 1);

      this.setState({
        data: [...splice_data, { name: time, Target: this.state.status.target_temperature, Current: this.state.status.temperature }],
      })

      if (this.state.boilPower == 0)
        this.setState({
          boilPower: this.state.status.boil_power_percentage
        })
    }

    if (this.state.statusInitialized) {
      this.notification(this.getActiveStep(), this.state.status.active_mash_step_name, "Mash")
      this.notification(this.getActiveStep(), this.state.status.active_boil_step_name, "Boil")

      if (this.getActiveStep() == "Stopped") {
        this.setState({ activeStepName: '-' })
      }
    }
    this.setState({ statusInitialized: true })
  }

  notification(getActiveStep, stepName, step) {
    if (getActiveStep === step && this.state.activeStepName !== stepName) {
      this.setState({ activeStepName: stepName });
      if (stepName !== "") {
        const action = (key) => (
          <Button color="Primary" onClick={() => { this.props.closeSnackbar(key) }}>
            {'Dismiss'}
          </Button>
        );
        this.props.enqueueSnackbar(step + " Step: " + stepName, {
          persist: true,
          action,
          variant: 'warning'
        });
      }
    }
  }

  getStatus() {
    ExecuteRestCall(GET_ACTIVE_STATUS, 'GET', (json) => {
      if (json != null && json != undefined && json != '')
        this.setState({ status: json }, this.updateStatus)
    }, null, null)
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
        this.props.enqueueSnackbar("Boiling power configured.", { variant: 'info', autoHideDuration: 2000, });
        return;
      }
      response.text().then(function (data) {
        throw Error(data);
      }).catch(error => {
        this.props.enqueueSnackbar(error.message, { variant: 'error', autoHideDuration: 2000, });
      });
    });
  };

  reportLog = (callback) => {
    this.setState({
      confirmDialogOpen: true,
      copyDialogMessage: true,
      confirmDialogMessage: JSON.stringify(this.state.status),
      confirmAction: (confirm) => {
        this.setState({ confirmDialogOpen: false })
        document.execCommand('copy');
      }
    });
    callback()
  }

  actionBrew = (message, url, callback) => {
    if (message !== '') {
      this.setState({
        confirmDialogOpen: true,
        copyDialogMessage: false,
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
                () => { this.props.enqueueSnackbar('Anti Cavitation test started.', { variant: 'info', autoHideDuration: 2000, }) })
            }}>Start</Button> : null}
        {this.state.status.active_step > 0 && this.state.status.brew_started === 1 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => {
              this.actionBrew('Do you want pause brew?', PAUSE_BREW)
            }}>Pause</Button> : null}
        {this.state.status.active_step > 0 && this.state.status.brew_started === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => {
              this.actionBrew('Do you want resume brew? Check if you\'ve secure water volume at kettle.', RESUME_BREW)
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
        {this.state.status.active_step === 0 && this.state.status.brew_started === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew('Do you want start boil?', START_BOIL) }}>Boil</Button> : null}
        <Button variant="contained" color="secondary" className={classes.button}
          onClick={() => { this.actionBrew('', this.state.status.pump_on ? STOP_PUMP : START_PUMP) }}>
          {this.state.status.pump_on ?
            <PlayCircleFilledWhite color="disabled" className={classes.button_pump} />
            :
            this.state.status.pump_is_resting ?
              <PauseCircleFilled size="small" color="disabled" className={classes.button_pump} /> :
              <Cancel size="small" color="disabled" className={classes.button_pump} />
          }
          PUMP
        </Button>
        <PopupState>
          {popupState => (
            <React.Fragment>
              <IconButton variant="contained" {...bindTrigger(popupState)}>
                <ArrowDropDown />
              </IconButton>
              <Menu {...bindMenu(popupState)}>
                <MenuItem key="placeholder" style={{ display: "none" }} />
                <MenuItem onClick={popupState.close}>Pump Prime</MenuItem>
                <MenuItem onClick={() => { this.reportLog(popupState.close) }}>Report Log</MenuItem>
              </Menu>
            </React.Fragment>
          )}
        </PopupState>
        <Divider />
        <Card className={classes.gadgetCard}>
          <CardContent>
            <BrewStatusGadget
              BrewStarted={this.state.status.brew_started}
              Temperature={this.state.status.temperature}
              TargetTemperature={this.state.status.target_temperature}
              BoilTemperature={this.state.status.boil_target_temperature}
              PWM={this.state.status.pwm}
              SpargePWM={this.state.status.sparge_pwm}
              ActiveStep={this.getActiveStep()}
              BoilTime={this.state.status.boil_time}
              StartTime={this.state.status.start_time > 0 ? this.state.status.start_time : null}
              EndTime={this.state.status.end_time > 0 ? this.state.status.end_time : null}
              PumpOn={this.state.status.pump_on}
              ActiveStepName={this.state.activeStepName}
              StepLocked={this.state.status.step_locked > 0}
              PumpIsResting={this.state.status.pump_is_resting > 0}
              SpargeTemperature={this.state.status.sparge_temperature}
              SpargeTargetTemperature={this.state.status.sparge_target_temperature}
              EnableSparge={this.state.status.enable_sparge}
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
          copy={this.state.copyDialogMessage}
        />
      </SectionContent >
    )
  }
}

export default withSnackbar(withStyles(styles)(Brew));