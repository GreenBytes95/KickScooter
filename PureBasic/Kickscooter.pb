IncludeFile "Module\Registry.pbi"
IncludeFile "Module\ST-LINK.pb"
IncludeFile "Module\dataPackage.pb"
IncludeFile "Module\GUI.pb"

GUI::Init()

Repeat
  GUI::Events()
  Delay(1)
ForEver


; IDE Options = PureBasic 5.62 (Windows - x86)
; CursorPosition = 12
; EnableXP
; UseIcon = Image\favicon.ico
; Executable = ..\bin\KickScooter.exe
; CompileSourceDirectory