static const char* closure="print \"testing closures and coroutines\"\n\
local A,B = 0,{g=10}\n\
function f(x)\n\
  local a = {}\n\
  for i=1,1000 do\n\
    local y = 0\n\
    do\n\
      a[i] = function () B.g = B.g+1; y = y+x; return y+A end\n\
    end\n\
  end\n\
  local dummy = function () return a[A] end\n\
  collectgarbage()\n\
  A = 1; assert(dummy() == a[1]); A = 0;\n\
  assert(a[1]() == x)\n\
  assert(a[3]() == x)\n\
  collectgarbage()\n\
  assert(B.g == 12)\n\
  return a\n\
end\n\
a = f(10)\n\
local x = {[1] = {}}\n\
setmetatable(x, {__mode = 'kv'})\n\
while x[1] do\n\
  local a = A..A..A..A\n\
  A = A+1\n\
end\n\
assert(a[1]() == 20+A)\n\
assert(a[1]() == 30+A)\n\
assert(a[2]() == 10+A)\n\
collectgarbage()\n\
assert(a[2]() == 20+A)\n\
assert(a[2]() == 30+A)\n\
assert(a[3]() == 20+A)\n\
assert(a[8]() == 10+A)\n\
assert(getmetatable(x).__mode == 'kv')\n\
assert(B.g == 19)\n\
a = {}\n\
for i=1,10 do\n\
  a[i] = {set = function(x) i=x end, get = function () return i end}\n\
  if i == 3 then break end\n\
end\n\
assert(a[4] == nil)\n\
a[1].set(10)\n\
assert(a[2].get() == 2)\n\
a[2].set('a')\n\
assert(a[3].get() == 3)\n\
assert(a[2].get() == 'a')\n\
a = {}\n\
for i, k in pairs{'a', 'b'} do\n\
  a[i] = {set = function(x, y) i=x; k=y end,\n\
          get = function () return i, k end}\n\
  if i == 2 then break end\n\
end\n\
a[1].set(10, 20)\n\
local r,s = a[2].get()\n\
assert(r == 2 and s == 'b')\n\
r,s = a[1].get()\n\
assert(r == 10 and s == 20)\n\
a[2].set('a', 'b')\n\
r,s = a[2].get()\n\
assert(r == \"a\"and s == \"b\")\n\
for i=1,3 do\n\
  f = function () return i end\n\
  break\n\
end\n\
assert(f() == 1)\n\
for k, v in pairs{\"a\", \"b\"} do\n\
  f = function () return k, v end\n\
  break\n\
end\n\
assert(({f()})[1] == 1)\n\
assert(({f()})[2] == \"a\")\n\
local b\n\
function f(x)\n\
  local first = 1\n\
  while 1 do\n\
    if x == 3 and not first then return end\n\
    local a = 'xuxu'\n\
    b = function (op, y)\n\
          if op == 'set' then\n\
            a = x+y\n\
          else\n\
            return a\n\
          end\n\
        end\n\
    if x == 1 then do break end\n\
    elseif x == 2 then return\n\
    else if x ~= 3 then error() end\n\
    end\n\
    first = nil\n\
  end\n\
end\n\
for i=1,3 do\n\
  f(i)\n\
  assert(b('get') == 'xuxu')\n\
  b('set', 10); assert(b('get') == 10+i)\n\
  b = nil\n\
end\n\
pcall(f, 4);\n\
assert(b('get') == 'xuxu')\n\
b('set', 10); assert(b('get') == 14)\n\
local w\n\
function f(x)\n\
  return function (y)\n\
    return function (z) return w+x+y+z end\n\
  end\n\
end\n\
y = f(10)\n\
w = 1.345\n\
assert(y(20)(30) == 60+w)\n\
local a = {}\n\
local i = 1\n\
repeat\n\
  local x = i\n\
  a[i] = function () i = x+1; return x end\n\
until i > 10 or a[i]() ~= x\n\
assert(i == 11 and a[1]() == 1 and a[3]() == 3 and i == 4)\n\
print'+'\n\
local function t ()\n\
  local function c(a,b) assert(a==\"test\"and b==\"OK\") end\n\
  local function v(f, ...) c(\"test\", f() ~= 1 and \"FAILED\"or \"OK\") end\n\
  local x = 1\n\
  return v(function() return x end)\n\
end\n\
t()\n\
print'  OK'";
