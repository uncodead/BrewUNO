'use strict';

Object.defineProperty(exports, "__esModule", {
    value: true
});

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _react = require('react');

var _react2 = _interopRequireDefault(_react);

var _propTypes = require('prop-types');

var _propTypes2 = _interopRequireDefault(_propTypes);

var _reactLifecyclesCompat = require('react-lifecycles-compat');

var _ValidatorForm = require('./ValidatorForm');

var _ValidatorForm2 = _interopRequireDefault(_ValidatorForm);

var _utils = require('./utils');

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; } /* eslint-disable */

/* eslint-enable */


var ValidatorComponent = function (_React$Component) {
    _inherits(ValidatorComponent, _React$Component);

    function ValidatorComponent() {
        var _ref;

        var _temp, _this, _ret;

        _classCallCheck(this, ValidatorComponent);

        for (var _len = arguments.length, args = Array(_len), _key = 0; _key < _len; _key++) {
            args[_key] = arguments[_key];
        }

        return _ret = (_temp = (_this = _possibleConstructorReturn(this, (_ref = ValidatorComponent.__proto__ || Object.getPrototypeOf(ValidatorComponent)).call.apply(_ref, [this].concat(args))), _this), _this.state = {
            isValid: true,
            value: _this.props.value,
            errorMessages: _this.props.errorMessages,
            validators: _this.props.validators
        }, _this.getErrorMessage = function () {
            var errorMessages = _this.state.errorMessages;

            var type = typeof errorMessages === 'undefined' ? 'undefined' : _typeof(errorMessages);

            if (type === 'string') {
                return errorMessages;
            } else if (type === 'object') {
                if (_this.invalid.length > 0) {
                    return errorMessages[_this.invalid[0]];
                }
            }
            // eslint-disable-next-line
            console.log('unknown errorMessages type', errorMessages);
            return true;
        }, _this.instantValidate = true, _this.invalid = [], _this.configure = function () {
            if (!_this.props.name) {
                throw new Error('Form field requires a name property when used');
            }
            _this.context.form.attachToForm(_this);
            _this.instantValidate = _this.context.form.instantValidate;
            _this.debounceTime = _this.context.form.debounceTime;
            _this.validateDebounced = (0, _utils.debounce)(_this.validate, _this.debounceTime);
        }, _this.validate = function (value) {
            var includeRequired = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : false;
            var dryRun = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : false;

            _this.invalid = [];
            var result = [];
            var valid = true;
            _this.state.validators.map(function (validator, i) {
                var obj = {};
                obj[i] = _ValidatorForm2.default.getValidator(validator, value, includeRequired);
                return result.push(obj);
            });
            result.map(function (item) {
                return Object.keys(item).map(function (key) {
                    if (!item[key]) {
                        valid = false;
                        _this.invalid.push(key);
                    }
                    return key;
                });
            });

            if (!dryRun) {
                _this.setState({ isValid: valid }, function () {
                    _this.props.validatorListener(_this.state.isValid);
                });
            }
        }, _this.isValid = function () {
            return _this.state.isValid;
        }, _this.makeInvalid = function () {
            _this.setState({ isValid: false });
        }, _this.makeValid = function () {
            _this.setState({ isValid: true });
        }, _temp), _possibleConstructorReturn(_this, _ret);
    }

    _createClass(ValidatorComponent, [{
        key: 'componentDidMount',
        value: function componentDidMount() {
            this.configure();
        }
    }, {
        key: 'shouldComponentUpdate',
        value: function shouldComponentUpdate(nextProps, nextState) {
            return this.state !== nextState || this.props !== nextProps;
        }
    }, {
        key: 'componentDidUpdate',
        value: function componentDidUpdate(prevProps, prevState) {
            if (this.instantValidate && this.props.value !== prevState.value) {
                this.validateDebounced(this.props.value, this.props.withRequiredValidator);
            }
        }
    }, {
        key: 'componentWillUnmount',
        value: function componentWillUnmount() {
            this.context.form.detachFromForm(this);
            this.validateDebounced.cancel();
        }
    }], [{
        key: 'getDerivedStateFromProps',
        value: function getDerivedStateFromProps(nextProps, prevState) {
            if (nextProps.validators && nextProps.errorMessages && (prevState.validators !== nextProps.validators || prevState.errorMessages !== nextProps.errorMessages)) {
                return {
                    value: nextProps.value,
                    validators: nextProps.validators,
                    errorMessages: nextProps.errorMessages
                };
            }

            return {
                value: nextProps.value
            };
        }
    }]);

    return ValidatorComponent;
}(_react2.default.Component);

ValidatorComponent.contextTypes = {
    form: _propTypes2.default.object
};

ValidatorComponent.propTypes = {
    errorMessages: _propTypes2.default.oneOfType([_propTypes2.default.array, _propTypes2.default.string]),
    validators: _propTypes2.default.array,
    name: _propTypes2.default.string,
    value: _propTypes2.default.any,
    validatorListener: _propTypes2.default.func,
    withRequiredValidator: _propTypes2.default.bool
};

ValidatorComponent.defaultProps = {
    errorMessages: 'error',
    validators: [],
    validatorListener: function validatorListener() {}
};

(0, _reactLifecyclesCompat.polyfill)(ValidatorComponent);

exports.default = ValidatorComponent;