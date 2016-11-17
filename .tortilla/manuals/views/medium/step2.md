# Step 2: Creating the add-on

In this step we will base our development process on the TDD methodology, so you will have a chance to look at the final target API that we desire. We will start by writing a test file:

[{]: <helper> (diffStep 2.1)

#### [Step 2.1: Add test file](https://github.com/DAB0mB/node-distance-addon/commit/823f652)

##### Added .npmignore
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
<b>+┊ ┊1┊test.js🚫↵</b>
</pre>

##### Added test.js
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
<b>+┊  ┊ 1┊const Distance &#x3D; require(&#x27;.&#x27;);</b>
<b>+┊  ┊ 2┊</b>
<b>+┊  ┊ 3┊let result;</b>
<b>+┊  ┊ 4┊let pointA &#x3D; { x: 0, y: 0 };</b>
<b>+┊  ┊ 5┊let pointB &#x3D; { x: 3, y: 4 };</b>
<b>+┊  ┊ 6┊</b>
<b>+┊  ┊ 7┊result &#x3D; Distance.calculate.sync(pointA, pointB);</b>
<b>+┊  ┊ 8┊</b>
<b>+┊  ┊ 9┊if (result !&#x3D;&#x3D; 5) throw Error(</b>
<b>+┊  ┊10┊  &#x27;#Sync: Result expected to equal 5 but instead got &#x27; + result</b>
<b>+┊  ┊11┊);</b>
<b>+┊  ┊12┊</b>
<b>+┊  ┊13┊console.log(&#x27;sync calculation passed&#x27;);</b>
<b>+┊  ┊14┊</b>
<b>+┊  ┊15┊result &#x3D; Distance.calculate.async(pointA, pointB, (err, result) &#x3D;&gt; {</b>
<b>+┊  ┊16┊  if (err) throw err;</b>
<b>+┊  ┊17┊</b>
<b>+┊  ┊18┊  if (result !&#x3D;&#x3D; 5) throw Error(</b>
<b>+┊  ┊19┊    &#x27;#Async: Result expected to equal 5 but instead got &#x27; + result</b>
<b>+┊  ┊20┊  );</b>
<b>+┊  ┊21┊</b>
<b>+┊  ┊22┊  console.log(&#x27;async calculation passed&#x27;);</b>
<b>+┊  ┊23┊});🚫↵</b>
</pre>

[}]: #

And the following NPM script should execute it:

[{]: <helper> (diffStep 2.2)

#### [Step 2.2: Add npm test script](https://github.com/DAB0mB/node-distance-addon/commit/35b20fd)

##### Changed package.json
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊ 6┊ 6┊  &quot;repository&quot;: {
 ┊ 7┊ 7┊    &quot;type&quot;: &quot;git&quot;,
 ┊ 8┊ 8┊    &quot;url&quot;: &quot;https://github.com/DAB0mB/node-distance-addon.git&quot;
<b>+┊  ┊ 9┊  },</b>
<b>+┊  ┊10┊  &quot;scripts&quot;: {</b>
<b>+┊  ┊11┊    &quot;test&quot;: &quot;node test&quot;</b>
 ┊ 9┊12┊  }
 ┊10┊13┊}
</pre>

[}]: #

Like I said in the introduction, it's a simple module which can calculate the distance between 2 given points. `calculate.sync` can do it synchronously and `calculate.async` can do it asynchronously. Now that you got the idea we will start configuring our add-on.

The first thing you'll need to do is to make sure that you have `node-gyp` installed:

    $ sudo npm install -g node-gyp

`node-gyp` is also dependent on many other packages, so before you go any further please take a look at the official installation instructions in their [README.md file](https://github.com/nodejs/node-gyp).

Assuming that you have installed everything properly, we will now need to create a `binding.gyp` file:

[{]: <helper> (diffStep 2.3 files="binding.gyp")

#### [Step 2.3: Create &#x27;binding.gyp&#x27; file](https://github.com/DAB0mB/node-distance-addon/commit/ff44399)

##### Added binding.gyp
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
<b>+┊  ┊ 1┊{</b>
<b>+┊  ┊ 2┊  &quot;targets&quot;: [</b>
<b>+┊  ┊ 3┊    {</b>
<b>+┊  ┊ 4┊      &quot;target_name&quot;: &quot;distance&quot;,</b>
<b>+┊  ┊ 5┊      &quot;sources&quot;: [</b>
<b>+┊  ┊ 6┊        &quot;src/distance.cc&quot;</b>
<b>+┊  ┊ 7┊      ],</b>
<b>+┊  ┊ 8┊      &quot;include_dirs&quot;: [&quot;&lt;!(node -e \&quot;require(&#x27;nan&#x27;)\&quot;)&quot;]</b>
<b>+┊  ┊ 9┊    }</b>
<b>+┊  ┊10┊  ]</b>
<b>+┊  ┊11┊}🚫↵</b>
</pre>

[}]: #

GYP stands for 'Generate Your Project' and was created by the Chromium team as a configuration file for building native projects. The configuration show above should be a good template for any future add-on you're looking to develop. Let's take a deeper look at it:

- `target_name` - Specifies the output dir of our add-on, in which case it should be `build/Release/distance`.
- `sources` - Should include **all** the cpp files that are associated with you add-on.
- `include_dirs` - Additional dirs that should be included when building the add-on. If you'll run the given script in the terminal you'll get the node-module path for Nan, a library which we're interested in during the build process.

More information about GYP configuration can be found [here](https://gyp.gsrc.io/docs/UserDocumentation.md).

Be sure to also add the specified flag to the `package.json` which basically says 'Hey, I have a GYP file which should be taken into consideration as well':

[{]: <helper> (diffStep 2.3 files="package.json")

#### [Step 2.3: Create &#x27;binding.gyp&#x27; file](https://github.com/DAB0mB/node-distance-addon/commit/ff44399)

##### Changed package.json
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊ 7┊ 7┊    &quot;type&quot;: &quot;git&quot;,
 ┊ 8┊ 8┊    &quot;url&quot;: &quot;https://github.com/DAB0mB/node-distance-addon.git&quot;
 ┊ 9┊ 9┊  },
<b>+┊  ┊10┊  &quot;gypfile&quot;: true,</b>
 ┊10┊11┊  &quot;scripts&quot;: {
 ┊11┊12┊    &quot;test&quot;: &quot;node test&quot;
 ┊12┊13┊  }
</pre>

[}]: #

Now we will add the following NPM scripts so whenever we run `npm run build` our project will be built:

[{]: <helper> (diffStep 2.4)

#### [Step 2.4: Add npm build scripts](https://github.com/DAB0mB/node-distance-addon/commit/a9a8815)

##### Changed .gitignore
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
<b>+┊ ┊1┊build</b>
 ┊1┊2┊node_modules🚫↵
</pre>

##### Changed package.json
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊ 9┊ 9┊  },
 ┊10┊10┊  &quot;gypfile&quot;: true,
 ┊11┊11┊  &quot;scripts&quot;: {
<b>+┊  ┊12┊    &quot;pre-publish&quot;: &quot;npm run build&quot;,</b>
<b>+┊  ┊13┊    &quot;build&quot;: &quot;node-gyp rebuild&quot;,</b>
<b>+┊  ┊14┊    &quot;test&quot;: &quot;npm run build &amp;&amp; node test&quot;</b>
 ┊13┊15┊  }
 ┊14┊16┊}
</pre>

[}]: #

The only thing left to do before jumping into implementation would be installing Nan:

    $ npm install nan --save

The basis for build process is set. We will create the entry file for our add-on:

[{]: <helper> (diffStep 2.6)

#### [Step 2.6: Create add-on entry file](https://github.com/DAB0mB/node-distance-addon/commit/793e892)

##### Added src&#x2F;distance.cc
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
<b>+┊ ┊1┊#include &lt;nan.h&gt;</b>
<b>+┊ ┊2┊#include &lt;v8.h&gt;</b>
<b>+┊ ┊3┊</b>
<b>+┊ ┊4┊NAN_MODULE_INIT(Init) {</b>
<b>+┊ ┊5┊</b>
<b>+┊ ┊6┊}</b>
<b>+┊ ┊7┊</b>
<b>+┊ ┊8┊NODE_MODULE(distance, Init)🚫↵</b>
</pre>

[}]: #

Every add-on should start with these two macro calls. They are both compiled into a piece of code which will register our module with ease. The `NODE_MODULE` macro template accepts the name of the target as the first argument (That one we set as `target_name` in the GYP file, remember?) and the initialization method for our module. The `NAN_MODULE_INIT` generates a function with the given name. It accepts `target` as the first argument which is equivalent to NodeJS's `exports`. Now we will create our first method stub for a synchronous distance calculation:

[{]: <helper> (diffStep 2.7)

#### [Step 2.7: Create &#x27;CalculateSync&#x27; method stub](https://github.com/DAB0mB/node-distance-addon/commit/90e3c72)

##### Changed src&#x2F;distance.cc
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊ 1┊ 1┊#include &lt;nan.h&gt;
 ┊ 2┊ 2┊#include &lt;v8.h&gt;
 ┊ 3┊ 3┊
<b>+┊  ┊ 4┊NAN_METHOD(CalculateSync) {</b>
<b>+┊  ┊ 5┊</b>
<b>+┊  ┊ 6┊}</b>
 ┊ 5┊ 7┊
<b>+┊  ┊ 8┊NAN_MODULE_INIT(Init) {</b>
<b>+┊  ┊ 9┊  NAN_EXPORT(target, CalculateSync);</b>
 ┊ 6┊10┊}
 ┊ 7┊11┊
 ┊ 8┊12┊NODE_MODULE(distance, Init)🚫↵
</pre>

[}]: #

We exported the `CalculateSync` by using the `NAN_EXPORT` macro, and we used `NAN_METHOD` to define a new node-valid function. It accepts `info` as the first argument and it is the same as JavaScript's `arguments` vector. We already know which arguments this function should accept, that's why I followed TDD methodology from the first place:

[{]: <helper> (diffStep 2.8)

#### [Step 2.8: Destructure arguments vector](https://github.com/DAB0mB/node-distance-addon/commit/ee1d560)

##### Changed src&#x2F;distance.cc
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊ 1┊ 1┊#include &lt;nan.h&gt;
 ┊ 2┊ 2┊#include &lt;v8.h&gt;
 ┊ 3┊ 3┊
<b>+┊  ┊ 4┊using Nan::To;</b>
<b>+┊  ┊ 5┊using v8::Local;</b>
<b>+┊  ┊ 6┊using v8::Object;</b>
 ┊ 5┊ 7┊
<b>+┊  ┊ 8┊NAN_METHOD(CalculateSync) {</b>
<b>+┊  ┊ 9┊  Local&lt;Object&gt; js_pointA &#x3D; To&lt;Object&gt;(info[0]).ToLocalChecked();</b>
<b>+┊  ┊10┊  Local&lt;Object&gt; js_pointB &#x3D; To&lt;Object&gt;(info[1]).ToLocalChecked();</b>
 ┊ 6┊11┊}
 ┊ 7┊12┊
 ┊ 8┊13┊NAN_MODULE_INIT(Init) {
</pre>

[}]: #

We use the `To()` function to convert the first argument into the desired type, and then we call the method `ToLocalChecked()`. This method is simply going to convert the result into v8's Local, unless the argument is undefined, in which case an error is going to be thrown. I like to prefix JS object with a `js_` so I know with what kind variable I'm dealing with. We should have two points containing `x` and `y` fields. Let's try to extract them out of the arguments vector and convert them into native C++ structures:

[{]: <helper> (diffStep 2.9)

#### [Step 2.9: Convert JS objects to native C++ structures](https://github.com/DAB0mB/node-distance-addon/commit/81998eb)

##### Changed src&#x2F;distance.cc
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊ 1┊ 1┊#include &lt;nan.h&gt;
 ┊ 2┊ 2┊#include &lt;v8.h&gt;
 ┊ 3┊ 3┊
<b>+┊  ┊ 4┊using Nan::New;</b>
 ┊ 4┊ 5┊using Nan::To;
 ┊ 5┊ 6┊using v8::Local;
 ┊ 6┊ 7┊using v8::Object;
<b>+┊  ┊ 8┊using v8::String;</b>
<b>+┊  ┊ 9┊</b>
<b>+┊  ┊10┊struct Point {</b>
<b>+┊  ┊11┊  double x;</b>
<b>+┊  ┊12┊  double y;</b>
<b>+┊  ┊13┊};</b>
 ┊ 7┊14┊
 ┊ 8┊15┊NAN_METHOD(CalculateSync) {
 ┊ 9┊16┊  Local&lt;Object&gt; js_pointA &#x3D; To&lt;Object&gt;(info[0]).ToLocalChecked();
 ┊10┊17┊  Local&lt;Object&gt; js_pointB &#x3D; To&lt;Object&gt;(info[1]).ToLocalChecked();
<b>+┊  ┊18┊</b>
<b>+┊  ┊19┊  Point* pointA &#x3D; new Point();</b>
<b>+┊  ┊20┊  pointA-&gt;x &#x3D; To&lt;double&gt;(js_pointA-&gt;Get(New&lt;String&gt;(&quot;x&quot;).ToLocalChecked())).FromJust();</b>
<b>+┊  ┊21┊  pointA-&gt;y &#x3D; To&lt;double&gt;(js_pointA-&gt;Get(New&lt;String&gt;(&quot;y&quot;).ToLocalChecked())).FromJust();</b>
<b>+┊  ┊22┊</b>
<b>+┊  ┊23┊  Point* pointB &#x3D; new Point();</b>
<b>+┊  ┊24┊  pointB-&gt;x &#x3D; To&lt;double&gt;(js_pointB-&gt;Get(New&lt;String&gt;(&quot;x&quot;).ToLocalChecked())).FromJust();</b>
<b>+┊  ┊25┊  pointB-&gt;y &#x3D; To&lt;double&gt;(js_pointB-&gt;Get(New&lt;String&gt;(&quot;y&quot;).ToLocalChecked())).FromJust();</b>
 ┊11┊26┊}
 ┊12┊27┊
 ┊13┊28┊NAN_MODULE_INIT(Init) {
</pre>

[}]: #

Then again we convert the `To()` function to convert the result into the desired data-type, only this time it's a primitive, so we use `FromJust()` instead of `ToLocalChecked()`. Note that v8 only uses double precision rather than a floating point. We can fetch properties from a given JS object with ease using the `Get()` method. Pay attention to use the `->` rather than a period because remember, a Local is actually a pointer! It is not the actual object.

Now all is left to do is defining the return value. Keep in mind that the value should be returned through v8's current scope, not natively, so using the `return` keyword would be useless. The return value can actually be defined through the provided `info` argument, like this:

[{]: <helper> (diffStep "2.10")

#### [Step 2.10: Add return value to &#x27;CalculateSync&#x27; method](https://github.com/DAB0mB/node-distance-addon/commit/1d6a7f6)

##### Changed src&#x2F;distance.cc
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊23┊23┊  Point* pointB &#x3D; new Point();
 ┊24┊24┊  pointB-&gt;x &#x3D; To&lt;double&gt;(js_pointB-&gt;Get(New&lt;String&gt;(&quot;x&quot;).ToLocalChecked())).FromJust();
 ┊25┊25┊  pointB-&gt;y &#x3D; To&lt;double&gt;(js_pointB-&gt;Get(New&lt;String&gt;(&quot;y&quot;).ToLocalChecked())).FromJust();
<b>+┊  ┊26┊</b>
<b>+┊  ┊27┊  info.GetReturnValue().Set(CalculateDistance(pointA, pointB));</b>
 ┊26┊28┊}
 ┊27┊29┊
 ┊28┊30┊NAN_MODULE_INIT(Init) {
</pre>

[}]: #

And of-course it requires us to add the core distance calculation method:

[{]: <helper> (diffStep 2.11)

#### [Step 2.11: Add core distance calculation method](https://github.com/DAB0mB/node-distance-addon/commit/5af3b3d)

##### Changed src&#x2F;distance.cc
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
<b>+┊  ┊ 1┊#include &lt;cstdlib&gt;</b>
<b>+┊  ┊ 2┊#include &lt;cmath&gt;</b>
 ┊ 1┊ 3┊#include &lt;nan.h&gt;
 ┊ 2┊ 4┊#include &lt;v8.h&gt;
 ┊ 3┊ 5┊
 ┊ 4┊ 6┊using Nan::New;
 ┊ 5┊ 7┊using Nan::To;
<b>+┊  ┊ 8┊using std::pow;</b>
<b>+┊  ┊ 9┊using std::sqrt;</b>
 ┊ 6┊10┊using v8::Local;
 ┊ 7┊11┊using v8::Object;
 ┊ 8┊12┊using v8::String;
</pre>
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊12┊16┊  double y;
 ┊13┊17┊};
 ┊14┊18┊
<b>+┊  ┊19┊double CalculateDistance(Point* pointA, Point* pointB) {</b>
<b>+┊  ┊20┊  return sqrt(pow(pointA-&gt;x - pointB-&gt;x, 2) + pow(pointA-&gt;y - pointB-&gt;y, 2));</b>
<b>+┊  ┊21┊}</b>
<b>+┊  ┊22┊</b>
 ┊15┊23┊NAN_METHOD(CalculateSync) {
 ┊16┊24┊  Local&lt;Object&gt; js_pointA &#x3D; To&lt;Object&gt;(info[0]).ToLocalChecked();
 ┊17┊25┊  Local&lt;Object&gt; js_pointB &#x3D; To&lt;Object&gt;(info[1]).ToLocalChecked();
</pre>

[}]: #

This is it for the synchronous calculation. Now we will add an async version of it. We will start by creating a method with everything we learned so far until the point where we have to return the result:

[{]: <helper> (diffStep 2.12)

#### [Step 2.12: Create &#x27;CalculateAsync&#x27; method with basic deconstructuring](https://github.com/DAB0mB/node-distance-addon/commit/2711bf5)

##### Changed src&#x2F;distance.cc
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊35┊35┊  info.GetReturnValue().Set(CalculateDistance(pointA, pointB));
 ┊36┊36┊}
 ┊37┊37┊
<b>+┊  ┊38┊NAN_METHOD(CalculateAsync) {</b>
<b>+┊  ┊39┊  Local&lt;Object&gt; js_pointA &#x3D; To&lt;Object&gt;(info[0]).ToLocalChecked();</b>
<b>+┊  ┊40┊  Local&lt;Object&gt; js_pointB &#x3D; To&lt;Object&gt;(info[1]).ToLocalChecked();</b>
<b>+┊  ┊41┊</b>
<b>+┊  ┊42┊  Point* pointA &#x3D; new Point();</b>
<b>+┊  ┊43┊  pointA-&gt;x &#x3D; To&lt;double&gt;(js_pointA-&gt;Get(New&lt;String&gt;(&quot;x&quot;).ToLocalChecked())).FromJust();</b>
<b>+┊  ┊44┊  pointA-&gt;y &#x3D; To&lt;double&gt;(js_pointA-&gt;Get(New&lt;String&gt;(&quot;y&quot;).ToLocalChecked())).FromJust();</b>
<b>+┊  ┊45┊</b>
<b>+┊  ┊46┊  Point* pointB &#x3D; new Point();</b>
<b>+┊  ┊47┊  pointB-&gt;x &#x3D; To&lt;double&gt;(js_pointB-&gt;Get(New&lt;String&gt;(&quot;x&quot;).ToLocalChecked())).FromJust();</b>
<b>+┊  ┊48┊  pointB-&gt;y &#x3D; To&lt;double&gt;(js_pointB-&gt;Get(New&lt;String&gt;(&quot;y&quot;).ToLocalChecked())).FromJust();</b>
<b>+┊  ┊49┊}</b>
<b>+┊  ┊50┊</b>
 ┊38┊51┊NAN_MODULE_INIT(Init) {
 ┊39┊52┊  NAN_EXPORT(target, CalculateSync);
<b>+┊  ┊53┊  NAN_EXPORT(target, CalculateAsync);</b>
 ┊40┊54┊}
 ┊41┊55┊
 ┊42┊56┊NODE_MODULE(distance, Init)🚫↵
</pre>

[}]: #

Here's the different part. We don't wanna simply return the value, we want to make the calculations in parallel with the event loop, and once we're finished we will interact with it once again. In our model there are two threads. The first thread is the event loop thread, and the second thread will be a worker thread managed by Nan, the library supports asynchronous I/O in NodeJS. Let's start implementing and I will give some more explanations as we go further:

[{]: <helper> (diffStep 2.13)

#### [Step 2.13: Queue distance worker](https://github.com/DAB0mB/node-distance-addon/commit/34f4a5d)

##### Changed src&#x2F;distance.cc
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊ 3┊ 3┊#include &lt;nan.h&gt;
 ┊ 4┊ 4┊#include &lt;v8.h&gt;
 ┊ 5┊ 5┊
<b>+┊  ┊ 6┊using Nan::AsyncQueueWorker;</b>
<b>+┊  ┊ 7┊using Nan::AsyncWorker;</b>
<b>+┊  ┊ 8┊using Nan::Callback;</b>
 ┊ 6┊ 9┊using Nan::New;
 ┊ 7┊10┊using Nan::To;
 ┊ 8┊11┊using std::pow;
 ┊ 9┊12┊using std::sqrt;
<b>+┊  ┊13┊using v8::Function;</b>
 ┊10┊14┊using v8::Local;
 ┊11┊15┊using v8::Object;
 ┊12┊16┊using v8::String;
</pre>
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊38┊42┊NAN_METHOD(CalculateAsync) {
 ┊39┊43┊  Local&lt;Object&gt; js_pointA &#x3D; To&lt;Object&gt;(info[0]).ToLocalChecked();
 ┊40┊44┊  Local&lt;Object&gt; js_pointB &#x3D; To&lt;Object&gt;(info[1]).ToLocalChecked();
<b>+┊  ┊45┊  Callback* callback &#x3D; new Callback(info[2].As&lt;Function&gt;());</b>
 ┊41┊46┊
 ┊42┊47┊  Point* pointA &#x3D; new Point();
 ┊43┊48┊  pointA-&gt;x &#x3D; To&lt;double&gt;(js_pointA-&gt;Get(New&lt;String&gt;(&quot;x&quot;).ToLocalChecked())).FromJust();
</pre>
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊46┊51┊  Point* pointB &#x3D; new Point();
 ┊47┊52┊  pointB-&gt;x &#x3D; To&lt;double&gt;(js_pointB-&gt;Get(New&lt;String&gt;(&quot;x&quot;).ToLocalChecked())).FromJust();
 ┊48┊53┊  pointB-&gt;y &#x3D; To&lt;double&gt;(js_pointB-&gt;Get(New&lt;String&gt;(&quot;y&quot;).ToLocalChecked())).FromJust();
<b>+┊  ┊54┊</b>
<b>+┊  ┊55┊  AsyncQueueWorker(new DistanceWorker(callback, pointA, pointB));</b>
 ┊49┊56┊}
 ┊50┊57┊
 ┊51┊58┊NAN_MODULE_INIT(Init) {
</pre>

[}]: #

Here we fetch the third argument which is the callback. We wrap it with Nan's Callback, which will make sure it is not garbage collected once the scope is being deleted, we want it to keep living until it's not relevant. At the bottom of the method, instead of returning a value explicitly, we queue our `DistanceWorker` into the workers queue. On that note, let's implement the DistanceWorker:

[{]: <helper> (diffStep 2.14)

#### [Step 2.14: Create &#x27;DistanceWorker&#x27; with a constructor and a deconstructor](https://github.com/DAB0mB/node-distance-addon/commit/8bbd87f)

##### Changed src&#x2F;distance.cc
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊24┊24┊  return sqrt(pow(pointA-&gt;x - pointB-&gt;x, 2) + pow(pointA-&gt;y - pointB-&gt;y, 2));
 ┊25┊25┊}
 ┊26┊26┊
<b>+┊  ┊27┊class DistanceWorker : public AsyncWorker {</b>
<b>+┊  ┊28┊ private:</b>
<b>+┊  ┊29┊  Point* pointA;</b>
<b>+┊  ┊30┊  Point* pointB;</b>
<b>+┊  ┊31┊</b>
<b>+┊  ┊32┊ public:</b>
<b>+┊  ┊33┊  DistanceWorker(Callback* callback, Point* pointA, Point* pointB) :</b>
<b>+┊  ┊34┊    AsyncWorker(callback), pointA(pointA), pointB(pointB) {}</b>
<b>+┊  ┊35┊</b>
<b>+┊  ┊36┊  ~DistanceWorker() {</b>
<b>+┊  ┊37┊    delete pointA;</b>
<b>+┊  ┊38┊    delete pointB;</b>
<b>+┊  ┊39┊  }</b>
<b>+┊  ┊40┊</b>
<b>+┊  ┊41┊  void Execute () {</b>
<b>+┊  ┊42┊</b>
<b>+┊  ┊43┊  }</b>
<b>+┊  ┊44┊</b>
<b>+┊  ┊45┊  void HandleOKCallback () {</b>
<b>+┊  ┊46┊</b>
<b>+┊  ┊47┊  }</b>
<b>+┊  ┊48┊};</b>
<b>+┊  ┊49┊</b>
 ┊27┊50┊NAN_METHOD(CalculateSync) {
 ┊28┊51┊  Local&lt;Object&gt; js_pointA &#x3D; To&lt;Object&gt;(info[0]).ToLocalChecked();
 ┊29┊52┊  Local&lt;Object&gt; js_pointB &#x3D; To&lt;Object&gt;(info[1]).ToLocalChecked();
</pre>

[}]: #

`AsyncWorker` is an abstract class that you can subclass to have much of the annoying asynchronous queuing and handling taken care of for you. It can even store arbitrary v8 objects for you and have them persist while the asynchronous work is in progress. The `execute()` method is being executed inside the worker-thread. It is not safe to access V8, or V8 data structures there, so everything we need for input and output should go on 'this'. The `HandleOKCallback()` method is executed when the async work is complete. This function will be run inside the main event loop so it is safe to use v8 again. Let's implement the core distance calculation on the worker thread:

[{]: <helper> (diffStep 2.15)

#### [Step 2.15: Execute distance calculation](https://github.com/DAB0mB/node-distance-addon/commit/9cd18e8)

##### Changed src&#x2F;distance.cc
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊26┊26┊
 ┊27┊27┊class DistanceWorker : public AsyncWorker {
 ┊28┊28┊ private:
<b>+┊  ┊29┊  double distance;</b>
 ┊29┊30┊  Point* pointA;
 ┊30┊31┊  Point* pointB;
 ┊31┊32┊
</pre>
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊39┊40┊  }
 ┊40┊41┊
 ┊41┊42┊  void Execute () {
<b>+┊  ┊43┊    distance &#x3D; CalculateDistance(pointA, pointB);</b>
 ┊43┊44┊  }
 ┊44┊45┊
 ┊45┊46┊  void HandleOKCallback () {
</pre>

[}]: #

And handle a successful invocation once the calculation is finished:

[{]: <helper> (diffStep 2.16)

#### [Step 2.16: Handle successful invokation](https://github.com/DAB0mB/node-distance-addon/commit/8dd60ff)

##### Changed src&#x2F;distance.cc
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊ 6┊ 6┊using Nan::AsyncQueueWorker;
 ┊ 7┊ 7┊using Nan::AsyncWorker;
 ┊ 8┊ 8┊using Nan::Callback;
<b>+┊  ┊ 9┊using Nan::HandleScope;</b>
 ┊ 9┊10┊using Nan::New;
<b>+┊  ┊11┊using Nan::Null;</b>
 ┊10┊12┊using Nan::To;
 ┊11┊13┊using std::pow;
 ┊12┊14┊using std::sqrt;
 ┊13┊15┊using v8::Function;
 ┊14┊16┊using v8::Local;
<b>+┊  ┊17┊using v8::Number;</b>
 ┊15┊18┊using v8::Object;
 ┊16┊19┊using v8::String;
<b>+┊  ┊20┊using v8::Value;</b>
 ┊17┊21┊
 ┊18┊22┊struct Point {
 ┊19┊23┊  double x;
</pre>
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
 ┊44┊48┊  }
 ┊45┊49┊
 ┊46┊50┊  void HandleOKCallback () {
<b>+┊  ┊51┊    HandleScope scope;</b>
 ┊47┊52┊
<b>+┊  ┊53┊    Local&lt;Value&gt; argv[] &#x3D; {</b>
<b>+┊  ┊54┊      Null(),</b>
<b>+┊  ┊55┊      New&lt;Number&gt;(distance)</b>
<b>+┊  ┊56┊    };</b>
<b>+┊  ┊57┊</b>
<b>+┊  ┊58┊    callback-&gt;Call(2, argv);</b>
 ┊48┊59┊  }
 ┊49┊60┊};
</pre>

[}]: #

Normally, when defining a NodeJS method (`NAN_METHOD` macro) a scope will be created for us automatically. In this function's context there is no scope exist, so we will have to create it using the `HandleScope` deceleration (The current scope is stored globally so even though we don't use it explicitly, v8 and Nan know what to do). We also created an arguments vector as the return value, following NodeJS's conventions, the first argument would be the error and the second argument would be the result.

This is it! Finally we will transform the add-on into a nicer looking node-module:

[{]: <helper> (diffStep 2.17)

#### [Step 2.17: Transform add-on into a nicer looking node-module](https://github.com/DAB0mB/node-distance-addon/commit/a250013)

##### Added index.js
<pre>
<i>╔══════╗</i>
<i>║ diff ║</i>
<i>╚══════╝</i>
<b>+┊ ┊1┊const Distance &#x3D; require(&#x27;./build/Release/distance&#x27;);</b>
<b>+┊ ┊2┊</b>
<b>+┊ ┊3┊exports.calculate &#x3D; {</b>
<b>+┊ ┊4┊  sync: Distance.CalculateSync,</b>
<b>+┊ ┊5┊  async: Distance.CalculateAsync</b>
<b>+┊ ┊6┊};🚫↵</b>
</pre>

[}]: #

And now, let's run our small test to see that it works, using the following command:

    $ npm run test

If everything went well, you should have the following messages printed to the terminal:

    sync calculation passed
    async calculation passed

[{]: <helper> (navStep)

⟸ <a href="https://github.com/DAB0mB/node-distance-addon/tree/master/.tortilla/manuals/views/medium/step1.md">PREVIOUS STEP</a> <b>║</b> <a href="https://github.com/DAB0mB/node-distance-addon/tree/master/.tortilla/manuals/views/medium/step3.md">NEXT STEP</a> ⟹

[}]: #
