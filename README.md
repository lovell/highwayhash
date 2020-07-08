# highwayhash

Node.js implementation of Google's [HighwayHash](https://github.com/google/highwayhash).

Based on [SipHash](https://en.wikipedia.org/wiki/SipHash),
it is believed to be robust against hash flooding and timing attacks
because memory accesses are sequential and the algorithm is branch-free.

This makes it suitable for random number generators and hash tables storing untrusted data.

64-bit hash values are platform independent and will not change for a given input.
This is important for applications that write hashes to persistent storage.

SIMD intrinsics (AVX2, SSE4.1) will be used when available at runtime.

Expect up to 8 million operations/second,
depending on the length of the input and the output type required.

As JavaScript lacks native support for 64-bit integers,
hash values are made available as hex, string, Buffer and
low/high 32-bit unsigned integer types.

If the input to be hashed is trusted,
a cryptographically-insecure alternative is
[FarmHash](https://github.com/lovell/farmhash).

Pre-compiled binaries are provided for the most common platforms.

## Requirements

* x64 CPU
* Node.js v10+

## Installation

```sh
npm install highwayhash
```

```sh
yarn add highwayhash
```

## Usage

```javascript
const highwayhash = require('highwayhash');
```

```javascript
const key = require('crypto').randomBytes(32);

const input = Buffer.from('The quick brown fox jumped over the lazy sleeping dog');

const hashAsString = highwayhash.asString(key, input);
// Example: '15456351453344120596'

const hashAsHexString = highwayhash.asHexString(key, input);
// Example: '143f2b6cc1fd7fd6'

const hashAsUInt32Low = highwayhash.asUInt32Low(key, input);
// Example: 1814773524

const hashAsUInt32High = highwayhash.asUInt32High(key, input);
// Example: 3598712257

const hashAsBuffer = highwayhash.asBuffer(key, input);
// Example: <Buffer 14 3f 2b 6c c1 fd 7f d6>

```

## API

* `key` is a Buffer containing 32 bytes (256-bit)
* `input` is a Buffer to calculate a hash value of

### asString(key, input)

Returns a String representing the 64-bit unsigned integer hash value of `input`.

### asHexString(key, input)

Returns a hexadecimal String representing the 64-bit unsigned integer hash value of `input`.
This is equivalent to but much faster than `asBuffer().toString('hex')`.

### asBuffer(key, input)

Returns a Buffer representing the 64-bit unsigned integer hash value of `input`.

This method is much slower then `asString` so only use this method when the hash value needs to be in a Buffer.

### asUInt32Low(key, input)

Returns a Number representing the low 32-bits of the 64-bit unsigned integer hash value of `input`.

### asUInt32High(key, input)

Returns a Number representing the high 32-bits of the 64-bit unsigned integer hash value of `input`.

## Benchmarks

* [Intel i3-4170](http://ark.intel.com/products/77490/Intel-Core-i3-4170-Processor-3M-Cache-3_70-GHz)
* Ubuntu 16.04.1 LTS
* Node.js v6.9.4
* Cryptographically strong pseudo-random input via OpenSSL's RAND_bytes

| Input size / bytes | Hash function  | Hash size / bits | Output data type  | Ops/sec   |
| -----------------: | :------------- | ---------------: | :---------------- | --------: |
|                100 | md5            |              128 | Buffer            |   577,384 |
|                100 | sha256         |              256 | Buffer            |   516,888 |
|                100 | FarmHash       |               32 | 32-bit int        | 3,870,645 |
|                100 | FarmHash       |               64 | string            | 1,332,578 |
|                100 | HighwayHash    |               32 | 32-bit int (low)  | 5,534,449 |
|                100 | HighwayHash    |               32 | 32-bit int (high) | 5,626,820 |
|                100 | HighwayHash    |               64 | string            | 2,583,533 |
|                100 | HighwayHash    |               64 | hex string        | 3,477,324 |
|               1000 | md5            |              128 | Buffer            |   343,203 |
|               1000 | sha256         |              256 | Buffer            |   259,395 |
|               1000 | FarmHash       |               32 | 32-bit int        | 3,836,197 |
|               1000 | FarmHash       |               64 | string            | 1,219,728 |
|               1000 | HighwayHash    |               32 | 32-bit int (low)  | 5,531,220 |
|               1000 | HighwayHash    |               32 | 32-bit int (high) | 5,609,610 |
|               1000 | HighwayHash    |               64 | string            | 2,616,148 |
|               1000 | HighwayHash    |               64 | hex string        | 3,520,123 |
|              10000 | md5            |              128 | Buffer            |    67,178 |
|              10000 | sha256         |              256 | Buffer            |    44,260 |
|              10000 | FarmHash       |               32 | 32-bit int        | 1,462,781 |
|              10000 | FarmHash       |               64 | string            |   832,073 |
|              10000 | HighwayHash    |               32 | 32-bit int (low)  | 3,949,544 |
|              10000 | HighwayHash    |               32 | 32-bit int (high) | 3,974,480 |
|              10000 | HighwayHash    |               64 | string            | 2,613,013 |
|              10000 | HighwayHash    |               64 | hex string        | 3,537,567 |

```sh
git clone https://github.com/lovell/highwayhash
cd highwayhash
npm install && npm test
cd bench
npm install && npm test
```

## Licence

Copyright 2016, 2017, 2018, 2019, 2020 Lovell Fuller.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Copyright 2015, 2016, 2017 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
