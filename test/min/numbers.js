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

OBJECTIVE: REWRITE NUMBERS IN THE LEAST AMOUNT OF CHARACTERS

*/
// {dec} that should remain as is
0;
42;
457;
999;
268435455;

// {dec} that should be shortened
8;
9;
80;
90;

// {dec}{dot}{fra}, where {fra} == 0, should become {dec}
0;
1;
2;
3;
268435455;

// {bin} that should become {dec}
0;
1;
3;
30755734;
246045872;
268435455;

// {oct} that should become {dec}
1;
2;
3;
4;
5;
6;
7;
623307;
73;
268435455;
268435455;
268435455;

// {hex} that should become {dec}
1;
255;
180101906;
268435455;
180101906;
268435455;

// {dec}{dot}{fra}, where {dec} == 0 should be written as .{fra}
.1;
.3;
.5;
.2456;

// Any N, where it ends with at least 3 "0"s in Decimal should become S
// is there a case where even with S notation is still better to write as {hex}?
1e3;
1e3;
1e3;
1e3;

// Any D where it starts with "0." and has at least 3 "0"s should becom S
1e-4;
231e-6;

// Any S that actually is D should lose its e{exp}
1;
1;
123;
1232;

// Any S that has {fra} should lose its {fra}
1230;
5732;
123456e17;

// Any S that has + should lose its +
123e7;

// Any {exp} should lose starting "0"s
123e7;
123e-7;
