const Distance = require('./build/Release/distance');

exports.calculate = {
  sync: Distance.CalculateSync,
  async: Distance.CalculateAsync
};