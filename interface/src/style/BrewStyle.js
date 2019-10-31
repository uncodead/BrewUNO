const BrewStyles = theme => ({
  loadingSettings: {
    margin: theme.spacing.unit,
  },
  loadingSettingsDetails: {
    margin: theme.spacing.unit * 4,
    textAlign: "center"
  },
  button: {
    margin: theme.spacing.unit * 0.45,
    padding: theme.spacing.unit * 0.7,
  },
  button_icons: {
    marginRight: theme.spacing.unit * 0,
    padding: 0,
  },
  button_pump: {
    marginRight: theme.spacing.unit * 0.33,
    padding: 0,
  },
  button_pop: {
    marginRight: theme.spacing.unit * 0.5,
    padding: 2,
  },
  input: {
    display: 'none',
  },
  gadgetCard: {
    background: "#303030",
  },
  slider: {
    padding: '22px 0px',
  },
  sliderThumb: {
    background: '#fff',
  },
  trackBefore: {
    background: '#536dfe',
  },
  trackAfter: {
    background: '#fff',
  },
  brewSettingsCard: {
    background: "#303030",
  },
  chartCard: {
    background: "#262626",
  },
  pumpColor1: {
    color: "#447bd6",
  },
  pumpColor2: {
    color: "#5c94f2",
  },
});

export default BrewStyles;