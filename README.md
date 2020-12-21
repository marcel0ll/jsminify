# jsminify

"jsminfy" is a JS minifier written in C. It uses an extended 
tree-sitter-javascript grammar to create a parser and uses tree-sitter-visitor
to visit each node of a JavaScript AST. For each visited node it is generated
javascript code that is minified.

## To Do

### General

General tasks related to this library

- [ ] include tree-sitter-visitor.a
- [ ] Test on Win
- [ ] Test on Mac
- [ ] Write tests
- [ ] Time this tool and compare against other tools (uglifyjs? minifyjs?)
- [ ] Check if this can be used as a npm module somehow with bindings (no idea how this work)
- [ ] Improve code
  * [ ] DEBUG flag
  * [ ] check memory allocation/disallocation
  * [ ] are the "includes" done right?

### Minification

Tasks related to optimzations that can be done to the code without problems.

- [ ] Remove " " after anonymous function. ex.: `var a = function (){}`
- [ ] Remove " " after break. ex.: `break ;`
- [ ] Remove " " after continue. ex.: `continue ;`

- [ ] Remove unnecessary ";" on last block statement
- [ ] Convert numbers to the least amount of characters(0xff -> 255)

### Compression
- [ ] Transform scoped parameters identifiers in shorter versions. ex.: `function(foo){foo()}` => `function(a){a()}`

## How I tested this (for now)

I used jsminify to minify "particle.js" and "phaser.js" then I used prettier
to beautify the code and diffed the files against the same files minified by 
Uglifyjs and beautified by prettier again.

The diff shows what other minifications are done by uglifyjs that could be 
implemented by jsminify.
