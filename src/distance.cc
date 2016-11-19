#include <cstdlib>
#include <cmath>
#include <nan.h>
#include <v8.h>

using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::New;
using Nan::To;
using std::pow;
using std::sqrt;
using v8::Function;
using v8::Local;
using v8::Object;
using v8::String;

struct Point {
  double x;
  double y;
};

double CalculateDistance(Point* pointA, Point* pointB) {
  return sqrt(pow(pointA->x - pointB->x, 2) + pow(pointA->y - pointB->y, 2));
}

class DistanceWorker : public AsyncWorker {
 private:
  Point* pointA;
  Point* pointB;

 public:
  DistanceWorker(Callback* callback, Point* pointA, Point* pointB) :
    AsyncWorker(callback), pointA(pointA), pointB(pointB) {}

  ~DistanceWorker() {
    delete pointA;
    delete pointB;
  }

  void Execute () {

  }

  void HandleOKCallback () {

  }
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

  info.GetReturnValue().Set(CalculateDistance(pointA, pointB));
}

NAN_METHOD(CalculateAsync) {
  Local<Object> js_pointA = To<Object>(info[0]).ToLocalChecked();
  Local<Object> js_pointB = To<Object>(info[1]).ToLocalChecked();
  Callback* callback = new Callback(info[2].As<Function>());

  Point* pointA = new Point();
  pointA->x = To<double>(js_pointA->Get(New<String>("x").ToLocalChecked())).FromJust();
  pointA->y = To<double>(js_pointA->Get(New<String>("y").ToLocalChecked())).FromJust();

  Point* pointB = new Point();
  pointB->x = To<double>(js_pointB->Get(New<String>("x").ToLocalChecked())).FromJust();
  pointB->y = To<double>(js_pointB->Get(New<String>("y").ToLocalChecked())).FromJust();

  AsyncQueueWorker(new DistanceWorker(callback, pointA, pointB));
}

NAN_MODULE_INIT(Init) {
  NAN_EXPORT(target, CalculateSync);
  NAN_EXPORT(target, CalculateAsync);
}

NODE_MODULE(distance, Init)