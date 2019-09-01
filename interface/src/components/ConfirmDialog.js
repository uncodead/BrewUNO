import React, { Component } from 'react';
import Dialog from '@material-ui/core/Dialog';
import DialogActions from '@material-ui/core/DialogActions';
import DialogContent from '@material-ui/core/DialogContent';
import DialogContentText from '@material-ui/core/DialogContentText';
import DialogTitle from '@material-ui/core/DialogTitle';
import Button from '@material-ui/core/Button';
import { CopyToClipboard } from 'react-copy-to-clipboard';
import IntText from '../components/IntText'

class ConfirmDialog extends Component {

  constructor(props) {
    super(props)
    this.state = { confirmDialogOpen: false }
  }

  componentWillReceiveProps(nextProps) {
    if (nextProps.confirmDialogOpen !== this.state.confirmDialogOpen) {
      this.setState({ confirmDialogOpen: nextProps.confirmDialogOpen });
    }
  }

  render() {
    return (
      <Dialog
        open={this.state.confirmDialogOpen}
        onClose={() => this.props.confirmAction(false)}
        aria-labelledby="alert-dialog-title"
        aria-describedby="alert-dialog-description">
        <DialogTitle id="alert-dialog-title"><IntText text="Confirmation.Dialog" /></DialogTitle>
        <DialogContent>
          <DialogContentText id="alert-dialog-description">
            {this.props.confirmDialogMessage}
          </DialogContentText>
        </DialogContent>
        <DialogActions>
          {this.props.copy ?
            <CopyToClipboard
              text={this.props.confirmDialogMessage}
              onCopy={() => {
                //do stuff here, like summon a confirmation prompt
              }}>
              <Button onClick={() => this.props.confirmAction(false)} color="secondary"><IntText text="Copy" /></Button>
            </CopyToClipboard>
            : null}
          <Button onClick={() => this.props.confirmAction(false)} color="secondary">
            <IntText text="No" />
          </Button>
          <Button onClick={() => this.props.confirmAction(true)} color="secondary" autoFocus>
            <IntText text="Yes" />
          </Button>
        </DialogActions>
      </Dialog>
    )
  }
}

export default ConfirmDialog;