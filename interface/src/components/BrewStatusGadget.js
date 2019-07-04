import React, { Component } from 'react';
import Grid from '@material-ui/core/Grid';
import Card from '@material-ui/core/Card';
import CardContent from '@material-ui/core/CardContent';
import Typography from '@material-ui/core/Typography';
import { PieChart, Pie, Cell, } from 'recharts';
import { withStyles } from '@material-ui/core/styles';
import { getDateTime, pad } from '../components/Utils';
import Chronometer from './Chronometer'


const styles = theme => ({
  temperatureCard: {
    background: "#31313152",
  },
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
    const PWMCOLORS = ['#f9a825', '#424242'];
    const SPARGEPWMCOLORS = ['#119636', '#424242'];
    const PROGRESSCOLORS = ['#1565c0', '#424242'];
    const TEMPERATURECOLORS = ['#c62828', '#424242'];
    const { classes } = this.props

    const getPWMData = (pwm) => {
      return [{ name: 'A', value: pwm }, { name: 'B', value: 1023 - pwm }]
    }

    const getProgressData = (progress) => {
      return [{ name: 'A', value: progress }, { name: 'B', value: 100 - progress }]
    }

    const getPWMPercentage = (pwm) => {
      return parseFloat(pwm * 100 / 1023).toFixed(1)
    }

    return (
      <Grid container spacing={16}>
        <Grid item xs={12}>
          <Grid container justify="center" spacing={16}>
            <BrewStatusGadgetItem className={classes.temperatureCard} title={"Main:"} titlesufix={this.props.TargetTemperature + 'ºC'} colors={TEMPERATURECOLORS} value={this.props.Temperature + 'ºC'} data={getProgressData(this.props.Temperature)} />
            <BrewStatusGadgetItem className={classes.temperatureCard} title="Main PWM" colors={PWMCOLORS} value={getPWMPercentage(this.props.PWM) + '%'} data={getPWMData(this.props.PWM)} />
            {this.props.EnableSparge ?
              <BrewStatusGadgetItem className={classes.temperatureCard} title={"Sparge:"} titlesufix={this.props.SpargeTargetTemperature + 'ºC'} colors={TEMPERATURECOLORS} value={this.props.SpargeTemperature + 'ºC'} data={getProgressData(this.props.SpargeTemperature)} />
              : null}
            {this.props.EnableSparge ?
              <BrewStatusGadgetItem className={classes.temperatureCard} title="Sparge PWM" colors={PWMCOLORS} value={getPWMPercentage(this.props.SpargePWM) + '%'} data={getPWMData(this.props.SpargePWM)} />
              : null}
            <BrewStatusGadgetItem className={classes.temperatureCard} title="Progress" colors={PROGRESSCOLORS} value={this.state.progressCompleted + '%'} data={getProgressData(this.state.progressCompleted)} />
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
            <div style={{ display: "flex" }}>
              <Typography color="textSecondary" variant="subtitle1" gutterBottom noWrap>{this.props.title}</Typography>
              &nbsp;
              <Typography color="textPrimary" variant="h6">{this.props.titlesufix}</Typography>
            </div>
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
                {this.props.data.map((entry, index) => <Cell fill={this.props.colors[index % this.props.colors.length]} />)}
              </Pie>
            </PieChart>
            <Typography top="20" align="center" variant="h5" >{this.props.value}</Typography>
          </CardContent>
        </Card>
      </Grid>
    )
  }
}

export default withStyles(styles)(BrewStatusGadget);

