import React, { Component } from 'react';
import SectionContent from '../components/SectionContent';
import MashSettings from './MashSettings';
import BoilSettings from './BoilSettings';
import CoolingSettings from './CoolingSettings';
import Button from '@material-ui/core/Button';
import { withStyles } from '@material-ui/core/styles';
import ConfirmDialog from '../components/ConfirmDialog';
import BrewFatherDialog from '../components/BrewFatherDialog';
import {
  START_PUMP, STOP_PUMP,
  GET_STATUS_WS, START_BREW, UNLOCK_STEP_BREW,
  NEXT_STEP_BREW, STOP_BREW, RESUME_BREW,
  CHANGE_BOIL_PERCENTAGE,
  START_BOIL, PAUSE_BREW,
  START_ANTICAVITATION, START_BOIL_COUNTER,
  GET_LOGS_TEMPERATURES
} from '../constants/Endpoints';
import { ExecuteRestCall, getDateTime } from '../components/Utils';
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
import CloudUploadIcon from '@material-ui/icons/CloudUpload';

import ResponsiveContainer from 'recharts/lib/component/ResponsiveContainer';
import LineChart from 'recharts/lib/chart/LineChart';
import Line from 'recharts/lib/cartesian/Line';
import XAxis from 'recharts/lib/cartesian/XAxis';
import YAxis from 'recharts/lib/cartesian/YAxis';
import CartesianGrid from 'recharts/lib/cartesian/CartesianGrid';
import Tooltip from 'recharts/lib/component/Tooltip';
import ReconnectingWebSocket from 'reconnecting-websocket';

import Files from 'react-files'

let interval;

class Brew extends Component {
  constructor(props) {
    super(props)
    this.state = {
      status: { temperature: '' },
      data: [],
      confirmDialogOpen: false,
      brewFatherDialogOpen: false,
      boilPower: 0,
      activeStepName: "-",
      statusInitialized: false,
      copyDialogMessage: false,
      boilPowerSetted: false,
      temperatures: []
    }
    this.mashSettings = React.createRef();
    this.boilSettings = React.createRef();
    this.coolingSettings = React.createRef();
    this.brewFatherRecipe = React.createRef();
  }

  wsStatus = new ReconnectingWebSocket(GET_STATUS_WS, [],
    {
      connectionTimeout: 1000,
      maxRetries: 10,
    });

  componentDidMount() {
    fetch(GET_LOGS_TEMPERATURES, {
      method: 'GET',
      headers: {
        "Content-Type": "text/plain"
      },
    }).then(response => {
      if (response.ok) {
        response.text().then(text => {
          let temperatures = JSON.parse('[' + text + ']')
          this.setState({ temperatures: temperatures })
        });
      }
    })

    this.wsStatus.onmessage = evt => {
      let temperatures;
      let jsonData = JSON.parse(evt.data)
      temperatures = [...this.state.temperatures];
      if (temperatures.length >= 200)
        temperatures.splice(0, temperatures.length - 200);

      if (jsonData.as > 0) {
        temperatures.push({ ttp: jsonData.ttp, tp: jsonData.tp, stt: jsonData.stt, st: jsonData.st, time: jsonData.tn });
      }

      let stepName = this.state.activeStepName;
      if (this.state.status.as <= 0)
        stepName = '-'

      if (!this.state.boilPowerSetted && this.state.status.as > 0 && this.state.status.bs === 1)
        this.setState({ boilPower: this.state.status.bpp, boilPowerSetted: true })

      if (this.state.status.bpp !== jsonData.bpp)
        this.setState({ boilPower: jsonData.bpp, boilPowerSetted: true })

      this.setState({
        status: jsonData,
        temperatures: temperatures,
        activeStepName: stepName,
      })

      Cookies.set('status', jsonData)
    }

    setInterval(() => {
      if (this.state.status.as > 0) {
        this.notification(this.getActiveStep(), this.state.status.masn + ' ' + this.state.status.amssn, <IntText text="Mash" />)
        this.notification(this.getActiveStep(), this.state.status.absn, <IntText text="Boil" />)
        this.notification(this.getActiveStep(), this.state.status.casn, <IntText text="Cooling" />)
      }
    }, 1000);
  }

  componentWillUnmount() {
    clearInterval(interval);
  }

  notification(getActiveStep, stepName, step) {
    if (getActiveStep.props.text === step.props.text && this.state.activeStepName !== stepName) {
      this.setState({ activeStepName: stepName });
      if (stepName !== "" && stepName !== " " && stepName !== undefined)
        this.notify(stepName, step.props.text === "Boil" ? 'success' : 'warning', true);
    }
  }

  notify(text, style, persit) {
    const action = (key) => (
      <Button color="Primary" onClick={() => { this.props.closeSnackbar(key); }}>
        <IntText text="Dismiss" />
      </Button>
    );
    this.props.enqueueSnackbar(text, {
      persist: persit,
      action,
      variant: style
    });
  }

  getActiveStep() {
    switch (this.state.status.as) {
      case 1:
        return <IntText text="Mash" />
      case 2:
        return <IntText text="Boil" />
      case 3:
        return <IntText text="Cooling" />
      case 4:
        return <IntText text="PumpPrime" />
      default:
        return <IntText text="Stopped" />
    }
  }

  handleChangeBoilPowerPercentage = (value) => {
    this.setState({ boilPower: value });
  }

  handleSaveChangeBoilPowerPercentage = (value) => {
    fetch(CHANGE_BOIL_PERCENTAGE, {
      method: 'POST',
      body: JSON.stringify({ "boil_power_percentage": this.state.boilPower }),
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json',
      },
    })
    this.setState({ boilPower: value });
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

  recipeSelected = (recipe) => {
    this.mashSettings.current.onImport(recipe.mash)
    this.boilSettings.current.onImport(recipe.boil)
    this.coolingSettings.current.onImport(recipe.cooling)
    this.state.brewFatherDialogOpen = false
  }

  import = (files) => {
    this.fileReader = new FileReader();
    this.fileReader.readAsText(files[files.length - 1]);
    this.fileReader.onload = event => {
      let json = JSON.parse(event.target.result)
      let mash = json.mash
      let boil = json.boil
      let cooling = json.cooling
      this.mashSettings.current.onImport(mash)
      this.boilSettings.current.onImport(boil)
      this.coolingSettings.current.onImport(cooling)
    };
  }

  export = () => {
    let mashItems = this.mashSettings.current.state.items
    let boilItems = this.boilSettings.current.state.items
    let coolingItems = this.coolingSettings.current.state.items

    let items = {
      'mash': mashItems,
      'boil': boilItems,
      'cooling': coolingItems
    }

    var dataStr = "data:text/json;charset=utf-8," + encodeURIComponent(JSON.stringify(items));
    var downloadAnchorNode = document.createElement('a');
    downloadAnchorNode.setAttribute("href", dataStr);
    downloadAnchorNode.setAttribute("download", "BrewUNO" + Math.random().toString(36).substring(7) + ".json");
    document.body.appendChild(downloadAnchorNode);
    downloadAnchorNode.click();
    downloadAnchorNode.remove();
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
              this.setState({ status: json })
            }, null, this.props)
            if (callback) callback()
          }
          this.setState({ confirmDialogOpen: false })
        }
      });
    else {
      ExecuteRestCall(url, 'POST', (json) => {
        Cookies.set('status', json)
        this.setState({ status: json })
      }, null, this.props)
      if (callback) callback()
    }
  }

  render() {
    const { classes } = this.props;
    return (
      <SectionContent>
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
        {this.state.status.as > 0 && this.state.status.as !== 4 && this.state.status.bs === 0 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => {
              this.actionBrew(<IntText text="Brew.ResumeConfirmation" />, RESUME_BREW)
            }}><IntText text="Resume" /></Button>
          : null}
        {this.state.status.as > 0 && this.state.status.as !== 4 ?
          <Button variant="contained" color="secondary" className={classes.button}
            onClick={() => { this.actionBrew(<IntText text="Brew.StopConfirmation" />, STOP_BREW, () => { this.setState({ temperatures: [], activeStepName: '-' }) }) }}><Stop size="small" color="action" className={classes.button_icons} /></Button>
          : null}
        {(this.state.status.as === 1 || this.state.status.as === 2 || this.state.status.as === 3) && this.state.status.bs === 1 && this.state.status.sl === 0 ?
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
                {this.state.status.as === 0 ?
                  <div>
                    <MenuItem onClick={this.export}><IntText text="ExportRecipe" /></MenuItem>
                    <MenuItem >
                      <Files
                        onChange={this.import}
                        accepts={['.json']}
                        maxFileSize={10000000}
                        minFileSize={0}
                        multiple
                        maxFiles={3}
                      >
                        <IntText text="ImportRecipe" />
                      </Files></MenuItem>
                    {this.state.status.bfid && this.state.status.bfkey ?
                      <MenuItem onClick={() => { this.setState({ brewFatherDialogOpen: true }); popupState.close() }}><IntText text="ImportFromBrewfather" /></MenuItem>
                      : null}
                  </div>
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
              CoolingPWM={this.state.status.cppt}
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
              CoolingTemperature={this.state.status.ct}
              CoolingTargetTemperature={this.state.status.ctt}
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
        {this.state.status.as > 0 && this.state.status.bs === 1 ?
          <Card className={classes.chartCard}>
            <CardContent>
              <ResponsiveContainer width="90%" height={320} >
                <LineChart data={this.state.temperatures} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
                  <XAxis dataKey="time" tick={{ fill: '#262626', fontSize: "12px", fontFamily: "Montserrat", }} />
                  <YAxis yAxisId="left" tick={{ fill: '#707070', fontSize: "12px", fontFamily: "Montserrat", }} />
                  <CartesianGrid vertical={false} strokeDasharray="3 3" />
                  <Tooltip />
                  <Line type="monotone" yAxisId="left" dataKey="tp" stroke="#f9a125" dot={null} />
                  <Line type="monotone" yAxisId="left" dataKey="ttp" stroke="#c64828" dot={null} />
                </LineChart>
              </ResponsiveContainer>
            </CardContent>
          </Card> : null}
        <Paper className={classes.brewSettingsCard}>
          <Grid container justify="center">
            <Grid item>
              <Grid container >
                <Grid item>
                  <MashSettings ref={this.mashSettings} listOnly={true} brewDay={true} selectedIndex={this.state.status.amsi} />
                </Grid>
                <Grid item>
                  <BoilSettings ref={this.boilSettings} listOnly={true} brewDay={true} selectedIndex={this.state.status.absi} />
                </Grid>
                <Grid item>
                  <CoolingSettings ref={this.coolingSettings} listOnly={true} brewDay={true} selectedIndex={this.state.status.acsi} />
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
        <BrewFatherDialog
          ref={this.brewFatherRecipe}
          brewFatherDialogOpen={this.state.brewFatherDialogOpen}
          confirmAction={() => { this.state.brewFatherDialogOpen = false }}
          recipeSelected={this.recipeSelected}
          apiId={this.state.status.bfid}
          apiKey={this.state.status.bfkey}
        />
      </SectionContent >
    )
  }
}

export default withSnackbar(withStyles(BrewStyles)(Brew));