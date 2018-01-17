static const char* pm="print('testing pattern matching')\n\
function f(s, p)\n\
  local i,e = string.find(s, p)\n\
  if i then return string.sub(s, i, e) end\n\
end\n\
a,b = string.find('', '')    -- empty patterns are tricky\n\
assert(a == 1 and b == 0);\n\
a,b = string.find('alo', '')\n\
assert(a == 1 and b == 0)\n\
a,b = string.find('a\\0o a\\0o a\\0o', 'a', 1)   -- first position\n\
assert(a == 1 and b == 1)\n\
a,b = string.find('a\\0o a\\0o a\\0o', 'a\\0o', 2)   -- starts in the midle\n\
assert(a == 5 and b == 7)\n\
a,b = string.find('a\\0o a\\0o a\\0o', 'a\\0o', 9)   -- starts in the midle\n\
assert(a == 9 and b == 11)\n\
a,b = string.find('a\\0a\\0a\\0a\\0\\0ab', '\\0ab', 2);  -- finds at the end\n\
assert(a == 9 and b == 11);\n\
a,b = string.find('a\\0a\\0a\\0a\\0\\0ab', 'b')    -- last position\n\
assert(a == 11 and b == 11)\n\
assert(string.find('a\\0a\\0a\\0a\\0\\0ab', 'b\\0') == nil)   -- check ending\n\
assert(string.find('', '\\0') == nil)\n\
assert(string.find('alo123alo', '12') == 4)\n\
assert(string.find('alo123alo', '^12') == nil)\n\
assert(f('aloALO', '%l*') == 'alo')\n\
assert(f('aLo_ALO', '%a*') == 'aLo')\n\
assert(f('aaab', 'a*') == 'aaa');\n\
assert(f('aaa', '^.*$') == 'aaa');\n\
assert(f('aaa', 'b*') == '');\n\
assert(f('aaa', 'ab*a') == 'aa')\n\
assert(f('aba', 'ab*a') == 'aba')\n\
assert(f('aaab', 'a+') == 'aaa')\n\
assert(f('aaa', '^.+$') == 'aaa')\n\
assert(f('aaa', 'b+') == nil)\n\
assert(f('aaa', 'ab+a') == nil)\n\
assert(f('aba', 'ab+a') == 'aba')\n\
assert(f('a$a', '.$') == 'a')\n\
assert(f('a$a', '.%$') == 'a$')\n\
assert(f('a$a', '.$.') == 'a$a')\n\
assert(f('a$a', '$$') == nil)\n\
assert(f('a$b', 'a$') == nil)\n\
assert(f('a$a', '$') == '')\n\
assert(f('', 'b*') == '')\n\
assert(f('aaa', 'bb*') == nil)\n\
assert(f('aaab', 'a-') == '')\n\
assert(f('aaa', '^.-$') == 'aaa')\n\
assert(f('aabaaabaaabaaaba', 'b.*b') == 'baaabaaabaaab')\n\
assert(f('aabaaabaaabaaaba', 'b.-b') == 'baaab')\n\
assert(f('alo xo', '.o$') == 'xo')\n\
assert(f(' \\n isto  assim', '%S%S*') == 'isto')\n\
assert(f(' \\n isto  assim', '%S*$') == 'assim')\n\
assert(f(' \\n isto  assim', '[a-z]*$') == 'assim')\n\
assert(f('um caracter ? extra', '[^%sa-z]') == '?')\n\
assert(f('', 'a?') == '')\n\
assert(f('a', 'a?') == 'a')\n\
assert(f('abl', 'a?b?l?') == 'abl')\n\
assert(f('  abl', 'a?b?l?') == '')\n\
assert(f('aa', '^aa?a?a') == 'aa')\n\
assert(f(']]]ab', '[^]]') == 'a')\n\
assert(f(\"0alo alo\", \"%x*\") == \"0a\")\n\
assert(f(\"alo alo\", \"%C+\") == \"alo alo\")\n\
print('+')\n\
assert(string.match('==========', '^([=]*)=%1$') == nil)\n\
local function range (i, j)\n\
  if i <= j then\n\
    return i, range(i+1, j)\n\
  end\n\
end\n\
local abc = string.char(range(0, 255));\n\
assert(string.len(abc) == 256)\n\
function strset (p)\n\
  local res = {s=''}\n\
  string.gsub(abc, p, function (c) res.s = res.s .. c end)\n\
  return res.s\n\
end;\n\
assert(string.len(strset('[\\200-\\210]')) == 11)\n\
assert(strset('[a-z]') == \"abcdefghijklmnopqrstuvwxyz\")\n\
assert(strset('[a-z%d]') == strset('[%da-uu-z]'))\n\
assert(strset('[a-]') == \"-a\")\n\
assert(strset('[^%W]') == strset('[%w]'))\n\
assert(strset('[]%%]') == '%]')\n\
assert(strset('[a%-z]') == '-az')\n\
assert(strset('[%^%[%-a%]%-b]') == '-[]^ab')\n\
assert(strset('%Z') == strset('[\\1-\\255]'))\n\
assert(strset('.') == strset('[\\1-\\255%z]'))\n\
print('+');\n\
assert(string.match(\"alo xyzK\", \"(%w+)K\") == \"xyz\")\n\
assert(string.match(\"254 K\", \"(%d*)K\") == \"\")\n\
assert(string.match(\"alo \", \"(%w*)$\") == \"\")\n\
assert(string.match(\"alo \", \"(%w+)$\") == nil)\n\
assert(string.find(\"(alo)\", \"%(a\") == 1)\n\
local a, b, c, d, e = string.match(\"alo alo\", \"^(((.).).* (%w*))$\")\n\
assert(a == 'alo alo' and b == 'al' and c == 'a' and d == 'alo' and e == nil)\n\
a, b, c, d  = string.match('0123456789', '(.+(.?)())')\n\
assert(a == '0123456789' and b == '' and c == 11 and d == nil)\n\
print('+')\n\
assert(string.gsub('ulo ulo', 'u', 'x') == 'xlo xlo')\n\
assert(string.gsub('alo ulo  ', ' +$', '') == 'alo ulo')  -- trim\n\
assert(string.gsub('  alo alo  ', '^%s*(.-)%s*$', '%1') == 'alo alo')  -- double trim\n\
assert(string.gsub('alo  alo  \\n 123\\n ', '%s+', ' ') == 'alo alo 123 ')\n\
t = \"abc d\"\n\
a, b = string.gsub(t, '(.)', '%1@')\n\
assert('@'..a == string.gsub(t, '', '@') and b == 5)\n\
a, b = string.gsub('abcd', '(.)', '%0@', 2)\n\
assert(a == 'a@b@cd' and b == 2)\n\
assert(string.gsub('alo alo', '()[al]', '%1') == '12o 56o')\n\
assert(string.gsub(\"abc=xyz\", \"(%w*)(%p)(%w+)\", \"%3%2%1-%0\") ==\n\
              \"xyz=abc-abc=xyz\")\n\
assert(string.gsub(\"abc\", \"%w\", \"%1%0\") == \"aabbcc\")\n\
assert(string.gsub(\"abc\", \"%w+\", \"%0%1\") == \"abcabc\")\n\
assert(string.gsub('aei', '$', '\\0óu') == 'aei\\0óu')\n\
assert(string.gsub('', '^', 'r') == 'r')\n\
assert(string.gsub('', '$', 'r') == 'r')\n\
print('+')\n\
assert(string.gsub(\"um (dois) tres (quatro)\", \"(%(%w+%))\", string.upper) ==\n\
            \"um (DOIS) tres (QUATRO)\")\n\
do\n\
  local function setglobal (n,v) rawset(_G, n, v) end\n\
  string.gsub(\"a=roberto,roberto=a\", \"(%w+)=(%w%w*)\", setglobal)\n\
  assert(_G.a==\"roberto\" and _G.roberto==\"a\")\n\
end\n\
function f(a,b) return string.gsub(a,'.',b) end\n\
assert(string.gsub(\"trocar tudo em |teste|b| e |beleza|al|\", \"|([^|]*)|([^|]*)|\", f) ==\n\
            \"trocar tudo em bbbbb e alalalalalal\")\n\
local function dostring (s) return loadstring(s)() or \"\" end\n\
assert(string.gsub(\"alo $a=1$ novamente $return a$\", \"$([^$]*)%$\", dostring) ==\n\
            \"alo  novamente 1\")\n\
x = string.gsub(\"$x=string.gsub('alo', '.', string.upper)$ assim vai para $return x$\",\n\
         \"$([^$]*)%$\", dostring)\n\
assert(x == ' assim vai para ALO')\n\
t = {}\n\
s = 'a alo jose  joao'\n\
r = string.gsub(s, '()(%w+)()', function (a,w,b)\n\
      assert(string.len(w) == b-a);\n\
      t[a] = b-a;\n\
    end)\n\
assert(s == r and t[1] == 1 and t[3] == 3 and t[7] == 4 and t[13] == 4)\n\
function isbalanced (s)\n\
  return string.find(string.gsub(s, \"%b()\", \"\"), \"[()]\") == nil\n\
end\n\
assert(isbalanced(\"(9 ((8))(\\0) 7) \\0\\0 a b ()(c)() a\"))\n\
assert(not isbalanced(\"(9 ((8) 7) a b (\\0 c) a\"))\n\
assert(string.gsub(\"alo 'oi' alo\", \"%b''\", '\"') == 'alo \" alo')\n\
local t = {\"apple\", \"orange\", \"lime\"; n=0}\n\
assert(string.gsub(\"x and x and x\", \"x\", function () t.n=t.n+1; return t[t.n] end)\n\
        == \"apple and orange and lime\")\n\
t = {n=0}\n\
string.gsub(\"first second word\", \"%w%w*\", function (w) t.n=t.n+1; t[t.n] = w end)\n\
assert(t[1] == \"first\" and t[2] == \"second\" and t[3] == \"word\" and t.n == 3)\n\
t = {n=0}\n\
assert(string.gsub(\"first second word\", \"%w+\",\n\
         function (w) t.n=t.n+1; t[t.n] = w end, 2) == \"first second word\")\n\
assert(t[1] == \"first\" and t[2] == \"second\" and t[3] == nil)\n\
assert(not pcall(string.gsub, \"alo\", \"(.\", print))\n\
assert(not pcall(string.gsub, \"alo\", \".)\", print))\n\
assert(not pcall(string.gsub, \"alo\", \"(.\", {}))\n\
assert(not pcall(string.gsub, \"alo\", \"(.)\", \"%2\"))\n\
assert(not pcall(string.gsub, \"alo\", \"(%1)\", \"a\"))\n\
assert(not pcall(string.gsub, \"alo\", \"(%0)\", \"a\"))\n\
assert(string.gsub(\"alo alo\", \".\", {}) == \"alo alo\")\n\
assert(string.gsub(\"alo alo\", \"(.)\", {a=\"AA\", l=\"\"}) == \"AAo AAo\")\n\
assert(string.gsub(\"alo alo\", \"(.).\", {a=\"AA\", l=\"K\"}) == \"AAo AAo\")\n\
assert(string.gsub(\"alo alo\", \"((.)(.?))\", {al=\"AA\", o=false}) == \"AAo AAo\")\n\
assert(string.gsub(\"alo alo\", \"().\", {2,5,6}) == \"256 alo\")\n\
t = {}; setmetatable(t, {__index = function (t,s) return string.upper(s) end})\n\
assert(string.gsub(\"a alo b hi\", \"%w%w+\", t) == \"a ALO b HI\")\n\
local a = 0\n\
for i in string.gmatch('abcde', '()') do assert(i == a+1); a=i end\n\
assert(a==6)\n\
t = {n=0}\n\
for w in string.gmatch(\"first second word\", \"%w+\") do\n\
      t.n=t.n+1; t[t.n] = w\n\
end\n\
assert(t[1] == \"first\" and t[2] == \"second\" and t[3] == \"word\")\n\
t = {3, 6, 9}\n\
for i in string.gmatch (\"xuxx uu ppar r\", \"()(.)%2\") do\n\
  assert(i == table.remove(t, 1))\n\
end\n\
assert(#t == 0)\n\
t = {}\n\
for i,j in string.gmatch(\"13 14 10 = 11, 15= 16, 22=23\", \"(%d+)%s*=%s*(%d+)\") do\n\
  t[i] = j\n\
end\n\
a = 0\n\
for k,v in pairs(t) do assert(k+1 == v+0); a=a+1 end\n\
assert(a == 3)\n\
assert(string.gsub(\"aaa aa a aaa a\", \"%f[%w]a\", \"x\") == \"xaa xa x xaa x\")\n\
assert(string.gsub(\"[[]] [][] [[[[\", \"%f[[].\", \"x\") == \"x[]] x]x] x[[[\")\n\
assert(string.gsub(\"01abc45de3\", \"%f[%d]\", \".\") == \".01abc.45de.3\")\n\
assert(string.gsub(\"01abc45 de3x\", \"%f[%D]%w\", \".\") == \"01.bc45 de3.\")\n\
assert(string.gsub(\"function\", \"%f[\\1-\\255]%w\", \".\") == \".unction\")\n\
assert(string.gsub(\"function\", \"%f[^\\1-\\255]\", \".\") == \"function.\")\n\
local i, e = string.find(\" alo aalo allo\", \"%f[%S].-%f[%s].-%f[%S]\")\n\
assert(i == 2 and e == 5)\n\
local k = string.match(\" alo aalo allo\", \"%f[%S](.-%f[%s].-%f[%S])\")\n\
assert(k == 'alo ')\n\
local a = {1, 5, 9, 14, 17,}\n\
for k in string.gmatch(\"alo alo th02 is 1hat\", \"()%f[%w%d]\") do\n\
  assert(table.remove(a, 1) == k)\n\
end\n\
assert(#a == 0)\n\
print('  OK')";
