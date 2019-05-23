import React, { Component } from 'react';
import Grid from '@material-ui/core/Grid';
import Card from '@material-ui/core/Card';
import CardActions from '@material-ui/core/CardActions';
import CardContent from '@material-ui/core/CardContent';
import Typography from '@material-ui/core/Typography';
import { PieChart, Pie, Sector, Cell, } from 'recharts';
import Paper from '@material-ui/core/Paper';
import { withStyles } from '@material-ui/core/styles';

const styles = theme => ({
  temperatureCard: {
    background: "#31313152",
  },
});

class BrewStatusGadget extends Component {
  render() {
    const PWMCOLORS = ['#1b5e20', '#CCCCCC'];
    const PROGRESSCOLORS = ['#1565c0', '#CCCCCC'];
    const RADIAN = Math.PI / 180;
    const { classes } = this.props;

    const getTemperatureData = (index, props) => {
      return [{ name: 'A', value: getTemperatureValue(index, props) }, { name: 'B', value: 100 - getTemperatureValue(index, props) }]
    }

    const getColor = (index) => {
      debugger
      switch (index) {
        case 0:
          return ['#c62828', '#CCCCCC'];
        case 1:
          return ['#f9a825', '#CCCCCC'];
      }
    }

    const getTemperatureValue = (index, props) => {
      switch (index) {
        case 0:
          return props.Temperature
        case 1:
          return props.TargetTemperature
      }
    }

    const getTemperatureText = (index) => {
      switch (index) {
        case 0:
          return 'Temperature'
        case 1:
          return 'Target Temperature'
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
            <BrewStatusGadgetItem className={classes.temperatureCard} title="Progress" colors={PROGRESSCOLORS} value={this.props.Progress + '%'} data={getProgressData(this.props.Progress)} />
          </Grid>
        </Grid>
        <Grid item xs={12}>
          <Grid container justify="center" spacing={16}>
            <Grid item>
              <Card className={this.props.className}>
                <CardContent>
                  <Typography color="textSecondary" variant="subtitle1" gutterBottom>Active Step</Typography>
                  <Typography variant="h5">Mash</Typography>
                </CardContent>
              </Card>
            </Grid>
            <Grid item>
              <Card className={this.props.className}>
                <CardContent>
                  <Typography color="textSecondary" variant="subtitle1" gutterBottom>Boil Time</Typography>
                  <Typography variant="h5">60 minutes</Typography>
                </CardContent>
              </Card>
            </Grid>
            <Grid item>
              <Card className={this.props.className}>
                <CardContent>
                  <Typography color="textSecondary" variant="subtitle1" gutterBottom>Step Started at</Typography>
                  <Typography variant="h5">12:41:23</Typography>
                </CardContent>
              </Card>
            </Grid>
            <Grid item>
              <Card className={this.props.className}>
                <CardContent>
                  <Typography color="textSecondary" variant="subtitle1" gutterBottom>Step End at</Typography>
                  <Typography variant="h5">12:51:23</Typography>
                </CardContent>
              </Card>
            </Grid>
            <Grid item>
              <Card className={this.props.className}>
                <CardContent>
                  <Typography color="textSecondary" variant="subtitle1" gutterBottom>CountDown</Typography>
                  <Typography variant="h5">00:51:23</Typography>
                </CardContent>
              </Card>
            </Grid>
          </Grid>
        </Grid>
      </Grid>
    )
  }
}

class BrewStatusGadgetItem extends Component {
  render() {
    const { classes } = this.props;
    const RADIAN = Math.PI / 180;
    return (
      <Grid item>
        <Card className={this.props.className}>
          <CardContent>
            <Typography color="textSecondary" variant="subtitle1" gutterBottom>{this.props.title}</Typography>
            <PieChart width={190} height={90}>
              <Pie data={this.props.data} cx={90} cy={90} startAngle={180} endAngle={0} innerRadius={60} outerRadius={80} paddingAngle={2} legendType='line'>
                {this.props.data.map((entry, index) => <Cell fill={this.props.colors[index % this.props.colors.length]} />)}
              </Pie>
            </PieChart>
            <Typography top="20" align="center" variant="h5">{this.props.value}</Typography>
          </CardContent>
        </Card>
      </Grid>
    )
  }
}

export default withStyles(styles)(BrewStatusGadget);

