# jsminify

"jsminfy" is a JS minifier written in C. It uses tree-sitter-javascript grammar
to create a parser and uses tree-sitter-visitor to visit each node of a
JavaScript AST. For each visited node it is generated javascript code that is
minified.

## Dependencies

[tree-sitter](https://github.com/tree-sitter/tree-sitter)
[tree-sitter-javascript](https://github.com/tree-sitter/tree-sitter-javascript)
[tree-sitter-visitor](https://github.com/marcel0ll/tree-sitter-visitor)
[hashmap](https://github.com/tidwall/hashmap.c)

## To Do

### General

General tasks related to this library

- [ ] Write about and publish this
- [ ] C related:
  * [ ] include tree-sitter-visitor.a ?
  * [ ] check memory allocation/deallocation
  * [ ] are the "includes" done right?
  * [x] DEBUG flag 
- [ ] Test on Win
- [ ] Test on Mac
- [ ] Write tests
- [ ] Check if this can be used as a npm module somehow with bindings (no idea how this work)
- [x] Time this tool and compare against other tools (uglifyjs? minifyjs?)

### Minification

Tasks related to optimzations that can be done to the code without problems.

- [x] Remove " " after anonymous function. ex.: `var a = function (){}`
- [x] Remove " " after break. ex.: `break ;`
- [x] Remove " " after continue. ex.: `continue ;`

- [x] Convert numbers to the least amount of characters(0xff -> 255)
  * [ ] improve scientific notation (there are a few cases which need improv.)

### Compression
- [ ] Transform scoped parameters identifiers in shorter versions. ex.: `function(foo){foo()}` => `function(a){a()}`
- [ ] Remove unnecessary ";" on last statement in a block statement
 
### Beautify
- [x] Beautify flag
- [ ] Ensure beatiful output

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
