import React, { Component } from 'react';
import Button from '@material-ui/core/Button';
import { withSnackbar } from 'notistack';
import IntText from '../components/IntText'
import Files from 'react-files'

class ImportConfig extends Component {
  constructor(props) {
    super(props)
  }

  import = (files) => {
    this.fileReader = new FileReader();
    this.fileReader.readAsText(files[files.length - 1]);
    this.fileReader.onload = event => {
      this.props.onImport(JSON.parse(event.target.result))
    };
  }

  export = () => {
    var dataStr = "data:text/json;charset=utf-8," + encodeURIComponent(JSON.stringify(this.props.items));
    var downloadAnchorNode = document.createElement('a');
    downloadAnchorNode.setAttribute("href", dataStr);
    downloadAnchorNode.setAttribute("download", this.props.name + ".json");
    document.body.appendChild(downloadAnchorNode);
    downloadAnchorNode.click();
    downloadAnchorNode.remove();
  }

  render() {
    return (
      <div>
        {this.props.items.length > 0 ?
          <Button variant="contained" fullWidth color="secondary" onClick={this.export}>{<IntText text="BrewSettings.Export" />}</Button>
          : null}
        <Files
          onChange={this.import}
          accepts={['.json']}
          maxFileSize={10000000}
          minFileSize={0}
          multiple
          maxFiles={3}
        >
          <Button variant="contained" fullWidth color="primary">{<IntText text="BrewSettings.Import" />}</Button>
        </Files>
      </div>
    )
  }
}

export default withSnackbar(ImportConfig);
