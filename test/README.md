# Tests for jsminify

There are 3 folders for tests:
* js: For original files 
* min: For minified files
* out: For console outputs

There are 4 bash files for tests:
* debug.sh: Outputs debug texts
* test-cmp.sh: Downloads and compares this library with uglifyjs
* test-min.sh: Minfies and compares minified files with /min
* test-out.sh: Minifies and executes files comparing log with /out

(WIP)Files in js/ can have hashbang options used to configure the execution.

