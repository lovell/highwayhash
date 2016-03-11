'use strict';

const crypto = require('crypto');
const Benchmark = require('benchmark');
const farmhash = require('farmhash');
const highwayhash = require('../index');

const key = crypto.randomBytes(32);

[100, 1000, 10000].forEach(function (length) {
  const input = crypto.randomBytes(length);
  console.log('Using input of length ' + length);

  (new Benchmark.Suite())
  .add('32-farmhash', function () {
    farmhash.hash32(input);
  })
  .add('32-highwayhash-low', function () {
    highwayhash.asUInt32Low(key, input);
  })
  .add('32-highwayhash-high', function () {
    highwayhash.asUInt32High(key, input);
  })
  .add('64-farmhash-uint64-as-string', function () {
    farmhash.hash64(input);
  })
  .add('64-highwayhash-uint64-as-string', function () {
    highwayhash.asString(key, input);
  })
  .on('cycle', function (event) {
    console.log(String(event.target));
  }).run();
});
