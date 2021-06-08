# jsminify

"jsminfy" is a JS minifier written in C. It uses tree-sitter-javascript grammar
to create a parser and uses tree-sitter-visitor to visit each node of a
JavaScript AST. For each visited node it generates javascript code that is
minified.

## Installing

`npm i @lotuz/jsminify`

## Usage

`jsminify [OPTIONS] [FILE]`

**FILE must always come last**

### OPTIONS

* -h, --help: For printing jsminify help
* -v, --version: For printing jsminify version
* -d, --debug: For debugging minification, also helpful for bug report
* -o, --output: Sets the output file
* (WIP) -c, --keep-comments: Keeps comments while minifying
* (WIP) -b, --beautify: For pretty printing file

## Building

Initialize git submodules with: `git submodules init --recursive --update`

Install node-gyp dependencies according to platform and run:

`node-gyp configure`
`node-gyp build`

## Debugging 

Debugging relies on gdb

Change `require` inside `index.js` to the one with DEBUG in the path

- `node-gyp rebuild --debug`
- `gdb node`
- `run index.js [OPTIONS] [FILE]`

## Dependencies (all installed as git submodules under libs/)

> please let me know better ways to handle this

- [tree-sitter](https://github.com/tree-sitter/tree-sitter)
- [tree-sitter-javascript](https://github.com/tree-sitter/tree-sitter-javascript)
- [tree-sitter-visitor](https://github.com/marcel0ll/tree-sitter-visitor)

## Publishing

* Bump version on jsminify.c and package.json
* Create git tag
* Push tag
* Wait for CI builds
* run `prebuildify-ci download`
* run `npm publish`

## How I tested this (for now)

I used jsminify to minify:

- angular.js
- bootstrap.js
- d3.js
- ember.prod.js
- htmlminifier.js
- jquery-3.4.1.js
- lodash.js
- math.js
- phaser.js
- react.js
- particles.js

then I used prettier to beautify the code and diffed the files against the same
process done by Uglifyjs.

The diffs shows what other minifications are done by uglifyjs that could be 
implemented by jsminify, missing minificaiton opportunities and bugs.

Better tests are currently being written in [test/](./test/README.md)

## Profiling

I am using `gcc.sh` to build jsminify and then using `perf` and `flamegraph`
to create somekind of visualization of what is going on. 

I would love to hear about better ways to profile this.

## Improvements

Due to how tree-sitter works it might be possible to only re-minify edited
parts of a file while developing. Not sure about this, and not sure if this
would be valuable to anyone as it is already really fast to minify using
jsminify.

## Roadmap

- [ ] 1.0.0
  * [ ] MINIFICATION
    - [ ] Write article about jsminify 1.0.0
    - [x] Write enough tests for common javascript
    - [x] Improve scientific number printing
    - [x] Improve operator spacing
    - [x] Improve decimal digits precision printing: ECMA-256 only requires up
          to 21 digits. Chrome apparently crops at 16. So:
          0.89081309152928522810 becomes 0.8908130915292852
    - [x] Remove comma dangle
    - [x] Check memory allocation/deallocation
    - [x] Check "includes"
    - [x] Test in Windows
    - [x] Test in Mac

- [ ] 1.1.0
  * [ ] COMPRESSION
    - [ ] Transform scoped parameters identifiers in shorter versions. ex.:
          `function(foo){foo()}` => `function(a){a()}`
    - [ ] Remove unnecessary ";" on last statement in a block statement
  * [ ] BEAUTIFY (should this be part of jsminify or another minimal library?)
    - [ ] Ensure beatiful output

