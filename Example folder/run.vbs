' Resources consulted:
' https://superuser.com/questions/140047/how-to-run-a-batch-file-without-launching-a-command-window
' https://stackoverflow.com/questions/16138831/getting-current-directory-in-vbscript
' https://stackoverflow.com/questions/2942554/adding-quotes-to-a-string-in-vbscript

'Insert path for your WallPaper Animator executable file here'
strExe = """C:\Your\Path\WallPaperAnimator.exe"" "
strArgs = """" & CreateObject("Scripting.FileSystemObject").GetParentFolderName(Wscript.ScriptFullName) & """"

CreateObject("Wscript.Shell").Run strExe & strArgs, 0, true