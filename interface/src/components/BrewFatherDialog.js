import React, { Component } from 'react';
import Dialog from '@material-ui/core/Dialog';
import DialogActions from '@material-ui/core/DialogActions';
import DialogContent from '@material-ui/core/DialogContent';
import DialogContentText from '@material-ui/core/DialogContentText';
import DialogTitle from '@material-ui/core/DialogTitle';
import Button from '@material-ui/core/Button';
import IntText from '../components/IntText'
import List from '@material-ui/core/List';
import ListItem from '@material-ui/core/ListItem';
import ListItemText from '@material-ui/core/ListItemText';
import Divider from '@material-ui/core/Divider';
import FileCopy from '@material-ui/icons/FileCopy';
import IconButton from '@material-ui/core/IconButton';
import Paper from '@material-ui/core/Paper';
import Typography from '@material-ui/core/Typography';
import { BREWFATHERAPIURL } from '../constants/Endpoints';
import LinearProgress from '@material-ui/core/LinearProgress';
import TablePagination from '@material-ui/core/TablePagination';

class BrewFatherDialog extends Component {

  constructor(props) {
    super(props)
    this.state = {
      brewFatherDialogOpen: false,
      recipes: [],
      loading: false,
      page: 0,
      rowsPerPage: 10
    }
  }

  componentDidMount() {
    this.setState({
      brewFatherDialogOpen: false
    })
  }

  fetchRecipe = (id) => {
    var myHeaders = new Headers();
    var auth = new Buffer(this.props.apiId + ':' + this.props.apiKey).toString('base64')
    myHeaders.append("Authorization", "Basic " + auth);
    myHeaders.append("page", this.state.page)
    myHeaders.append("rowsPerPage", this.state.rowsPerPage)

    var requestOptions = {
      method: 'GET',
      headers: myHeaders
    };

    var url = BREWFATHERAPIURL
    if (id != undefined)
      url += '/' + id

    this.setState({ loading: true })
    fetch(url, requestOptions)
      .then(response => response.json())
      .then(result => {
        if (id != undefined)
          this.setState({ recipe: result, loading: false }, this.handleImportRecipe)
        else
          this.setState({ recipes: result, loading: false })
      })
      .catch(error => console.log('error', error));
  }

  componentWillReceiveProps(nextProps) {
    if (nextProps.brewFatherDialogOpen !== this.state.brewFatherDialogOpen) {
      this.setState({ brewFatherDialogOpen: nextProps.brewFatherDialogOpen });
    }
  }

  handleImportRecipe = () => {
    var buRecipe = {
      "mash": [],
      "boil": [],
      "cooling": []
    }
    var i = 0
    this.state.recipe.mash.steps.forEach(mash => {
      buRecipe.mash.push({
        n: mash.name,
        t: mash.stepTemp,
        tm: mash.stepTime,
        ho: true,
        r: true,
        fp: false,
        sl: false,
        index: i++
      })
    });
    this.state.recipe.hops.forEach(hop => {
      if (hop.use == 'First Wort')
        buRecipe.mash.push({
          n: hop.name + ' (First Wort)',
          t: buRecipe.mash[buRecipe.mash.length - 1].t,
          tm: 1,
          ho: true,
          r: true,
          fp: false,
          sl: false,
          index: i++
        })
    });
    i = 0;
    this.state.recipe.hops.forEach(hop => {
      if (hop.use == 'Boil')
        buRecipe.boil.push({
          n: hop.name,
          a: hop.amount,
          tm: hop.time != null ? hop.time : 0,
          index: i++
        })
    });
    i = 0;
    this.state.recipe.hops.forEach(hop => {
      if (hop.use == 'Aroma')
        buRecipe.cooling.push({
          n: hop.name,
          a: hop.amount,
          tm: hop.time != null ? hop.time : 0,
          t: hop.temp != null ? hop.temp : "",
          ho: hop.temp != null ? true : false,
          index: i++
        })
    });

    this.props.recipeSelected(buRecipe);
  }

  handleGetRecipes = () => {
    this.fetchRecipe()
  }

  handleRecipeSelected = (id) => {
    this.fetchRecipe(id)
  }

  handleClose = () => {
    this.setState({ brewFatherDialogOpen: false });
    this.props.confirmAction()
  };

  handleChangePage = (event, newPage) => {
    this.setState({ page: newPage }, this.fetchRecipe)
  };

  handleChangeRowsPerPage = (event) => {
    this.setState({ page: 0, rowsPerPage: parseInt(event.target.value, 10) }, this.fetchRecipe)
  };

  render() {
    return (
      <Dialog open={this.state.brewFatherDialogOpen} onClose={this.handleClose} aria-labelledby="form-dialog-title" fullWidth={true} maxWidth={'md'}>
        <DialogTitle id="form-dialog-title"><IntText text={"Brewfather"} /></DialogTitle>
        <DialogContent>
          <DialogContentText>
            {this.state.recipes.length > 0 ?
              <TablePagination
                component="div"
                labelDisplayedRows={(from, to, count) => { return '' }}
                labelRowsPerPage=''
                page={this.state.page}
                onChangePage={this.handleChangePage}
                rowsPerPage={this.state.rowsPerPage}
                onChangeRowsPerPage={this.handleChangeRowsPerPage}
              /> : null}
            <Paper style={{ height: 400, maxHeight: 300, overflow: 'auto' }}>
              {this.state.recipes.map((value, index) => (
                <List component="nav">
                  <ListItem color="black">
                    <ListItemText primary={value.name} secondary={
                      <React.Fragment>
                        <Typography
                          component="span"
                          variant="body2"
                          color="textPrimary">
                          {value.style.name} ({value.type})
                        </Typography>
                        @{value.author}
                      </React.Fragment>
                    }></ListItemText>
                    <IconButton aria-label="Edit">
                      <FileCopy fontSize="default" onClick={() => { this.handleRecipeSelected(value._id) }} />
                    </IconButton>
                  </ListItem>
                  <Divider />
                </List>
              ))}
            </Paper>
            {this.state.loading ? <LinearProgress /> : null}
          </DialogContentText>
        </DialogContent>
        <DialogActions>
          <Button onClick={this.handleGetRecipes} color="secondary">
            <IntText text={"GetRecipes"} />
          </Button>
          <Button onClick={this.handleClose} color="secondary">
            <IntText text={"Cancel"} />
          </Button>
        </DialogActions>
      </Dialog>
    )
  }
}

export default BrewFatherDialog;