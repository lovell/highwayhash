'use strict';

const HighwayHash = require('./build/Release/highwayhash');

const assertKey = function (key) {
  if (!(Buffer.isBuffer(key) && key.length === 32)) {
    throw new Error('Expected key to be a 32 byte Buffer: ' + key);
  }
};

const assertInput = function (input) {
  if (!Buffer.isBuffer(input)) {
    throw new Error('Expected input to be a Buffer: ' + input);
  }
};

module.exports = {
  asBuffer: function (key, input) {
    assertKey(key);
    assertInput(input);
    return HighwayHash.AsBuffer(key, input);
  },
  asString: function (key, input) {
    assertKey(key);
    assertInput(input);
    return HighwayHash.AsString(key, input);
  },
  asHexString: function (key, input) {
    assertKey(key);
    assertInput(input);
    return HighwayHash.AsHexString(key, input);
  },
  asUInt32Low: function (key, input) {
    assertKey(key);
    assertInput(input);
    return HighwayHash.AsUInt32Low(key, input);
  },
  asUInt32High: function (key, input) {
    assertKey(key);
    assertInput(input);
    return HighwayHash.AsUInt32High(key, input);
  }
};
