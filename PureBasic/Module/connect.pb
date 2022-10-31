DeclareModule connect
  
  #CONNECT_ID                 = 0
  
  #CONNECT_HEADER_SEND        = $F0
  #CONNECT_HEADER_RECV        = $F1
  #CONNECT_HEADER_SIZE        = 12
  
  #CONNECT_INSTRUCTION_ADC    = $01
  
  Structure cPack_t
    pdata.a[#CONNECT_HEADER_SIZE + 1]
  EndStructure
  
  Structure connectPackage_t
    header.a
    pack.cPack_t
  EndStructure
  
  Structure recvPackage_t
    ADC_PINVBAT.w
    ADC_CURRENTDC.w
    ADC_PIN_THROTTLE.w
    ADC_PIN_BRAKE.w
    ADC_PIN_BRAKEIO.w
  EndStructure
  
  Global connectPackage.connectPackage_t
  Global recvPackage.recvPackage_t
  
  Global dataMemory.l = AllocateMemory(1024)
  
  Declare.l open(COM.l)
  Declare.l send()
  Declare.l ADC()
  
  Declare.w getPINVBAT()
  
  Declare.a CRC(pdata.l, Count)
  
EndDeclareModule

Module connect
  
  Procedure.l open(COM.l)
    If OpenSerialPort(#CONNECT_ID, "COM" + Str(COM), 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
      ProcedureReturn 1
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.l send()
    connectPackage\header = #CONNECT_HEADER_SEND
    connectPackage\pack\pdata[#CONNECT_HEADER_SIZE-1]  = CRC(@connectPackage\pack\pdata[0], #CONNECT_HEADER_SIZE - 2)
    WriteSerialPortData(#CONNECT_ID, @connectPackage, SizeOf(connectPackage_t) - 1)
    While AvailableSerialPortInput(#CONNECT_ID) <= 0
      Delay(5)
    Wend
    If AvailableSerialPortInput(#CONNECT_ID) > 0
      ProcedureReturn 1
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.l ADC()
    connectPackage\pack\pdata[0] = #CONNECT_INSTRUCTION_ADC
    If send()
      ReadSerialPortData(#CONNECT_ID, dataMemory, AvailableSerialPortInput(#CONNECT_ID))
      recvPackage\ADC_PINVBAT       = PeekW(dataMemory + 1 + 1)
      recvPackage\ADC_CURRENTDC     = PeekW(dataMemory + 1 + 1 + 2)
      recvPackage\ADC_PIN_THROTTLE  = PeekW(dataMemory + 1 + 1 + 2 + 2)
      recvPackage\ADC_PIN_BRAKE     = PeekW(dataMemory + 1 + 1 + 2 + 2 + 2)
      recvPackage\ADC_PIN_BRAKEIO   = PeekA(dataMemory + 1 + 1 + 2 + 2 + 2 + 2)
    EndIf
  EndProcedure
  
  Procedure.w analogRead(PIN_VBAT)
    ProcedureReturn recvPackage\ADC_PINVBAT
  EndProcedure
  
  Procedure.a CRC(pdata.l, Count)
    Protected CRC.a, Byte.a, i, x
    
      For i=0 To Count - 1
        Byte=PeekA(pdata + i)
        For x=1 To 8
          If (Byte ! CRC) & 1
            CRC = ((CRC!$18)>>1)|$80
          Else
            CRC >> 1
          EndIf
          Byte>>1
        Next x
      Next i
    ProcedureReturn CRC
  EndProcedure
  
EndModule
; IDE Options = PureBasic 5.62 (Windows - x86)
; CursorPosition = 76
; Folding = --
; EnableXP