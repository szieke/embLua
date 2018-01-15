static const char* helperScript="local helperScriptCounter = 0\n\
function includedScript ()\n\
	print(\"print from helperScript: \".. tostring(helperScriptCounter) .. \"\\n\")\n\
	helperScriptCounter = helperScriptCounter+1\n\
end";
