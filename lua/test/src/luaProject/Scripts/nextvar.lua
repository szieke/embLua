print('testing tables, next, and for')

local a = {}

-- make sure table has lots of space in hash part
for i=1,100 do a[i.."+"] = true end
for i=1,100 do a[i.."+"] = nil end
-- fill hash part with numeric indices testing size operator
for i=1,100 do
  a[i] = true
  assert(#a == i)
end


-- test size operation on empty tables
assert(#{} == 0)
assert(#{nil} == 0)
assert(#{nil, nil} == 0)
assert(#{nil, nil, nil} == 0)
assert(#{nil, nil, nil, nil} == 0)
print'+'


local nofind = {}

a,b,c = 1,2,3
a,b,c = nil

local function find (name)
  local n,v
  while 1 do
    n,v = next(_G, n)
    if not n then return nofind end
    assert(v ~= nil)
    if n == name then return v end
  end
end

local function find1 (name)
  for n,v in pairs(_G) do
    if n==name then return v end
  end
  return nil  -- not found
end

do   -- create 100 new global variables
  for i=1,100 do _G[i] = i end
end



assert(print==find("print") and print == find1("print"))
assert(_G["print"]==find("print"))
assert(assert==find1("assert"))
assert(nofind==find("return"))
assert(not find1("return"))
_G["ret" .. "urn"] = nil
assert(nofind==find("return"))
_G["xxx"] = 1
assert(xxx==find("xxx"))
print('+')


-- remove those 100 new global variables
for i=1,100 do _G[i] = nil end

do   -- clear global table
  local a = {}
  local preserve = {io = 1, string = 1, table = 1}
  for n,v in pairs(_G) do a[n]=v end
  for n,v in pairs(a) do
    if not preserve[n] and type(v) ~= "function" and
       not string.find(n, "^[%u_]") then
     _G[n] = nil
    end
    collectgarbage()
  end
end

print'+'


assert(#{} == 0)
assert(#{[-1] = 2} == 0)
assert(#{1,2,3,nil,nil} == 3)
for i=0,40 do
  local a = {}
  for j=1,i do a[j]=j end
  assert(#(a) == i)
end


assert(table.maxn{} == 0)
assert(table.maxn{["1000"] = true} == 0)
assert(table.maxn{["1000"] = true, [24.5] = 3} == 24.5)
assert(table.maxn{[1000] = true} == 1000)
print("+")


-- erasing values
local t = {[{1}] = 1, [{2}] = 2, [string.rep("x ", 4)] = 3,
           [100.3] = 4, [4] = 5}

local n = 0
for k, v in pairs( t ) do
  n = n+1
  assert(t[k] == v)
  t[k] = nil
  collectgarbage()
  assert(t[k] == nil)
end
assert(n == 5)


local function test (a)
  table.insert(a, 10); table.insert(a, 2, 20);
  table.insert(a, 1, -1); table.insert(a, 40);
  table.insert(a, #(a)+1, 50)
  table.insert(a, 2, -2)
  assert(table.remove(a,1) == -1)
  assert(table.remove(a,1) == -2)
  assert(table.remove(a,1) == 10)
  assert(table.remove(a,1) == 20)
  assert(table.remove(a,1) == 40)
  assert(table.remove(a,1) == 50)
  assert(table.remove(a,1) == nil)
end

a = {n=0, [-7] = "ban"}
test(a)
assert(a.n == 0 and a[-7] == "ban")

a = {[-7] = "ban"};
test(a)
assert(a.n == nil and #(a) == 0 and a[-7] == "ban")


table.insert(a, 1, 10); table.insert(a, 1, 20); table.insert(a, 1, -1)
assert(table.remove(a) == 10)
assert(table.remove(a) == 20)
assert(table.remove(a) == -1)

a = {'c', 'd'}
table.insert(a, 3, 'a')
table.insert(a, 'b')
assert(table.remove(a, 1) == 'c')
assert(table.remove(a, 1) == 'd')
assert(table.remove(a, 1) == 'a')
assert(table.remove(a, 1) == 'b')
assert(#(a) == 0 and a.n == nil)
print("+")

a = {}
for i=1,1000 do
  a[i] = i; a[i-1] = nil
end
assert(next(a,nil) == 1000 and next(a,1000) == nil)

assert(next({}) == nil)
assert(next({}, nil) == nil)

for a,b in pairs{} do error"not here" end
for i=1,0 do error'not here' end
for i=0,1,-1 do error'not here' end
a = nil; for i=1,1 do assert(not a); a=1 end; assert(a)
a = nil; for i=1,1,-1 do assert(not a); a=1 end; assert(a)

a = 0; for i=0, 1, 0.1 do a=a+1 end; assert(a==10)
-- precision problems
a = 0; for i=1, 0, -0.01 do a=a+1 end; assert(a==101)
a = 0; for i=0, 0.999999999, 0.1 do a=a+1 end; assert(a==10)
a = 0; for i=1, 1, 1 do a=a+1 end; assert(a==1)
a = 0; for i=1, 0.99999, 1 do a=a+1 end; assert(a==0)
a = 0; for i=99999, 1e5, -1 do a=a+1 end; assert(a==0)
a = 0; for i=1, 0.99999, -1 do a=a+1 end; assert(a==1)

-- conversion
a = 0; for i="10","1","-2" do a=a+1 end; assert(a==5)


collectgarbage()


-- testing generic 'for'

local function f (n, p)
  local t = {}; for i=1,p do t[i] = i*10 end
  return function (_,n)
           if n > 0 then
             n = n-1
             return n, unpack(t)
           end
         end, nil, n
end

local x = 0
for n,a,b,c,d in f(5,3) do
  x = x+1
  assert(a == 10 and b == 20 and c == 30 and d == nil)
end
assert(x == 5)

print("  OK")
