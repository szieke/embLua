print "testing closures"

local A,B = 0,{g=10}
function f(x)
  local a = {}
  for i=1,1000 do
    local y = 0
    do
      a[i] = function () B.g = B.g+1; y = y+x; return y+A end
    end
  end
  local dummy = function () return a[A] end
  collectgarbage()
  A = 1; assert(dummy() == a[1]); A = 0;
  assert(a[1]() == x)
  assert(a[3]() == x)
  collectgarbage()
  assert(B.g == 12)
  return a
end

a = f(10)
-- force a GC in this level
local x = {[1] = {}}   -- to detect a GC
setmetatable(x, {__mode = 'kv'})
while x[1] do   -- repeat until GC
  local a = A..A..A..A  -- create garbage
  A = A+1
end
assert(a[1]() == 20+A)
assert(a[1]() == 30+A)
assert(a[2]() == 10+A)
collectgarbage()
assert(a[2]() == 20+A)
assert(a[2]() == 30+A)
assert(a[3]() == 20+A)
assert(a[8]() == 10+A)
assert(getmetatable(x).__mode == 'kv')
assert(B.g == 19)

-- testing closures with 'for' control variable
a = {}
for i=1,10 do
  a[i] = {set = function(x) i=x end, get = function () return i end}
  if i == 3 then break end
end
assert(a[4] == nil)
a[1].set(10)
assert(a[2].get() == 2)
a[2].set('a')
assert(a[3].get() == 3)
assert(a[2].get() == 'a')

a = {}
for i, k in pairs{'a', 'b'} do
  a[i] = {set = function(x, y) i=x; k=y end,
          get = function () return i, k end}
  if i == 2 then break end
end
a[1].set(10, 20)
local r,s = a[2].get()
assert(r == 2 and s == 'b')
r,s = a[1].get()
assert(r == 10 and s == 20)
a[2].set('a', 'b')
r,s = a[2].get()
assert(r == "a" and s == "b")


-- testing closures with 'for' control variable x break
for i=1,3 do
  f = function () return i end
  break
end
assert(f() == 1)

for k, v in pairs{"a", "b"} do
  f = function () return k, v end
  break
end
assert(({f()})[1] == 1)
assert(({f()})[2] == "a")


-- testing closure x break x return x errors

local b
function f(x)
  local first = 1
  while 1 do
    if x == 3 and not first then return end
    local a = 'xuxu'
    b = function (op, y)
          if op == 'set' then
            a = x+y
          else
            return a
          end
        end
    if x == 1 then do break end
    elseif x == 2 then return
    else if x ~= 3 then error() end
    end
    first = nil
  end
end

for i=1,3 do
  f(i)
  assert(b('get') == 'xuxu')
  b('set', 10); assert(b('get') == 10+i)
  b = nil
end

pcall(f, 4);
assert(b('get') == 'xuxu')
b('set', 10); assert(b('get') == 14)


local w
-- testing multi-level closure
function f(x)
  return function (y)
    return function (z) return w+x+y+z end
  end
end

y = f(10)
w = 1.345
assert(y(20)(30) == 60+w)

-- testing closures x repeat-until

local a = {}
local i = 1
repeat
  local x = i
  a[i] = function () i = x+1; return x end
until i > 10 or a[i]() ~= x
assert(i == 11 and a[1]() == 1 and a[3]() == 3 and i == 4)

print'+'


-- test for correctly closing upvalues in tail calls of vararg functions
local function t ()
  local function c(a,b) assert(a=="test" and b=="OK") end
  local function v(f, ...) c("test", f() ~= 1 and "FAILED" or "OK") end
  local x = 1
  return v(function() return x end)
end
t()

print'  OK'
