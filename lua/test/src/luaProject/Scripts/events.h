static const char* events="print('testing metatables')\n\
assert(getmetatable{} == nil)\n\
assert(getmetatable(4) == nil)\n\
assert(getmetatable(nil) == nil)\n\
a={}; setmetatable(a, {__metatable = \"xuxu\",\n\
                    __tostring=function(x) return x.name end})\n\
assert(getmetatable(a) == \"xuxu\")\n\
assert(pcall(setmetatable, a, {}) == false)\n\
a.name = \"gororoba\"\n\
assert(tostring(a) == \"gororoba\")\n\
local a, t = {10,20,30; x=\"10\", y=\"20\"}, {}\n\
assert(setmetatable(a,t) == a)\n\
assert(getmetatable(a) == t)\n\
assert(setmetatable(a,nil) == a)\n\
assert(getmetatable(a) == nil)\n\
assert(setmetatable(a,t) == a)\n\
function f (t, i, e)\n\
  assert(not e)\n\
  local p = rawget(t, \"parent\")\n\
  return (p and p[i]+3), \"dummy return\"\n\
end\n\
t.__index = f\n\
a.parent = {z=25, x=12, [4] = 24}\n\
assert(a[1] == 10 and a.z == 28 and a[4] == 27 and a.x == \"10\")\n\
collectgarbage()\n\
a = setmetatable({}, t)\n\
function f(t, i, v) rawset(t, i, v-3) end\n\
t.__newindex = f\n\
a[1] = 30; a.x = \"101\"; a[5] = 200\n\
assert(a[1] == 27 and a.x == 98 and a[5] == 197)\n\
local c = {}\n\
a = setmetatable({}, t)\n\
t.__newindex = c\n\
a[1] = 10; a[2] = 20; a[3] = 90\n\
assert(c[1] == 10 and c[2] == 20 and c[3] == 90)\n\
do\n\
  local a;\n\
  a = setmetatable({}, {__index = setmetatable({},\n\
                     {__index = setmetatable({},\n\
                     {__index = function (_,n) return a[n-3]+4, \"lixo\" end})})})\n\
  a[0] = 20\n\
  for i=0,10 do\n\
    assert(a[i*3] == 20 + i*4)\n\
  end\n\
end\n\
do  -- newindex\n\
  local foi\n\
  local a = {}\n\
  for i=1,10 do a[i] = 0; a['a'..i] = 0; end\n\
  setmetatable(a, {__newindex = function (t,k,v) foi=true; rawset(t,k,v) end})\n\
  foi = false; a[1]=0; assert(not foi)\n\
  foi = false; a['a1']=0; assert(not foi)\n\
  foi = false; a['a11']=0; assert(foi)\n\
  foi = false; a[11]=0; assert(foi)\n\
  foi = false; a[1]=nil; assert(not foi)\n\
  foi = false; a[1]=nil; assert(foi)\n\
end\n\
function f (t, ...) return t, {...} end\n\
t.__call = f\n\
do\n\
  local x,y = a(unpack{'a', 1})\n\
  assert(x==a and y[1]=='a' and y[2]==1 and y[3]==nil)\n\
  x,y = a()\n\
  assert(x==a and y[1]==nil)\n\
end\n\
local b = setmetatable({}, t)\n\
setmetatable(b,t)\n\
function f(op)\n\
  return function (...) cap = {[0] = op, ...} ; return (...) end\n\
end\n\
t.__add = f(\"add\")\n\
t.__sub = f(\"sub\")\n\
t.__mul = f(\"mul\")\n\
t.__div = f(\"div\")\n\
t.__mod = f(\"mod\")\n\
t.__unm = f(\"unm\")\n\
t.__pow = f(\"pow\")\n\
assert(b+5 == b)\n\
assert(cap[0] == \"add\" and cap[1] == b and cap[2] == 5 and cap[3]==nil)\n\
assert(b+'5' == b)\n\
assert(cap[0] == \"add\" and cap[1] == b and cap[2] == '5' and cap[3]==nil)\n\
assert(5+b == 5)\n\
assert(cap[0] == \"add\" and cap[1] == 5 and cap[2] == b and cap[3]==nil)\n\
assert('5'+b == '5')\n\
assert(cap[0] == \"add\" and cap[1] == '5' and cap[2] == b and cap[3]==nil)\n\
b=b-3; assert(getmetatable(b) == t)\n\
assert(5-a == 5)\n\
assert(cap[0] == \"sub\" and cap[1] == 5 and cap[2] == a and cap[3]==nil)\n\
assert('5'-a == '5')\n\
assert(cap[0] == \"sub\" and cap[1] == '5' and cap[2] == a and cap[3]==nil)\n\
assert(a*a == a)\n\
assert(cap[0] == \"mul\" and cap[1] == a and cap[2] == a and cap[3]==nil)\n\
assert(a/0 == a)\n\
assert(cap[0] == \"div\" and cap[1] == a and cap[2] == 0 and cap[3]==nil)\n\
assert(a%2 == a)\n\
assert(cap[0] == \"mod\" and cap[1] == a and cap[2] == 2 and cap[3]==nil)\n\
assert(-a == a)\n\
assert(cap[0] == \"unm\" and cap[1] == a)\n\
assert(a^4 == a)\n\
assert(cap[0] == \"pow\" and cap[1] == a and cap[2] == 4 and cap[3]==nil)\n\
assert(a^'4' == a)\n\
assert(cap[0] == \"pow\" and cap[1] == a and cap[2] == '4' and cap[3]==nil)\n\
assert(4^a == 4)\n\
assert(cap[0] == \"pow\" and cap[1] == 4 and cap[2] == a and cap[3]==nil)\n\
assert('4'^a == '4')\n\
assert(cap[0] == \"pow\" and cap[1] == '4' and cap[2] == a and cap[3]==nil)\n\
t = {}\n\
t.__lt = function (a,b,c)\n\
  collectgarbage()\n\
  assert(c == nil)\n\
  if type(a) == 'table' then a = a.x end\n\
  if type(b) == 'table' then b = b.x end\n\
 return a<b, \"dummy\"\n\
end\n\
function Op(x) return setmetatable({x=x}, t) end\n\
local function test ()\n\
  assert(not(Op(1)<Op(1)) and (Op(1)<Op(2)) and not(Op(2)<Op(1)))\n\
  assert(not(Op('a')<Op('a')) and (Op('a')<Op('b')) and not(Op('b')<Op('a')))\n\
  assert((Op(1)<=Op(1)) and (Op(1)<=Op(2)) and not(Op(2)<=Op(1)))\n\
  assert((Op('a')<=Op('a')) and (Op('a')<=Op('b')) and not(Op('b')<=Op('a')))\n\
  assert(not(Op(1)>Op(1)) and not(Op(1)>Op(2)) and (Op(2)>Op(1)))\n\
  assert(not(Op('a')>Op('a')) and not(Op('a')>Op('b')) and (Op('b')>Op('a')))\n\
  assert((Op(1)>=Op(1)) and not(Op(1)>=Op(2)) and (Op(2)>=Op(1)))\n\
  assert((Op('a')>=Op('a')) and not(Op('a')>=Op('b')) and (Op('b')>=Op('a')))\n\
end\n\
test()\n\
t.__le = function (a,b,c)\n\
  assert(c == nil)\n\
  if type(a) == 'table' then a = a.x end\n\
  if type(b) == 'table' then b = b.x end\n\
 return a<=b, \"dummy\"\n\
end\n\
test()  -- retest comparisons, now using both `lt' and `le'\n\
local function Set(x)\n\
  local y = {}\n\
  for _,k in pairs(x) do y[k] = 1 end\n\
  return setmetatable(y, t)\n\
end\n\
t.__lt = function (a,b)\n\
  for k in pairs(a) do\n\
    if not b[k] then return false end\n\
    b[k] = nil\n\
  end\n\
  return next(b) ~= nil\n\
end\n\
t.__le = nil\n\
assert(Set{1,2,3} < Set{1,2,3,4})\n\
assert(not(Set{1,2,3,4} < Set{1,2,3,4}))\n\
assert((Set{1,2,3,4} <= Set{1,2,3,4}))\n\
assert((Set{1,2,3,4} >= Set{1,2,3,4}))\n\
assert((Set{1,3} <= Set{3,5}))   -- wrong!! model needs a `le' method ;-)\n\
t.__le = function (a,b)\n\
  for k in pairs(a) do\n\
    if not b[k] then return false end\n\
  end\n\
  return true\n\
end\n\
assert(not (Set{1,3} <= Set{3,5}))   -- now its OK!\n\
assert(not(Set{1,3} <= Set{3,5}))\n\
assert(not(Set{1,3} >= Set{3,5}))\n\
t.__eq = function (a,b)\n\
  for k in pairs(a) do\n\
    if not b[k] then return false end\n\
    b[k] = nil\n\
  end\n\
  return next(b) == nil\n\
end\n\
local s = Set{1,3,5}\n\
assert(s == Set{3,5,1})\n\
assert(not rawequal(s, Set{3,5,1}))\n\
assert(rawequal(s, s))\n\
assert(Set{1,3,5,1} == Set{3,5,1})\n\
assert(Set{1,3,5} ~= Set{3,5,1,6})\n\
t[Set{1,3,5}] = 1\n\
assert(t[Set{1,3,5}] == nil)   -- `__eq' is not valid for table accesses\n\
t.__concat = function (a,b,c)\n\
  assert(c == nil)\n\
  if type(a) == 'table' then a = a.val end\n\
  if type(b) == 'table' then b = b.val end\n\
  if A then return a..b\n\
  else\n\
    return setmetatable({val=a..b}, t)\n\
  end\n\
end\n\
c = {val=\"c\"}; setmetatable(c, t)\n\
d = {val=\"d\"}; setmetatable(d, t)\n\
A = true\n\
assert(c..d == 'cd')\n\
assert(0 ..\"a\"..\"b\"..c..d..\"e\"..\"f\"..(5+3)..\"g\" == \"0abcdef8g\")\n\
A = false\n\
x = c..d\n\
assert(getmetatable(x) == t and x.val == 'cd')\n\
x = 0 ..\"a\"..\"b\"..c..d..\"e\"..\"f\"..\"g\"\n\
assert(x.val == \"0abcdefg\")\n\
local t1, t2, c, d\n\
t1 = {};  c = {}; setmetatable(c, t1)\n\
d = {}\n\
t1.__eq = function () return true end\n\
t1.__lt = function () return true end\n\
setmetatable(d, t1)\n\
assert(c == d and c < d and not(d <= c))\n\
t2 = {}\n\
t2.__eq = t1.__eq\n\
t2.__lt = t1.__lt\n\
setmetatable(d, t2)\n\
assert(c == d and c < d and not(d <= c))\n\
local i\n\
local tt = {\n\
  __call = function (t, ...)\n\
    i = i+1\n\
    if t.f then return t.f(...)\n\
    else return {...}\n\
    end\n\
  end\n\
}\n\
local a = setmetatable({}, tt)\n\
local b = setmetatable({f=a}, tt)\n\
local c = setmetatable({f=b}, tt)\n\
i = 0\n\
x = c(3,4,5)\n\
assert(i == 3 and x[1] == 3 and x[3] == 5)\n\
print'+'\n\
a = {}\n\
rawset(a, \"x\", 1, 2, 3)\n\
assert(a.x == 1 and rawget(a, \"x\", 3) == 1)\n\
print '+'\n\
print '  OK'\n\
return 12";
