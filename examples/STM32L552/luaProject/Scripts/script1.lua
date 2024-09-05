function script1Function ()
	print('\nscript1: ' .. tostring(uc.counter()) ..'\n')
	
	--Call a function from an other script (helperScript.lua).
	includedScript()
	
				 --Split a string and print the result.
	local example = 'an example string'
	for el in string.gmatch(example, '%S+') do
		print(el .. ' ')
	end
	print('\n')
	--[[ test muli line comment:
	    line 2 
	    line 3
	--]]
	
	--Call C-function func1 and print the result.
	local result1, result2 = uc.func1('Testmessage')
	print('result1: '.. tostring(result1) .. '  result2: '.. tostring(result2) .. '\n')
	
	--Call C-function func2 and print the result.
	local array = {0,1,2,3,4}
	local resultTable = uc.func2(array)
	print('test print:', 'string', 3, '\n')
	print('resultTable1: '.. tostring(resultTable[0]) .. '  resultTable2: '.. tostring(resultTable[1]) .. '\n')
	
	--Print the heap usage, collect garbage and then print the heap usage again.
	print('heap usage before collect: ' .. tostring(collectgarbage('count')*1024) .. '\n')
	collectgarbage('collect')
	print('heap usage after collect: ' .. tostring(collectgarbage('count')*1024) .. '\n')
	error({code=121})
end

for loopCounter=0,2 do 
	local status, err = pcall(script1Function)
	print(err.code .. '\\n')
end
