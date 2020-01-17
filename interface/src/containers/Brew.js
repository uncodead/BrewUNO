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
  START_ANTICAVITATION, START_BOIL_COUNTER
} from '../constants/Endpoints';
import { ExecuteRestCall } from '../components/Utils';
import { Event } from '../components/Tracking'
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
import Cookies from 'js-cookie';

let interval;

class Brew extends Component {
  constructor(props) {
    super(props)
    this.state = {
      status: { temperature: '' },
      data: [],
      confirmDialogOpen: false,
      boilPower: 0,
      activeStepName: "-",
      statusInitialized: false,
      copyDialogMessage: false
    }
    interval = setInterval(() => { this.getStatus(); }, 5000);
  }

  componentDidMount() {
    if (Cookies.get('status') !== undefined)
      this.setState({ status: JSON.parse(Cookies.get('status')) }, this.updateStatus)
  }

  componentWillUnmount() {
    clearInterval(interval);
  }

  updateStatus() {
    if (this.state.status.as > 0 && this.state.status.bs == 1)
      this.setState({ boilPower: this.state.status.bpp })

    if (this.state.statusInitialized) {
      this.notification(this.getActiveStep(), this.state.status.masn + ' ' + this.state.status.amssn, <IntText text="Mash" />)
      this.notification(this.getActiveStep(), this.state.status.absn, <IntText text="Boil" />)

    }
    this.setState({ statusInitialized: true })
  }

  notification(getActiveStep, stepName, step) {
    if (getActiveStep.props.text === step.props.text && this.state.activeStepName !== stepName) {
      this.setState({ activeStepName: stepName });
      if (stepName !== "" && stepName !== " " && stepName !== undefined) {
        const action = (key) => (
          <Button color="Primary" onClick={() => { this.props.closeSnackbar(key) }}>
            <IntText text="Dismiss" />
          </Button>
        );
        this.props.enqueueSnackbar(stepName, {
          persist: true,
          action,
          variant: step.props.text === "Boil" ? 'success' : 'warning'
        });
      }
    }
  }

  getStatus() {
    if (Cookies.get('status') !== undefined && this.state.status === null)
      this.setState({ status: JSON.parse(Cookies.get('status')) }, this.updateStatus)

    ExecuteRestCall(GET_ACTIVE_STATUS, 'GET', json => {
      if (json !== null && json !== undefined && json !== '') {
        Cookies.set('status', json)
        this.setState({ status: json }, this.updateStatus)
      }
    })
  }

  getActiveStep() {
    switch (this.state.status.as) {
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
    Cookies.remove('status')
    Event("Brew", url, "/brew")
    if (message !== '')
      this.setState({
        confirmDialogOpen: true,
        copyDialogMessage: false,
        confirmDialogMessage: message,
        confirmAction: (confirm) => {
          if (confirm) {
            ExecuteRestCall(url, 'POST', (json) => {
              Cookies.set('status', json)
              this.setState({ status: json }, this.updateStatus)
            }, null, this.props)
            if (callback) callback()
          }
          this.setState({ confirmDialogOpen: false })
        }
      });
    else {
      ExecuteRestCall(url, 'POST', (json) => {
        Cookies.set('status', json)
        this.setState({ status: json }, this.updateStatus)
      }, null, this.props)
      if (callback) callback()
    }
  }

  render() {
    const { classes } = this.props;
    return (
      <SectionContent title="">
        {this.state.status.as === 0 && this.state.status.bs === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => {
              this.actionBrew(<IntText text="Brew.StartConfirmation" />, START_BREW)
            }}><IntText text="Start" /></Button> : null}
        {this.state.status.as > 0 && this.state.status.bs === 1 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => {
              this.actionBrew(<IntText text="Brew.PauseConfirmation" />, PAUSE_BREW)
            }}><PauseCircleFilled size="small" color="action" className={classes.button_icons} /></Button>
          : null}
        {this.state.status.as > 0 && this.state.status.as !== 3 && this.state.status.bs === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => {
              this.actionBrew(<IntText text="Brew.ResumeConfirmation" />, RESUME_BREW)
            }}><IntText text="Resume" /></Button>
          : null}
        {this.state.status.as > 0 && this.state.status.as !== 3 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew(<IntText text="Brew.StopConfirmation" />, STOP_BREW, () => { this.setState({ data: [], activeStepName: '-' }) }) }}><Stop size="small" color="action" className={classes.button_icons} /></Button>
          : null}
        {this.state.status.as === 1 && this.state.status.bs === 1 && this.state.status.sl === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew(<IntText text="Brew.NextConfirmation" />, NEXT_STEP_BREW) }}><SkipNext size="small" color="action" className={classes.button_icons} /></Button>
          : null}
        {this.state.status.as === 1 && this.state.status.bs === 1 && this.state.status.sl === 1 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew(<IntText text="Brew.UnLockconfirmation" />, UNLOCK_STEP_BREW) }}><LockOpen size="small" color="action" className={classes.button_icons} /></Button>
          : null}
        {this.state.status.as === 0 && this.state.status.bs === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew(<IntText text="Brew.BoilConfirmation" />, START_BOIL) }}><IntText text="Boil" /></Button>
          : null}
        <Button variant="contained" color="secondary" className={classes.button}
          onClick={() => { this.actionBrew('', this.state.status.po ? STOP_PUMP : START_PUMP) }}>
          {this.state.status.po ?
            <SvgIcon {...this.props} color="action">
              <path d="M20 13.641c0 2-.779 4.109-2.34 5.67a7.992 7.992 0 0 1-11.32-.002C4.78 17.75 4 15.641 4 13.641A8.02 8.02 0 0 1 6.34 8L12 2.35 17.66 8A8.016 8.016 0 0 1 20 13.641z" />
            </SvgIcon>
            : //pause icon
            this.state.status.pir ?
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
                {this.state.status.as === 2 ?
                  <MenuItem onClick={() => { this.actionBrew(<IntText text="Brew.StartBoilCounterConfirmation" />, START_BOIL_COUNTER) }}><IntText text="StartBoilCounter" /></MenuItem>
                  : null
                }
                {this.state.status.as === 0 ?
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
              BrewStarted={this.state.status.bs}
              Temperature={this.state.status.tp}
              TargetTemperature={this.state.status.ttp > 0 ? this.state.status.ttp : ''}
              BoilTemperature={this.state.status.btp}
              BoilTargetTemperature={this.state.status.btt}
              PWM={this.state.status.pp}
              SpargePWM={this.state.status.spp}
              BoilPWM={this.state.status.bppt}
              TimeNotSet={this.state.status.tns}
              Count={this.state.status.c}
              ActiveStep={this.getActiveStep()}
              TimeNow={this.state.status.tn > 0 ? this.state.status.tn : null}
              StartTime={this.state.status.st > 0 ? this.state.status.st : null}
              ElapsedTime={this.state.status.elapsed_time > 0 ? this.state.status.elapsed_time : null}
              EndTime={this.state.status.et > 0 ? this.state.status.et : null}
              ActiveStepName={this.state.activeStepName}
              StepLocked={this.state.status.sl > 0}
              SpargeTemperature={this.state.status.stp}
              AuxOneSendorEnabled={this.state.status.axs !== ''}
              AuxTwoSendorEnabled={this.state.status.axss !== ''}
              AuxThreeSendorEnabled={this.state.status.axsss !== ''}
              AuxOneTemperature={this.state.status.atp}
              AuxTwoTemperature={this.state.status.attp}
              AuxThreeTemperature={this.state.status.atttp}
              SpargeTargetTemperature={this.state.status.stt > 0 ? this.state.status.stt : ''}
              EnableSparge={this.state.status.es}
              EnableBoilKettle={this.state.status.eb === 1}
              TempUnit={this.state.status.tu}
            />
          </CardContent>
        </Card>
        <Divider />
        {this.state.status.as === 2 && this.state.status.bs === 1 ?
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
        <Paper className={classes.brewSettingsCard}>
          <Grid container justify="center">
            <Grid item>
              <Grid container >
                <Grid item>
                  <MashSettings listOnly={true} brewDay={true} selectedIndex={this.state.status.amsi} />
                </Grid>
                <Grid item>
                  <BoilSettings listOnly={true} brewDay={true} selectedIndex={this.state.status.absi} />
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