local helperScriptCounter = 0
function includedScript ()
	print("print from helperScript: " .. tostring(helperScriptCounter) .. "\n")
	helperScriptCounter = helperScriptCounter+1
end