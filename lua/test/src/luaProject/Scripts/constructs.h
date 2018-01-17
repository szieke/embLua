static const char* constructs="print \"testing syntax\"\n\
assert(2^3^2 == 2^(3^2));\n\
assert(2^3*4 == (2^3)*4);\n\
assert(2^-2 == 1/4 and -2^- -2 == - - -4);\n\
assert(not nil and 2 and not(2>3 or 3<2));\n\
assert(-3-1-5 == 0+0-9);\n\
assert(-2^2 == -4 and (-2)^2 == 4 and 2*2-3-1 == 0);\n\
assert(2*1+3/3 == 3 and 1+2 .. 3*1 == \"33\");\n\
assert(not(2+1 > 3*1) and \"a\"..\"b\" > \"a\");\n\
assert(not ((true or false) and nil))\n\
assert(      true or false  and nil)\n\
local a,b = 1,nil;\n\
assert(-(1 or 2) == -1 and (1 and 2)+(-1.25 or -4) == 0.75);\n\
x = ((b or a)+1 == 2 and (10 or a)+1 == 11); assert(x);\n\
x = (((2<3) or 1) == true and (2<3 and 4) == 4); assert(x);\n\
x,y=1,2;\n\
assert((x>y) and x or y == 2);\n\
x,y=2,1;\n\
assert((x>y) and x or y == 2);\n\
assert(1234567890 == tonumber('1234567890') and 1234567890+1 == 1234567891)\n\
repeat until 1; repeat until true;\n\
while false do end; while nil do end;\n\
do  -- test old bug (first name could not be an `upvalue')\n\
 local a; function f(x) x={a=1}; x={x=1}; x={G=1} end\n\
end\n\
function f (i)\n\
  if type(i) ~= 'number' then return i,'jojo'; end;\n\
  if i > 0 then return i, f(i-1); end;\n\
end\n\
x = {f(3), f(5), f(10);};\n\
assert(x[1] == 3 and x[2] == 5 and x[3] == 10 and x[4] == 9 and x[12] == 1);\n\
assert(x[nil] == nil)\n\
x = {f'alo', f'xixi', nil};\n\
assert(x[1] == 'alo' and x[2] == 'xixi' and x[3] == nil);\n\
x = {f'alo'..'xixi'};\n\
assert(x[1] == 'aloxixi')\n\
x = {f{}}\n\
assert(x[2] == 'jojo' and type(x[1]) == 'table')\n\
local f = function (i)\n\
  if i < 10 then return 'a';\n\
  elseif i < 20 then return 'b';\n\
  elseif i < 30 then return 'c';\n\
  end;\n\
end\n\
assert(f(3) == 'a' and f(12) == 'b' and f(26) == 'c' and f(100) == nil)\n\
for i=1,1000 do break; end;\n\
n=100;\n\
i=3;\n\
t = {};\n\
a=nil\n\
while not a do\n\
  a=0; for i=1,n do for i=i,1,-1 do a=a+1; t[i]=1; end; end;\n\
end\n\
assert(a == n*(n+1)/2 and i==3);\n\
assert(t[1] and t[n] and not t[0] and not t[n+1])\n\
function f(b)\n\
  local x = 1;\n\
  repeat\n\
    local a;\n\
    if b==1 then local b=1; x=10; break\n\
    elseif b==2 then x=20; break;\n\
    elseif b==3 then x=30;\n\
    else local a,b,c,d=2; x=x+1;\n\
    end\n\
  until x>=12;\n\
  return x;\n\
end;\n\
assert(f(1) == 10 and f(2) == 20 and f(3) == 30 and f(4)==12)\n\
local f = function (i)\n\
  if i < 10 then return 'a'\n\
  elseif i < 20 then return 'b'\n\
  elseif i < 30 then return 'c'\n\
  else return 8\n\
  end\n\
end\n\
assert(f(3) == 'a' and f(12) == 'b' and f(26) == 'c' and f(100) == 8)\n\
local a, b = nil, 23\n\
x = {f(100)*2+3 or a, a or b+2}\n\
assert(x[1] == 19 and x[2] == 25)\n\
x = {f=2+3 or a, a = b+2}\n\
assert(x.f == 5 and x.a == 25)\n\
a={y=1}\n\
x = {a.y}\n\
assert(x[1] == 1)\n\
function f(i)\n\
  while 1 do\n\
    if i>0 then i=i-1;\n\
    else return; end;\n\
  end;\n\
end;\n\
function g(i)\n\
  while 1 do\n\
    if i>0 then i=i-1\n\
    else return end\n\
  end\n\
end\n\
f(10); g(10);\n\
do\n\
  function f () return 1,2,3; end\n\
  local a, b, c = f();\n\
  assert(a==1 and b==2 and c==3)\n\
  a, b, c = (f());\n\
  assert(a==1 and b==nil and c==nil)\n\
end\n\
local a,b = 3 and f();\n\
assert(a==1 and b==nil)\n\
function g() f(); return; end;\n\
assert(g() == nil)\n\
function g() return nil or f() end\n\
a,b = g()\n\
assert(a==1 and b==nil)\n\
print'+';\n\
f = [[\n\
return function ( a , b , c , d , e )\n\
  local x = a >= b or c or ( d and e ) or nil\n\
  return x\n\
end , { a = 1 , b = 2 >= 1 , } or { 1 };\n\
]]\n\
f = string.gsub(f, \"%s+\", \"\\n\");   -- force a SETLINE between opcodes\n\
f,a = loadstring(f)();\n\
assert(a.a == 1 and a.b)\n\
function g (a,b,c,d,e)\n\
  if not (a>=b or c or d and e or nil) then return 0; else return 1; end;\n\
end\n\
function h (a,b,c,d,e)\n\
  while (a>=b or c or (d and e) or nil) do return 1; end;\n\
  return 0;\n\
end;\n\
assert(f(2,1) == true and g(2,1) == 1 and h(2,1) == 1)\n\
assert(f(1,2,'a') == 'a' and g(1,2,'a') == 1 and h(1,2,'a') == 1)\n\
assert(f(1,2,'a')\n\
~=          -- force SETLINE before nil\n\
nil, \"\")\n\
assert(f(1,2,'a') == 'a' and g(1,2,'a') == 1 and h(1,2,'a') == 1)\n\
assert(f(1,2,nil,1,'x') == 'x' and g(1,2,nil,1,'x') == 1 and\n\
                                   h(1,2,nil,1,'x') == 1)\n\
assert(f(1,2,nil,nil,'x') == nil and g(1,2,nil,nil,'x') == 0 and\n\
                                     h(1,2,nil,nil,'x') == 0)\n\
assert(f(1,2,nil,1,nil) == nil and g(1,2,nil,1,nil) == 0 and\n\
                                   h(1,2,nil,1,nil) == 0)\n\
assert(1 and 2<3 == true and 2<3 and 'a'<'b' == true)\n\
x = 2<3 and not 3; assert(x==false)\n\
x = 2<1 or (2>1 and 'a'); assert(x=='a')\n\
do\n\
  local a; if nil then a=1; else a=2; end;    -- this nil comes as PUSHNIL 2\n\
  assert(a==2)\n\
end\n\
function F(a)\n\
  return a,2,3\n\
end\n\
a,b = F(1)~=nil; assert(a == true and b == nil);\n\
a,b = F(nil)==nil; assert(a == true and b == nil)\n\
print'  OK'";
