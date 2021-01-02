'use strict';

const crypto = require('crypto');
const Benchmark = require('benchmark');
const farmhash = require('farmhash');
const highwayhasher = require('highwayhasher');
const highwayhash = require('../index');

const key = crypto.randomBytes(32);

(async () => {
  const highwayhasherModule = await highwayhasher.HighwayHash.loadModule();

  [100, 1000, 10000, 100000].forEach(function (length) {
    const input = crypto.randomBytes(length);
    console.log('Using input of length ' + length);

    (new Benchmark.Suite())
      .add('md5', function () {
        crypto.createHash('md5').update(input).digest();
      })
      .add('sha256', function () {
        crypto.createHash('sha256').update(input).digest();
      })
      .add('highwayhasher', function () {
        const hasher = highwayhasherModule.create(key);
        hasher.append(input);
        hasher.finalize64();
      })
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
      .add('64-highwayhash-uint64-as-hex-string', function () {
        highwayhash.asHexString(key, input);
      })
      .add('64-highwayhash-as-buffer', function () {
        highwayhash.asBuffer(key, input);
      })
      .on('cycle', function (event) {
        console.log(String(event.target));
      }).run();
  });
})();
