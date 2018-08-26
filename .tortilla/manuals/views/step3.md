# Step 3: Creating the add-on

[//]: # (head-end)


In this step we will base our development process on the TDD methodology, so you will have a chance to look at the final target API that we desire. We will start by writing a test file:

[{]: <helper> (diffStep 3.1)

#### [Step 3.1: Add test file](https://github.com/DAB0mB/node-distance-addon/commit/dae3a3f)

##### Added .npmignore
```diff
@@ -0,0 +1 @@
+┊ ┊1┊test.js🚫↵
```

##### Added test.js
```diff
@@ -0,0 +1,23 @@
+┊  ┊ 1┊const Distance = require('.');
+┊  ┊ 2┊
+┊  ┊ 3┊let result;
+┊  ┊ 4┊let pointA = { x: 0, y: 0 };
+┊  ┊ 5┊let pointB = { x: 3, y: 4 };
+┊  ┊ 6┊
+┊  ┊ 7┊result = Distance.calculate.sync(pointA, pointB);
+┊  ┊ 8┊
+┊  ┊ 9┊if (result !== 5) throw Error(
+┊  ┊10┊  '#Sync: Result expected to equal 5 but instead got ' + result
+┊  ┊11┊);
+┊  ┊12┊
+┊  ┊13┊console.log('sync calculation passed');
+┊  ┊14┊
+┊  ┊15┊result = Distance.calculate.async(pointA, pointB, (err, result) => {
+┊  ┊16┊  if (err) throw err;
+┊  ┊17┊
+┊  ┊18┊  if (result !== 5) throw Error(
+┊  ┊19┊    '#Async: Result expected to equal 5 but instead got ' + result
+┊  ┊20┊  );
+┊  ┊21┊
+┊  ┊22┊  console.log('async calculation passed');
+┊  ┊23┊});🚫↵
```

[}]: #

And the following NPM script should execute it:

[{]: <helper> (diffStep 3.2)

#### [Step 3.2: Add npm test script](https://github.com/DAB0mB/node-distance-addon/commit/bd7080f)

##### Changed package.json
```diff
@@ -6,5 +6,8 @@
 ┊ 6┊ 6┊  "repository": {
 ┊ 7┊ 7┊    "type": "git",
 ┊ 8┊ 8┊    "url": "https://DAB0mB@github.com/DAB0mB/node-distance-addon.git"
+┊  ┊ 9┊  },
+┊  ┊10┊  "scripts": {
+┊  ┊11┊    "test": "node test"
 ┊ 9┊12┊  }
 ┊10┊13┊}
```

[}]: #

Like I said in the introduction, it's a simple module which can calculate the distance between 2 given points. `calculate.sync` can do it synchronously and `calculate.async` can do it asynchronously. Now that you got the idea we will start configuring our add-on.

The first thing you'll need to do is to make sure that you have `node-gyp` installed:

    $ sudo npm install -g node-gyp

`node-gyp` is also dependent on many other packages, so before you go any further please take a look at the official installation instructions in their [README.md file](https://github.com/nodejs/node-gyp).

Assuming that you have installed everything properly, we will now need to create a `binding.gyp` file:

[{]: <helper> (diffStep 3.3 files="binding.gyp")

#### [Step 3.3: Create &#x27;binding.gyp&#x27; file](https://github.com/DAB0mB/node-distance-addon/commit/1214e64)

##### Added binding.gyp
```diff
@@ -0,0 +1,11 @@
+┊  ┊ 1┊{
+┊  ┊ 2┊  "targets": [
+┊  ┊ 3┊    {
+┊  ┊ 4┊      "target_name": "distance",
+┊  ┊ 5┊      "sources": [
+┊  ┊ 6┊        "src/distance.cc"
+┊  ┊ 7┊      ],
+┊  ┊ 8┊      "include_dirs": ["<!(node -e \"require('nan')\")"]
+┊  ┊ 9┊    }
+┊  ┊10┊  ]
+┊  ┊11┊}🚫↵
```

[}]: #

GYP stands for 'Generate Your Project' and was created by the Chromium team as a configuration file for building native projects. The configuration show above should be a good template for any future add-on you're looking to develop. Let's take a deeper look at it:

- `target_name` - Specifies the output dir of our add-on, in which case it should be `build/Release/distance`.
- `sources` - Should include **all** the cpp files that are associated with you add-on.
- `include_dirs` - Additional dirs that should be included when building the add-on. If you'll run the given script in the terminal you'll get the node-module path for Nan, a library which we're interested in during the build process.

More information about GYP configuration can be found [here](https://gyp.gsrc.io/docs/UserDocumentation.md).

Be sure to also add the specified flag to the `package.json` which basically says 'Hey, I have a GYP file which should be taken into consideration as well':

[{]: <helper> (diffStep 3.3 files="package.json")

#### [Step 3.3: Create &#x27;binding.gyp&#x27; file](https://github.com/DAB0mB/node-distance-addon/commit/1214e64)

##### Changed package.json
```diff
@@ -7,6 +7,7 @@
 ┊ 7┊ 7┊    "type": "git",
 ┊ 8┊ 8┊    "url": "https://DAB0mB@github.com/DAB0mB/node-distance-addon.git"
 ┊ 9┊ 9┊  },
+┊  ┊10┊  "gypfile": true,
 ┊10┊11┊  "scripts": {
 ┊11┊12┊    "test": "node test"
 ┊12┊13┊  }
```

[}]: #

Now we will add the following NPM scripts so whenever we run `npm run build` our project will be built:

[{]: <helper> (diffStep 3.4)

#### [Step 3.4: Add npm build scripts](https://github.com/DAB0mB/node-distance-addon/commit/89982a6)

##### Changed .gitignore
```diff
@@ -1 +1,2 @@
+┊ ┊1┊build
 ┊1┊2┊node_modules🚫↵
```

##### Changed package.json
```diff
@@ -9,6 +9,8 @@
 ┊ 9┊ 9┊  },
 ┊10┊10┊  "gypfile": true,
 ┊11┊11┊  "scripts": {
-┊12┊  ┊    "test": "node test"
+┊  ┊12┊    "pre-publish": "npm run build",
+┊  ┊13┊    "build": "node-gyp rebuild",
+┊  ┊14┊    "test": "npm run build && node test"
 ┊13┊15┊  }
 ┊14┊16┊}
```

[}]: #

The only thing left to do before jumping into implementation would be installing Nan:

    $ npm install nan --save

The basis for build process is set. We will create the entry file for our add-on:

[{]: <helper> (diffStep 3.6)

#### [Step 3.6: Create add-on entry file](https://github.com/DAB0mB/node-distance-addon/commit/739cc68)

##### Added src&#x2F;distance.cc
```diff
@@ -0,0 +1,8 @@
+┊ ┊1┊#include <nan.h>
+┊ ┊2┊#include <v8.h>
+┊ ┊3┊
+┊ ┊4┊NAN_MODULE_INIT(Init) {
+┊ ┊5┊
+┊ ┊6┊}
+┊ ┊7┊
+┊ ┊8┊NODE_MODULE(distance, Init)🚫↵
```

[}]: #

Every add-on should start with these two macro calls. They are both compiled into a piece of code which will register our module with ease. The `NODE_MODULE` macro template accepts the name of the target as the first argument (That one we set as `target_name` in the GYP file, remember?) and the initialization method for our module. The `NAN_MODULE_INIT` generates a function with the given name. It accepts `target` as the first argument which is equivalent to NodeJS's `exports`. Now we will create our first method stub for a synchronous distance calculation:

[{]: <helper> (diffStep 3.7)

#### [Step 3.7: Create &#x27;CalculateSync&#x27; method stub](https://github.com/DAB0mB/node-distance-addon/commit/58bfb4c)

##### Changed src&#x2F;distance.cc
```diff
@@ -1,8 +1,12 @@
 ┊ 1┊ 1┊#include <nan.h>
 ┊ 2┊ 2┊#include <v8.h>
 ┊ 3┊ 3┊
-┊ 4┊  ┊NAN_MODULE_INIT(Init) {
+┊  ┊ 4┊NAN_METHOD(CalculateSync) {
+┊  ┊ 5┊
+┊  ┊ 6┊}
 ┊ 5┊ 7┊
+┊  ┊ 8┊NAN_MODULE_INIT(Init) {
+┊  ┊ 9┊  NAN_EXPORT(target, CalculateSync);
 ┊ 6┊10┊}
 ┊ 7┊11┊
 ┊ 8┊12┊NODE_MODULE(distance, Init)🚫↵
```

[}]: #

We exported the `CalculateSync` by using the `NAN_EXPORT` macro, and we used `NAN_METHOD` to define a new node-valid function. It accepts `info` as the first argument and it is the same as JavaScript's `arguments` vector. We already know which arguments this function should accept, that's why I followed TDD methodology from the first place:

[{]: <helper> (diffStep 3.8)

#### [Step 3.8: Destructure arguments vector](https://github.com/DAB0mB/node-distance-addon/commit/088ad0e)

##### Changed src&#x2F;distance.cc
```diff
@@ -1,8 +1,13 @@
 ┊ 1┊ 1┊#include <nan.h>
 ┊ 2┊ 2┊#include <v8.h>
 ┊ 3┊ 3┊
-┊ 4┊  ┊NAN_METHOD(CalculateSync) {
+┊  ┊ 4┊using Nan::To;
+┊  ┊ 5┊using v8::Local;
+┊  ┊ 6┊using v8::Object;
 ┊ 5┊ 7┊
+┊  ┊ 8┊NAN_METHOD(CalculateSync) {
+┊  ┊ 9┊  Local<Object> js_pointA = To<Object>(info[0]).ToLocalChecked();
+┊  ┊10┊  Local<Object> js_pointB = To<Object>(info[1]).ToLocalChecked();
 ┊ 6┊11┊}
 ┊ 7┊12┊
 ┊ 8┊13┊NAN_MODULE_INIT(Init) {
```

[}]: #

We use the `To()` function to convert the first argument into the desired type, and then we call the method `ToLocalChecked()`. This method is simply going to convert the result into v8's Local, unless the argument is undefined, in which case an error is going to be thrown. I like to prefix JS object with a `js_` so I know with what kind variable I'm dealing with. We should have two points containing `x` and `y` fields. Let's try to extract them out of the arguments vector and convert them into native C++ structures:

[{]: <helper> (diffStep 3.9)

#### [Step 3.9: Convert JS objects to native C++ structures](https://github.com/DAB0mB/node-distance-addon/commit/a4d8040)

##### Changed src&#x2F;distance.cc
```diff
@@ -1,13 +1,28 @@
 ┊ 1┊ 1┊#include <nan.h>
 ┊ 2┊ 2┊#include <v8.h>
 ┊ 3┊ 3┊
+┊  ┊ 4┊using Nan::New;
 ┊ 4┊ 5┊using Nan::To;
 ┊ 5┊ 6┊using v8::Local;
 ┊ 6┊ 7┊using v8::Object;
+┊  ┊ 8┊using v8::String;
+┊  ┊ 9┊
+┊  ┊10┊struct Point {
+┊  ┊11┊  double x;
+┊  ┊12┊  double y;
+┊  ┊13┊};
 ┊ 7┊14┊
 ┊ 8┊15┊NAN_METHOD(CalculateSync) {
 ┊ 9┊16┊  Local<Object> js_pointA = To<Object>(info[0]).ToLocalChecked();
 ┊10┊17┊  Local<Object> js_pointB = To<Object>(info[1]).ToLocalChecked();
+┊  ┊18┊
+┊  ┊19┊  Point* pointA = new Point();
+┊  ┊20┊  pointA->x = To<double>(js_pointA->Get(New<String>("x").ToLocalChecked())).FromJust();
+┊  ┊21┊  pointA->y = To<double>(js_pointA->Get(New<String>("y").ToLocalChecked())).FromJust();
+┊  ┊22┊
+┊  ┊23┊  Point* pointB = new Point();
+┊  ┊24┊  pointB->x = To<double>(js_pointB->Get(New<String>("x").ToLocalChecked())).FromJust();
+┊  ┊25┊  pointB->y = To<double>(js_pointB->Get(New<String>("y").ToLocalChecked())).FromJust();
 ┊11┊26┊}
 ┊12┊27┊
 ┊13┊28┊NAN_MODULE_INIT(Init) {
```

[}]: #

Then again we convert the `To()` function to convert the result into the desired data-type, only this time it's a primitive, so we use `FromJust()` instead of `ToLocalChecked()`. Note that v8 only uses double precision rather than a floating point. We can fetch properties from a given JS object with ease using the `Get()` method. Pay attention to use the `->` rather than a period because remember, a Local is actually a pointer! It is not the actual object.

Now all is left to do is defining the return value. Keep in mind that the value should be returned through v8's current scope, not natively, so using the `return` keyword would be useless. The return value can actually be defined through the provided `info` argument, like this:

[{]: <helper> (diffStep "3.10")

#### [Step 3.10: Add return value to &#x27;CalculateSync&#x27; method](https://github.com/DAB0mB/node-distance-addon/commit/69de539)

##### Changed src&#x2F;distance.cc
```diff
@@ -23,6 +23,8 @@
 ┊23┊23┊  Point* pointB = new Point();
 ┊24┊24┊  pointB->x = To<double>(js_pointB->Get(New<String>("x").ToLocalChecked())).FromJust();
 ┊25┊25┊  pointB->y = To<double>(js_pointB->Get(New<String>("y").ToLocalChecked())).FromJust();
+┊  ┊26┊
+┊  ┊27┊  info.GetReturnValue().Set(CalculateDistance(pointA, pointB));
 ┊26┊28┊}
 ┊27┊29┊
 ┊28┊30┊NAN_MODULE_INIT(Init) {
```

[}]: #

And of-course it requires us to add the core distance calculation method:

[{]: <helper> (diffStep 3.11)

#### [Step 3.11: Add core distance calculation method](https://github.com/DAB0mB/node-distance-addon/commit/a2171c6)

##### Changed src&#x2F;distance.cc
```diff
@@ -1,8 +1,12 @@
+┊  ┊ 1┊#include <cstdlib>
+┊  ┊ 2┊#include <cmath>
 ┊ 1┊ 3┊#include <nan.h>
 ┊ 2┊ 4┊#include <v8.h>
 ┊ 3┊ 5┊
 ┊ 4┊ 6┊using Nan::New;
 ┊ 5┊ 7┊using Nan::To;
+┊  ┊ 8┊using std::pow;
+┊  ┊ 9┊using std::sqrt;
 ┊ 6┊10┊using v8::Local;
 ┊ 7┊11┊using v8::Object;
 ┊ 8┊12┊using v8::String;
```
```diff
@@ -12,6 +16,10 @@
 ┊12┊16┊  double y;
 ┊13┊17┊};
 ┊14┊18┊
+┊  ┊19┊double CalculateDistance(Point* pointA, Point* pointB) {
+┊  ┊20┊  return sqrt(pow(pointA->x - pointB->x, 2) + pow(pointA->y - pointB->y, 2));
+┊  ┊21┊}
+┊  ┊22┊
 ┊15┊23┊NAN_METHOD(CalculateSync) {
 ┊16┊24┊  Local<Object> js_pointA = To<Object>(info[0]).ToLocalChecked();
 ┊17┊25┊  Local<Object> js_pointB = To<Object>(info[1]).ToLocalChecked();
```

[}]: #

This is it for the synchronous calculation. Now we will add an async version of it. We will start by creating a method with everything we learned so far until the point where we have to return the result:

[{]: <helper> (diffStep 3.12)

#### [Step 3.12: Create &#x27;CalculateAsync&#x27; method with basic deconstructuring](https://github.com/DAB0mB/node-distance-addon/commit/d53559e)

##### Changed src&#x2F;distance.cc
```diff
@@ -35,8 +35,22 @@
 ┊35┊35┊  info.GetReturnValue().Set(CalculateDistance(pointA, pointB));
 ┊36┊36┊}
 ┊37┊37┊
+┊  ┊38┊NAN_METHOD(CalculateAsync) {
+┊  ┊39┊  Local<Object> js_pointA = To<Object>(info[0]).ToLocalChecked();
+┊  ┊40┊  Local<Object> js_pointB = To<Object>(info[1]).ToLocalChecked();
+┊  ┊41┊
+┊  ┊42┊  Point* pointA = new Point();
+┊  ┊43┊  pointA->x = To<double>(js_pointA->Get(New<String>("x").ToLocalChecked())).FromJust();
+┊  ┊44┊  pointA->y = To<double>(js_pointA->Get(New<String>("y").ToLocalChecked())).FromJust();
+┊  ┊45┊
+┊  ┊46┊  Point* pointB = new Point();
+┊  ┊47┊  pointB->x = To<double>(js_pointB->Get(New<String>("x").ToLocalChecked())).FromJust();
+┊  ┊48┊  pointB->y = To<double>(js_pointB->Get(New<String>("y").ToLocalChecked())).FromJust();
+┊  ┊49┊}
+┊  ┊50┊
 ┊38┊51┊NAN_MODULE_INIT(Init) {
 ┊39┊52┊  NAN_EXPORT(target, CalculateSync);
+┊  ┊53┊  NAN_EXPORT(target, CalculateAsync);
 ┊40┊54┊}
 ┊41┊55┊
 ┊42┊56┊NODE_MODULE(distance, Init)🚫↵
```

[}]: #

Here's the different part. We don't wanna simply return the value, we want to make the calculations in parallel with the event loop, and once we're finished we will interact with it once again. In our model there are two threads. The first thread is the event loop thread, and the second thread will be a worker thread managed by Nan, the library supports asynchronous I/O in NodeJS. Let's start implementing and I will give some more explanations as we go further:

[{]: <helper> (diffStep 3.13)

#### [Step 3.13: Queue distance worker](https://github.com/DAB0mB/node-distance-addon/commit/31273c5)

##### Changed src&#x2F;distance.cc
```diff
@@ -3,10 +3,14 @@
 ┊ 3┊ 3┊#include <nan.h>
 ┊ 4┊ 4┊#include <v8.h>
 ┊ 5┊ 5┊
+┊  ┊ 6┊using Nan::AsyncQueueWorker;
+┊  ┊ 7┊using Nan::AsyncWorker;
+┊  ┊ 8┊using Nan::Callback;
 ┊ 6┊ 9┊using Nan::New;
 ┊ 7┊10┊using Nan::To;
 ┊ 8┊11┊using std::pow;
 ┊ 9┊12┊using std::sqrt;
+┊  ┊13┊using v8::Function;
 ┊10┊14┊using v8::Local;
 ┊11┊15┊using v8::Object;
 ┊12┊16┊using v8::String;
```
```diff
@@ -38,6 +42,7 @@
 ┊38┊42┊NAN_METHOD(CalculateAsync) {
 ┊39┊43┊  Local<Object> js_pointA = To<Object>(info[0]).ToLocalChecked();
 ┊40┊44┊  Local<Object> js_pointB = To<Object>(info[1]).ToLocalChecked();
+┊  ┊45┊  Callback* callback = new Callback(info[2].As<Function>());
 ┊41┊46┊
 ┊42┊47┊  Point* pointA = new Point();
 ┊43┊48┊  pointA->x = To<double>(js_pointA->Get(New<String>("x").ToLocalChecked())).FromJust();
```
```diff
@@ -46,6 +51,8 @@
 ┊46┊51┊  Point* pointB = new Point();
 ┊47┊52┊  pointB->x = To<double>(js_pointB->Get(New<String>("x").ToLocalChecked())).FromJust();
 ┊48┊53┊  pointB->y = To<double>(js_pointB->Get(New<String>("y").ToLocalChecked())).FromJust();
+┊  ┊54┊
+┊  ┊55┊  AsyncQueueWorker(new DistanceWorker(callback, pointA, pointB));
 ┊49┊56┊}
 ┊50┊57┊
 ┊51┊58┊NAN_MODULE_INIT(Init) {
```

[}]: #

Here we fetch the third argument which is the callback. We wrap it with Nan's Callback, which will make sure it is not garbage collected once the scope is being deleted, we want it to keep living until it's not relevant. At the bottom of the method, instead of returning a value explicitly, we queue our `DistanceWorker` into the workers queue. On that note, let's implement the DistanceWorker:

[{]: <helper> (diffStep 3.14)

#### [Step 3.14: Create &#x27;DistanceWorker&#x27; with a constructor and a deconstructor](https://github.com/DAB0mB/node-distance-addon/commit/326332b)

##### Changed src&#x2F;distance.cc
```diff
@@ -24,6 +24,29 @@
 ┊24┊24┊  return sqrt(pow(pointA->x - pointB->x, 2) + pow(pointA->y - pointB->y, 2));
 ┊25┊25┊}
 ┊26┊26┊
+┊  ┊27┊class DistanceWorker : public AsyncWorker {
+┊  ┊28┊ private:
+┊  ┊29┊  Point* pointA;
+┊  ┊30┊  Point* pointB;
+┊  ┊31┊
+┊  ┊32┊ public:
+┊  ┊33┊  DistanceWorker(Callback* callback, Point* pointA, Point* pointB) :
+┊  ┊34┊    AsyncWorker(callback), pointA(pointA), pointB(pointB) {}
+┊  ┊35┊
+┊  ┊36┊  ~DistanceWorker() {
+┊  ┊37┊    delete pointA;
+┊  ┊38┊    delete pointB;
+┊  ┊39┊  }
+┊  ┊40┊
+┊  ┊41┊  void Execute () {
+┊  ┊42┊
+┊  ┊43┊  }
+┊  ┊44┊
+┊  ┊45┊  void HandleOKCallback () {
+┊  ┊46┊
+┊  ┊47┊  }
+┊  ┊48┊};
+┊  ┊49┊
 ┊27┊50┊NAN_METHOD(CalculateSync) {
 ┊28┊51┊  Local<Object> js_pointA = To<Object>(info[0]).ToLocalChecked();
 ┊29┊52┊  Local<Object> js_pointB = To<Object>(info[1]).ToLocalChecked();
```

[}]: #

`AsyncWorker` is an abstract class that you can subclass to have much of the annoying asynchronous queuing and handling taken care of for you. It can even store arbitrary v8 objects for you and have them persist while the asynchronous work is in progress. The `execute()` method is being executed inside the worker-thread. It is not safe to access V8, or V8 data structures there, so everything we need for input and output should go on 'this'. The `HandleOKCallback()` method is executed when the async work is complete. This function will be run inside the main event loop so it is safe to use v8 again. Let's implement the core distance calculation on the worker thread:

[{]: <helper> (diffStep 3.15)

#### [Step 3.15: Execute distance calculation](https://github.com/DAB0mB/node-distance-addon/commit/9e76ad0)

##### Changed src&#x2F;distance.cc
```diff
@@ -26,6 +26,7 @@
 ┊26┊26┊
 ┊27┊27┊class DistanceWorker : public AsyncWorker {
 ┊28┊28┊ private:
+┊  ┊29┊  double distance;
 ┊29┊30┊  Point* pointA;
 ┊30┊31┊  Point* pointB;
 ┊31┊32┊
```
```diff
@@ -39,7 +40,7 @@
 ┊39┊40┊  }
 ┊40┊41┊
 ┊41┊42┊  void Execute () {
-┊42┊  ┊
+┊  ┊43┊    distance = CalculateDistance(pointA, pointB);
 ┊43┊44┊  }
 ┊44┊45┊
 ┊45┊46┊  void HandleOKCallback () {
```

[}]: #

And handle a successful invocation once the calculation is finished:

[{]: <helper> (diffStep 3.16)

#### [Step 3.16: Handle successful invokation](https://github.com/DAB0mB/node-distance-addon/commit/d80c62e)

##### Changed src&#x2F;distance.cc
```diff
@@ -6,14 +6,18 @@
 ┊ 6┊ 6┊using Nan::AsyncQueueWorker;
 ┊ 7┊ 7┊using Nan::AsyncWorker;
 ┊ 8┊ 8┊using Nan::Callback;
+┊  ┊ 9┊using Nan::HandleScope;
 ┊ 9┊10┊using Nan::New;
+┊  ┊11┊using Nan::Null;
 ┊10┊12┊using Nan::To;
 ┊11┊13┊using std::pow;
 ┊12┊14┊using std::sqrt;
 ┊13┊15┊using v8::Function;
 ┊14┊16┊using v8::Local;
+┊  ┊17┊using v8::Number;
 ┊15┊18┊using v8::Object;
 ┊16┊19┊using v8::String;
+┊  ┊20┊using v8::Value;
 ┊17┊21┊
 ┊18┊22┊struct Point {
 ┊19┊23┊  double x;
```
```diff
@@ -44,7 +48,14 @@
 ┊44┊48┊  }
 ┊45┊49┊
 ┊46┊50┊  void HandleOKCallback () {
+┊  ┊51┊    HandleScope scope;
 ┊47┊52┊
+┊  ┊53┊    Local<Value> argv[] = {
+┊  ┊54┊      Null(),
+┊  ┊55┊      New<Number>(distance)
+┊  ┊56┊    };
+┊  ┊57┊
+┊  ┊58┊    callback->Call(2, argv);
 ┊48┊59┊  }
 ┊49┊60┊};
```

[}]: #

Normally, when defining a NodeJS method (`NAN_METHOD` macro) a scope will be created for us automatically. In this function's context there is no scope exist, so we will have to create it using the `HandleScope` deceleration (The current scope is stored globally so even though we don't use it explicitly, v8 and Nan know what to do). We also created an arguments vector as the return value, following NodeJS's conventions, the first argument would be the error and the second argument would be the result.

This is it! Finally we will transform the add-on into a nicer looking node-module:

[{]: <helper> (diffStep 3.17)

#### [Step 3.17: Transform add-on into a nicer looking node-module](https://github.com/DAB0mB/node-distance-addon/commit/dd39994)

##### Added index.js
```diff
@@ -0,0 +1,6 @@
+┊ ┊1┊const Distance = require('./build/Release/distance');
+┊ ┊2┊
+┊ ┊3┊exports.calculate = {
+┊ ┊4┊  sync: Distance.CalculateSync,
+┊ ┊5┊  async: Distance.CalculateAsync
+┊ ┊6┊};🚫↵
```

[}]: #

And now, let's run our small test to see that it works, using the following command:

    $ npm run test

If everything went well, you should have the following messages printed to the terminal:

    sync calculation passed
    async calculation passed

That's it. You've just learned the very basics of how to use C++ within NodeJS. There's a lot more to learn when it comes to building an add-on, and I'm not just talking about learning v8 and Nan's API. Think about the possibilities, the C++ community have been active for years and there are so much great libraries out there that are not necessarily relevant to NodeJS due to its efficiency, like [Boost](http://www.boost.org/), [OpenCV](http://opencv.org/), [CGAL](http://www.cgal.org/) and many more.

This tutorial didn't focus on algorithmics obviously, but it's a good starting point though. Take it from here, and unleash your creativity!


[//]: # (foot-start)

[{]: <helper> (navStep)

| [< Previous Step](https://github.com/DAB0mB/node-distance-addon/tree/master@1.1.0/.tortilla/manuals/views/step2.md) |
|:----------------------|

[}]: #
