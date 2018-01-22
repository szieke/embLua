static const char* nextvar="print('testing tables, next, and for')\n\
local a = {}\n\
for i=1,100 do a[i..\"+\"] = true end\n\
for i=1,100 do a[i..\"+\"] = nil end\n\
for i=1,100 do\n\
  a[i] = true\n\
  assert(#a == i)\n\
end\n\
assert(#{} == 0)\n\
assert(#{nil} == 0)\n\
assert(#{nil, nil} == 0)\n\
assert(#{nil, nil, nil} == 0)\n\
assert(#{nil, nil, nil, nil} == 0)\n\
print'+'\n\
local nofind = {}\n\
a,b,c = 1,2,3\n\
a,b,c = nil\n\
local function find (name)\n\
  local n,v\n\
  while 1 do\n\
    n,v = next(_G, n)\n\
    if not n then return nofind end\n\
    assert(v ~= nil)\n\
    if n == name then return v end\n\
  end\n\
end\n\
local function find1 (name)\n\
  for n,v in pairs(_G) do\n\
    if n==name then return v end\n\
  end\n\
  return nil  -- not found\n\
end\n\
do   -- create 100 new global variables\n\
  for i=1,100 do _G[i] = i end\n\
end\n\
assert(print==find(\"print\") and print == find1(\"print\"))\n\
assert(_G[\"print\"]==find(\"print\"))\n\
assert(assert==find1(\"assert\"))\n\
assert(nofind==find(\"return\"))\n\
assert(not find1(\"return\"))\n\
_G[\"ret\" .. \"urn\"] = nil\n\
assert(nofind==find(\"return\"))\n\
_G[\"xxx\"] = 1\n\
assert(xxx==find(\"xxx\"))\n\
print('+')\n\
for i=1,100 do _G[i] = nil end\n\
do   -- clear global table\n\
  local a = {}\n\
  local preserve = {io = 1, string = 1, table = 1}\n\
  for n,v in pairs(_G) do a[n]=v end\n\
  for n,v in pairs(a) do\n\
    if not preserve[n] and type(v) ~= \"function\" and\n\
       not string.find(n, \"^[%u_]\") then\n\
     _G[n] = nil\n\
    end\n\
    collectgarbage()\n\
  end\n\
end\n\
print'+'\n\
assert(#{} == 0)\n\
assert(#{[-1] = 2} == 0)\n\
assert(#{1,2,3,nil,nil} == 3)\n\
for i=0,40 do\n\
  local a = {}\n\
  for j=1,i do a[j]=j end\n\
  assert(#(a) == i)\n\
end\n\
print(\"+\")\n\
local t = {[{1}] = 1, [{2}] = 2, [string.rep(\"x \", 4)] = 3,\n\
           [100.3] = 4, [4] = 5}\n\
local n = 0\n\
for k, v in pairs( t ) do\n\
  n = n+1\n\
  assert(t[k] == v)\n\
  t[k] = nil\n\
  collectgarbage()\n\
  assert(t[k] == nil)\n\
end\n\
assert(n == 5)\n\
local function test (a)\n\
  table.insert(a, 10); table.insert(a, 2, 20);\n\
  table.insert(a, 1, -1); table.insert(a, 40);\n\
  table.insert(a, #(a)+1, 50)\n\
  table.insert(a, 2, -2)\n\
  assert(table.remove(a,1) == -1)\n\
  assert(table.remove(a,1) == -2)\n\
  assert(table.remove(a,1) == 10)\n\
  assert(table.remove(a,1) == 20)\n\
  assert(table.remove(a,1) == 40)\n\
  assert(table.remove(a,1) == 50)\n\
  assert(table.remove(a,1) == nil)\n\
end\n\
a = {n=0, [-7] = \"ban\"}\n\
test(a)\n\
assert(a.n == 0 and a[-7] == \"ban\")\n\
a = {[-7] = \"ban\"};\n\
test(a)\n\
assert(a.n == nil and #(a) == 0 and a[-7] == \"ban\")\n\
table.insert(a, 1, 10); table.insert(a, 1, 20); table.insert(a, 1, -1)\n\
assert(table.remove(a) == 10)\n\
assert(table.remove(a) == 20)\n\
assert(table.remove(a) == -1)\n\
a = {'c', 'd'}\n\
table.insert(a, 3, 'a')\n\
table.insert(a, 'b')\n\
assert(table.remove(a, 1) == 'c')\n\
assert(table.remove(a, 1) == 'd')\n\
assert(table.remove(a, 1) == 'a')\n\
assert(table.remove(a, 1) == 'b')\n\
assert(#(a) == 0 and a.n == nil)\n\
print(\"+\")\n\
a = {}\n\
for i=1,1000 do\n\
  a[i] = i; a[i-1] = nil\n\
end\n\
assert(next(a,nil) == 1000 and next(a,1000) == nil)\n\
assert(next({}) == nil)\n\
assert(next({}, nil) == nil)\n\
for a,b in pairs{} do error\"not here\" end\n\
for i=1,0 do error'not here' end\n\
for i=0,1,-1 do error'not here' end\n\
a = nil; for i=1,1 do assert(not a); a=1 end; assert(a)\n\
a = nil; for i=1,1,-1 do assert(not a); a=1 end; assert(a)\n\
a = 0; for i=0, 1, 0.1 do a=a+1 end; assert(a==10)\n\
a = 0; for i=1, 0, -0.01 do a=a+1 end; assert(a==101)\n\
a = 0; for i=0, 0.999999999, 0.1 do a=a+1 end; assert(a==10)\n\
a = 0; for i=1, 1, 1 do a=a+1 end; assert(a==1)\n\
a = 0; for i=1, 0.99999, 1 do a=a+1 end; assert(a==0)\n\
a = 0; for i=99999, 1e5, -1 do a=a+1 end; assert(a==0)\n\
a = 0; for i=1, 0.99999, -1 do a=a+1 end; assert(a==1)\n\
a = 0; for i=\"10\",\"1\",\"-2\" do a=a+1 end; assert(a==5)\n\
collectgarbage()\n\
local function f (n, p)\n\
  local t = {}; for i=1,p do t[i] = i*10 end\n\
  return function (_,n)\n\
           if n > 0 then\n\
             n = n-1\n\
             return n, unpack(t)\n\
           end\n\
         end, nil, n\n\
end\n\
local x = 0\n\
for n,a,b,c,d in f(5,3) do\n\
  x = x+1\n\
  assert(a == 10 and b == 20 and c == 30 and d == nil)\n\
end\n\
assert(x == 5)\n\
print(\"  OK\")";
