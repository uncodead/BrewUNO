"use strict";

Object.defineProperty(exports, "__esModule", {
    value: true
});
var debounce = function debounce(func, wait, immediate) {
    var timeout = void 0;
    function cancel() {
        if (timeout !== undefined) {
            clearTimeout(timeout);
        }
    }
    var debounced = function debounced() {
        for (var _len = arguments.length, args = Array(_len), _key = 0; _key < _len; _key++) {
            args[_key] = arguments[_key];
        }

        var context = this;
        var later = function delayed() {
            timeout = null;
            if (!immediate) {
                func.apply(context, args);
            }
        };
        var callNow = immediate && !timeout;
        clearTimeout(timeout);
        timeout = setTimeout(later, wait);
        if (callNow) {
            func.apply(context, args);
        }
    };
    debounced.cancel = cancel;
    return debounced;
};

exports.debounce = debounce;