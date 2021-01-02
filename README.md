# jsminify

"jsminfy" is a JS minifier written in C. It uses tree-sitter-javascript grammar
to create a parser and uses tree-sitter-visitor to visit each node of a
JavaScript AST. For each visited node it generates javascript code that is
minified.

## Installing

`npm i @lotuz/jsminify`

TODO: also allow to install this library bin from other repos

## Usage

`jsminify [OPTIONS] [FILE]`

### OPTIONS

* -h, --help: For printing jsminify help
* -v, --version: For printing jsminify version
* -d, --debug: For debugging minification, also helpful for bug report
* (WIP) -b, --beautify: For pretty printing file

## Building

Clone dependencies in the following folder structure:

```
/workspace
|
|----- tree-sitter
|----- tree-sitter-javascript
|----- tree-sitter-visitor
|----- hashmap
|----- jsminify
```

`node-gyp configure`
`node-gyp build`

## Dependencies (all installed as git submodules under libs/)

[tree-sitter](https://github.com/tree-sitter/tree-sitter)
[tree-sitter-javascript](https://github.com/tree-sitter/tree-sitter-javascript)
[tree-sitter-visitor](https://github.com/marcel0ll/tree-sitter-visitor)
[hashmap](https://github.com/tidwall/hashmap.c)

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

## Improvements

Due to how tree-sitter works it might be possible to only re-minify edited
parts of a file while developing.

## Roadmap

- [ ] 1.0.0
  * [ ] MINIFICATION
    - [ ] Write article about jsminify 1.0.0
    - [ ] Write enough tests for common javascript
    - [ ] Test in Windows
    - [ ] Test in Mac
    - [ ] Ensure memory allocation/deallocation
    - [ ] Ensure that "includes" are done right?
    - [ ] Improve scientific number printing
    - [ ] Improve decimal digits precision printing: ECMA-256 only requires up
          to 21 digits. Chrome apparently crops at 16. So:
          0.89081309152928522810 becomes 0.8908130915292852
  * [ ] COMPRESSION
    - [ ] Transform scoped parameters identifiers in shorter versions. ex.:
          `function(foo){foo()}` => `function(a){a()}`
    - [ ] Remove unnecessary ";" on last statement in a block statement
  * [ ] BEAUTIFY (should this be part of jsminify or another minimal library?)
    - [ ] Ensure beatiful output

