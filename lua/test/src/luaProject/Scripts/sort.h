static const char* sort="print\"testing sort\"\n\
function check (a, f)\n\
  f = f or function (x,y) return x<y end;\n\
  for n=#a,2,-1 do\n\
    assert(not f(a[n], a[n-1]))\n\
  end\n\
end\n\
a = {\"Jan\", \"Feb\", \"Mar\", \"Apr\", \"May\", \"Jun\", \"Jul\", \"Aug\", \"Sep\",\n\
     \"Oct\", \"Nov\", \"Dec\"}\n\
table.sort(a)\n\
check(a)\n\
limit = 300\n\
if rawget(_G, \"_soft\") then limit = 300 end\n\
a = {}\n\
for i=1,limit do\n\
  a[i] = math.random()\n\
end\n\
table.sort(a)\n\
check(a)\n\
table.sort(a)\n\
check(a)\n\
a = {}\n\
for i=1,limit do\n\
  a[i] = math.random()\n\
end\n\
i=0\n\
table.sort(a, function(x,y) i=i+1; return y<x end)\n\
check(a, function(x,y) return y<x end)\n\
table.sort{}  -- empty array\n\
for i=1,limit do a[i] = false end\n\
table.sort(a, function(x,y) return nil end)\n\
check(a, function(x,y) return nil end)\n\
for i,v in pairs(a) do assert(not v or i=='n' and v==limit) end\n\
a = {\"álo\", \"\\0first :-)\", \"alo\", \"then this one\", \"45\", \"and a new\"}\n\
table.sort(a)\n\
check(a)\n\
table.sort(a, function (x, y)\n\
          loadstring(string.format(\"a[%q] = ''\", x))()\n\
          collectgarbage()\n\
          return x<y\n\
        end)\n\
tt = {__lt = function (a,b) return a.val < b.val end}\n\
a = {}\n\
for i=1,10 do  a[i] = {val=math.random(100)}; setmetatable(a[i], tt); end\n\
table.sort(a)\n\
check(a, tt.__lt)\n\
check(a)\n\
print\"  OK\"";
