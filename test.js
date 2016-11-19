const Distance = require('.');

let result;
let pointA = { x: 0, y: 0 };
let pointB = { x: 3, y: 4 };

result = Distance.calculate.sync(pointA, pointB);

if (result !== 5) throw Error(
  '#Sync: Result expected to equal 5 but instead got ' + result
);

console.log('sync calculation passed');

result = Distance.calculate.async(pointA, pointB, (err, result) => {
  if (err) throw err;

  if (result !== 5) throw Error(
    '#Async: Result expected to equal 5 but instead got ' + result
  );

  console.log('async calculation passed');
});