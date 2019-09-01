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
  START_BOIL, PAUSE_BREW,
  START_ANTICAVITATION
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
import Slider from 'rc-slider';
import 'rc-slider/assets/index.css';
import Divider from '@material-ui/core/Divider';
import Card from '@material-ui/core/Card';
import CardContent from '@material-ui/core/CardContent';
import Grid from '@material-ui/core/Grid';
import Paper from '@material-ui/core/Paper';
import BrewStatusGadget from '../components/BrewStatusGadget'
import PauseCircleFilled from '@material-ui/icons/PauseCircleFilled';
import Stop from '@material-ui/icons/Stop'
import LockOpen from '@material-ui/icons/LockOpen'
import Menu from '@material-ui/core/Menu';
import MenuItem from '@material-ui/core/MenuItem';
import PopupState, { bindTrigger, bindMenu } from 'material-ui-popup-state';
import IconButton from '@material-ui/core/IconButton';
import ArrowDropDown from '@material-ui/icons/ArrowDropDown';
import SkipNext from '@material-ui/icons/SkipNext';
import SvgIcon from '@material-ui/core/SvgIcon';
import { withSnackbar } from 'notistack';
import BrewStyles from '../style/BrewStyle'
import IntText from '../components/IntText'

let interval;

class Brew extends Component {
  constructor(props) {
    super(props)
    this.getStatus();
    this.state = {
      status: { temperature: '' },
      data: [],
      confirmDialogOpen: false,
      boilPower: 0,
      activeStepName: "",
      statusInitialized: false,
      copyDialogMessage: false
    }
    interval = setInterval(() => { this.getStatus(); }, 5000);
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
        boilPower: this.state.status.boil_power_percentage
      })
    }

    if (this.state.statusInitialized) {
      this.notification(this.getActiveStep(), this.state.status.active_mash_step_name + ' ' + this.state.status.active_mash_step_sufix_name, <IntText text="Mash" />)
      this.notification(this.getActiveStep(), this.state.status.active_boil_step_name, <IntText text="Boil" />)

      if (this.getActiveStep() == "Stopped") {
        this.setState({ activeStepName: '-' })
      }
    }
    this.setState({ statusInitialized: true })
  }

  notification(getActiveStep, stepName, step) {
    if (getActiveStep === step && this.state.activeStepName !== stepName) {
      this.setState({ activeStepName: stepName });
      if (stepName !== "" && stepName !== undefined) {
        const action = (key) => (
          <Button color="Primary" onClick={() => { this.props.closeSnackbar(key) }}>
            <IntText text="Dismiss" />
          </Button>
        );
        this.props.enqueueSnackbar(stepName, {
          persist: true,
          action,
          variant: 'warning'
        });
      }
    }
  }

  getStatus() {
    ExecuteRestCall(GET_ACTIVE_STATUS, 'GET', json => {
      if (json != null && json != undefined && json != '')
        this.setState({ status: json }, this.updateStatus)
    })
  }

  getActiveStep() {
    switch (this.state.status.active_step) {
      case 1:
        return <IntText text="Mash" />
      case 2:
        return <IntText text="Boil" />
      case 3:
        return <IntText text="PumpPrime" />
      default:
        return <IntText text="Stopped" />
    }
  }

  handleChangeBoilPowerPercentage = (value) => {
    this.setState({ boilPower: value });
  }

  handleSaveChangeBoilPowerPercentage = (value) => {
    this.setState({ boilPower: value });
    fetch(CHANGE_BOIL_PERCENTAGE, {
      method: 'POST',
      body: JSON.stringify({ "boil_power_percentage": this.state.boilPower }),
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json',
      },
    })
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
            if (callback) callback()
          }
          this.setState({ confirmDialogOpen: false })
        }
      });
    }
    else {
      ExecuteRestCall(url, 'POST', (json) => { this.setState({ status: json }) }, null, this.props)
      if (callback) callback()
    }
  }

  componentDidMount() {
    this.getStatus();
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
              this.actionBrew(<IntText text="Brew.StartConfirmation" />, START_BREW)
            }}><IntText text="Start" /></Button> : null}
        {this.state.status.active_step > 0 && this.state.status.brew_started === 1 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => {
              this.actionBrew(<IntText text="Brew.PauseConfirmation" />, PAUSE_BREW)
            }}><PauseCircleFilled size="small" color="action" className={classes.button_icons} /></Button>
          : null}
        {this.state.status.active_step > 0 && this.state.status.active_step !== 3 && this.state.status.brew_started === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => {
              this.actionBrew(<IntText text="Brew.ResumeConfirmation" />, RESUME_BREW)
            }}><IntText text="Resume" /></Button>
          : null}
        {this.state.status.active_step > 0 && this.state.status.active_step !== 3 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew(<IntText text="Brew.StopConfirmation" />, STOP_BREW, () => { this.setState({ data: [] }) }) }}><Stop size="small" color="action" className={classes.button_icons} /></Button>
          : null}
        {this.state.status.active_step === 1 && this.state.status.brew_started === 1 && this.state.status.pid_tuning === 0 && this.state.status.step_locked === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew(<IntText text="Brew.NextConfirmation" />, NEXT_STEP_BREW) }}><SkipNext size="small" color="action" className={classes.button_icons} /></Button>
          : null}
        {this.state.status.active_step === 1 && this.state.status.brew_started === 1 && this.state.status.step_locked === 1 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew(<IntText text="Brew.UnLockconfirmation" />, UNLOCK_STEP_BREW) }}><LockOpen size="small" color="action" className={classes.button_icons} /></Button>
          : null}
        {this.state.status.active_step === 0 && this.state.status.brew_started === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew(<IntText text="Brew.BoilConfirmation" />, START_BOIL) }}><IntText text="Boil" /></Button>
          : null}
        <Button variant="contained" color="secondary" className={classes.button}
          onClick={() => { this.actionBrew('', this.state.status.pump_on ? STOP_PUMP : START_PUMP) }}>
          {this.state.status.pump_on ?
            <SvgIcon {...this.props} color="action">
              <path d="M20 13.641c0 2-.779 4.109-2.34 5.67a7.992 7.992 0 0 1-11.32-.002C4.78 17.75 4 15.641 4 13.641A8.02 8.02 0 0 1 6.34 8L12 2.35 17.66 8A8.016 8.016 0 0 1 20 13.641z" />
            </SvgIcon>
            : //pause icon
            this.state.status.pump_is_resting ?
              <SvgIcon {...this.props} color="disabled">
                <path d="M17.66 8L12 2.35 6.34 8A8.02 8.02 0 0 0 4 13.641c0 2 .78 4.109 2.34 5.668a7.987 7.987 0 0 0 11.32.001c1.561-1.561 2.34-3.67 2.34-5.67S19.221 9.56 17.66 8zm-6.359 9.922H8.604V8.855h2.697v9.067zm4.068 0h-2.682V8.855h2.682v9.067z" />
              </SvgIcon>
              : //stop icon
              <SvgIcon {...this.props} color="disabled">
                <path d="M17.66 8L12 2.35 6.34 8A8.02 8.02 0 0 0 4 13.641c0 2 .78 4.109 2.34 5.67a7.98 7.98 0 0 0 11.32 0c1.561-1.561 2.34-3.67 2.34-5.67A8.016 8.016 0 0 0 17.66 8zM6 14c.01-2 .62-3.27 1.76-4.4L12 5.27l4.24 4.38C17.38 10.77 17.99 12 18 14c0 0-.313 5.5-6 5.625C6.737 19.74 6 14 6 14z" />
              </SvgIcon>
          }

        </Button>
        <PopupState>
          {popupState => (
            <React.Fragment>
              <IconButton variant="contained" {...bindTrigger(popupState)} size="small" className={classes.button_pop}>
                <ArrowDropDown />
              </IconButton>
              <Menu {...bindMenu(popupState)}>
                <MenuItem key="placeholder" style={{ display: "none" }} />
                {this.state.status.active_step === 0 ?
                  <MenuItem onClick={() => {
                    this.actionBrew(<IntText text="Brew.PumpPrimeConfirmation" />, START_ANTICAVITATION,
                      () => {
                        this.props.enqueueSnackbar(<IntText text="Brew.PumpPrimeStarted" />, { variant: 'info', autoHideDuration: 2000, })
                        popupState.close()
                      })
                  }}><IntText text="PumpPrime" /></MenuItem>
                  : null}
                <MenuItem onClick={() => { this.reportLog(popupState.close) }}><IntText text="Brew.ReportLog" /></MenuItem>
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
              TargetTemperature={this.state.status.target_temperature > 0 ? this.state.status.target_temperature : ''}
              BoilTemperature={this.state.status.boil_target_temperature}
              PWM={this.state.status.pwm_percentage}
              SpargePWM={this.state.status.sparge_pwm_percentage}
              ActiveStep={this.getActiveStep()}
              BoilTime={this.state.status.boil_time}
              StartTime={this.state.status.start_time > 0 ? this.state.status.start_time : null}
              EndTime={this.state.status.end_time > 0 ? this.state.status.end_time : null}
              PumpOn={this.state.status.pump_on}
              ActiveStepName={this.state.activeStepName}
              StepLocked={this.state.status.step_locked > 0}
              PumpIsResting={this.state.status.pump_is_resting > 0}
              SpargeTemperature={this.state.status.sparge_temperature}
              SpargeTargetTemperature={this.state.status.sparge_target_temperature > 0 ? this.state.status.sparge_target_temperature : ''}
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
                  <Typography color="textSecondary" variant="subtitle1" gutterBottom><IntText text="Brew.BoilPower" /> {this.state.boilPower}%</Typography>
                  <Slider
                    railStyle={{ backgroundColor: '#313131', height: 5 }}
                    trackStyle={{ backgroundColor: '#5c6bc0', height: 5 }}
                    handleStyle={{
                      borderColor: '#5c6bc0',
                      height: 22,
                      width: 22,
                      marginLeft: -14,
                      marginTop: -9,
                      backgroundColor: '#5c6bc0',
                    }}
                    value={this.state.boilPower}
                    onChange={this.handleChangeBoilPowerPercentage}
                    onAfterChange={this.handleSaveChangeBoilPowerPercentage}
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

export default withSnackbar(withStyles(BrewStyles)(Brew));