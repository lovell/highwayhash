// Copyright 2017 Lovell Fuller.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <napi.h>

#include "highwayhash/highwayhash_target.h"
#include "highwayhash/instruction_sets.h"

inline uint64_t
AsUInt64(uint8_t const *key, char const *value, size_t const length) {
  using highwayhash::HHKey;
  using highwayhash::HHResult64;
  using highwayhash::InstructionSets;
  using highwayhash::HighwayHash;

  // Convert key from uint8_t[32] to uint64_t[4] HHKey
  HHKey const key64 HH_ALIGNAS(32) = {
    *reinterpret_cast<uint64_t const*>(&key[0]),
    *reinterpret_cast<uint64_t const*>(&key[8]),
    *reinterpret_cast<uint64_t const*>(&key[16]),
    *reinterpret_cast<uint64_t const*>(&key[24])
  };
  // Generate hash
  HHResult64 result;
  InstructionSets::Run<HighwayHash>(key64, value, length, &result);
  return static_cast<uint64_t>(result);
}

// Convert from uint64_t to hex string
inline std::string AsHex(uint64_t const hash) {
  static const char* lut = "0123456789abcdef";
  std::string hex(16, '0');
  hex[0]  = lut[(hash >> 4)  & 0x0f];
  hex[1]  = lut[ hash        & 0x0f];
  hex[2]  = lut[(hash >> 12) & 0x0f];
  hex[3]  = lut[(hash >> 8)  & 0x0f];
  hex[4]  = lut[(hash >> 20) & 0x0f];
  hex[5]  = lut[(hash >> 16) & 0x0f];
  hex[6]  = lut[(hash >> 28) & 0x0f];
  hex[7]  = lut[(hash >> 24) & 0x0f];
  hex[8]  = lut[(hash >> 36) & 0x0f];
  hex[9]  = lut[(hash >> 32) & 0x0f];
  hex[10] = lut[(hash >> 44) & 0x0f];
  hex[11] = lut[(hash >> 40) & 0x0f];
  hex[12] = lut[(hash >> 52) & 0x0f];
  hex[13] = lut[(hash >> 48) & 0x0f];
  hex[14] = lut[(hash >> 60) & 0x0f];
  hex[15] = lut[(hash >> 56) & 0x0f];
  return hex;
}

// Hash methods

Napi::Value AsBuffer(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::TypedArrayOf<uint8_t> key = info[0].As<Napi::TypedArrayOf<uint8_t>>();
  Napi::TypedArrayOf<char> value = info[1].As<Napi::TypedArrayOf<char>>();

  uint64_t const hash = AsUInt64(key.Data(), value.Data(), value.ByteLength());

  return Napi::Buffer<char>::Copy(env, reinterpret_cast<char const*>(&hash), 8);
}

Napi::Value AsString(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::TypedArrayOf<uint8_t> key = info[0].As<Napi::TypedArrayOf<uint8_t>>();
  Napi::TypedArrayOf<char> value = info[1].As<Napi::TypedArrayOf<char>>();

  uint64_t const hash = AsUInt64(key.Data(), value.Data(), value.ByteLength());

  return Napi::String::New(env, std::to_string(hash));
}

Napi::Value AsHexString(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::TypedArrayOf<uint8_t> key = info[0].As<Napi::TypedArrayOf<uint8_t>>();
  Napi::TypedArrayOf<char> value = info[1].As<Napi::TypedArrayOf<char>>();

  uint64_t const hash = AsUInt64(key.Data(), value.Data(), value.ByteLength());

  return Napi::String::New(env, AsHex(hash));
}

Napi::Value AsUInt32Low(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::TypedArrayOf<uint8_t> key = info[0].As<Napi::TypedArrayOf<uint8_t>>();
  Napi::TypedArrayOf<char> value = info[1].As<Napi::TypedArrayOf<char>>();

  uint64_t const hash = AsUInt64(key.Data(), value.Data(), value.ByteLength());

  return Napi::Number::New(env, static_cast<uint32_t>(hash));
}

Napi::Value AsUInt32High(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::TypedArrayOf<uint8_t> key = info[0].As<Napi::TypedArrayOf<uint8_t>>();
  Napi::TypedArrayOf<char> value = info[1].As<Napi::TypedArrayOf<char>>();

  uint64_t const hash = AsUInt64(key.Data(), value.Data(), value.ByteLength());

  return Napi::Number::New(env, static_cast<uint32_t>(hash >> 32));
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports["AsBuffer"] = Napi::Function::New(env, AsBuffer);
  exports["AsString"] = Napi::Function::New(env, AsString);
  exports["AsHexString"] = Napi::Function::New(env, AsHexString);
  exports["AsUInt32Low"] = Napi::Function::New(env, AsUInt32Low);
  exports["AsUInt32High"] = Napi::Function::New(env, AsUInt32High);
  return exports;
}

NODE_API_MODULE(highwayhash, Init)
