static const char* errors="print(\"testing errors\")\n\
function doit (s)\n\
  local f, msg = loadstring(s)\n\
  if f == nil then return msg end\n\
  local cond, msg = pcall(f)\n\
  return (not cond) and msg\n\
end\n\
function checkmessage (prog, msg)\n\
  assert(string.find(doit(prog), msg, 1, true))\n\
end\n\
function checksyntax (prog, extra, token, line)\n\
  local msg = doit(prog)\n\
  token = string.gsub(token, \"(%p)\", \"%%%1\")\n\
  local pt = string.format([[^%%[string \".*\"%%]:%d: .- near '%s'$]],\n\
                           line, token)\n\
  assert(string.find(msg, pt))\n\
  assert(string.find(msg, msg, 1, true))\n\
end\n\
assert(doit(\"error('hi', 0)\") == 'hi')\n\
assert(doit(\"error()\") == nil)\n\
assert(doit(\"unpack({}, 1, n=2^30)\"))\n\
assert(doit(\"a=math.sin()\"))\n\
assert(not doit(\"tostring(1)\") and doit(\"tostring()\"))\n\
assert(doit\"tonumber()\")\n\
assert(doit\"repeat until 1; a\")\n\
assert(doit\"return;;\")\n\
assert(doit\"assert(false)\")\n\
assert(doit\"assert(nil)\")\n\
assert(doit\"a=math.sin\\n(3)\")\n\
assert(doit(\"function a (... , ...) end\"))\n\
assert(doit(\"function a (, ...) end\"))\n\
checkmessage(\"a={}; do local a=1 end a:bbbb(3)\", \"method 'bbbb'\")\n\
checkmessage(\"local a={}; a.bbbb(3)\", \"field 'bbbb'\")\n\
assert(not string.find(doit\"a={13}; local bbbb=1; a[bbbb](3)\", \"'bbbb'\"))\n\
checkmessage(\"a={13}; local bbbb=1; a[bbbb](3)\", \"number\")\n\
aaa = nil\n\
checkmessage(\"aaa.bbb:ddd(9)\", \"global 'aaa'\")\n\
checkmessage(\"local aaa={bbb=1}; aaa.bbb:ddd(9)\", \"field 'bbb'\")\n\
checkmessage(\"local aaa={bbb={}}; aaa.bbb:ddd(9)\", \"method 'ddd'\")\n\
checkmessage(\"local a,b,c; (function () a = b+1 end)()\", \"upvalue 'b'\")\n\
assert(not doit\"local aaa={bbb={ddd=next}}; aaa.bbb:ddd(nil)\")\n\
checkmessage(\"b=1; local aaa='a'; x=aaa+b\", \"local 'aaa'\")\n\
checkmessage(\"aaa={}; x=3/aaa\", \"global 'aaa'\")\n\
checkmessage(\"aaa='2'; b=nil;x=aaa*b\", \"global 'b'\")\n\
checkmessage(\"aaa={}; x=-aaa\", \"global 'aaa'\")\n\
assert(not string.find(doit\"aaa={}; x=(aaa or aaa)+(aaa and aaa)\", \"'aaa'\"))\n\
assert(not string.find(doit\"aaa={}; (aaa or aaa)()\", \"'aaa'\"))\n\
checkmessage([[\n\
prefix = nil\n\
insert = nil\n\
while 1 do\n\
  local a\n\
  if nil then break end\n\
  insert(prefix, a)\n\
end]], \"global 'insert'\")\n\
checkmessage([[collectgarbage(\"nooption\")]], \"invalid option\")\n\
checkmessage([[x = print .. \"a\"]], \"concatenate\")\n\
print'+'\n\
function lineerror (s)\n\
  local err,msg = pcall(loadstring(s))\n\
  local line = string.match(msg, \":(%d+):\")\n\
  return line and line+0\n\
end\n\
assert(lineerror\"local a\\n for i=1,'a' do \\n print(i) \\n end\" == 2)\n\
assert(lineerror\"\\n local a \\n for k,v in 3 \\n do \\n print(k) \\n end\" == 3)\n\
assert(lineerror\"\\n\\n for k,v in \\n 3 \\n do \\n print(k) \\n end\" == 4)\n\
assert(lineerror\"function a.x.y ()\\na=a+1\\nend\" == 1)\n\
local p = [[\n\
function g() f() end\n\
function f(x) error('a', X) end\n\
g()\n\
]]\n\
X=3;assert(lineerror(p) == 3)\n\
X=0;assert(lineerror(p) == nil)\n\
X=1;assert(lineerror(p) == 2)\n\
X=2;assert(lineerror(p) == 1)\n\
lineerror = nil\n\
C = 0\n\
function f() error{msg='x'} end\n\
res, msg = xpcall(f, function (r) return {msg=r.msg..'y'} end)\n\
assert(msg.msg == 'xy')\n\
print('+')\n\
checksyntax(\"syntax error\", \"\", \"error\", 1)\n\
checksyntax(\"1.000\", \"\", \"1.000\", 1)\n\
checksyntax(\"[[a]]\", \"\", \"[[a]]\", 1)\n\
checksyntax(\"'aa'\", \"\", \"'aa'\", 1)\n\
doit('I = loadstring(\"a=9+\"); a=3')\n\
assert(a==3 and I == nil)\n\
print('+')\n\
lim = 1000\n\
if rawget(_G, \"_soft\") then lim = 100 end\n\
for i=1,lim do\n\
  doit('a = ')\n\
  doit('a = 4+nil')\n\
end\n\
s = \"\\nfunction foo ()\\n  local \"\n\
for j = 1,300 do\n\
  s = s..\"a\"..j..\", \"\n\
end\n\
s = s..\"b\\n\"\n\
local a,b = loadstring(s)\n\
assert(string.find(b, \"line 2\"))\n\
print('  OK')";
