static const char* gc="print('testing garbage collection')\n\
collectgarbage()\n\
_G[\"while\"] = 234\n\
limit = 5000\n\
contCreate = 0\n\
print('+')\n\
while contCreate <= limit do\n\
  local a = {}; a = nil\n\
  contCreate = contCreate+1\n\
end\n\
a = \"a\"\n\
contCreate = 0\n\
print('+')\n\
while contCreate <= limit do\n\
  a = contCreate .. \"b\";\n\
  a = string.gsub(a, '(%d%d*)', string.upper)\n\
  a = \"a\"\n\
  contCreate = contCreate+1\n\
end\n\
contCreate = 0\n\
a = {}\n\
print('+')\n\
function a:test ()\n\
  while contCreate <= limit do\n\
    loadstring(string.format(\"function temp(a) return 'a%d' end\", contCreate))()\n\
    assert(temp() == string.format('a%d', contCreate))\n\
    contCreate = contCreate+1\n\
  end\n\
end\n\
a:test()\n\
do local f = function () end end\n\
print(\"+\")\n\
prog = [[\n\
do\n\
  a = 10;\n\
  function foo(x,y)\n\
    a = sin(a+0.456-0.23e-12);\n\
    return function (z) return sin(%x+z) end\n\
  end\n\
  local x = function (w) a=a+w; end\n\
end\n\
]]\n\
do\n\
  local step = 1\n\
  if rawget(_G, \"_soft\") then step = 13 end\n\
  for i=1, string.len(prog), step do\n\
    for j=i, string.len(prog), step do\n\
      pcall(loadstring(string.sub(prog, i, j)))\n\
    end\n\
  end\n\
end\n\
assert(_G[\"while\"] == 234)\n\
local bytes = collectgarbage( \"count\" )\n\
while 1 do\n\
  local nbytes = collectgarbage( \"count\" )\n\
  if nbytes < bytes then break end   -- run until gc\n\
  bytes = nbytes\n\
  a = {}\n\
end\n\
local function dosteps (siz)\n\
  collectgarbage()\n\
  collectgarbage\"stop\"\n\
  local a = {}\n\
  for i=1,100 do a[i] = {{}}; local b = {} end\n\
  local x = collectgarbage( \"count\" )\n\
  local i = 0\n\
  repeat\n\
    i = i+1\n\
  until collectgarbage(\"step\", siz)\n\
  assert(collectgarbage( \"count\" ) < x)\n\
  return i\n\
end\n\
print(\"+\")\n\
assert(dosteps(0) > 10)\n\
assert(dosteps(6) < dosteps(2))\n\
assert(dosteps(10000) == 1)\n\
assert(collectgarbage(\"step\", 1000000) == true)\n\
assert(collectgarbage(\"step\", 1000000))\n\
lim = 15\n\
a = {}\n\
for i=1,lim do a[{}] = i end\n\
b = {}\n\
for k,v in pairs(a) do b[k]=v end\n\
for n in pairs(b) do\n\
  a[n] = nil\n\
  assert(type(n) == 'table' and next(n) == nil)\n\
  collectgarbage()\n\
end\n\
b = nil\n\
collectgarbage()\n\
for n in pairs(a) do error'cannot be here' end\n\
for i=1,lim do a[i] = i end\n\
for i=1,lim do assert(a[i] == i) end\n\
print('+')\n\
a = {}; setmetatable(a, {__mode = 'k'});\n\
for i=1,lim do a[{}] = i end\n\
for i=1,lim do local t={}; a[t]=t end\n\
for i=1,lim do a[i] = i end\n\
for i=1,lim do local s=string.rep('@', i); a[s] = s..'#' end\n\
collectgarbage()\n\
a = {}; setmetatable(a, {__mode = 'v'});\n\
a[1] = string.rep('b', 21)\n\
collectgarbage()\n\
assert(a[1])   -- strings are *values*\n\
a[1] = nil\n\
for i=1,lim do a[i] = {} end\n\
for i=1,lim do a[i..'x'] = {} end\n\
for i=1,lim do local t={}; a[t]=t end\n\
for i=1,lim do a[i+lim]=i..'x' end\n\
collectgarbage()\n\
local i = 0\n\
for k,v in pairs(a) do assert(k==v or k-lim..'x' == v); i=i+1 end\n\
assert(i == 2*lim)\n\
print('+')\n\
a = {}; setmetatable(a, {__mode = 'vk'});\n\
local x, y, z = {}, {}, {}\n\
a[1], a[2], a[3] = x, y, z\n\
a[string.rep('$', 11)] = string.rep('$', 11)\n\
for i=4,lim do a[i] = {} end\n\
for i=1,lim do a[{}] = i end\n\
for i=1,lim do local t={}; a[t]=t end\n\
collectgarbage()\n\
assert(next(a) ~= nil)\n\
local i = 0\n\
for k,v in pairs(a) do\n\
  assert((k == 1 and v == x) or\n\
         (k == 2 and v == y) or\n\
         (k == 3 and v == z) or k==v);\n\
  i = i+1\n\
end\n\
assert(i == 4)\n\
x,y,z=nil\n\
collectgarbage()\n\
assert(next(a) == string.rep('$', 11))\n\
print('  OK')";
