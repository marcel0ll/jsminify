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


## Numbers
//#! --beautify --keep-comments
/*
references:
https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number
https://en.wikipedia.org/wiki/Floating-point_arithmetic
https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Lexical_grammar#Numeric_literals

---

The JavaScript Number type is a double-precision 64-bit binary format IEEE 754:
 
Chrome Number. 
MAX_SAFE_INTEGER (2^53)-1 = 9007199254740991;
MIN_SAFE_INTEGER -((2^53)-1) = -9007199254740991;
MAX_VALUE = 1.7976931348623157e+308;
MIN_VALUE = 5e-324;

{dec} integer part of a float number
{fra} fractional part of a float number
{oct} integer number written as octal
{hex} integer number written as hexadecimal
{bin} integer number writter as binary
{exp} integer part of scientific notation number

Javascript numbers can be written as:

{dec} = {exp} = [+-]?[0-9]+
{fra} = \.[0-9]+
{oct} = [0-7]+
{hex} = [0-9a-fA-F]+

N: Number
D: Decimals
O: Octals
B: Binary
X: Hexadecimals
S: Scientific notation

D: {dec}
D: {dec}{fra}
D: {fra}
O: 0{oct}
D: 0{dec} // if rule above does not yet apply, so 07 is {oct}, but 08 is {dec}
O: 0o{oct}
O: 0O{oct}
X: 0x{hex}
X: 0X{hex}
B: 0b{bin}
B: 0B{bin}
S: {dec}e{exp}
S: {dec}e{exp}
S: {dec}{fra}e{exp}
S: {dec}e{exp}
S: {fra}e{exp}

Only Decimals can have {fra} or {dot}

Javascript numbers can have "_" as separators
Javascript numbers can also be of type BigInt with an "n" at the end
Both of above notations do not work with old octal format -> 0{oct}

Max {dec}, {oct}, {bin} number that is worth converting to {dec} is: 1099511627775;
Min {dec}, {oct}, {bin} number that is worth converting to {hex} is: 1099511627776;
This is due to the "0x" notation preceding hex numbers

OBJECTIVE: REWRITE NUMBERS IN THE LEAST AMOUNT OF CHARACTERS
