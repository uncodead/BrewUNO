import React from 'react';
import PropTypes from 'prop-types';

import Paper from '@material-ui/core/Paper';
import { withStyles } from '@material-ui/core/styles';
import Typography from '@material-ui/core/Typography';

import Divider from '@material-ui/core/Divider';

const styles = theme => ({
  content: {
    padding: theme.spacing.unit * 2,
    margin: theme.spacing.unit * 2,
    minWidth: 350
  }
});

function SectionContent(props) {
  const { children, classes, title } = props;
  return (
    <div>
      {title != '' ?
        <Paper className={classes.content}>
          <Typography variant="display1">
            {title}
          </Typography>
        </Paper>
        : null}
      <Paper className={classes.content}>
        {children}
      </Paper>

    </div>
  );
}

SectionContent.propTypes = {
  classes: PropTypes.object.isRequired,
  children: PropTypes.oneOfType([
    PropTypes.arrayOf(PropTypes.node),
    PropTypes.node
  ]).isRequired,
  title: PropTypes.string.isRequired
};

export default withStyles(styles)(SectionContent);
