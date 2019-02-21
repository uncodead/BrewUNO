import React from 'react';
import PropTypes from 'prop-types';

import Paper from '@material-ui/core/Paper';
import { withStyles } from '@material-ui/core/styles';
import Typography from '@material-ui/core/Typography';
import Checkbox from '@material-ui/core/Checkbox';

const styles = theme => ({
  content: {
    padding: theme.spacing.unit * 2,
    margin: theme.spacing.unit * 2,
  }
});

function SectionContent(props) {
  const { children, classes, title, selected } = props;
  return (
    <Paper className={classes.content}>
      <Typography variant="display1">
        {title}
        {selected ? <Checkbox checked /> : null}
      </Typography>
      {children}
    </Paper>
  );
}

SectionContent.propTypes = {
  classes: PropTypes.object.isRequired,
  children: PropTypes.oneOfType([
    PropTypes.arrayOf(PropTypes.node),
    PropTypes.node
  ]).isRequired,
  title: PropTypes.string.isRequired,
  selected: PropTypes.bool
};

export default withStyles(styles)(SectionContent);
