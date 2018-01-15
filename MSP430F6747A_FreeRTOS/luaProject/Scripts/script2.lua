print('\nscript2: ' .. tostring(uc.counter()) ..'\n')

--Call a function from an other script (helperScript.lua).
includedScript()

--Print the heap usage.
print('heap usage: ' .. tostring(collectgarbage('count')*1024) .. '\n')
