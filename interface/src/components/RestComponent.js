import React from 'react';
import { withSnackbar } from 'notistack';

/*
* It is unlikely this application will grow complex enough to require redux.
*
* This HOC acts as an interface to a REST service, providing data and change
* event callbacks to the wrapped components along with a function to persist the
* changes.
*/
export const restComponent = (endpointUrl, FormComponent) => {

  return withSnackbar(
    class extends React.Component {

      constructor(props) {
        super(props);

        this.state = {
          data: null,
          fetched: false,
          errorMessage: null,
          floats: [],
        };

        this.setState = this.setState.bind(this);
        this.loadData = this.loadData.bind(this);
        this.saveData = this.saveData.bind(this);
        this.setData = this.setData.bind(this);
      }

      setData(data) {
        this.setState({
          data: data,
          fetched: true,
          errorMessage: null
        });
      }

      loadData() {
        this.setState({
          data: null,
          fetched: false,
          errorMessage: null
        });
        fetch(endpointUrl)
          .then(response => {
            if (response.status === 200) {
              return response.json();
            }
            throw Error("Invalid status code: " + response.status);
          })
          .then(json => { this.setState({ data: json, fetched: true }) })
          .catch(error => {
            this.props.enqueueSnackbar("Problem fetching: " + error.message, { variant: 'error', autoHideDuration: 2000, });
            this.setState({ data: null, fetched: true, errorMessage: error.message });
          });
      }

      saveData(e) {
        this.setState({ fetched: false });
        this.state.floats.map((name, i) => {
          const { data } = this.state;
          data[name] = parseFloat(data[name])
          this.setState({ data });
        });
        fetch(endpointUrl, {
          method: 'POST',
          body: JSON.stringify(this.state.data),
          headers: new Headers({
            'Content-Type': 'application/json'
          })
        })
          .then(response => {
            if (response.status === 200) {
              return response.json();
            }
            throw Error("Invalid status code: " + response.status);
          })
          .then(json => {
            this.props.enqueueSnackbar("Changes successfully applied.", { variant: 'info', autoHideDuration: 2000, });
            this.setState({ data: json, fetched: true });
          }).catch(error => {
            this.props.enqueueSnackbar("Problem saving: " + error.message, { variant: 'error', autoHideDuration: 2000, });
            this.setState({ data: null, fetched: true, errorMessage: error.message });
          });
      }

      handleFloatValueChange = name => event => {
        if (!this.state.floats.includes(name))
          this.setState({
            floats: [...this.state.floats, name],
          });
        this.handleValueChange(name)(event)
      };

      handleValueChange = name => event => {
        const { data } = this.state;
        data[name] = event.target.value;
        this.setState({ data });
      };

      handleCheckboxChange = name => event => {
        const { data } = this.state;
        data[name] = event.target.checked;
        this.setState({ data });
      }

      render() {
        return <FormComponent
          handleValueChange={this.handleValueChange}
          handleFloatValueChange={this.handleFloatValueChange}
          handleCheckboxChange={this.handleCheckboxChange}
          setData={this.setData}
          saveData={this.saveData}
          loadData={this.loadData}
          {...this.state}
          {...this.props}
        />;
      }

    }
  );
}
