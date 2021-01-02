#!/usr/bin/env node
// const { jsminify } = require("./build/Debug/jsminify");
const { jsminify } = require("node-gyp-build")(__dirname);

if (process.argv) {
  const [n, i, ...args] = process.argv;
  jsminify(...args);
}

module.exports = jsminify;
