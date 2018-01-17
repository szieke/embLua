static const char* script2="print('\\nscript2: ' .. tostring(uc.counter()) ..'\\n')\n\
includedScript()\n\
print('sin(10): ' .. math.sin(10) .. '\\n')\n\
print('heap usage: ' .. tostring(collectgarbage('count')*1024) .. '\\n')";
