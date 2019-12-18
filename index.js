'use strict';

const AbortInspector = require('bindings')('AbortInspector');

let monitoring = false;

function start () {
  if (!monitoring) {
    monitoring = true;
    AbortInspector.monitor();
  }

  return null;
}

module.exports = start();
