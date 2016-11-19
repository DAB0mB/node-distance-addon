#include <nan.h>
#include <v8.h>

using Nan::To;
using v8::Local;
using v8::Object;

NAN_METHOD(CalculateSync) {
  Local<Object> js_pointA = To<Object>(info[0]).ToLocalChecked();
  Local<Object> js_pointB = To<Object>(info[1]).ToLocalChecked();
}

NAN_MODULE_INIT(Init) {
  NAN_EXPORT(target, CalculateSync);
}

NODE_MODULE(distance, Init)