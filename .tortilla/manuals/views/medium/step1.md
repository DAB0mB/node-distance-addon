# Step 1: Introduction to v8

![v8-logo](https://cloud.githubusercontent.com/assets/7648874/20428176/d766fba8-ad6e-11e6-884f-badc847d5e24.png)

The v8 JavaScript Engine is an open source JavaScript engine developed by The [Chromium Project](https://www.chromium.org/) for the [Google Chrome web browser](https://www.google.com/chrome/). It is intended to be used both in a browser and as a standalone high-performance engine that can be integrated into independent projects like [Couchbase](http://www.couchbase.com/), [MongoDB](https://www.mongodb.com/) and [Node.js](https://nodejs.org/). There lots of benchmarks out there but I will not bore you with diagrams, we all know that that this engine has proven itself to be worthy and it is being used world-widely and probably for a good reason.

> "Ambition is a dream with a v8 engine" -Elvis Presley.

If you're a JavaScript developer you must be familiar with the event-loop and the scoping system of v8, so it's a good thing that you understand the concept, but you never got to actually look at its source code and explicitly use it. A detailed documentation for v8's different API versions is available [here](https://v8docs.nodesource.com/). I assume that you don't bother reading any of my references that I provide along this tutorial (As a lazy blog-reader myself), I will post here that first thing you gonna see once you enter v8's documentation web-site:

![v8-docs](https://cloud.githubusercontent.com/assets/7648874/20429800/ffeb3a4a-ad77-11e6-8fae-d892304169db.jpg)

Throughout history v8 have changed a lot and it wore many forms. As a result, add-ons are not usable across different versions of the platform since each one supports a different API, which will break our process during run-time. In NodeJS team they came up with a very convenient solution called [Nan](https://github.com/nodejs/nan). Nan stands for "Native Abstractions for NodeJS" and is basically a header file filled with macro and utility goodness for making add-on development for NodeJS easier across all versions of v8, without inspecting `NODE_MODULE_VERSION` macro all the time. In this tutorial I'm gonna refer both of them as if they are bundled in a single framework.

Eventually JavaScript is just a rehash of v8, everything you know so far is still valid but it uses a different idiom. To prevent some misconceptions, here are some important points regards JavaScript's equivalents which I think you should follow:

#### Scopes and Variables

In v8 a scope is referred as `Isolate` (`v8::Isolate`) and a variable is referred as `Local` (`v8::Local<T>`). A local is a pointer to an object. All v8 objects are accessed using locals, they are necessary because of the way the v8 garbage collector works. An isolate can be thought of as a container for any number of locals. When you've finished with your locals, instead of deleting each one individually you can simply delete their scope.

##### JavaScript

```js
let obj = {
  foo: 'foo',
  bar: 'bar',
  baz: 'baz'
};

console.log(obj.foo);
console.log(obj.bar);
console.log(obj.baz);
```

##### C++

```cpp
using Nan::New;
using std::cendl;
using std::cout;
using v8::Local;
using v8::Object;
using v8::String;

Local<Object> obj = New<Object>();

obj->Set(New<String>("foo").ToLocalChecked(), New<String>("foo").ToLocalChecked());
obj->Set(New<String>("bar").ToLocalChecked(), New<String>("bar").ToLocalChecked());
obj->Set(New<String>("baz").ToLocalChecked(), New<String>("baz").ToLocalChecked());

cout << obj->Get(New<String>("foo").ToLocalChecked()) << cendl;
cout << obj->Get(New<String>("bar").ToLocalChecked()) << cendl;
cout << obj->Get(New<String>("baz").ToLocalChecked()) << cendl;
```

#### Asynchronous Callbacks

Asynchronous logic can be implemented using the `AsyncWorker` (`Nan::AsyncWorker`) and invoked by `AsyncQueueWorker` (`Nan::AsyncQueueWorker`). Thanks to these two you can have much of the annoying asynchronous queuing and handling taken care of for you. It can even store arbitrary V8 objects for you and have them persist while the asynchronous work is in progress.

##### JavaScript

```js
setImmediate(() => {
  callback(null, 'result');
});
```

##### C++

```cpp
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;
using std::string;
using v8::Local;
using v8::String;

class ResultWorker : AsyncWorker {
 private:
  string* result

 public:
  ResultWorker(Callback* callback) : AsyncWorker(callback) {}

  ~ResultDistance() {
    delete result;
  }

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on 'this'.
  void Execute () {
    result = new string("result");
  }

  // Executed when the async work is complete.
  // This function will be run inside the main event loop
  // so it is safe to use V8 again.
  void HandleOKCallback () {
    HandleScope scope;

    Local<Value> argv[] = {
      Null(),
      New<String>(result).ToLocalChecked()
    };

    callback->Call(2, argv);
  }
};

AsyncQueueWorker(new ResultWorker(callback));
```

#### Modules Registration and Methods Definition

v8 and Nan provide us with some handy macros (`NODE_MODULE`, `NAN_MODULE_INIT`, `NAN_METHOD` and `NODE_SET_METHOD`) which will help us register a new NodeJS module and define its methods. This might be confusing for some, since we can't see the function's signature it would appear as if variables are just being magically created in the stack, but once the macros are being pre-processed they will just turn into ordinary functions. In the example below I commented the original signature so you can have more clew on what's going on.

##### JavaScript

```js
exports.fn = (a, b) => a + b;
```

##### C++

```cpp
using Nan::To;
using v8::Local;
using v8::Object;

// void Fn(FunctionCallbackInfo<Value>& info)
NAN_METHOD(Fn) {
  double a = To<double>(info[0]).FromJust();
  double b = To<double>(info[1]).FromJust();

  info.GetReturnValue().Set(a + b);
}

// void Init(Local<Object> target)
NAN_MODULE_INIT(Init) {
  NODE_SET_METHOD(target, Fn);
}

// First argument would be the entry file's name
NODE_MODULE(addon, Init);
```

---

As you can see when dealing with v8 explicitly is a time consuming process which requires you to do lots of extra-work. With that said, keep in mind that only a small portion of your code is gonna interact with the engine since the core logic should be written using native C++ and other third-party libraries. You always need to find the right balance. Always make sure that your add-on doesn't require too much data to be passed otherwise the conversion process is gonna be hard and inefficient, and think twice before you choose this approach for the sake of simplicity. Overall the estimated optimization should be around 150% and up, depends on the task, first check your JavaScript code snippet, check for unnecessary logic and optimize it, and if you're really sure that it is fully optimized and you're still striving for more performance, *only then* move to C++.

So far I went through the very basics which will help you create this bridge between the two platforms. The v8 lacks of detailed documentation, tutorials and examples. [Nan](https://github.com/nodejs/nan) however is a bit more documented IMHO, so when I approach the API documentation I would start from there, and if I didn't find anything useful I would look at [v8's latest API docs](https://v8docs.nodesource.com/). It's not a hard material to learn but it's different so it might be a bit challenging for some, but remember, practice practice practice.

Speaking of practice, let's move on to the next step where we gonna implement our first add-on for distance calculation between two points.

[{]: <helper> (navStep)

⟸ <a href="https://github.com/DAB0mB/node-distance-addon/tree/master/.tortilla/README.md">INTRO</a> <b>║</b> <a href="https://github.com/DAB0mB/node-distance-addon/tree/master/.tortilla/manuals/views/medium/step2.md">NEXT STEP</a> ⟹

[}]: #
