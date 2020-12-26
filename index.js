#!/usr/bin/env node
const {jsminify} = require('node-gyp-build')(__dirname);

module.exports = jsminify;

if (process.argv) {
  const [n, i, ...args] = process.argv;
  jsminify(...args);
}


