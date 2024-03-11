' Resources consulted:
' http://bioinformatica.uab.cat/base/base3.asp?sitio=vbscript&anar=commen
' https://stackoverflow.com/questions/17660117/how-do-i-rename-a-file-using-vbscript
' https://devblogs.microsoft.com/scripting/how-can-i-get-a-list-of-all-the-files-in-a-folder-and-its-subfolders/
' https://www.guru99.com/vbscript-looping.html


Set oFSO = WScript.CreateObject("Scripting.FileSystemObject")

Set files = oFSO.GetFolder(".\frames").Files

i=0
For Each oFile in files
	i_str=i
	If (i < 10) Then
		i_str="00" & i
	ElseIf (i < 100) Then
		i_str="0" & i
	End If
	oFSO.MoveFile "frames\" & oFile.Name, "frames\frame" & i_str & ".gif"
	i=i+1
Next