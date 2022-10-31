
#FLASH                  = "-f"
#COMPILE                = "-b"

#PROGRAM                = "C:\Keil_v5\UV4\UV4.exe"
#LOGFILE                = "C:\Keil_v5\UV4\log.log"
#NAMEPROJECT            = "KickScooter.uvprojx"
#COMMAND                = #COMPILE
#OUTPUTSTRINGFILE       = "console.log"

Declare.l PrintNpp(patchString.s, newLine.l)

Global exCode.l
Global Compiler.l
Global patchParam.s = ProgramParameter()

OpenConsole()

Compiler = RunProgram(#PROGRAM, #COMMAND + " " + Chr(34) + patchParam + Chr(34) + " -o " + Chr(34) + #LOGFILE + Chr(34) + " -j0", "", #PB_Program_Open)
If Compiler
  
  If #COMMAND = #COMPILE
    PrintNpp("Compile: ", #False)
  EndIf
  
  If #COMMAND = #FLASH
    PrintNpp("Flash: ", #False)
  EndIf
  
  PrintNpp(patchParam, #True)
  
  While ProgramRunning(Compiler)
    Delay(100)
  Wend
  exCode = ProgramExitCode(Compiler)
  CloseProgram(Compiler)
  
  DeleteFile(#OUTPUTSTRINGFILE)
  CopyFile(#LOGFILE, #OUTPUTSTRINGFILE)
  
  If ReadFile(0, #OUTPUTSTRINGFILE)
    While Eof(0) = 0
      PrintNpp(ReadString(0), #True)
    Wend
    CloseFile(0)
    CopyFile(GetPathPart(patchParam) + "\Objects\" + GetFilePart(patchParam, #PB_FileSystem_NoExtension) + ".hex", GetPathPart(patchParam) + "\Hex\" + GetFilePart(patchParam, #PB_FileSystem_NoExtension) + ".hex")
  Else
    PrintNpp("Failed log " + #OUTPUTSTRINGFILE, #True)
  EndIf
  
  PrintNpp("ExitCode: " + Str(exCode), #True)
  
Else
  PrintNpp("Failed Program " + #PROGRAM, #True)
EndIf

Procedure.l PrintNpp(patchString.s, newLine.l)
  Protected pOffset.l = 0
  Protected pLine.a = $0A
  For oi = 0 To MemoryStringLength(@patchString) - 1
    WriteConsoleData(@patchString + pOffset, 1)
    pOffset = pOffset + 2
  Next
  If newLine = #True
    WriteConsoleData(@pLine, 1)
  EndIf
EndProcedure

; IDE Options = PureBasic 5.62 (Windows - x86)
; ExecutableFormat = Console
; CursorPosition = 48
; FirstLine = 10
; Folding = -
; EnableXP
; EnableAdmin
; EnableOnError
; Executable = ..\bin\compile.exe
; CompileSourceDirectory