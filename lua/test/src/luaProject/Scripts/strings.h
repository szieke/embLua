static const char* strings="print('testing strings and string library')\n\
assert('alo' < 'alo1')\n\
assert('' < 'a')\n\
assert('alo\\0alo' < 'alo\\0b')\n\
assert('alo\\0alo\\0\\0' > 'alo\\0alo\\0')\n\
assert('alo' < 'alo\\0')\n\
assert('alo\\0' > 'alo')\n\
assert('\\0' < '\\1')\n\
assert('\\0\\0' < '\\0\\1')\n\
assert('\\1\\0a\\0a' <= '\\1\\0a\\0a')\n\
assert(not ('\\1\\0a\\0b' <= '\\1\\0a\\0a'))\n\
assert('\\0\\0\\0' < '\\0\\0\\0\\0')\n\
assert(not('\\0\\0\\0\\0' < '\\0\\0\\0'))\n\
assert('\\0\\0\\0' <= '\\0\\0\\0\\0')\n\
assert(not('\\0\\0\\0\\0' <= '\\0\\0\\0'))\n\
assert('\\0\\0\\0' <= '\\0\\0\\0')\n\
assert('\\0\\0\\0' >= '\\0\\0\\0')\n\
assert(not ('\\0\\0b' < '\\0\\0a\\0'))\n\
print('+')\n\
assert(string.sub(\"123456789\",2,4) == \"234\")\n\
assert(string.sub(\"123456789\",7) == \"789\")\n\
assert(string.sub(\"123456789\",7,6) == \"\")\n\
assert(string.sub(\"123456789\",7,7) == \"7\")\n\
assert(string.sub(\"123456789\",0,0) == \"\")\n\
assert(string.sub(\"123456789\",-10,10) == \"123456789\")\n\
assert(string.sub(\"123456789\",1,9) == \"123456789\")\n\
assert(string.sub(\"123456789\",-10,-20) == \"\")\n\
assert(string.sub(\"123456789\",-1) == \"9\")\n\
assert(string.sub(\"123456789\",-4) == \"6789\")\n\
assert(string.sub(\"123456789\",-6, -4) == \"456\")\n\
assert(string.sub(\"\\000123456789\",3,5) == \"234\")\n\
assert((\"\\000123456789\"):sub(8) == \"789\")\n\
print('+')\n\
assert(string.find(\"123456789\", \"345\") == 3)\n\
a,b = string.find(\"123456789\", \"345\")\n\
assert(string.sub(\"123456789\", a, b) == \"345\")\n\
assert(string.find(\"1234567890123456789\", \"345\", 3) == 3)\n\
assert(string.find(\"1234567890123456789\", \"345\", 4) == 13)\n\
assert(string.find(\"1234567890123456789\", \"346\", 4) == nil)\n\
assert(string.find(\"1234567890123456789\", \".45\", -9) == 13)\n\
assert(string.find(\"abcdefg\", \"\\0\", 5, 1) == nil)\n\
assert(string.find(\"\", \"\") == 1)\n\
assert(string.find('', 'aaa', 1) == nil)\n\
assert(('alo(.)alo'):find('(.)', 1, 1) == 4)\n\
print('+')\n\
assert(string.len(\"\") == 0)\n\
assert(string.len(\"\\0\\0\\0\") == 3)\n\
assert(string.len(\"1234567890\") == 10)\n\
assert(#\"\" == 0)\n\
assert(#\"\\0\\0\\0\" == 3)\n\
assert(#\"1234567890\" == 10)\n\
assert(string.byte(\"a\") == 97)\n\
assert(string.byte(string.char(255)) == 255)\n\
assert(string.byte(string.char(0)) == 0)\n\
assert(string.byte(\"\\0\") == 0)\n\
assert(string.byte(\"\\0\\0alo\\0x\", -1) == string.byte('x'))\n\
assert(string.byte(\"ba\", 2) == 97)\n\
assert(string.byte(\"\\n\\n\", 2, -1) == 10)\n\
assert(string.byte(\"\\n\\n\", 2, 2) == 10)\n\
assert(string.byte(\"\") == nil)\n\
assert(string.byte(\"hi\", -3) == nil)\n\
assert(string.byte(\"hi\", 3) == nil)\n\
assert(string.byte(\"hi\", 9, 10) == nil)\n\
assert(string.byte(\"hi\", 2, 1) == nil)\n\
assert(string.char() == \"\")\n\
assert(string.char(0, 255, 0) == \"\\0\\255\\0\")\n\
assert(string.char(0, string.byte(\"a\"), 0) == \"\\0a\\0\")\n\
assert(string.char(string.byte(\"al\\0ou\", 1, -1)) == \"al\\0ou\")\n\
assert(string.char(string.byte(\"al\\0ou\", 1, 0)) == \"\")\n\
assert(string.char(string.byte(\"al\\0ou\", -10, 100)) == \"al\\0ou\")\n\
print('+')\n\
assert(string.upper(\"ab\\0c\") == \"AB\\0C\")\n\
assert(string.lower(\"\\0ABCc%$\") == \"\\0abcc%$\")\n\
assert(string.rep('teste', 0) == '')\n\
assert(string.rep('tes\\00te', 2) == 'tes\\0tetes\\000te')\n\
assert(string.rep('', 10) == '')\n\
assert(string.reverse\"\" == \"\")\n\
assert(string.reverse\"\\0\\1\\2\\3\" == \"\\3\\2\\1\\0\")\n\
assert(string.reverse\"\\0001234\" == \"4321\\0\")\n\
for i=0,30 do assert(string.len(string.rep('a', i)) == i) end\n\
assert(type(tostring(nil)) == 'string')\n\
assert(type(tostring(12)) == 'string')\n\
assert(''..12 == '12' and type(12 .. '') == 'string')\n\
assert(string.find(tostring{}, 'table:'))\n\
assert(string.find(tostring(print), 'function:'))\n\
assert(tostring(123456789) == '123456789')\n\
assert(#tostring('\\0') == 1)\n\
assert(tostring(true) == \"true\")\n\
assert(tostring(false) == \"false\")\n\
print('+')\n\
x = '\"ilo\"\\n\\\\'\n\
assert(string.format('%q%s', x, x) == '\"\\\\\"ilo\\\\\"\\\\\\n\\\\\\\\\"\"ilo\"\\n\\\\')\n\
assert(string.format(\"\\0%c\\0%c%x\\0\", string.byte(\"a\"), string.byte(\"b\"), 140) ==\n\
              \"\\0a\\0b8c\\0\")\n\
assert(string.format('') == \"\")\n\
assert(string.format(\"%c\",34)..string.format(\"%c\",48)..string.format(\"%c\",90)..string.format(\"%c\",100) ==\n\
       string.format(\"%c%c%c%c\", 34, 48, 90, 100))\n\
assert(string.format(\"%s\\0 is not \\0%s\", 'not be', 'be') == 'not be\\0 is not \\0be')\n\
assert(string.format(\"%%%d %010d\", 10, 23) == \"%10 0000000023\")\n\
assert(tonumber(string.format(\"%f\", 10.3)) == 10.3)\n\
x = string.format('\"%-50s\"', 'a')\n\
assert(#x == 52)\n\
assert(string.sub(x, 1, 4) == '\"a  ')\n\
assert(string.format(\"-%.20s.20s\", string.rep(\"%\", 2000)) == \"-\"..string.rep(\"%\", 20)..\".20s\")\n\
assert(string.format('\"-%20s.20s\"', string.rep(\"%\", 2000)) ==\n\
       string.format(\"%q\", \"-\"..string.rep(\"%\", 2000)..\".20s\"))\n\
assert(loadstring(\"return 1\\n--comentario sem EOL no final\")() == 1)\n\
assert(table.concat{} == \"\")\n\
assert(table.concat({}, 'x') == \"\")\n\
assert(table.concat({'\\0', '\\0\\1', '\\0\\1\\2'}, '.\\0.') == \"\\0.\\0.\\0\\1.\\0.\\0\\1\\2\")\n\
local a = {}; for i=1,3000 do a[i] = \"xuxu\" end\n\
assert(table.concat(a, \"123\")..\"123\" == string.rep(\"xuxu123\", 3000))\n\
assert(table.concat(a, \"b\", 20, 20) == \"xuxu\")\n\
assert(table.concat(a, \"\", 20, 21) == \"xuxuxuxu\")\n\
assert(table.concat(a, \"\", 22, 21) == \"\")\n\
assert(table.concat(a, \"3\", 2999) == \"xuxu3xuxu\")\n\
a = {\"a\",\"b\",\"c\"}\n\
assert(table.concat(a, \",\", 1, 0) == \"\")\n\
assert(table.concat(a, \",\", 1, 1) == \"a\")\n\
assert(table.concat(a, \",\", 1, 2) == \"a,b\")\n\
assert(table.concat(a, \",\", 2) == \"b,c\")\n\
assert(table.concat(a, \",\", 3) == \"c\")\n\
assert(table.concat(a, \",\", 4) == \"\")\n\
print('  OK')";
