#include <nan.h>
#include <v8.h>

using Nan::New;
using Nan::To;
using v8::Local;
using v8::Object;
using v8::String;

struct Point {
  double x;
  double y;
};

NAN_METHOD(CalculateSync) {
  Local<Object> js_pointA = To<Object>(info[0]).ToLocalChecked();
  Local<Object> js_pointB = To<Object>(info[1]).ToLocalChecked();

  Point* pointA = new Point();
  pointA->x = To<double>(js_pointA->Get(New<String>("x").ToLocalChecked())).FromJust();
  pointA->y = To<double>(js_pointA->Get(New<String>("y").ToLocalChecked())).FromJust();

  Point* pointB = new Point();
  pointB->x = To<double>(js_pointB->Get(New<String>("x").ToLocalChecked())).FromJust();
  pointB->y = To<double>(js_pointB->Get(New<String>("y").ToLocalChecked())).FromJust();
}

NAN_MODULE_INIT(Init) {
  NAN_EXPORT(target, CalculateSync);
}

NODE_MODULE(distance, Init)