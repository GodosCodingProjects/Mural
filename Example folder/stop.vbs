' Resources consulted:
' https://stackoverflow.com/questions/1790340/how-to-terminate-a-process-in-vbscript

Set oShell = CreateObject ("Wscript.Shell")
oShell.Run "taskkill /f /im WallPaperAnimator.exe", , true