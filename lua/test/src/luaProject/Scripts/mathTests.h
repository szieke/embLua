static const char* mathTests="print(\"testing numbers and math lib\")\n\
do\n\
  local a,b,c = \"2\", \" 3e0 \", \" 10  \"\n\
  assert(a+b == 5 and -b == -3 and b+\"2\" == 5 and \"10\"-c == 0)\n\
  assert(type(a) == 'string' and type(b) == 'string' and type(c) == 'string')\n\
  assert(a == \"2\" and b == \" 3e0 \" and c == \" 10  \" and -c == -\"  10 \")\n\
  assert(c%a == 0 and a^b == 8)\n\
end\n\
do\n\
  local a,b = math.modf(3.5)\n\
  assert(a == 3 and b == 0.5)\n\
  assert(math.huge > 10e30)\n\
  assert(-math.huge < -10e30)\n\
end\n\
function f(...)\n\
  if select('#', ...) == 1 then\n\
    return (...)\n\
  else\n\
    return \"***\"\n\
  end\n\
end\n\
assert(tonumber{} == nil)\n\
assert(tonumber'+0.01' == 1/100 and tonumber'+.01' == 0.01 and\n\
       tonumber'.01' == 0.01    and tonumber'-1.' == -1 and\n\
       tonumber'+1.' == 1)\n\
assert(tonumber'+ 0.01' == nil and tonumber'+.e1' == nil and\n\
       tonumber'1e' == nil     and tonumber'1.0e+' == nil and\n\
       tonumber'.' == nil)\n\
assert(tonumber('-12') == -10-2)\n\
assert(tonumber('-1.2e2') == - - -120)\n\
assert(f(tonumber('1  a')) == nil)\n\
assert(f(tonumber('e1')) == nil)\n\
assert(f(tonumber('e  1')) == nil)\n\
assert(f(tonumber(' 3.4.5 ')) == nil)\n\
assert(f(tonumber('')) == nil)\n\
assert(f(tonumber('', 8)) == nil)\n\
assert(f(tonumber('  ')) == nil)\n\
assert(f(tonumber('  ', 9)) == nil)\n\
assert(f(tonumber('99', 8)) == nil)\n\
assert(tonumber('  1010  ', 2) == 10)\n\
assert(tonumber('10', 36) == 36)\n\
assert(tonumber('fFfa', 15) == nil)\n\
assert(100.0 == 1E2 and .01 == 1e-2)\n\
assert(1.1 == '1.'+'.1')\n\
function eq (a,b,limit)\n\
  if not limit then limit = 10E-10 end\n\
  return math.abs(a-b) <= limit\n\
end\n\
assert(0.1e-30 > 0.9E-31 and 0.9E30 < 0.1e31)\n\
assert(0.123456 > 0.123455)\n\
assert(not(1<1) and (1<2) and not(2<1))\n\
assert(not('a'<'a') and ('a'<'b') and not('b'<'a'))\n\
assert((1<=1) and (1<=2) and not(2<=1))\n\
assert(('a'<='a') and ('a'<='b') and not('b'<='a'))\n\
assert(not(1>1) and not(1>2) and (2>1))\n\
assert(not('a'>'a') and not('a'>'b') and ('b'>'a'))\n\
assert((1>=1) and not(1>=2) and (2>=1))\n\
assert(('a'>='a') and not('a'>='b') and ('b'>='a'))\n\
assert(-4%3 == 2)\n\
assert(4%-3 == -2)\n\
assert(math.pi - math.pi % 1 == 3)\n\
assert(math.pi - math.pi % 0.001 == 3.141)\n\
local function testbit(a, n)\n\
  return a/2^n % 2 >= 1\n\
end\n\
assert(eq(math.tan(math.pi/4), 1))\n\
assert(eq(math.atan(1), math.pi/4) and eq(math.acos(0), math.pi/2) and\n\
       eq(math.asin(1), math.pi/2))\n\
assert(eq(math.deg(math.pi/2), 90) and eq(math.rad(90), math.pi/2))\n\
assert(math.abs(-10) == 10)\n\
assert(eq(math.atan(1,0), math.pi/2))\n\
assert(math.ceil(4.5) == 5.0)\n\
assert(math.floor(4.5) == 4.0)\n\
assert(math.fmod(10,3) == 1)\n\
assert(eq(math.sqrt(10)^2, 10))\n\
assert(tonumber(' 1.3e-2 ') == 1.3e-2)\n\
assert(tonumber(' -1.0000001 ') == -1.0000001)\n\
assert(8388609 + -8388609 == 0)\n\
assert(8388608 + -8388608 == 0)\n\
assert(8388607 + -8388607 == 0)\n\
if rawget(_G, \"_soft\") then return end\n\
print('+')\n\
do   -- testing NaN\n\
  local NaN = 10e500 - 10e400\n\
  assert(NaN ~= NaN)\n\
  assert(not (NaN < NaN))\n\
  assert(not (NaN <= NaN))\n\
  assert(not (NaN > NaN))\n\
  assert(not (NaN >= NaN))\n\
  assert(not (0 < NaN))\n\
  assert(not (NaN < 0))\n\
  local a = {}\n\
  assert(not pcall(function () a[NaN] = 1 end))\n\
  assert(a[NaN] == nil)\n\
  a[1] = 1\n\
  assert(not pcall(function () a[NaN] = 1 end))\n\
  assert(a[NaN] == nil)\n\
end\n\
a = nil\n\
local a,b = '10', '20'\n\
assert(a*b == 200 and a+b == 30 and a-b == -10 and a/b == 0.5 and -b == -20)\n\
assert(a == '10' and b == '20')\n\
math.randomseed(0)\n\
local i = 0\n\
local Max = 0\n\
local Min = 2\n\
repeat\n\
  local t = math.random()\n\
  Max = math.max(Max, t)\n\
  Min = math.min(Min, t)\n\
  i=i+1\n\
  flag = eq(Max, 1, 0.001) and eq(Min, 0, 0.001)\n\
until flag or i>10000\n\
assert(0 <= Min and Max<1)\n\
assert(flag);\n\
for i=1,10 do\n\
  local t = math.random(5)\n\
  assert(1 <= t and t <= 5)\n\
end\n\
i = 0\n\
Max = -200\n\
Min = 200\n\
repeat\n\
  local t = math.random(-10,0)\n\
  Max = math.max(Max, t)\n\
  Min = math.min(Min, t)\n\
  i=i+1\n\
  flag = (Max == 0 and Min == -10)\n\
until flag or i>10000\n\
assert(-10 <= Min and Max<=0)\n\
assert(flag);\n\
print('  OK')";
