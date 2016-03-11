# highwayhash

Node.js implementation of Google's AVX2+ [HighwayHash](https://github.com/google/highwayhash).

Based on [SipHash](https://en.wikipedia.org/wiki/SipHash),
it is believed to be robust against hash flooding and timing attacks
because memory accesses are sequential and the algorithm is branch-free.

This makes it suitable for random number generators and hash tables storing untrusted data.

Expect data throughput of up to 11 GB/s,
which translates to about 3 million operations/second when used with Node.js.

As JavaScript lacks native support for 64-bit integers,
hash values are made available as string, Buffer and
low/high 32-bit unsigned integer types.

If the input to be hashed is trusted, a cryptographically-insecure alternative is
[FarmHash](https://github.com/lovell/farmhash).
It can be almost an order of magnitude faster,
especially for smaller inputs when only 32-bit hash values are required.

## Requirements

* Node.js v4+
* [CPU with AVX2](https://en.wikipedia.org/wiki/Advanced_Vector_Extensions#CPUs_with_AVX2)

## Installation

```sh
npm install highwayhash
```

## Usage

```javascript
const highwayhash = require('highwayhash');
```

```javascript
const key = new Buffer([
  0x55, 0xce, 0x85, 0x31, 0x06, 0x5e, 0xdc, 0x68,
  0x0b, 0x46, 0x14, 0xb6, 0x0c, 0xfe, 0x80, 0xcc,
  0x7d, 0xcf, 0x89, 0xe5, 0x83, 0xfe, 0x9a, 0xae,
  0x1c, 0x8b, 0xee, 0xeb, 0x3e, 0xe3, 0x1d, 0x1d
]);

const input = 'The quick brown fox jumped over the lazy sleeping dog';

const hashAsString = highwayhash.asString(key, input);
// '13898145506225518925'

const hashAsUInt32Low = highwayhash.asUInt32Low(key, input);
// 3477305677

const hashAsUInt32High = highwayhash.asUInt32High(key, input);
// 3235914163

const hashAsBuffer = highwayhash.asBuffer(key, input);
// <Buffer 4d 79 43 cf b3 21 e0 c0>

```

## API

* `key` is a Buffer containing 32 bytes (256-bit)
* `input` is a Buffer to calculate a hash value of

### asString(key, input)

Returns a String representing the 64-bit unsigned integer hash value of `input`.

### asBuffer(key, input)

Returns a Buffer representing the 64-bit unsigned integer hash value of `input`.

This method is much slower then `asString` so only use this method when the hash value needs to be in a Buffer.

### asUInt32Low(key, input)

Returns a Number representing the low 32-bits of the 64-bit unsigned integer hash value of `input`.

### asUInt32High(key, input)

Returns a Number representing the high 32-bits of the 64-bit unsigned integer hash value of `input`.

## Benchmarks

[Intel i3-4170](http://ark.intel.com/products/77490/Intel-Core-i3-4170-Processor-3M-Cache-3_70-GHz)

| Input size / bytes | Hash function  | Hash size / bits | Output data type  | Ops/sec   |
| -----------------: | :------------- | ---------------: | :---------------- | --------: |
|                100 | FarmHash       |               32 | string            | 9,007,638 |
|                100 | FarmHash       |               64 | string            | 1,702,653 |
|                100 | HighwayHash    |               32 | 32-bit int (low)  | 4,338,413 |
|                100 | HighwayHash    |               32 | 32-bit int (high) | 4,372,574 |
|                100 | HighwayHash    |               64 | string            | 3,278,853 |
|               1000 | FarmHash       |               32 | string            | 3,352,427 |
|               1000 | FarmHash       |               64 | string            | 1,506,128 |
|               1000 | HighwayHash    |               32 | 32-bit int (low)  | 3,090,887 |
|               1000 | HighwayHash    |               32 | 32-bit int (high) | 3,082,453 |
|               1000 | HighwayHash    |               64 | string            | 2,580,694 |
|              10000 | FarmHash       |               32 | string            | 1,535,208 |
|              10000 | FarmHash       |               64 | string            |   978,921 |
|              10000 | HighwayHash    |               32 | 32-bit int (low)  | 1,082,865 |
|              10000 | HighwayHash    |               32 | 32-bit int (high) | 1,081,761 |
|              10000 | HighwayHash    |               64 | string            |   901,997 |

```sh
git clone https://github.com/lovell/highwayhash
cd highwayhash
npm install && npm test
cd bench
npm install && npm test
```

## Licence

Copyright 2016 Lovell Fuller.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Copyright 2015 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
