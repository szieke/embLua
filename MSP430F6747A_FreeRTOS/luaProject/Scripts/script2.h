static const char* script2="print('\\nscript2: ' .. tostring(uc.counter()) ..'\\n')\n\
includedScript()\n\
print('heap usage: ' .. tostring(collectgarbage('count')*1024) .. '\\n')";
