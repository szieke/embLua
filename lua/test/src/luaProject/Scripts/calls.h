static const char* calls="print(\"testing functions and calls\")\n\
assert(type(1<2) == 'boolean')\n\
assert(type(true) == 'boolean' and type(false) == 'boolean')\n\
assert(type(nil) == 'nil' and type(-3) == 'number' and type'x' == 'string' and\n\
       type{} == 'table' and type(type) == 'function')\n\
assert(type(assert) == type(print))\n\
f = nil\n\
function f (x) return a:x (x) end\n\
assert(type(f) == 'function')\n\
fact = false\n\
do\n\
  local res = 1\n\
  local function fact (n)\n\
    if n==0 then return res\n\
    else return n*fact(n-1)\n\
    end\n\
  end\n\
  assert(fact(5) == 120)\n\
end\n\
assert(fact == false)\n\
a = {i = 10}\n\
self = 20\n\
function a:x (x) return x+self.i end\n\
function a.y (x) return x+self end\n\
assert(a:x(1)+10 == a.y(1))\n\
a.t = {i=-100}\n\
a[\"t\"].x = function (self, a,b) return self.i+a+b end\n\
assert(a.t:x(2,3) == -95)\n\
do\n\
  local a = {x=0}\n\
  function a:add (x) self.x, a.y = self.x+x, 20; return self end\n\
  assert(a:add(10):add(20):add(30).x == 60 and a.y == 20)\n\
end\n\
local a = {b={c={}}}\n\
function a.b.c.f1 (x) return x+1 end\n\
function a.b.c:f2 (x,y) self[x] = y end\n\
assert(a.b.c.f1(4) == 5)\n\
a.b.c:f2('k', 12); assert(a.b.c.k == 12)\n\
print('+')\n\
t = nil   -- 'declare' t\n\
function f(a,b,c) local d = 'a'; t={a,b,c,d} end\n\
f(      -- this line change must be valid\n\
  1,2)\n\
assert(t[1] == 1 and t[2] == 2 and t[3] == nil and t[4] == 'a')\n\
f(1,2,   -- this one too\n\
      3,4)\n\
assert(t[1] == 1 and t[2] == 2 and t[3] == 3 and t[4] == 'a')\n\
function fat(x)\n\
  if x <= 1 then return 1\n\
  else return x*loadstring(\"return fat(\" .. x-1 .. \")\")()\n\
  end\n\
end\n\
assert(loadstring \"loadstring 'assert(fat(6)==720)' () \")()\n\
a = loadstring('return fat(5), 3')\n\
a,b = a()\n\
assert(a == 120 and b == 3)\n\
print('+')\n\
function err_on_n (n)\n\
  if n==0 then error(); exit(1);\n\
  else err_on_n (n-1); exit(1);\n\
  end\n\
end\n\
do\n\
  function dummy (n)\n\
    if n > 0 then\n\
      assert(not pcall(err_on_n, n))\n\
      dummy(n-1)\n\
    end\n\
  end\n\
end\n\
dummy(10)\n\
function deep (n)\n\
  if n>0 then deep(n-1) end\n\
end\n\
deep(10)\n\
deep(200)\n\
function deep (n) if n>0 then return deep(n-1) else return 101 end end\n\
assert(deep(30000) == 101)\n\
a = {}\n\
function a:deep (n) if n>0 then return self:deep(n-1) else return 101 end end\n\
assert(a:deep(30000) == 101)\n\
print('+')\n\
a = nil\n\
(function (x) a=x end)(23)\n\
assert(a == 23 and (function (x) return x*2 end)(20) == 40)\n\
local x,y,z,a\n\
a = {}; lim = 2000\n\
for i=1, lim do a[i]=i end\n\
assert(select(lim, unpack(a)) == lim and select('#', unpack(a)) == lim)\n\
x = unpack(a)\n\
assert(x == 1)\n\
x = {unpack(a)}\n\
assert(#x == lim and x[1] == 1 and x[lim] == lim)\n\
x = {unpack(a, lim-2)}\n\
assert(#x == 3 and x[1] == lim-2 and x[3] == lim)\n\
x = {unpack(a, 10, 6)}\n\
assert(next(x) == nil)   -- no elements\n\
x = {unpack(a, 11, 10)}\n\
assert(next(x) == nil)   -- no elements\n\
x,y = unpack(a, 10, 10)\n\
assert(x == 10 and y == nil)\n\
x,y,z = unpack(a, 10, 11)\n\
assert(x == 10 and y == 11 and z == nil)\n\
a,x = unpack{1}\n\
assert(a==1 and x==nil)\n\
a,x = unpack({1,2}, 1, 1)\n\
assert(a==1 and x==nil)\n\
Y = function (le)\n\
      local function a (f)\n\
        return le(function (x) return f(f)(x) end)\n\
      end\n\
      return a(a)\n\
    end\n\
F = function (f)\n\
      return function (n)\n\
               if n == 0 then return 1\n\
               else return n*f(n-1) end\n\
             end\n\
    end\n\
fat = Y(F)\n\
assert(fat(0) == 1 and fat(4) == 24 and Y(F)(5)==5*Y(F)(4))\n\
local function g (z)\n\
  local function f (a,b,c,d)\n\
    return function (x,y) return a+b+c+d+a+x+y+z end\n\
  end\n\
  return f(z,z+1,z+2,z+3)\n\
end\n\
f = g(10)\n\
assert(f(9, 16) == 10+11+12+13+10+9+16+10)\n\
Y, F, f = nil\n\
print('+')\n\
function unlpack (t, i)\n\
  i = i or 1\n\
  if (i <= #t) then\n\
    return t[i], unlpack(t, i+1)\n\
  end\n\
end\n\
function equaltab (t1, t2)\n\
  assert(#t1 == #t2)\n\
  for i,v1 in ipairs(t1) do\n\
    assert(v1 == t2[i])\n\
  end\n\
end\n\
local function pack (...)\n\
  local x = {...}\n\
  x.n = select('#', ...)\n\
  return x\n\
end\n\
function f() return 1,2,30,4 end\n\
function ret2 (a,b) return a,b end\n\
local a,b,c,d = unlpack{1,2,3}\n\
assert(a==1 and b==2 and c==3 and d==nil)\n\
a = {1,2,3,4,false,10,'alo',false,assert}\n\
equaltab(pack(unlpack(a)), a)\n\
equaltab(pack(unlpack(a), -1), {1,-1})\n\
a,b,c,d = ret2(f()), ret2(f())\n\
assert(a==1 and b==1 and c==2 and d==nil)\n\
a,b,c,d = unlpack(pack(ret2(f()), ret2(f())))\n\
assert(a==1 and b==1 and c==2 and d==nil)\n\
a,b,c,d = unlpack(pack(ret2(f()), (ret2(f()))))\n\
assert(a==1 and b==1 and c==nil and d==nil)\n\
a = ret2{ unlpack{1,2,3}, unlpack{3,2,1}, unlpack{\"a\", \"b\"}}\n\
assert(a[1] == 1 and a[2] == 3 and a[3] == \"a\" and a[4] == \"b\")\n\
rawget({}, \"x\", 1)\n\
rawset({}, \"x\", 1, 2)\n\
assert(math.sin(1,2) == math.sin(1))\n\
table.sort({10,9,8,4,19,23,0,0}, function (a,b) return a<b end, \"extra arg\")\n\
x = \"-- a comment\\0\\0\\0\\n  x = 10 + \\n23; \\\n\
     local a = function () x = 'hi' end; \\\n\
     return '\\0'\"\n\
local i = 0\n\
function read1 (x)\n\
  return function ()\n\
    collectgarbage()\n\
    i=i+1\n\
    return string.sub(x, i, i)\n\
  end\n\
end\n\
i = 0\n\
local a, b = load(read1(\"*a = 123\"))\n\
assert(not a and type(b) == \"string\" and i == 2)\n\
a, b = load(function () error(\"hhi\") end)\n\
assert(not a and string.find(b, \"hhi\"))\n\
x = [[\n\
  return function (x)\n\
    return function (y)\n\
     return function (z)\n\
       return x+y+z\n\
     end\n\
   end\n\
  end\n\
]]\n\
a = assert(load(read1(x)))\n\
assert(a()(2)(3)(10) == 15)\n\
assert((function () return nil end)(4) == nil)\n\
assert((function () local a; return a end)(4) == nil)\n\
assert((function (a) return a end)() == nil)\n\
print('  OK')\n\
return deep";
