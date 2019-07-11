import React, { Component } from 'react';
import Grid from '@material-ui/core/Grid';
import Card from '@material-ui/core/Card';
import CardContent from '@material-ui/core/CardContent';
import Typography from '@material-ui/core/Typography';
import { PieChart, Pie, Cell, } from 'recharts';
import { withStyles } from '@material-ui/core/styles';
import { getDateTime, pad } from '../components/Utils';
import Chronometer from './Chronometer'
import { Line, Circle } from 'rc-progress';
import {
  MuiThemeProvider,
  createMuiTheme,
} from '@material-ui/core/styles';
import Divider from '@material-ui/core/Divider';

const themeMain = createMuiTheme({
  palette: {
    primary: {
      main: '#b5b5b5',
    },
    secondary: {
      main: '#f44336',
    },
  },
});
const themeSparge = createMuiTheme({
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
    if (!this.props.BrewStarted && this.props.StepLocked) {
      this.chronometer._handleStop();
    }
    else if (this.props.StepLocked) {
      this.chronometer._handleStart();
    }

    if (!this.props.BrewStarted || this.props.StartTime <= 0 || this.props.EndTime <= 0) {
      this.setState({
        countdown: '00:00:00',
        progressCompleted: 0
      })
      return;
    }
    var dateEntered = getDateTime(this.props.EndTime);
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
        progressCompleted: Math.round(((now - getDateTime(this.props.StartTime)) / (getDateTime(this.props.EndTime) - getDateTime(this.props.StartTime))) * 100)
      })
    }
  }

  render() {
    const SPARGEPWMCOLORS = ['#ffca28', '#424242'];
    const TEMPERATURECOLORS = ['#f44336', '#424242'];
    const { classes } = this.props

    const getProgressData = (progress) => {
      return [{ name: 'A', value: progress }, { name: 'B', value: 100 - progress }]
    }

    return (
      <Grid container spacing={16}>
        <Grid item xs={12}>
          <Grid container justify="center" spacing={16}>
            <BrewStatusGadgetItem className={classes.temperatureCard} theme={themeMain} title={"Main:"} colorPWM={"#76ff03"} PWM={this.props.PWM} titlesufix={this.props.TargetTemperature + 'ºC'} colors={TEMPERATURECOLORS} value={this.props.Temperature + 'ºC'} data={getProgressData(this.props.Temperature)} />
            {this.props.EnableSparge ?
              <BrewStatusGadgetItem className={classes.temperatureCard} theme={themeSparge} title={"Sparge:"} colorPWM={"#2196f3"} PWM={this.props.SpargePWM} titlesufix={this.props.SpargeTargetTemperature + 'ºC'} colors={SPARGEPWMCOLORS} value={this.props.SpargeTemperature + 'ºC'} data={getProgressData(this.props.SpargeTemperature)} />
              : null}

        {!this.props.StepLocked ?
          <Grid item>
            <Card className={this.props.className} style={cardStyle}>
              <CardContent>
                <Typography color="textSecondary" variant="caption" gutterBottom>Timer</Typography>
                <Typography variant="h4">{this.state.countdown != undefined ? this.state.countdown : '-'}</Typography>
                &nbsp;
                <div style={{paddingTop: 10, paddingBotton: 10}}>
                <Divider variant="fullWidth" />
                </div>
                &nbsp;
                <div style={{paddingTop: 10}}>
                <Typography color="textSecondary" variant="caption" gutterBottom>Active Step{/*} - {this.props.ActiveStep}*/}</Typography>
                <Typography variant="subtitle1">{this.props.ActiveStepName != "" ? this.props.ActiveStepName : '-'}</Typography>
                </div>
              </CardContent>
            </Card>
          </Grid>
          : null
        }
        {this.props.StepLocked ?
          <Grid item>
            <Chronometer BrewStarted={this.props.BrewStarted} StartTime={this.props.EndTime} title="Step Locked" onRef={ref => (this.chronometer = ref)} />
          </Grid>
          : null
        }
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
                <Typography color="primary" variant="subtitle2" gutterBottom noWrap>{this.props.title}</Typography>
                &nbsp;&nbsp;
              <Typography color="secondary" variant="subtitle2">{this.props.titlesufix}</Typography>
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
            <Typography top="20" align="center" variant="h5" >{this.props.value}</Typography>
            &nbsp;
            <div style={{ display: "flex" }}>
              <Typography color="textSecondary" variant="subtitle2">PWM:</Typography>
              &nbsp;
              <Typography color="secondaryText" variant="subtitle2">{this.props.PWM}%</Typography>
            </div>
            <Line percent={this.props.PWM} strokeWidth="8" strokeColor={this.props.colorPWM} />
          </CardContent>
        </Card>
      </Grid>

    )
  }
}

export default withStyles(styles)(BrewStatusGadget);

