static const char* script1="function script1Function ()\n\
	print('\\nscript1: ' .. tostring(uc.counter()) ..'\\n')\n\
	\n\
	includedScript()\n\
	\n\
	local example = 'an example string'\n\
	for el in string.gmatch(example, '%S+') do\n\
		print(el .. ' ')\n\
	end\n\
	print('\\n')\n\
	\n\
	local result1, result2 = uc.func1('Testmessage')\n\
	print('result1: '.. tostring(result1) .. '  result2: '.. tostring(result2) .. '\\n')\n\
	\n\
	local array = {0,1,2,3,4}\n\
	local resultTable = uc.func2(array)\n\
	print('test print:', 'string', 3, '\\n')\n\
	print('resultTable1: '.. tostring(resultTable[0]) .. '  resultTable2: '.. tostring(resultTable[1]) .. '\\n')\n\
	\n\
	print('heap usage before collect: ' .. tostring(collectgarbage('count')*1024) .. '\\n')\n\
	collectgarbage('collect')\n\
	print('heap usage after collect: ' .. tostring(collectgarbage('count')*1024) .. '\\n')\n\
end\n\
for loopCounter=0,2 do \n\
	print(pcall(script1Function), '\\n')\n\
end";
