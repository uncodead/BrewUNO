import React, { Component } from 'react';
import Card from '@material-ui/core/Card';
import CardActions from '@material-ui/core/CardActions';
import CardContent from '@material-ui/core/CardContent';
import Button from '@material-ui/core/Button';
import Typography from '@material-ui/core/Typography';
import SectionContent from '../components/SectionContent';
import { withStyles } from '@material-ui/core/styles';
import IntText from '../components/IntText'
import Cookies from 'js-cookie';

const styles = theme => ({
  card: {
    minWidth: 275,
  },
  bullet: {
    display: 'inline-block',
    margin: '0 2px',
    transform: 'scale(0.8)',
  },
  pos: {
    marginBottom: 12,
  },
  pos2x: {
    marginBottom: 20,
  },
});

class About extends Component {
  constructor(props) {
    super(props)
    this.state = {
      version: ''
    }
  }

  componentDidMount() {
    var status = Cookies.get('status')
    if (status !== undefined)
      this.setState({ version: JSON.parse(status).v });
  }

  render() {
    const { classes } = this.props;
    return (
      <SectionContent title={<IntText text="About" />}>
        <Card className={this.props.className}>
          <CardContent>
            {this.state.version !== '' ?
              <Typography className={classes.pos} variant="h6" color="textSecondary">
                Version: {this.state.version}
              </Typography>
              : null
            }
            <Typography variant="h6" component="h1" className={classes.pos}>
              <IntText text="Author" />:
            </Typography>
            <Typography className={classes.pos} variant="h7" color="textSecondary">
              Bruno Leitão de Oliveira
            </Typography>
            <Typography className={classes.pos2x} variant="h7" color="textSecondary">
              uncodead@gmail.com
            </Typography>
            <Typography variant="h6" component="h1" className={classes.pos}>
              <IntText text="Thanks" />:
            </Typography>
            <Typography className={classes.pos} variant="h8" color="textSecondary">
              Luciano Rampanelli
            </Typography>
            <Typography className={classes.pos} variant="h8" color="textSecondary">
              Eduardo 'Dumpa' Sanches
            </Typography>
            <Typography className={classes.pos} variant="h8" color="textSecondary">
              Guilherme Wood
            </Typography>
            <Typography className={classes.pos} variant="h8" color="textSecondary">
              Paiakan
            </Typography>
            <Typography className={classes.pos} variant="h8" color="textSecondary">
              rjwats/esp8266-react
            </Typography>
          </CardContent>
          <CardActions>
            <Button variant="contained" color="secondary" className={classes.button} size="small" target="_blank" href="https://github.com/uncodead/BrewUNO">Github</Button>
          </CardActions>
        </Card>
      </SectionContent >
    )
  }
}

export default withStyles(styles)(About);