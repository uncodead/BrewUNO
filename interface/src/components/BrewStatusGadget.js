import React, { Component } from 'react';
import Grid from '@material-ui/core/Grid';
import Card from '@material-ui/core/Card';
import CardContent from '@material-ui/core/CardContent';
import Typography from '@material-ui/core/Typography';
import { PieChart, Pie, Cell, } from 'recharts';
import { withStyles } from '@material-ui/core/styles';
import PauseCircleFilled from '@material-ui/icons/PauseCircleFilled';
import Chronometer from './Chronometer'
import { getDateTime, pad } from '../components/Utils';
import PlayCircleFilledWhite from '@material-ui/icons/PlayCircleFilledWhite';
import Cancel from '@material-ui/icons/Cancel';

const styles = theme => ({
  temperatureCard: {
    background: "#31313152",
  },
  pumpColor1: {
    color: "#447bd6",
  },
  pumpColor2: {
    color: "#5c94f2",
  }
});

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
    const PWMCOLORS = ['#1b5e20', '#424242'];
    const PROGRESSCOLORS = ['#1565c0', '#424242'];
    const { classes } = this.props;

    const getTemperatureData = (index, props) => {
      return [{ name: 'A', value: getTemperatureValue(index, props) }, { name: 'B', value: 100 - getTemperatureValue(index, props) }]
    }

    const getColor = (index) => {
      switch (index) {
        case 0:
          return ['#c62828', '#424242'];
        case 1:
          return ['#f9a825', '#424242'];
      }
    }

    const getTemperatureValue = (index, props) => {
      switch (index) {
        case 0:
          return props.Temperature != undefined ? props.Temperature : 0
        case 1:
          return props.TargetTemperature != undefined ? props.TargetTemperature : 0
      }
    }

    const getTemperatureText = (index) => {
      switch (index) {
        case 0:
          return 'Temperature'
        case 1:
          return 'Target ºC'
      }
    }

    const getPWMData = (props) => {
      return [{ name: 'A', value: props.PWM }, { name: 'B', value: 1023 - props.PWM }]
    }

    const getProgressData = (progress) => {
      return [{ name: 'A', value: progress }, { name: 'B', value: 100 - progress }]
    }

    return (
      <Grid container spacing={16}>
        <Grid item xs={12}>
          <Grid container justify="center" spacing={16}>
            {[0, 1].map(value => (
              <BrewStatusGadgetItem className={classes.temperatureCard} title={getTemperatureText(value)} colors={getColor(value)} value={getTemperatureValue(value, this.props) + 'ºC'} data={getTemperatureData(value, this.props)} />
            ))}
            <BrewStatusGadgetItem className={classes.temperatureCard} title="PWM" colors={PWMCOLORS} value={this.props.PWM} data={getPWMData(this.props)} />
            <BrewStatusGadgetItem className={classes.temperatureCard} title="PWM Sparge" colors={PWMCOLORS} value={this.props.SpargePWM} data={getPWMData(this.props)} />
            { /*<BrewStatusGadgetItem className={classes.temperatureCard} title="Progress" colors={PROGRESSCOLORS} value={this.state.progressCompleted + '%'} data={getProgressData(this.state.progressCompleted)} /> */}
          </Grid>
        </Grid>
        <Grid item xs={12}>
          <Grid container justify="center" spacing={16}>
            <Grid item>
              <Card className={this.props.className}>
                <CardContent>
                  <Typography color="textSecondary" variant="subtitle1" gutterBottom>Active Step - {this.props.ActiveStep}</Typography>
                  <Typography variant="h5">{this.props.ActiveStepName != "" ? this.props.ActiveStepName : '-'}</Typography>
                </CardContent>
              </Card>
            </Grid>
          </Grid>
        </Grid>
        <Grid item xs={12}>
          <Grid container justify="center" spacing={16}>
            <Grid item>
              <Card className={this.props.className}>
                <CardContent align="center">
                  <Typography color="textSecondary" variant="subtitle1" gutterBottom>Pump</Typography>
                  {this.props.PumpOn !== undefined && this.props.PumpOn === 1 ?
                    new Date().getSeconds() % 2 == 0 ?
                      <PlayCircleFilledWhite className={classes.pumpColor1} color="primary" style={{ fontSize: 28 }} align="center" /> :
                      <PlayCircleFilledWhite className={classes.pumpColor2} color="secondary" style={{ fontSize: 28 }} align="center" />
                    :
                    this.props.PumpIsResting ?
                      <PauseCircleFilled style={{ fontSize: 28 }} align="center" color="disabled" /> :
                      <Cancel style={{ fontSize: 28 }} align="center" color="disabled" />
                  }
                </CardContent>
              </Card>
            </Grid>
            {!this.props.StepLocked ?
              <Grid item>
                <Card className={this.props.className}>
                  <CardContent>
                    <Typography color="textSecondary" variant="subtitle1" gutterBottom>Timer</Typography>
                    <Typography variant="h5">{this.state.countdown != undefined ? this.state.countdown : '-'}</Typography>
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
      <Grid item>
        <Card className={this.props.className}>
          <CardContent>
            <Typography color="textSecondary" variant="subtitle1" gutterBottom>{this.props.title}</Typography>
            <PieChart width={100} height={45}>
              <Pie data={this.props.data}
                cx={45} cy={40}
                startAngle={180}
                endAngle={0}
                innerRadius={30}
                outerRadius={40}
                paddingAngle={3}
                stroke={0}
              >

                {/*
              <Pie data={this.props.data}
                cx={45} cy={40}
                startAngle={180} endAngle={0} 
                innerRadius={30} outerRadius={40} 
                paddingAngle={3} legendType='line'>
                {this.props.data.map((entry, index) => <Cell fill={this.props.colors[index % this.props.colors.length]} />)}
              </Pie>
              */}

                {this.props.data.map((entry, index) => <Cell fill={this.props.colors[index % this.props.colors.length]} />)}
              </Pie>
            </PieChart>
            <Typography top="20" align="center" variant="h5">{this.props.value}</Typography>
            {/*
            <LinearProgress
              variant="determinate"
              color="secondary"
              value={50}
            />*/}
          </CardContent>
        </Card>
      </Grid>
    )
  }
}

export default withStyles(styles)(BrewStatusGadget);

