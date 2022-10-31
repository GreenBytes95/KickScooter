
DeclareModule stlink
  
  Declare.l flashReadBuffer(adress.l, pbuf.l, len.l, timeout.l)
  Declare.l flashWriteBuffer(adress.l, pbuf.l, len.l, timeout.l)
  
EndDeclareModule

Module stlink
  
  Procedure.l flashReadBuffer(adress.l, pbuf.l, len.l, timeout.l)
    Protected pRUN = RunProgram("ST-LINK_CLI.exe", "-r8 " + Hex(adress) + " " + Hex(len) + " -HardRst", "", #PB_Program_Open | #PB_Program_Read | #PB_Program_Hide)
    Protected Output$ = "", sId.l = 1, icount.l = 0, timer.l = ElapsedMilliseconds()
    If IsProgram(pRUN)
      Repeat
        If AvailableProgramOutput(pRUN)
          Output$ = ReadProgramString(pRUN)
          Debug Output$
          ReplaceString(Output$, #CR$, "")
          ReplaceString(Output$, #LF$, "")
          ReplaceString(Output$, #TAB$, "")
          If FindString(Output$, " : ") And Left(Output$, 2) = "0x"
            sId.l = 1
            Repeat
              sData$ = StringField(Right(Output$, Len(Output$) - 13),sId, "  ")
              If sData$ = ""
                Break
              Else
                If icount < len
                  PokeA(pbuf + icount, Val("$" + ReplaceString(sData$, " ", "")))
                  byteList$ = byteList$ + sData$
                  icount = icount + 1
                Else
                  Break 2
                EndIf
              EndIf
              sId = sId + 1
            ForEver
          EndIf
          If FindString(Output$, "reset is performed")
            Break
          EndIf
          timer.l = ElapsedMilliseconds()
        EndIf
        If (ElapsedMilliseconds() - timer) > (timeout * 1000)
          CloseProgram(pRUN)
          ProcedureReturn 0
        EndIf
      ForEver
      CloseProgram(pRUN)
      ProcedureReturn 1
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.l runWrite(param.s, timeout.l)
    Protected pRUN = RunProgram("ST-LINK_CLI.exe", param.s + " -HardRst", "", #PB_Program_Open | #PB_Program_Read | #PB_Program_Hide)
    Protected timer.l = ElapsedMilliseconds()
    Protected Output$
    If IsProgram(pRUN)
      Repeat
        If AvailableProgramOutput(pRUN)
          Output$ = ReadProgramString(pRUN)
          timer.l = ElapsedMilliseconds()
          Debug Output$
        EndIf
        If Not IsProgram(pRUN) Or FindString(Output$, "reset is performed")
          CloseProgram(pRUN)
          ProcedureReturn 1
        EndIf
        If (ElapsedMilliseconds() - timer) > (timeout * 1000)
          CloseProgram(pRUN)
          ProcedureReturn 0
        EndIf
      ForEver
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.l flashWriteBuffer(adress.l, pbuf.l, len.l, timeout.l)
    Protected parseText.s = " "
    Protected icount.l , i.l
    
    For i.l = 0 To len - 1
      parseText + "-w8 " + RSet(Hex(adress + i), 8, "0") + " " + RSet(Hex(PeekA(pbuf + i)), 2, "0") + " "
      icount + 1
      If icount > 10
        icount = 0
        If runWrite(parseText, timeout.l) = 0
          ProcedureReturn 0
        EndIf
        parseText = ""
      EndIf
    Next
    
    If parseText <> ""
       If runWrite(parseText, timeout.l) = 0
          ProcedureReturn 0
       EndIf
    EndIf
    
    ProcedureReturn 1
  EndProcedure
  
EndModule
; IDE Options = PureBasic 5.62 (Windows - x86)
; CursorPosition = 17
; FirstLine = 3
; Folding = -
; EnableXP
; CompileSourceDirectory