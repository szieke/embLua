static const char* locals="print('testing local variables plus some extra stuff')\n\
do\n\
  local i = 10\n\
  do local i = 100; assert(i==100) end\n\
  do local i = 1000; assert(i==1000) end\n\
  assert(i == 10)\n\
  if i ~= 10 then\n\
    local i = 20\n\
  else\n\
    local i = 30\n\
    assert(i == 30)\n\
  end\n\
end\n\
f = nil\n\
local f\n\
x = 1\n\
a = nil\n\
loadstring('local a = {}')()\n\
assert(type(a) ~= 'table')\n\
function f (a)\n\
  local _1, _2, _3, _4, _5\n\
  local _6, _7, _8, _9, _10\n\
  local x = 3\n\
  local b = a\n\
  local c,d = a,b\n\
  if (d == b) then\n\
    local x = 'q'\n\
    x = b\n\
    assert(x == 2)\n\
  else\n\
    assert(nil)\n\
  end\n\
  assert(x == 3)\n\
  local f = 10\n\
end\n\
local b=10\n\
local a; repeat local b; a,b=1,2; assert(a+1==b); until a+b==3\n\
assert(x == 1)\n\
f(2)\n\
assert(type(f) == 'function')\n\
local a\n\
local p = 4\n\
for i=2,30 do\n\
  for j=-3,3 do\n\
    assert(loadstring(string.format([[local a=%s;a=a+\n\
                                            %s;\n\
                                      assert(a\n\
                                      ==2^%s)]], j, p-j, i))) ()\n\
    assert(loadstring(string.format([[local a=%s;\n\
                                      a=a-%s;\n\
                                      assert(a==-2^%s)]], -j, p-j, i))) ()\n\
    assert(loadstring(string.format([[local a,b=0,%s;\n\
                                      a=b-%s;\n\
                                      assert(a==-2^%s)]], -j, p-j, i))) ()\n\
  end\n\
  p =2*p\n\
end\n\
print'+'\n\
if rawget(_G, \"querytab\") then\n\
  -- testing clearing of dead elements from tables\n\
  collectgarbage(\"stop\")   -- stop GC\n\
  local a = {[{}] = 4, [3] = 0, alo = 1,\n\
             a1234567890123456789012345678901234567890 = 10}\n\
  local t = querytab(a)\n\
  for k,_ in pairs(a) do a[k] = nil end\n\
  collectgarbage()   -- restore GC and collect dead fiels in `a'\n\
  for i=0,t-1 do\n\
    local k = querytab(a, i)\n\
    assert(k == nil or type(k) == 'number' or k == 'alo')\n\
  end\n\
end\n\
print('  OK')\n\
return 5,f";
