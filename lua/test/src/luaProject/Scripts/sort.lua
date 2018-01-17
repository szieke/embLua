print"testing sort"


function check (a, f)
  f = f or function (x,y) return x<y end;
  for n=#a,2,-1 do
    assert(not f(a[n], a[n-1]))
  end
end

a = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep",
     "Oct", "Nov", "Dec"}

table.sort(a)
check(a)

limit = 300
if rawget(_G, "_soft") then limit = 300 end

a = {}
for i=1,limit do
  a[i] = 3474747 / i
end

table.sort(a)
check(a)

table.sort(a)
check(a)

a = {}
for i=1,limit do
  a[i] = 7847589 / i
end

i=0
table.sort(a, function(x,y) i=i+1; return y<x end)
check(a, function(x,y) return y<x end)


table.sort{}  -- empty array

for i=1,limit do a[i] = false end
table.sort(a, function(x,y) return nil end)
check(a, function(x,y) return nil end)

for i,v in pairs(a) do assert(not v or i=='n' and v==limit) end

a = {"álo", "\0first :-)", "alo", "then this one", "45", "and a new"}
table.sort(a)
check(a)

table.sort(a, function (x, y)
          loadstring(string.format("a[%q] = ''", x))()
          collectgarbage()
          return x<y
        end)


tt = {__lt = function (a,b) return a.val < b.val end}
a = {}
for i=1,10 do  a[i] = {val=455358 / i}; setmetatable(a[i], tt); end
table.sort(a)
check(a, tt.__lt)
check(a)

print"  OK"
