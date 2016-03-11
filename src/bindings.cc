#ifndef __AVX2__
#error *** CPU does not support AVX2 intrinsics ***
#endif

#include <node.h>

#include "nan.h"
#include "upstream/highway_tree_hash.h"

inline uint64_t AsUInt64(uint8_t const *key, uint8_t const *value, size_t const length) {
  // Convert key from uint8_t[32] to uint64_t[4]
  uint64_t key64[4];
  key64[0] = *reinterpret_cast<uint64_t const*>(&key[0]);
  key64[1] = *reinterpret_cast<uint64_t const*>(&key[8]);
  key64[2] = *reinterpret_cast<uint64_t const*>(&key[16]);
  key64[3] = *reinterpret_cast<uint64_t const*>(&key[24]);
  // Generate hash
  return HighwayTreeHash(key64, value, length);
}

// Hash methods

NAN_METHOD(AsBuffer) {
  Nan::HandleScope();
  Nan::TypedArrayContents<uint8_t> key(info[0]);
  Nan::TypedArrayContents<uint8_t> value(info[1]);

  uint64_t const hash = AsUInt64(*key, *value, value.length());

  info.GetReturnValue().Set(
    Nan::CopyBuffer(reinterpret_cast<char const*>(&hash), 8).ToLocalChecked()
  );
}

NAN_METHOD(AsString) {
  Nan::HandleScope();
  Nan::TypedArrayContents<uint8_t> key(info[0]);
  Nan::TypedArrayContents<uint8_t> value(info[1]);

  uint64_t const hash = AsUInt64(*key, *value, value.length());

  info.GetReturnValue().Set(Nan::New(std::to_string(hash)).ToLocalChecked());
}

NAN_METHOD(AsUInt32Low) {
  Nan::HandleScope();
  Nan::TypedArrayContents<uint8_t> key(info[0]);
  Nan::TypedArrayContents<uint8_t> value(info[1]);

  uint64_t const hash = AsUInt64(*key, *value, value.length());

  info.GetReturnValue().Set(Nan::New(static_cast<uint32_t>(hash & 0xFFFFFFFF)));
}

NAN_METHOD(AsUInt32High) {
  Nan::HandleScope();
  Nan::TypedArrayContents<uint8_t> key(info[0]);
  Nan::TypedArrayContents<uint8_t> value(info[1]);

  uint64_t const hash = AsUInt64(*key, *value, value.length());

  info.GetReturnValue().Set(Nan::New(static_cast<uint32_t>(hash >> 32)));
}

// Init
NAN_MODULE_INIT(init) {
  Nan::Set(target, Nan::New("AsBuffer").ToLocalChecked(),
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(AsBuffer)).ToLocalChecked());
  Nan::Set(target, Nan::New("AsString").ToLocalChecked(),
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(AsString)).ToLocalChecked());
  Nan::Set(target, Nan::New("AsUInt32Low").ToLocalChecked(),
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(AsUInt32Low)).ToLocalChecked());
  Nan::Set(target, Nan::New("AsUInt32High").ToLocalChecked(),
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(AsUInt32High)).ToLocalChecked());
}

NODE_MODULE(highwayhash, init)
