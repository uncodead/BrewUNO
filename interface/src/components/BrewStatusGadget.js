import React, { Component } from 'react';
import Grid from '@material-ui/core/Grid';
import Card from '@material-ui/core/Card';
import CardContent from '@material-ui/core/CardContent';
import Typography from '@material-ui/core/Typography';
import { PieChart, Pie, Cell, } from 'recharts';
import { withStyles } from '@material-ui/core/styles';
import { getDateTime, pad } from '../components/Utils';
import { Line } from 'rc-progress';
import { MuiThemeProvider, createMuiTheme } from '@material-ui/core/styles';
import Divider from '@material-ui/core/Divider';
import IntText from './IntText'

const themeMain = createMuiTheme({
  palette: {
    primary: {
      main: '#b5b5b5',
    },
    secondary: {
      main: '#ffca28',
    },
  },
});
const themeSparge = createMuiTheme({
  palette: {
    primary: {
      main: '#b5b5b5',
    },
    secondary: {
      main: '#ff7301',
    },
  },
});
const themeBoil = createMuiTheme({
  palette: {
    primary: {
      main: '#b5b5b5',
    },
    secondary: {
      main: '#f44336',
    },
  },
});
const themeAuxiliary = createMuiTheme({
  palette: {
    primary: {
      main: '#b5b5b5',
    },
    secondary: {
      main: '#ffca28',
    },
  },
});

const styles = theme => ({
  temperatureCard: {
    background: "#313131",
  },
});

var cardStyle = {
  background: '#313131',
  display: 'block',
}

class BrewStatusGadget extends Component {
  constructor(props) {
    super(props)

    this.state = {
      countdown: 0,
      progressCompleted: 0,
    }
  }

  timerProgress = setInterval(() => {
    this.brewProgress();
  }, 1000);

  brewProgress() {
    if (!this.props.BrewStarted || this.props.StartTime <= 0 || this.props.EndTime <= 0) {
      this.setState({ countdown: '00:00:00', progressCompleted: 0 })
      return;
    }
    if (this.props.TimeNotSet === 1) {
      if (this.props.Count !== null && this.props.Count !== undefined && this.props.Count !== '')
        this.setState({ countdown: this.props.Count })
      else
        this.setState({ countdown: '00:00:00' })
      return;
    }
    var dateEntered = getDateTime(this.props.EndTime);
    var now = new Date();
    var difference = !this.props.StepLocked ? dateEntered.getTime() - now.getTime() : now.getTime() - dateEntered.getTime();
    if (difference <= 0 && !this.props.StepLocked)
      this.setState({ countdown: '00:00:00', progressCompleted: 100 })
    else {
      var seconds = Math.floor(difference / 1000);
      var minutes = Math.floor(seconds / 60);
      var hours = Math.floor(minutes / 60);
      var days = Math.floor(hours / 24);
      minutes %= 60; seconds %= 60;
      this.setState({
        countdown: pad(hours, 2) + ':' + pad(minutes, 2) + ':' + pad(seconds, 2),
      })
    }
  }

  render() {
    const SPARGEPWMCOLORS = ['#ff7301', '#424242'];
    const BOILPWMCOLORS = ['#f44336', '#424242'];
    const TEMPERATURECOLORS = ['#ffca28', '#424242'];
    const { classes } = this.props

    const getProgressData = (progress) => {
      return [{ name: 'A', value: progress }, { name: 'B', value: 100 - progress }]
    }


    return (
      <Grid container spacing={16}>
        <Grid item xs={12}>
          <Grid container justify="center" spacing={16}>
            {this.props.ActiveStep.props.text === 'Mash' || this.props.ActiveStep.props.text === 'Stopped' || this.props.EnableBoilKettle ?
              <BrewStatusGadgetItem className={classes.temperatureCard} theme={themeMain} title={"Main"} colorPWM={"#83f316"} PWM={this.props.PWM} TempUnit={this.props.TempUnit} titlesufix={this.props.TargetTemperature} colors={TEMPERATURECOLORS} value={this.props.Temperature} data={getProgressData(this.props.Temperature)} />
              : null}
            {this.props.EnableSparge ?
              <BrewStatusGadgetItem className={classes.temperatureCard} theme={themeSparge} title={"Secondary"} colorPWM={"#2892ff"} PWM={this.props.SpargePWM} TempUnit={this.props.TempUnit} titlesufix={this.props.SpargeTargetTemperature} colors={SPARGEPWMCOLORS} value={this.props.SpargeTemperature} data={getProgressData(this.props.SpargeTemperature)} />
              : null}
            {this.props.EnableBoilKettle || this.props.ActiveStep.props.text === 'Boil' ?
              <BrewStatusGadgetItem className={classes.temperatureCard} theme={themeBoil} title={"Boil"} colorPWM={"#ffca28"} PWM={this.props.BoilPWM} TempUnit={this.props.TempUnit} titlesufix={this.props.BoilTargetTemperature} colors={BOILPWMCOLORS} value={this.props.BoilTemperature} data={getProgressData(this.props.BoilTemperature)} />
              : null}

            <Grid item>
              <Card className={this.props.className} style={cardStyle}>
                <CardContent>
                  <div>
                    <Typography color="textSecondary" variant="subtitle2" gutterBottom><IntText text="Timer" /></Typography>
                    <Typography variant="h4">{this.state.countdown !== undefined ? this.state.countdown : '-'}</Typography>
                  </div>
                  &nbsp;
                    <div style={{ paddingTop: 7, paddingBotton: 0 }}>
                    <Divider variant="fullWidth" />
                  </div>
                  &nbsp;
                    <div style={{ paddingTop: 7 }}>
                    <Typography color="textSecondary" variant="caption" gutterBottom><IntText text="Brew.ActiveStep" /></Typography>
                    <Typography variant="subtitle1">{this.props.ActiveStepName !== "" ? this.props.ActiveStepName : '-'}</Typography>
                  </div>
                </CardContent>
              </Card>
            </Grid>
          </Grid>
          <Grid container justify="center" spacing={16}>
            {this.props.AuxOneSendorEnabled ?
              <BrewStatusAuxItem className={classes.temperatureCard} theme={themeAuxiliary} title={"Aux 1"} TempUnit={this.props.TempUnit} titlesufix={this.props.AuxOneTemperature} colors={TEMPERATURECOLORS} value={this.props.AuxOneTemperature} />
              : null}
            {this.props.AuxTwoSendorEnabled ?
              <BrewStatusAuxItem className={classes.temperatureCard} theme={themeAuxiliary} title={"Aux 2"} TempUnit={this.props.TempUnit} titlesufix={this.props.AuxTwoTemperature} colors={TEMPERATURECOLORS} value={this.props.AuxTwoTemperature} />
              : null}
            {this.props.AuxThreeSendorEnabled ?
              <BrewStatusAuxItem className={classes.temperatureCard} theme={themeAuxiliary} title={"Aux 3"} TempUnit={this.props.TempUnit} titlesufix={this.props.AuxThreeTemperature} colors={TEMPERATURECOLORS} value={this.props.AuxThreeTemperature} />
              : null}
          </Grid>
        </Grid>
      </Grid>
    )
  }
}

class BrewStatusGadgetItem extends Component {
  render() {
    return (
      <Grid item >
        <Card className={this.props.className}>
          <CardContent >
            <MuiThemeProvider theme={this.props.theme}>
              <div style={{ display: "flex" }}>
                <Typography color="primary" variant="subtitle2" gutterBottom noWrap><IntText text={this.props.title} /></Typography>
                &nbsp;&nbsp;
              <Typography color="secondary" variant="subtitle2">{this.props.titlesufix} º{this.props.TempUnit}</Typography>
              </div>
            </MuiThemeProvider>
            <PieChart width={100} height={50}>
              <Pie data={this.props.data}
                cx={45} cy={45}
                startAngle={180}
                endAngle={0}
                innerRadius={35}
                outerRadius={50}
                paddingAngle={3}
                stroke={0}
              >
                {this.props.data.map((entry, index) => <Cell fill={this.props.colors[index % this.props.colors.length]} />)}
              </Pie>
            </PieChart>
            <Typography top="20" align="center" variant="h5" >{this.props.value} º{this.props.TempUnit}</Typography>
            &nbsp;
            <div style={{ display: "flex" }}>
              <Typography color="textSecondary" variant="subtitle2">PWM:</Typography>
              &nbsp;
              <Typography color="secondaryText" variant="subtitle2">{this.props.PWM}%</Typography>
            </div>
            <Line percent={this.props.PWM} strokeWidth="8" strokeColor={this.props.colorPWM} trailWidth="4" trailColor="#424242" />
          </CardContent>
        </Card>
      </Grid>

    )
  }
}

class BrewStatusAuxItem extends Component {
  render() {
    return (
      <Grid item>
        <Card className={this.props.className} style={cardStyle}>
          <CardContent >
            <MuiThemeProvider theme={this.props.theme}>
              <div style={{ display: "flex" }}>
                <Typography color="primary" variant="subtitle2" gutterBottom noWrap><IntText text={this.props.title} /></Typography>
              </div>
              <div style={{ display: "flex" }}>
                <Typography color="secondary" variant="subtitle2">{this.props.titlesufix} º{this.props.TempUnit}</Typography>
              </div>
            </MuiThemeProvider>
          </CardContent>
        </Card>
      </Grid>
    )
  }
}

export default withStyles(styles)(BrewStatusGadget);

