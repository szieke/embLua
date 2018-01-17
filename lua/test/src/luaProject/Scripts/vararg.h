static const char* vararg="print('testing vararg')\n\
_G.arg = nil\n\
function c12 (...)\n\
  assert(arg == nil)\n\
  local x = {...}; x.n = #x\n\
  local res = (x.n==2 and x[1] == 1 and x[2] == 2)\n\
  if res then res = 55 end\n\
  return res, 2\n\
end\n\
function vararg (...) return arg end\n\
local call = function (f, args) return f(unpack(args, 1, args.n)) end\n\
assert(c12(1,2)==55)\n\
a,b = assert(call(c12, {1,2}))\n\
assert(a == 55 and b == 2)\n\
a = call(c12, {1,2;n=2})\n\
assert(a == 55 and b == 2)\n\
a = call(c12, {1,2;n=1})\n\
assert(not a)\n\
assert(c12(1,2,3) == false)\n\
local a = vararg(call(next, {_G,nil;n=2}))\n\
local b,c = next(_G)\n\
a = call(print, {'+'})\n\
assert(a == nil)\n\
lim = 20\n\
local i, a = 1, {}\n\
while i <= lim do a[i] = i+0.3; i=i+1 end\n\
print(\"+\")\n\
function oneless (a, ...) return ... end\n\
function f (n, a, ...)\n\
  local b\n\
  assert(arg == nil)\n\
  if n == 0 then\n\
    local b, c, d = ...\n\
    return a, b, c, d, oneless(oneless(oneless(...)))\n\
  else\n\
    n, b, a = n-1, ..., a\n\
    assert(b == ...)\n\
    return f(n, a, ...)\n\
  end\n\
end\n\
a,b,c,d,e = assert(f(10,5,4,3,2,1))\n\
assert(a==5 and b==4 and c==3 and d==2 and e==1)\n\
a,b,c,d,e = f(4)\n\
assert(a==nil and b==nil and c==nil and d==nil and e==nil)\n\
f = loadstring[[ return {...} ]]\n\
x = f(2,3)\n\
assert(x[1] == 2 and x[2] == 3 and x[3] == nil)\n\
f = loadstring[[\n\
  local x = {...}\n\
  for i=1,select('#', ...) do assert(x[i] == select(i, ...)) end\n\
  assert(x[select('#', ...)+1] == nil)\n\
  return true\n\
]]\n\
assert(f(\"a\", \"b\", nil, {}, assert))\n\
assert(f())\n\
a = {select(3, unpack{10,20,30,40})}\n\
assert(#a == 2 and a[1] == 30 and a[2] == 40)\n\
a = {select(1)}\n\
assert(next(a) == nil)\n\
a = {select(-1, 3, 5, 7)}\n\
assert(a[1] == 7 and a[2] == nil)\n\
a = {select(-2, 3, 5, 7)}\n\
assert(a[1] == 5 and a[2] == 7 and a[3] == nil)\n\
pcall(select, 10000)\n\
pcall(select, -10000)\n\
print('  OK')";
