import React, { Component } from 'react';
import Card from '@material-ui/core/Card';
import CardContent from '@material-ui/core/CardContent';
import Typography from '@material-ui/core/Typography';
import { getDateTime, pad } from '../components/Utils';

class Chronometer extends React.Component {
    componentDidMount() {
        this.props.onRef(this)
        this._handleStart()
    }

    componentWillUnmount() {
        this.props.onRef(undefined)
        this._handleStop()
    }

    constructor(props) {
        super(props);

        var difference = new Date().getTime() - getDateTime(this.props.StartTime);
        var seconds = Math.floor(difference / 1000);
        var minutes = Math.floor(seconds / 60);
        minutes %= 60; seconds %= 60;
        this.state = {
            minutes: minutes,
            seconds: seconds,
            millis: 0,
            running: false
        };
    }

    _handleStart(event) {
        var _this = this;
        if (!this.state.running) {
            this.interval = setInterval(() => {
                this.tick();
            }, 100)

            this.setState({ running: true })
        }
    }

    _handleStop(event) {
        if (this.state.running) {
            clearInterval(this.interval);
            this.setState({ running: false })
        }
    }

    _handleReset(event) {
        this._handleStop();
        this.update(0, 0, 0);
    }

    zeroPad(value) {
        return value < 10 ? `0${value}` : value;
    }

    update(millis, seconds, minutes) {
        this.setState({
            millis: millis,
            seconds: seconds,
            minutes: minutes
        });
    }

    tick() {
        let millis = this.state.millis + 1;
        let seconds = this.state.seconds;
        let minutes = this.state.minutes;

        if (millis === 10) {
            millis = 0;
            seconds = seconds + 1;
        }

        if (seconds === 60) {
            millis = 0;
            seconds = 0;
            minutes = minutes + 1;
        }

        this.update(millis, seconds, minutes);
    }

    render() {
        return (
            <div>
                <Typography color="textSecondary" variant="caption" gutterBottom>{this.props.title}</Typography>
                <Typography variant="h4">{this.zeroPad(this.state.minutes)}:{this.zeroPad(this.state.seconds)}</Typography>
            </div>
        )
    }
}

export default Chronometer;