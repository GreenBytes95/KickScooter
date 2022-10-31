DeclareModule dataPackage
  
  #DATAP_CONNECT                  = 1
  
  #DATAP_HEADER_SEND              = $F0
  #DATAP_HEADER_RECV              = $F1
  #DATAP_HEADER_SIZE              = 12
  
  #DATAP_INSTRUCTION_ADC          = $01
  #DATAP_INSTRUCTION_DEV          = $02
  #DATAP_INSTRUCTION_BLDC         = $03
  #DATAP_INSTRUCTION_MOSFET       = $04
  #DATAP_INSTRUCTION_DEBUG        = $05
  #DATAP_INSTRUCTION_READ_CONFIG  = $06
  #DATAP_INSTRUCTION_WRITE_CONFIG = $07
  #DATAP_INSTRUCTION_ERASE_FLASH  = $08
  #DATAP_INSTRUCTION_TURN         = $09
  #DATAP_INSTRUCTION_CONTROL      = $0A
  #DATAP_INSTRUCTION_VERSION      = $0B
  #DATAP_INSTRUCTION_UPDATE_CONFIG= $0C
  
  #PIN_VBAT                       = $01
  #PIN_CURRENT_DC                 = $02
  #PIN_THROTTLE                   = $03
  #PIN_BRAKE                      = $04
  #PIN_HALL_A                     = $05
  #PIN_HALL_B                     = $06
  #PIN_HALL_C                     = $07
  
  #TIMER_BLDC_A                   = #PIN_HALL_A
  #TIMER_BLDC_B                   = #PIN_HALL_B
  #TIMER_BLDC_C                   = #PIN_HALL_C
  
  Structure dPack_t
    pdata.a[#DATAP_HEADER_SIZE + 1]
  EndStructure
  
  Structure dataPackage_t
    header.a
    pack.dPack_t
  EndStructure
  
  Structure recvPackageADC_t
    PIN_VBAT.w
    PIN_CURRENT_DC.w
    PIN_THROTTLE.w
    PIN_BRAKE.w
  EndStructure
  
  Structure recvPackageDEV_t
    devHall.a
    devCurrent.w
    devVoltage.w
    devThrottle.w
    devBrake.w
  EndStructure
  
  Structure recvPackageBLDC_t
    engineTurnover.w
    engineDegree.w
    engineCurrentPWM.w
    engineReverse.a
    PIN_HALL_A.a
    PIN_HALL_B.a
    PIN_HALL_C.a
  EndStructure
  
  Structure recvPackageMOSFET_t
    TIMER_BLDC_A.w
    TIMER_BLDC_B.w
    TIMER_BLDC_C.w
  EndStructure
  
  Structure recvPackageRCONFIG_t
    adress.l
    value.l
  EndStructure
  
  Structure recvControl_t
    current.w
    voltage.w
    turnover.w
    reverse.a
  EndStructure
  
  Structure recvPackageVERSION_t
    version.a
    flash.l
    flashMax.l
  EndStructure 
  
  Structure recvPackage_t
    ADC.recvPackageADC_t
    DEV.recvPackageDEV_t
    BLDC.recvPackageBLDC_t
    MOSFET.recvPackageMOSFET_t
    RCONFIG.recvPackageRCONFIG_t
    ;...
    VERSION.recvPackageVERSION_t
    ;...
    CONTROL.recvControl_t
  EndStructure
  
  Global dataPackage.dataPackage_t
  Global recvPackage.recvPackage_t
  Global dataMemory.l = AllocateMemory(1024)
  Global timeout.a
  
  Declare.l open(port.l, ptimeout.a)
  Declare.l close()
  Declare.l send()
  
  Declare.l ADC()
  Declare.l DEV()
  Declare.l BLDC()
  Declare.l MOSFET()
  
  Declare.l VERSION()
  
  Declare.w analogRead(pin.a)
  Declare.a digitalRead(pin.a)
  
  Declare.a devHall()
  Declare.w devCurrent()
  Declare.w devVoltage()
  Declare.w devThrottle()
  Declare.w devBrake()
  
  Declare.w engineTurnover()
  Declare.w engineDegree()
  Declare.w engineCurrentPWM()
  Declare.a engineReverse()
  
  Declare.w engineMosfet(timer.a)
  
  Declare.l flashRead(adress.l)
  Declare.l flashReadBuffer(adress.l, pbuf.l, len.l)
  
  Declare.l flashWrite(adress.l, pbuf.l)
  Declare.l flashWriteBuffer(adress.l, pbuf.l, len)
  
  Declare.l flashErase()
  
  Declare.l update()
  
  Declare.a boardVersion()
  Declare.l flashAdress()
  Declare.l flashMax()
  
  Declare.l controlRemote(throttle.w, brake.w, reverse.a, engine.a, priority.a)
  
  Declare.a CRC(pdata.l, Count)
  
EndDeclareModule

Module dataPackage
  
  Procedure.l open(port.l, ptimeout.a)
    If OpenSerialPort(#DATAP_CONNECT, "COM" + Str(port), 115200, #PB_SerialPort_NoParity, 8, 1, #PB_SerialPort_NoHandshake, 1024, 1024)
      Debug "Подключение вышло"
      timeout = ptimeout
      ProcedureReturn 1
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.l close()
    If IsSerialPort(#DATAP_CONNECT)
      CloseSerialPort(#DATAP_CONNECT)
    EndIf
  EndProcedure
  
  Procedure.l send()
    Protected time.l
    dataPackage\header = #DATAP_HEADER_SEND
    dataPackage\pack\pdata[#DATAP_HEADER_SIZE-1]  = CRC(@dataPackage\pack\pdata[0], #DATAP_HEADER_SIZE - 2)
    If Not IsSerialPort(#DATAP_CONNECT)
      ProcedureReturn 0
    EndIf
    WriteSerialPortData(#DATAP_CONNECT, @dataPackage, SizeOf(dataPackage_t) - 1)
    time.l = ElapsedMilliseconds()
    While AvailableSerialPortInput(#DATAP_CONNECT) <= 0
      Delay(5)
      If (ElapsedMilliseconds() - time) > (timeout * 1000)
        ProcedureReturn 0
      EndIf
    Wend
    Delay(5)
    If AvailableSerialPortInput(#DATAP_CONNECT) > 0
      ProcedureReturn 1
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.l ADC()
    dataPackage\pack\pdata[0] = #DATAP_INSTRUCTION_ADC
    If send()
      ReadSerialPortData(#DATAP_CONNECT, dataMemory, AvailableSerialPortInput(#DATAP_CONNECT))
      CopyMemory(dataMemory + 2, @recvPackage\ADC, SizeOf(recvPackageADC_t))
      ProcedureReturn 1
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.l DEV()
    dataPackage\pack\pdata[0] = #DATAP_INSTRUCTION_DEV
    If send()
      ReadSerialPortData(#DATAP_CONNECT, dataMemory, AvailableSerialPortInput(#DATAP_CONNECT))
      CopyMemory(dataMemory + 2, @recvPackage\DEV, SizeOf(recvPackageDEV_t))
      ProcedureReturn 1
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.l BLDC()
    dataPackage\pack\pdata[0] = #DATAP_INSTRUCTION_BLDC
    If send()
      ReadSerialPortData(#DATAP_CONNECT, dataMemory, AvailableSerialPortInput(#DATAP_CONNECT))
      CopyMemory(dataMemory + 2, @recvPackage\BLDC, SizeOf(recvPackageBLDC_t))
      ProcedureReturn 1
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.l MOSFET()
    dataPackage\pack\pdata[0] = #DATAP_INSTRUCTION_MOSFET
    If send()
      ReadSerialPortData(#DATAP_CONNECT, dataMemory, AvailableSerialPortInput(#DATAP_CONNECT))
      CopyMemory(dataMemory + 2, @recvPackage\MOSFET, SizeOf(recvPackageMOSFET_t))
      ProcedureReturn 1
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.l VERSION()
    dataPackage\pack\pdata[0] = #DATAP_INSTRUCTION_VERSION
    If send()
      ReadSerialPortData(#DATAP_CONNECT, dataMemory, AvailableSerialPortInput(#DATAP_CONNECT))
      CopyMemory(dataMemory + 2, @recvPackage\VERSION, SizeOf(recvPackageVERSION_t))
      ProcedureReturn 1
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.w analogRead(pin.a)
    If pin = #PIN_VBAT
      ProcedureReturn recvPackage\ADC\PIN_VBAT
    EndIf
    If pin = #PIN_CURRENT_DC
      ProcedureReturn recvPackage\ADC\PIN_CURRENT_DC
    EndIf
    If pin = #PIN_THROTTLE
      ProcedureReturn recvPackage\ADC\PIN_THROTTLE
    EndIf
    If pin = #PIN_BRAKE
      ProcedureReturn recvPackage\ADC\PIN_BRAKE
    EndIf
  EndProcedure
  
  Procedure.a digitalRead(pin.a)
    If pin = #PIN_HALL_A
      ProcedureReturn recvPackage\BLDC\PIN_HALL_A
    EndIf
    If pin = #PIN_HALL_B
      ProcedureReturn recvPackage\BLDC\PIN_HALL_B
    EndIf
    If pin = #PIN_HALL_C
      ProcedureReturn recvPackage\BLDC\PIN_HALL_C
    EndIf
  EndProcedure
  
  Procedure.a devHall()
    ProcedureReturn recvPackage\DEV\devHall
  EndProcedure
  
  Procedure.w devCurrent()
    ProcedureReturn recvPackage\DEV\devCurrent
  EndProcedure
  
  Procedure.w devVoltage()
    ProcedureReturn recvPackage\DEV\devVoltage
  EndProcedure
  
  Procedure.w devThrottle()
    ProcedureReturn recvPackage\DEV\devThrottle
  EndProcedure
  
  Procedure.w devBrake()
    ProcedureReturn recvPackage\DEV\devBrake
  EndProcedure
  
  Procedure.w engineTurnover()
    ProcedureReturn recvPackage\BLDC\engineTurnover
  EndProcedure
  
  Procedure.w engineDegree()
    ProcedureReturn recvPackage\BLDC\engineDegree
  EndProcedure
  
  Procedure.w engineCurrentPWM()
    ProcedureReturn recvPackage\BLDC\engineCurrentPWM
  EndProcedure
  
  Procedure.a engineReverse()
    ProcedureReturn recvPackage\BLDC\engineReverse
  EndProcedure
  
  Procedure.w engineMosfet(timer.a)
    If timer = #TIMER_BLDC_A
      ProcedureReturn recvPackage\MOSFET\TIMER_BLDC_A
    EndIf
    If timer = #TIMER_BLDC_B
      ProcedureReturn recvPackage\MOSFET\TIMER_BLDC_B
    EndIf
    If timer = #TIMER_BLDC_C
      ProcedureReturn recvPackage\MOSFET\TIMER_BLDC_C
    EndIf
  EndProcedure
  
  Procedure.l flashRead(adress.l)
    dataPackage\pack\pdata[0] = #DATAP_INSTRUCTION_READ_CONFIG
    PokeL(@dataPackage\pack\pdata[1], adress)
    If send()
      ReadSerialPortData(#DATAP_CONNECT, dataMemory, AvailableSerialPortInput(#DATAP_CONNECT))
      CopyMemory(dataMemory + 2, @recvPackage\RCONFIG, SizeOf(recvPackageRCONFIG_t) - 1)
      ProcedureReturn recvPackage\RCONFIG\value
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.l flashReadBuffer(adress.l, pbuf.l, len.l)
    Protected icount = 0
    Repeat
      flashRead(adress + icount)
      CopyMemory(dataMemory + 2 + 4, pbuf + icount, 4)
      icount = icount + 4
      If icount > len
        Break
      EndIf
    ForEver
  EndProcedure
  
  Procedure.l flashWrite(adress.l, pbuf.l)
    dataPackage\pack\pdata[0] = #DATAP_INSTRUCTION_WRITE_CONFIG
    PokeL(@dataPackage\pack\pdata[1], adress)
    CopyMemory(pbuf, @dataPackage\pack\pdata[5], 4)
    If send()
      ReadSerialPortData(#DATAP_CONNECT, dataMemory, AvailableSerialPortInput(#DATAP_CONNECT))
      CopyMemory(dataMemory + 2, @recvPackage\RCONFIG, SizeOf(recvPackageRCONFIG_t) - 1)
      ProcedureReturn recvPackage\RCONFIG\value
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.l flashWriteBuffer(adress.l, pbuf.l, len)
    Protected icount = 0
    Repeat
      flashWrite(adress + icount, pbuf + icount)
      icount = icount + 4
      If icount > len
        Break
      EndIf
    ForEver
  EndProcedure
  
  Procedure.l flashErase()
    dataPackage\pack\pdata[0] = #DATAP_INSTRUCTION_ERASE_FLASH
    If send()
      ReadSerialPortData(#DATAP_CONNECT, dataMemory, AvailableSerialPortInput(#DATAP_CONNECT))
      ProcedureReturn 1
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.a boardVersion()
    ProcedureReturn recvPackage\VERSION\version
  EndProcedure
  
  Procedure.l flashAdress()
    ProcedureReturn recvPackage\VERSION\flash
  EndProcedure
  
  Procedure.l flashMax()
    ProcedureReturn recvPackage\VERSION\flashMax
  EndProcedure
  
  Procedure.l update()
    dataPackage\pack\pdata[0] = #DATAP_INSTRUCTION_UPDATE_CONFIG
    If send()
      ReadSerialPortData(#DATAP_CONNECT, dataMemory, AvailableSerialPortInput(#DATAP_CONNECT))
      ProcedureReturn 1
    EndIf
    ProcedureReturn 0
  EndProcedure
  
  Procedure.l controlRemote(throttle.w, brake.w, reverse.a, engine.a, priority.a)
    dataPackage\pack\pdata[0] = #DATAP_INSTRUCTION_CONTROL
    PokeW(@dataPackage\pack\pdata[1], throttle)
    PokeW(@dataPackage\pack\pdata[3], brake)
    PokeA(@dataPackage\pack\pdata[5], engine)
    PokeA(@dataPackage\pack\pdata[6], priority)
    PokeA(@dataPackage\pack\pdata[7], 1)
    PokeA(@dataPackage\pack\pdata[8], reverse)
    If send()
      ReadSerialPortData(#DATAP_CONNECT, dataMemory, AvailableSerialPortInput(#DATAP_CONNECT))
      CopyMemory(dataMemory + 2, @recvPackage\CONTROL, SizeOf(recvControl_t) - 1)
      ProcedureReturn 1
    EndIf
    ProcedureReturn 0
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
; CursorPosition = 405
; FirstLine = 307
; Folding = -PAw8-
; EnableXP