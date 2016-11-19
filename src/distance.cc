#include <nan.h>
#include <v8.h>

NAN_METHOD(CalculateSync) {

}

NAN_MODULE_INIT(Init) {
  NAN_EXPORT(target, CalculateSync);
}

NODE_MODULE(distance, Init)