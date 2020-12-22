# Tests for jsminify

There are 3 folders for tests:
* js: For original files 
* min: For minified files
* out: For console outputs

The test suite for jsminify simply minifies files in js/ then compares same
filename to /min. If testsuite is set to execute, then it executes files in
/min and compares output with /out files.

Files in js/ can have hashbang options used to configure the execution.
