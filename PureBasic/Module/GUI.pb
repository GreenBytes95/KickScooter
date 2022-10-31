DeclareModule GUI
  
  ;-----------------------------------------------------------
  ;-       GUI Constants
  ;{----------------------------------------------------------
  
  #NAME                               = "Kickscooter #GreenBytes"
  #WINDOWS_WIDTH                      = 950
  #WINDOWS_HEIGHT                     = 550
  
  #MAX_LIST                           = 100
  #MAX_LIST_COLUMN                    = 10
  
  #TYPE_BLOCK                         = $01
  #TYPE_LIST                          = $02
  #TYPE_INT8T                         = $03
  #TYPE_INT16T                        = $04
  #TYPE_FINT16T                       = $05
  #TYPE_INT32HEX                      = $06
  
  #DEV_STLINK                         = $00
  #DEV_RS232                          = $01
  
  #CONNECT_STATUS_OFF                  = $00
  #CONNECT_STATUS_ON                   = $01
  
  Enumeration
    #WINDOWS_ID                       = 0
    #WINDOWS_LIST
    
    #GADGET_TOOLBAR
    
    #GADGET_TABLIST
    #GADGET_PARAM_LIST
    
    #GADGET_TAB_HOME
    #GADGET_TAB_CONNECT
    #GADGET_TAB_CONTROL
    #GADGET_TAB_SETTING
    #GADGET_TAB_PARAM
    #GADGET_TAB_CONSOLE_ADC
    #GADGET_TAB_CONSOLE_ADC_VBAT
    #GADGET_TAB_CONSOLE_ADC_CURRENT
    #GADGET_TAB_CONSOLE_ADC_THROTTLE
    #GADGET_TAB_CONSOLE_ADC_BRAKE
    #GADGET_TAB_CONSOLE_SENS
    #GADGET_TAB_CONSOLE_SENS_VBAT
    #GADGET_TAB_CONSOLE_SENS_CURRENT
    #GADGET_TAB_CONSOLE_SENS_THROTTLE
    #GADGET_TAB_CONSOLE_SENS_BRAKE
    #GADGET_TAB_CONSOLE_ENGINE
    #GADGET_TAB_CONSOLE_ENGINE_DEGREE
    
    #GADGET_CONNECT_LIST
    
    #GADGET_SETTING_LIST
    
    #GADGET_CONSOLE_ADC_LIST
    
    #GADGET_CONSOLE_SENS_LIST
    
    #GADGET_CONSOLE_ENGINE_LIST
    
    #GADGET_CONTROL_LIST
    
    #GADGET_CONTROL_REMOTE
    #GADGET_CONTROL_ENGINE
    #GADGET_CONTROL_REVERSE
    #GADGET_CONTROL_THROTTLE
    #GADGET_CONTROL_BRAKE
    #GADGET_CONTROL_THROTTLE_T
    #GADGET_CONTROL_BRAKE_T
    
    #GADGET_WL_TEXT
    #GADGET_WL_COMBO
    #GADGET_WL_BUTTON
    
    #ICON_CONNECT
    #ICON_DISCONNECT
    #ICON_SIN
    #ICON_PARAM
    #ICON_TABLE
    #ICON_CONSOLE
    #ICON_HOME
    #ICON_CUBE
    #ICON_SAVE
    #ICON_FOLDER
    #ICON_CHECK
    
    #FONT_CONSOLAS
  EndEnumeration
  
  Enumeration
    #GADGET_TOOLBAR_CONNECT         = 0
    #GADGET_TOOLBAR_DISCONNECT
    #GADGET_TOOLBAR_FOLDER
    #GADGET_TOOLBAR_SAVE
    #GADGET_TOOLBAR_CHECK
  EndEnumeration
  
  Enumeration
    #GADGET_TABLIST_HOME            = 0
    #GADGET_TABLIST_CONNECT
    #GADGET_TABLIST_CUBE
    #GADGET_TABLIST_PARAM
    #GADGET_TABLIST_CONSOLE
    #GADGET_TABLIST_ADC
    #GADGET_TABLIST_ADC_VOLT
    #GADGET_TABLIST_ADC_CUR
    #GADGET_TABLIST_ADC_TH
    #GADGET_TABLIST_ADC_BR
    #GADGET_TABLIST_SENS
    #GADGET_TABLIST_SENS_VOLT
    #GADGET_TABLIST_SENS_CUR
    #GADGET_TABLIST_SENS_TH
    #GADGET_TABLIST_SENS_BR
    #GADGET_TABLIST_ENGINE
    #GADGET_TABLIST_ENGINE_HALL
    #GADGET_TABLIST_ENGINE_TURN
    #GADGET_TABLIST_ENGINE_PWM
    #GADGET_TABLIST_SETTING
  EndEnumeration
  
  ;}----------------------------------------------------------
  ;-       GUI Structure
  ;{----------------------------------------------------------
  
  Structure graph_t
    index.l
    width.l 
    height.l 
    offset.l 
    in_min.l
    in_max.l
    values.l[1024]
  EndStructure
  
  Structure Connect_t
    flash.l
    flashmax.l
    device.a
    comPort.a
    version.a
    timeout.a
    status.l
    index.l
  EndStructure
  
  Structure Setting_t
    
  EndStructure
  
  Structure Config_t
    ;Расположение фаз
		phase.a;
    ;Тип управления (ENGINE_CONTROL_PWM)
    control.a;
    ;Тип ШИМ подаваемый на двигатель (ENGINE_PWM_SINMOD)
    pwm.a;
    ;Установка градусность холлов (ENGINE_HALL_120)
    hall.a;
    ;Инерция двигателя (TRUE)
    inertion.a;
    ;Сторона движения двигателя
    reverse.a;
    ;Кол-во магнитов
    magnet.a;
    ;Ослабление поля (FALSE)
    fw.a;
    ;Максимальный угол опережения
    fwcorner.a;
    ;Максимальный ток ослабления
    fwcurrent.w;
    ;С каких оборотов начнет работать ослабление поля
    fwturn.w;
    ;Максимальный фазный ток в А
    phaseCurrent.w;
    ;Максимальный общий ток в А
    current.w;
    ;Максимальные обороты
    turnMax.w;
    ;Максимальный ток торможения
    brakeCurrent.w
    ;Диаметр колеса
    wheelDiameter.w
    ;Минимальная позиция аналогово задания в попугаях
    throttleMin.w;
    ;Максимальная позиция аналогово задания в попугаях
    throttleMax.w;
    ;Минимальная позиция тормоза в попугаях
    brakeMin.w;
    ;Максимальная позиция тормоза в попугаях
    brakeMax.w;
    ;Минимальное напряжение (отключает двигатель)
    voltageMin.w;
    ;Минимальное напряжение для информарования о разряде АКБ
    voltageMid.w;
    ;Максимальное напряжение АКБ, информирование о перезаряде
    voltageMax.w;
    ;Плавность задания аналогово задания
    throttlePID.a;
    ;Плавность срабатывания торможения
    breakPID.a;
    ;Количество попугаев на 1 Вольт
    bitVoltage.w;
    ;Количество попугаев на 1 Ампер
    bitCurrent.w;
    
    bitByte.a
    bitData.l;
  EndStructure
  
  Structure list_t
    type.l
    index.l
    text.s
    max.l
    min.l
    pdata.l
    hsetting.s
  EndStructure
  
  Global Dim arrayData.list_t(#MAX_LIST_COLUMN + 1, #MAX_LIST + 1)
  
  Global Connect.Connect_t
  Global Setting.Setting_t
  Global Config.Config_t
  
  Global Dim graph.graph_t(30)
  
  Global conThrottle.w
  Global connectStatus.l
  Global timerUpdate = ElapsedMilliseconds()
  
  ;}----------------------------------------------------------
  ;-       GUI Declare
  ;{----------------------------------------------------------
  
  Declare.l Init()
  Declare.l Events()
  Declare.l CatchResource()
  Declare.l ToolBarHideButtonItem(toolbar.l, item.l, hide.l = #True, press.l = #False)
  Declare.l ListRequester(title.s, Message.s, Text.s, index.l = 1)
  Declare.l addGraphValue(*pgraph.graph_t, value.l, throtling.l = 1)
  Declare.l createGraph(*pgraph.graph_t, x.l, y.l, w.l, h.l, inmin.l, inmax.l)
  Declare.l AddListItem(gadget.l, type.l, pdata.l, colum.l, name.s, text.s = "", index.l = 0, min.l = 0, max.l = 0, hsetting.s = "")
  Declare.l EventsListItem(gadget.l, colum.l)
  Declare.l UpdateListItem()
  Declare.l initSetting()
  Declare.l treeTab(index.l)
  Declare.l uMap(x.l, in_min.l, in_max.l, out_min.l, out_max.l)
  
  Declare.l openListConnect()
  Declare.l openListSetting()
  Declare.l openListConfig()
  Declare.l openListConsoleADC()
  Declare.l openListConsoleSENS()
  Declare.l openListConsoleENGINE()
  Declare.l openListControl()
  ;}----------------------------------------------------------

EndDeclareModule

Module GUI
  
  Procedure.l Init()
    UsePNGImageDecoder()
    CatchResource()
    initSetting()
    
    If OpenWindow(#WINDOWS_ID, 0, 0, #WINDOWS_WIDTH, #WINDOWS_HEIGHT, #NAME, #PB_Window_SystemMenu | #PB_Window_ScreenCentered)
      If CreateToolBar(#GADGET_TOOLBAR, WindowID(#WINDOWS_ID), #PB_ToolBar_Large | #PB_ToolBar_Text)
        ToolBarImageButton(#GADGET_TOOLBAR_CONNECT, ImageID(#ICON_CONNECT), #PB_ToolBar_Normal, "Подключиться")
        ToolBarImageButton(#GADGET_TOOLBAR_DISCONNECT, ImageID(#ICON_DISCONNECT), #PB_ToolBar_Normal, "Отключится")
        ToolBarSeparator()
        ToolBarImageButton(#GADGET_TOOLBAR_FOLDER, ImageID(#ICON_FOLDER), #PB_ToolBar_Normal, "Открыть")
        ToolBarImageButton(#GADGET_TOOLBAR_SAVE, ImageID(#ICON_SAVE), #PB_ToolBar_Normal, "Сохранить")
        ToolBarSeparator()
        ToolBarImageButton(#GADGET_TOOLBAR_CHECK, ImageID(#ICON_CHECK), #PB_ToolBar_Normal, "Записать")
      EndIf
      

      
      ToolBarHideButtonItem(#GADGET_TOOLBAR, #GADGET_TOOLBAR_DISCONNECT, #True)
      
      TreeGadget(#GADGET_TABLIST, 5, 53, 200, #WINDOWS_HEIGHT - 59)
      
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_HOME, "Приветствие", ImageID(#ICON_HOME), 0)
      
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_CONNECT, "Подключение", ImageID(#ICON_CONNECT), 0)
      
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_CUBE, "Управление", ImageID(#ICON_CUBE), 0)
      
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_PARAM, "Конфигурация", ImageID(#ICON_PARAM), 0)
      
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_CONSOLE, "Отладчик", ImageID(#ICON_CONSOLE), 0)
      
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_ADC, "АЦП", ImageID(#ICON_CONSOLE), 1)
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_ADC_VOLT, "analogRead(PIN_VBAT)", ImageID(#ICON_CONSOLE), 2)
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_ADC_CUR, "analogRead(PIN_CURRENT_DC)", ImageID(#ICON_CONSOLE), 2)
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_ADC_TH, "analogRead(PIN_THROTTLE)", ImageID(#ICON_CONSOLE), 2)
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_ADC_BR, "analogRead(PIN_BRAKE)", ImageID(#ICON_CONSOLE), 2)
      
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_SENS, "Датчики", ImageID(#ICON_CONSOLE), 1)
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_SENS_VOLT, "devVoltage()", ImageID(#ICON_CONSOLE), 2)
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_SENS_CUR, "devCurrent()", ImageID(#ICON_CONSOLE), 2)
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_SENS_TH, "devThrottle()", ImageID(#ICON_CONSOLE), 2)
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_SENS_BR, "devBrake()", ImageID(#ICON_CONSOLE), 2)
      
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_ENGINE, "Двигатель", ImageID(#ICON_CONSOLE), 1)
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_ENGINE_HALL, "engineDegree()", ImageID(#ICON_CONSOLE), 2)
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_ENGINE_TURN, "engineTurnover()", ImageID(#ICON_CONSOLE), 2)
      
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_ENGINE_PWM, "engineMosfet()", ImageID(#ICON_CONSOLE), 2)
      
      AddGadgetItem (#GADGET_TABLIST, #GADGET_TABLIST_SETTING, "Настройки", ImageID(#ICON_PARAM), 0)
      
      ContainerGadget(#GADGET_TAB_HOME, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        
      CloseGadgetList()
      
      ContainerGadget(#GADGET_TAB_CONNECT, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        ListIconGadget(#GADGET_CONNECT_LIST, 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, "Параметр", 430, #PB_ListIcon_GridLines | #PB_ListIcon_FullRowSelect)
        AddGadgetColumn(#GADGET_CONNECT_LIST, 1, "Значение", 300)
        openListConnect()
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONNECT, #True)
      
      ContainerGadget(#GADGET_TAB_SETTING, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        ListIconGadget(#GADGET_SETTING_LIST, 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, "Параметр", 430, #PB_ListIcon_GridLines | #PB_ListIcon_FullRowSelect)
        AddGadgetColumn(#GADGET_SETTING_LIST, 1, "Значение", 300)
        openListSetting()
      CloseGadgetList()
      HideGadget(#GADGET_TAB_SETTING, #True)
      
      ContainerGadget(#GADGET_TAB_PARAM, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        ListIconGadget(#GADGET_PARAM_LIST, 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, "Параметр", 430, #PB_ListIcon_GridLines | #PB_ListIcon_FullRowSelect)
        AddGadgetColumn(#GADGET_PARAM_LIST, 1, "Значение", 280)
        openListConfig()
      CloseGadgetList()
      HideGadget(#GADGET_TAB_PARAM, #True)
      
      ContainerGadget(#GADGET_TAB_CONSOLE_ADC, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        ListIconGadget(#GADGET_CONSOLE_ADC_LIST, 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, "Процедура", 200, #PB_ListIcon_GridLines | #PB_ListIcon_FullRowSelect)
        AddGadgetColumn(#GADGET_CONSOLE_ADC_LIST, 1, "Описание", 400)
        AddGadgetColumn(#GADGET_CONSOLE_ADC_LIST, 2, "Значение", 120)
        openListConsoleADC()
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONSOLE_ADC, #True)
      
      ContainerGadget(#GADGET_TAB_CONSOLE_ADC_VBAT, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        createGraph(@graph(0), 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, 0, 4096)
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONSOLE_ADC_VBAT, #True)
      
      ContainerGadget(#GADGET_TAB_CONSOLE_ADC_CURRENT, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        createGraph(@graph(1), 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, 0, 4096)
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONSOLE_ADC_CURRENT, #True)
      
      ContainerGadget(#GADGET_TAB_CONSOLE_ADC_THROTTLE, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        createGraph(@graph(2), 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, 0, 4096)
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONSOLE_ADC_THROTTLE, #True)
      
      ContainerGadget(#GADGET_TAB_CONSOLE_ADC_BRAKE, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        createGraph(@graph(3), 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, 0, 4096)
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONSOLE_ADC_BRAKE, #True)
      
      ContainerGadget(#GADGET_TAB_CONSOLE_SENS, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        ListIconGadget(#GADGET_CONSOLE_SENS_LIST, 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, "Процедура", 200, #PB_ListIcon_GridLines | #PB_ListIcon_FullRowSelect)
        AddGadgetColumn(#GADGET_CONSOLE_SENS_LIST, 1, "Описание", 400)
        AddGadgetColumn(#GADGET_CONSOLE_SENS_LIST, 2, "Значение", 120)
        openListConsoleSENS()
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONSOLE_SENS, #True)
      
      ContainerGadget(#GADGET_TAB_CONSOLE_SENS_VBAT, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        createGraph(@graph(4), 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, 0, 1000)
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONSOLE_SENS_VBAT, #True)
      
      ContainerGadget(#GADGET_TAB_CONSOLE_SENS_CURRENT, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        createGraph(@graph(5), 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, -500, 500)
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONSOLE_SENS_CURRENT, #True)
      
      ContainerGadget(#GADGET_TAB_CONSOLE_SENS_THROTTLE, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        createGraph(@graph(6), 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, 0, 4096)
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONSOLE_SENS_THROTTLE, #True)
      
      ContainerGadget(#GADGET_TAB_CONSOLE_SENS_BRAKE, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        createGraph(@graph(7), 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, 0, 4096)
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONSOLE_SENS_BRAKE, #True)
      
      ContainerGadget(#GADGET_TAB_CONSOLE_ENGINE, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        ListIconGadget(#GADGET_CONSOLE_ENGINE_LIST, 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, "Процедура", 200, #PB_ListIcon_GridLines | #PB_ListIcon_FullRowSelect)
        AddGadgetColumn(#GADGET_CONSOLE_ENGINE_LIST, 1, "Описание", 400)
        AddGadgetColumn(#GADGET_CONSOLE_ENGINE_LIST, 2, "Значение", 120)
        openListConsoleENGINE()
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONSOLE_ENGINE, #True)
      
      ContainerGadget(#GADGET_TAB_CONSOLE_ENGINE_DEGREE, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        createGraph(@graph(8), 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, 0, 365)
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONSOLE_ENGINE_DEGREE, #True)
      
      ContainerGadget(#GADGET_TAB_CONTROL, 210, 53, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59, #PB_Container_BorderLess)
        ListIconGadget(#GADGET_CONTROL_LIST, 0, 0, #WINDOWS_WIDTH - 15 - 200, #WINDOWS_HEIGHT - 59 - 120, "Параметр", 430, #PB_ListIcon_GridLines | #PB_ListIcon_FullRowSelect)
        AddGadgetColumn(#GADGET_CONTROL_LIST, 1, "Значение", 280)
        CheckBoxGadget(#GADGET_CONTROL_REMOTE,   10,  380, 250, 20, "Удаленное управление")
        CheckBoxGadget(#GADGET_CONTROL_ENGINE,   10,  400, 250, 20, "Активация двигателя")
        CheckBoxGadget(#GADGET_CONTROL_REVERSE,  10,  420, 250, 20, "Реверс управления")
        TextGadget    (#GADGET_CONTROL_THROTTLE_T, 10,  440, ((#WINDOWS_WIDTH - 15 - 200) - 20) / 2, 20, "Управление газом", #PB_Text_Center)
        TrackBarGadget(#GADGET_CONTROL_THROTTLE,    10,  460, ((#WINDOWS_WIDTH - 15 - 200) - 20) / 2, 20, 0, 100)
        
        TextGadget    (#GADGET_CONTROL_BRAKE_T, 10 + ((#WINDOWS_WIDTH - 15 - 200) - 20) / 2,  440,((#WINDOWS_WIDTH - 15 - 200) - 20) / 2, 20, "Управление торможением", #PB_Text_Center)
        TrackBarGadget(#GADGET_CONTROL_BRAKE, 10 + ((#WINDOWS_WIDTH - 15 - 200) - 20) / 2,  460,((#WINDOWS_WIDTH - 15 - 200) - 20) / 2, 20, 0, 100)
        
        openListControl()
      CloseGadgetList()
      HideGadget(#GADGET_TAB_CONTROL, #True)
      
    EndIf
    
    If OpenWindow(#WINDOWS_LIST, 0, 0, 400, 50 + 20 + 20 + 20 + 20 + 20 + 20, "", #PB_Window_SystemMenu | #PB_Window_WindowCentered, WindowID(#WINDOWS_ID))
      TextGadget(#GADGET_WL_TEXT, 20, 20, 400 - 40, 50, "")
      ComboBoxGadget(#GADGET_WL_COMBO, 20, 50 + 20 + 20, 400 - 40, 20)
      ButtonGadget(#GADGET_WL_BUTTON, (400 / 2) - 40, 50 + 20 + 20 + 20 + 20, 80, 20, "OK")
    EndIf
    HideWindow(#WINDOWS_LIST, #True)
    ;DisableWindow(#WINDOWS_ID, #True)
  EndProcedure
  
  Procedure.l Events()
    Protected Ev = WindowEvent()
    Select Ev
      Case #PB_Event_CloseWindow
        End
      Case #PB_Event_Menu
        Select EventMenu()
          Case #GADGET_TOOLBAR_CONNECT
            If Connect\device = #DEV_RS232
              If dataPackage::open(Connect\comPort, Connect\timeout)
                dataPackage::VERSION()
                Connect\version   = dataPackage::boardVersion()
                If Connect\version > 0
                  Connect\flash     = dataPackage::flashAdress()
                  Connect\flashmax  = dataPackage::flashMax()
                  
                  dataPackage::flashReadBuffer((Connect\flash + Connect\flashmax) - (SizeOf(Config_t) + 4), @Config, SizeOf(Config_t) - 1)
                  
                  Connect\status = #True
                EndIf
                
                UpdateListItem()
              EndIf
            EndIf
            
            If Connect\device = #DEV_STLINK
              
              If stlink::flashReadBuffer((Connect\flash + Connect\flashmax) - (SizeOf(Config_t) + 4), @Config, SizeOf(Config_t) - 1, Connect\timeout)
              
                Connect\status = #True
                
                UpdateListItem()
                
              EndIf
              
            EndIf
            
            If Connect\status = #True
              If Connect\device = #DEV_RS232
                MessageRequester(#NAME, "Подключение к устройству выполненно успешно", #PB_MessageRequester_Info)
              EndIf
              If Connect\device = #DEV_STLINK
                MessageRequester(#NAME, "Подключение к устройству выполненно успешно" + #CRLF$ + #CRLF$ + "ВНИМАНИЕ!" + #CRLF$ + "В режиме подключения ST-LINK возможно только редактировать конфигурацию", #PB_MessageRequester_Info)
              EndIf
              ToolBarHideButtonItem(#GADGET_TOOLBAR, #GADGET_TOOLBAR_CONNECT, #True)
              ToolBarHideButtonItem(#GADGET_TOOLBAR, #GADGET_TOOLBAR_DISCONNECT, #False, #True)
            Else
              MessageRequester(#NAME, "Подключение к устройству НЕ выполнено." + #CRLF$ + "Проверьте подключение программатора к плате", #PB_MessageRequester_Error)
            EndIf
          Case #GADGET_TOOLBAR_DISCONNECT
            If Connect\device = #DEV_RS232
              dataPackage::close()
            EndIf
            ToolBarHideButtonItem(#GADGET_TOOLBAR, #GADGET_TOOLBAR_CONNECT, #False)
            ToolBarHideButtonItem(#GADGET_TOOLBAR, #GADGET_TOOLBAR_DISCONNECT, #True)
            Connect\status = #False
          Case #GADGET_TOOLBAR_CHECK
            If Connect\status = #True
              If Connect\device = #DEV_RS232
                dataPackage::flashErase()
                dataPackage::flashWriteBuffer((Connect\flash + Connect\flashmax) - (SizeOf(Config_t) + 4), @Config, SizeOf(Config_t) - 1)
                dataPackage::update()
              EndIf
              If Connect\device = #DEV_STLINK
                stlink::flashWriteBuffer((Connect\flash + Connect\flashmax) - (SizeOf(Config_t) + 4), @Config, SizeOf(Config_t) - 1, Connect\timeout)
              EndIf
              MessageRequester("Запись конфигурации", "Запись конфигурации выполнена успешно", #PB_MessageRequester_Info)
            Else
              MessageRequester("Запись конфигурации", "Для записи конфигурации выполните подключение к устройству", #PB_MessageRequester_Error)
            EndIf
        EndSelect
      Case #PB_Event_Gadget
        Select EventGadget()
          Case #GADGET_TABLIST
            Select GetGadgetState(#GADGET_TABLIST)
              Case #GADGET_TABLIST_HOME
                treeTab(#GADGET_TAB_HOME)
              Case #GADGET_TABLIST_CONNECT
                treeTab(#GADGET_TAB_CONNECT)
              Case #GADGET_TABLIST_CUBE
                treeTab(#GADGET_TAB_CONTROL)
              Case #GADGET_TABLIST_SETTING
                treeTab(#GADGET_TAB_SETTING)
              Case #GADGET_TABLIST_PARAM
                treeTab(#GADGET_TAB_PARAM)
              Case #GADGET_TABLIST_ADC
                treeTab(#GADGET_TAB_CONSOLE_ADC)
              Case #GADGET_TABLIST_ADC_VOLT
                treeTab(#GADGET_TAB_CONSOLE_ADC_VBAT)
              Case #GADGET_TABLIST_ADC_CUR
                treeTab(#GADGET_TAB_CONSOLE_ADC_CURRENT)
              Case #GADGET_TABLIST_ADC_TH
                treeTab(#GADGET_TAB_CONSOLE_ADC_THROTTLE)
              Case #GADGET_TABLIST_ADC_BR
                treeTab(#GADGET_TAB_CONSOLE_ADC_BRAKE)
              Case #GADGET_TABLIST_SENS
                treeTab(#GADGET_TAB_CONSOLE_SENS)
              Case #GADGET_TABLIST_SENS_VOLT
                treeTab(#GADGET_TAB_CONSOLE_SENS_VBAT)
              Case #GADGET_TABLIST_SENS_CUR
                treeTab(#GADGET_TAB_CONSOLE_SENS_CURRENT)
              Case #GADGET_TABLIST_SENS_TH
                treeTab(#GADGET_TAB_CONSOLE_SENS_THROTTLE)
              Case #GADGET_TABLIST_SENS_BR
                treeTab(#GADGET_TAB_CONSOLE_SENS_BRAKE)
              Case #GADGET_TABLIST_ENGINE
                treeTab(#GADGET_TAB_CONSOLE_ENGINE)
              Case #GADGET_TABLIST_ENGINE_HALL
                treeTab(#GADGET_TAB_CONSOLE_ENGINE_DEGREE)
            EndSelect
          Case #GADGET_CONNECT_LIST
            If EventType() = #PB_EventType_LeftDoubleClick And Connect\status = #False
              EventsListItem(#GADGET_CONNECT_LIST, 0)
            EndIf
          Case #GADGET_SETTING_LIST
            If EventType() = #PB_EventType_LeftDoubleClick
              EventsListItem(#GADGET_SETTING_LIST, 1)
            EndIf
          Case #GADGET_PARAM_LIST
            If EventType() = #PB_EventType_LeftDoubleClick
              EventsListItem(#GADGET_PARAM_LIST, 2)
            EndIf
        EndSelect
    EndSelect
    If Connect\status = #True
      If Connect\device = #DEV_RS232
        If Connect\index = #GADGET_TAB_CONSOLE_ADC And (ElapsedMilliseconds() - timerUpdate) > 100
          dataPackage::ADC()
          arrayData(3, #MAX_LIST)\index = 0
          openListConsoleADC()
          timerUpdate = ElapsedMilliseconds()
        EndIf
        If Connect\index = #GADGET_TAB_CONSOLE_ADC_VBAT
          dataPackage::ADC()
          addGraphValue(@graph(0), dataPackage::recvPackage\ADC\PIN_VBAT, 20)
        EndIf
        If Connect\index = #GADGET_TAB_CONSOLE_ADC_CURRENT
          dataPackage::ADC()
          addGraphValue(@graph(1), dataPackage::recvPackage\ADC\PIN_CURRENT_DC, 20)
        EndIf
        If Connect\index = #GADGET_TAB_CONSOLE_ADC_THROTTLE
          dataPackage::ADC()
          addGraphValue(@graph(2), dataPackage::recvPackage\ADC\PIN_THROTTLE, 20)
        EndIf
        If Connect\index = #GADGET_TAB_CONSOLE_ADC_BRAKE
          dataPackage::ADC()
          addGraphValue(@graph(3), dataPackage::recvPackage\ADC\PIN_BRAKE, 20)
        EndIf
        If Connect\index = #GADGET_TAB_CONSOLE_SENS And (ElapsedMilliseconds() - timerUpdate) > 100
          dataPackage::DEV()
          arrayData(4, #MAX_LIST)\index = 0
          openListConsoleSENS()
          timerUpdate = ElapsedMilliseconds()
        EndIf
        If Connect\index = #GADGET_TAB_CONSOLE_SENS_VBAT
          dataPackage::DEV()
          addGraphValue(@graph(4), dataPackage::recvPackage\DEV\devVoltage, 20)
        EndIf
        If Connect\index = #GADGET_TAB_CONSOLE_SENS_CURRENT
          dataPackage::DEV()
          addGraphValue(@graph(5), dataPackage::recvPackage\DEV\devCurrent, 20)
        EndIf
        If Connect\index = #GADGET_TAB_CONSOLE_SENS_THROTTLE
          dataPackage::DEV()
          addGraphValue(@graph(6), dataPackage::recvPackage\DEV\devThrottle, 20)
        EndIf
        If Connect\index = #GADGET_TAB_CONSOLE_SENS_BRAKE
          dataPackage::DEV()
          addGraphValue(@graph(7), dataPackage::recvPackage\DEV\devBrake, 20)
        EndIf
        If Connect\index = #GADGET_TAB_CONSOLE_ENGINE And (ElapsedMilliseconds() - timerUpdate) > 100
          dataPackage::BLDC()
          dataPackage::MOSFET()
          arrayData(5, #MAX_LIST)\index = 0
          openListConsoleENGINE()
          timerUpdate = ElapsedMilliseconds()
        EndIf
        If Connect\index = #GADGET_TAB_CONSOLE_ENGINE_DEGREE
          dataPackage::BLDC()
          addGraphValue(@graph(8), dataPackage::recvPackage\BLDC\engineDegree, 10)
        EndIf
        If Connect\index = #GADGET_TAB_CONTROL
          If (ElapsedMilliseconds() - timerUpdate) > 1000
            If Config\control = 0
              conThrottle = (GetGadgetState(#GADGET_CONTROL_THROTTLE) * 10)
            EndIf
            If Config\control = 1
              conThrottle = uMap(GetGadgetState(#GADGET_CONTROL_THROTTLE), 0, 100, 0, Config\current)
            EndIf
            If GetGadgetState(#GADGET_CONTROL_THROTTLE) = 0
              conThrottle = 0
            EndIf
            
            dataPackage::controlRemote(conThrottle, 0, GetGadgetState(#GADGET_CONTROL_REVERSE), GetGadgetState(#GADGET_CONTROL_ENGINE), GetGadgetState(#GADGET_CONTROL_REMOTE))
            arrayData(6, #MAX_LIST)\index = 0
            openListControl()
            timerUpdate = ElapsedMilliseconds()
           EndIf
        EndIf
      EndIf
    EndIf
    ProcedureReturn Ev
  EndProcedure
  
  Procedure.l CatchResource()
    CatchImage(#ICON_CONNECT,         ?ICON_CONNECT)
    CatchImage(#ICON_DISCONNECT,      ?ICON_DISCONNECT)
    CatchImage(#ICON_SIN,             ?ICON_SIN)
    CatchImage(#ICON_PARAM,           ?ICON_PARAM)
    CatchImage(#ICON_TABLE,           ?ICON_TABLE)
    CatchImage(#ICON_CONSOLE,         ?ICON_CONSOLE)
    CatchImage(#ICON_HOME,            ?ICON_HOME)
    CatchImage(#ICON_CUBE,            ?ICON_CUBE)
    CatchImage(#ICON_SAVE,            ?ICON_SAVE)
    CatchImage(#ICON_FOLDER,          ?ICON_FOLDER)
    CatchImage(#ICON_CHECK,           ?ICON_CHECK)
    
    LoadFont(#FONT_CONSOLAS, "Consolas", 9)
  EndProcedure
  
  Procedure.l ToolBarHideButtonItem(toolbar.l, item.l, hide.l = #True, press.l = #False)
    Protected htb=ToolBarID(toolbar)
    Protected tbbi.TBBUTTONINFO
    tbbi\cbSize = SizeOf(TBBUTTONINFO)
    tbbi\dwMask = #TBIF_BYINDEX | #TBIF_STATE
    If hide = #True
      tbbi\fsState = #TBSTATE_HIDDEN
    Else
      tbbi\fsState = #TBSTATE_ENABLED
    EndIf
    If press = #True
      tbbi\fsState = tbbi\fsState | #TBSTATE_PRESSED
    EndIf
    SendMessage_(htb, #TB_SETBUTTONINFO, item, tbbi)
  EndProcedure
  
  Procedure.l ListRequester(title.s, Message.s, Text.s, index.l = 1)
    Protected idx.l = 1, Evp.l
    
    SetWindowTitle(#WINDOWS_LIST, title)
    SetGadgetText(#GADGET_WL_TEXT, Message)
    ClearGadgetItems(#GADGET_WL_COMBO)
    
    Repeat
      If StringField(Text, idx, "|") = ""
        Break
      EndIf
      AddGadgetItem(#GADGET_WL_COMBO, idx - 1,  StringField(Text, idx, "|"))
      idx = idx + 1
    ForEver
    
    SetGadgetState(#GADGET_WL_COMBO, index - 1)
    
    HideWindow(#WINDOWS_LIST, #False)
    DisableWindow(#WINDOWS_ID, #True)
    
    Repeat
      Evp = WaitWindowEvent()
      Select Evp
        Case #PB_Event_CloseWindow
          HideWindow(#WINDOWS_LIST, #True)
          DisableWindow(#WINDOWS_ID, #False)
          ProcedureReturn 0
        Case #PB_Event_Gadget
          Select EventGadget()
            Case #GADGET_WL_BUTTON
              HideWindow(#WINDOWS_LIST, #True)
              DisableWindow(#WINDOWS_ID, #False)
              ProcedureReturn GetGadgetState(#GADGET_WL_COMBO) + 1
          EndSelect
      EndSelect
    ForEver
  EndProcedure
  
  Procedure.l addGraphValue(*pgraph.graph_t, value.l, throtling.l = 1)
    Protected ph_min.l = 100, ph_max.l = 0
    For d=0 To *pgraph\width-1 
      *pgraph\values[d]=*pgraph\values[d+throtling] 
    Next  
    *pgraph\values[*pgraph\width-1]=uMap(value, *pgraph\in_min, *pgraph\in_max, 0, 100)
    If StartDrawing(CanvasOutput(*pgraph\index)) 
      DrawingMode(#PB_2DDrawing_Default)
      Box(0,0,*pgraph\width,*pgraph\height,#Black)
      For a=0 To *pgraph\height Step 10 
        LineXY(0,a,*pgraph\width,a,$7F00) 
      Next
      For a=0 To *pgraph\width+*pgraph\offset Step 10 
        LineXY(a-*pgraph\offset,0,a-*pgraph\offset,*pgraph\height,$7F00) 
      Next
      For d=0 To *pgraph\width 
        bottom.l=*pgraph\height 
        thepercent.f=(*pgraph\values[d]/100) 
        actualheight.l=Int(thepercent* *pgraph\height) 
        Y1.l=bottom-actualheight 
        thepercent.f=(*pgraph\values[d+throtling]/100) 
        actualheight.l=Int(thepercent* *pgraph\height) 
        Y2.l=bottom-actualheight 
        LineXY(d,Y1,d+throtling,Y2,$FF00) 
        If ph_min > *pgraph\values[d] And *pgraph\values[d] > 0
          ph_min = *pgraph\values[d]
        EndIf
        If ph_max < *pgraph\values[d]
          ph_max = *pgraph\values[d]
        EndIf
      Next
      Box(*pgraph\width - 5 - 200, 10, 200, 55, $000000)
      DrawingFont(FontID(#FONT_CONSOLAS))
      DrawText(*pgraph\width - 5 - 200 + 5, 10 + 5, "Значение: " + Str(value), $FFFFFF)
      DrawText(*pgraph\width - 5 - 200 + 5, 10 + 5 + 15, "Максимальное: " + Str(uMap(ph_max, 0, 100, *pgraph\in_min, *pgraph\in_max)) + "(" + Str(ph_max) + "%)", $FFFFFF)
      DrawText(*pgraph\width - 5 - 200 + 5, 10 + 5 + 30, "Минимальное: " + Str(uMap(ph_min, 0, 100, *pgraph\in_min, *pgraph\in_max)) + "(" + Str(ph_min) + "%)", $FFFFFF)
      StopDrawing()
    EndIf
  EndProcedure
  
  Procedure.l createGraph(*pgraph.graph_t, x.l, y.l, w.l, h.l, inmin.l, inmax.l)
    *pgraph\index = CanvasGadget(#PB_Any, x, y, w, h)
    
    *pgraph\height  = h
    *pgraph\width   = w
    
    *pgraph\in_min  = inmin
    *pgraph\in_max  = inmax
    
    ProcedureReturn *pgraph\index
  EndProcedure
  
  Procedure.l AddListItem(gadget.l, type.l, pdata.l, colum.l, name.s, text.s = "", index.l = 0, min.l = 0, max.l = 0, hsetting.s = "")
    Protected pval.l, pindex.l = arrayData(colum, #MAX_LIST)\index
        
    arrayData(colum, pindex)\type     = type
    arrayData(colum, pindex)\text     = text
    arrayData(colum, pindex)\index    = (index + 1)
    arrayData(colum, pindex)\max      = max
    arrayData(colum, pindex)\min      = min
    arrayData(colum, pindex)\pdata    = pdata
    arrayData(colum, pindex)\hsetting = hsetting
    
    If GetGadgetAttribute(gadget, #PB_ListIcon_ColumnCount) = 3
      name = StringField(name, 1, "|") + Chr(10) + StringField(name, 2, "|")
    EndIf
    
    arrayData(colum, #MAX_LIST)\index = arrayData(colum, #MAX_LIST)\index + 1
    
    If arrayData(colum, pindex)\type = #TYPE_BLOCK
      AddGadgetItem(gadget, pindex, name)
      
      SetGadgetItemColor(gadget, pindex, #PB_Gadget_BackColor,   $000000)
      SetGadgetItemColor(gadget, pindex, #PB_Gadget_FrontColor,  $FFFFFF)
    EndIf
    
    If arrayData(colum, pindex)\type = #TYPE_LIST
      AddGadgetItem(gadget, pindex, name + Chr(10) + StringField( text, arrayData(colum, pindex)\index, "|"))
    EndIf
    
    If arrayData(colum, pindex)\type = #TYPE_INT8T Or arrayData(colum, pindex)\type = #TYPE_INT16T
      AddGadgetItem(gadget, pindex, name + Chr(10) + text)
    EndIf
    
    If arrayData(colum, pindex)\type = #TYPE_FINT16T
      pval = Val(text)
      If pval > 9
        text = Left(Str(pval), Len(Str(pval)) - 1) + "." + Right(Str(pval), 1)
      Else
        text = "0." + Str(pval)
      EndIf
      AddGadgetItem(gadget, pindex, name + Chr(10) + text)
    EndIf
    
    If arrayData(colum, pindex)\type = #TYPE_INT32HEX
      AddGadgetItem(gadget, pindex, name + Chr(10) + RSet(Hex(PeekL(arrayData(colum, pindex)\pdata)),8, "0"))
    EndIf
    
  EndProcedure
  
  Procedure.l EventsListItem(gadget.l, colum.l)
    Protected in$, pval.l, pindex.l = GetGadgetState(gadget)
    If pindex < 0 : ProcedureReturn 0 : EndIf
    
    If arrayData(colum, pindex)\type = #TYPE_BLOCK
      
    EndIf
    
    If arrayData(colum, pindex)\type = #TYPE_LIST
      pval = ListRequester(GetGadgetItemText(gadget, pindex), "Выберите нужное значение из списка", arrayData(colum, pindex)\text, arrayData(colum, pindex)\index)
      If pval = 0 : ProcedureReturn 0 : EndIf
      arrayData(colum, pindex)\index = pval
      SetGadgetItemText(gadget, pindex, StringField( arrayData(colum, pindex)\text, arrayData(colum, pindex)\index, "|"), 1)
      
      PokeA(arrayData(colum, pindex)\pdata, arrayData(colum, pindex)\index - 1)
      If arrayData(colum, pindex)\hsetting <> ""
        Registry::WriteValue(#HKEY_CURRENT_USER, "Software\" + #NAME, arrayData(colum, pindex)\hsetting,  Str(arrayData(colum, pindex)\index - 1), #REG_MULTI_SZ)
      EndIf
    EndIf
    
    If arrayData(colum, pindex)\type = #TYPE_INT8T
      in$ = InputRequester(GetGadgetItemText(gadget, pindex), "Введите значение от " + Str(arrayData(colum, pindex)\min) + " до " + Str(arrayData(colum, pindex)\max), GetGadgetItemText(gadget, pindex, 1))
      pval = Val(in$)
      If pval < arrayData(colum, pindex)\min Or pval > arrayData(colum, pindex)\max Or (pval = 0 And in$ <> "0")
        MessageRequester("Ошибка", "Значение не соотвествует требуемым параметрам", #PB_MessageRequester_Error)
      Else
        SetGadgetItemText(gadget, pindex, Str(pval), 1)
        
        PokeA(arrayData(colum, pindex)\pdata, pval)
        If arrayData(colum, pindex)\hsetting <> ""
          Registry::WriteValue(#HKEY_CURRENT_USER, "Software\" + #NAME, arrayData(colum, pindex)\hsetting,  Str(pval), #REG_MULTI_SZ)
        EndIf
      EndIf
    EndIf
    
    If arrayData(colum, pindex)\type = #TYPE_INT16T
      in$ = InputRequester(GetGadgetItemText(gadget, pindex), "Введите значение от " + Str(arrayData(colum, pindex)\min) + " до " + Str(arrayData(colum, pindex)\max), GetGadgetItemText(gadget, pindex, 1))
      pval = Val(in$)
      If pval < arrayData(colum, pindex)\min Or pval > arrayData(colum, pindex)\max Or (pval = 0 And in$ <> "0")
        MessageRequester("Ошибка", "Значение не соотвествует требуемым параметрам", #PB_MessageRequester_Error)
      Else
        SetGadgetItemText(gadget, pindex, Str(pval), 1)
        
        PokeW(arrayData(colum, pindex)\pdata, pval)
        
        If arrayData(colum, pindex)\hsetting <> ""
          Registry::WriteValue(#HKEY_CURRENT_USER, "Software\" + #NAME, arrayData(colum, pindex)\hsetting,  Str(pval), #REG_MULTI_SZ)
        EndIf
      EndIf
    EndIf
    
    If arrayData(colum, pindex)\type = #TYPE_FINT16T
      in$ = InputRequester(GetGadgetItemText(gadget, pindex), "Введите значение от " + Str(arrayData(colum, pindex)\min) + " до " + Str(arrayData(colum, pindex)\max), GetGadgetItemText(gadget, pindex, 1))
      pval = Val(ReplaceString(in$, ".", ""))
      If pval < arrayData(colum, pindex)\min Or pval > arrayData(colum, pindex)\max Or (pval = 0 And in$ <> "0")
        MessageRequester("Ошибка", "Значение не соотвествует требуемым параметрам", #PB_MessageRequester_Error)
      Else
        If pval > 9
          in$ = Left(Str(pval), Len(Str(pval)) - 1) + "." + Right(Str(pval), 1)
        Else
          in$ = "0." + Str(pval)
        EndIf
        SetGadgetItemText(gadget, pindex, in$, 1)
        
        PokeW(arrayData(colum, pindex)\pdata, pval)
        
        If arrayData(colum, pindex)\hsetting <> ""
          Registry::WriteValue(#HKEY_CURRENT_USER, "Software\" + #NAME, arrayData(colum, pindex)\hsetting,  Str(pval), #REG_MULTI_SZ)
        EndIf
      EndIf
    EndIf
    
    If arrayData(colum, pindex)\type = #TYPE_INT32HEX
      in$ = InputRequester(GetGadgetItemText(gadget, pindex), "Введите HEX значение", GetGadgetItemText(gadget, pindex, 1))
      pval = Val("$" + in$)
      
      
      SetGadgetItemText(gadget, pindex, RSet(Hex(pval), 8, "0"), 1)
      
      PokeL(arrayData(colum, pindex)\pdata, pval)
      
      If arrayData(colum, pindex)\hsetting <> ""
          Registry::WriteValue(#HKEY_CURRENT_USER, "Software\" + #NAME, arrayData(colum, pindex)\hsetting,  Str(pval), #REG_MULTI_SZ)
        EndIf
    EndIf
    
  EndProcedure
  
  Procedure.l UpdateListItem()
    Protected pColumn.l
    For pColumn = 0 To #MAX_LIST_COLUMN
      arrayData(pColumn, #MAX_LIST)\index = 0
    Next
    openListConnect()
    openListSetting()
    openListConfig()
  EndProcedure
  
  Procedure.l defineSetting()
    Registry::WriteValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "name", #NAME, #REG_MULTI_SZ)
    Registry::WriteValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "Connect::device",  "0", #REG_MULTI_SZ)
    Registry::WriteValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "Connect::comPort", "4", #REG_MULTI_SZ)
    Registry::WriteValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "Connect::timeout", "10", #REG_MULTI_SZ)
    Registry::WriteValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "Connect::version", "1", #REG_MULTI_SZ)
    Registry::WriteValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "Connect::flash",   "$08000000", #REG_MULTI_SZ)
    Registry::WriteValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "Connect::flashmax","$FA00", #REG_MULTI_SZ)
  EndProcedure
  
  Procedure.l initSetting()
    If Registry::ReadValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "name") = ""
      defineSetting()
    EndIf
    
    Connect\device    = Val(Registry::ReadValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "Connect::device"))
    Connect\comPort   = Val(Registry::ReadValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "Connect::comPort"))
    Connect\timeout   = Val(Registry::ReadValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "Connect::timeout"))
    Connect\version   = Val(Registry::ReadValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "Connect::version"))
    Connect\flash     = Val(Registry::ReadValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "Connect::flash"))
    Connect\flashmax  = Val(Registry::ReadValue(#HKEY_CURRENT_USER, "Software\" + #NAME, "Connect::flashmax"))
    
  EndProcedure
  
  Procedure.l treeTab(index.l)
    HideGadget(#GADGET_TAB_HOME, #True)
    HideGadget(#GADGET_TAB_CONNECT, #True)
    HideGadget(#GADGET_TAB_SETTING, #True)
    HideGadget(#GADGET_TAB_PARAM, #True)
    HideGadget(#GADGET_TAB_CONSOLE_ADC, #True)
    HideGadget(#GADGET_TAB_CONSOLE_ADC_VBAT, #True)
    HideGadget(#GADGET_TAB_CONSOLE_ADC_CURRENT, #True)
    HideGadget(#GADGET_TAB_CONSOLE_ADC_THROTTLE, #True)
    HideGadget(#GADGET_TAB_CONSOLE_ADC_BRAKE, #True)
    HideGadget(#GADGET_TAB_CONSOLE_SENS, #True)
    HideGadget(#GADGET_TAB_CONSOLE_SENS_VBAT, #True)
    HideGadget(#GADGET_TAB_CONSOLE_SENS_CURRENT, #True)
    HideGadget(#GADGET_TAB_CONSOLE_SENS_THROTTLE, #True)
    HideGadget(#GADGET_TAB_CONSOLE_SENS_BRAKE, #True)
    HideGadget(#GADGET_TAB_CONSOLE_ENGINE, #True)
    HideGadget(#GADGET_TAB_CONSOLE_ENGINE_DEGREE, #True)
    HideGadget(#GADGET_TAB_CONTROL, #True)
    
    Connect\index = index
    
    HideGadget(index, #False)
  EndProcedure
  
  Procedure.l uMap(x.l, in_min.l, in_max.l, out_min.l, out_max.l)
    ProcedureReturn (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  EndProcedure

  
  Procedure.l openListConnect()
    ClearGadgetItems(#GADGET_CONNECT_LIST)
    
    AddListItem(#GADGET_CONNECT_LIST, #TYPE_BLOCK,      0, 0, "Настройка подключения")
    AddListItem(#GADGET_CONNECT_LIST, #TYPE_LIST,       @Connect\device,    0, "Устройство связи",  "ST-LINK|RS232", Connect\device, 0, 0, "Connect::device")
    AddListItem(#GADGET_CONNECT_LIST, #TYPE_INT8T,      @Connect\timeout,   0, "Время ожидания соединения (сек)",     Str(Connect\timeout), 0, 0, 250, "Connect::timeout")
    AddListItem(#GADGET_CONNECT_LIST, #TYPE_INT8T,      @Connect\comPort,   0, "Порт COM",          Str(Connect\comPort), 0, 0, 100, "Connect::comPort")
    
    AddListItem(#GADGET_CONNECT_LIST, #TYPE_BLOCK,      0, 0, "Настройка платы")
    AddListItem(#GADGET_CONNECT_LIST, #TYPE_INT8T,      @Connect\version,   0, "Номер платы",       Str(Connect\version), 0, 0, 100, "Connect::version")
    AddListItem(#GADGET_CONNECT_LIST, #TYPE_BLOCK,      0, 0, "Настройка доступа процессора")
    AddListItem(#GADGET_CONNECT_LIST, #TYPE_INT32HEX,   @Connect\flash,     0, "Адрес Flash памяти", "", 0, 0, 0, "Connect::flash")
    AddListItem(#GADGET_CONNECT_LIST, #TYPE_INT32HEX,   @Connect\flashmax,  0, "Размер Flash памяти", "", 0, 0, 0, "Connect::flashmax")
  EndProcedure
  
  Procedure.l openListSetting()
    
  EndProcedure
  
  Procedure.l openListConfig()
    ClearGadgetItems(#GADGET_PARAM_LIST)
    
    AddListItem(#GADGET_PARAM_LIST, #TYPE_BLOCK,      0,                    2, "Настройка управления")
    AddListItem(#GADGET_PARAM_LIST, #TYPE_LIST,       @Config\pwm,          2, "Тип управления двигателем",  "Синус|Меандр", Config\pwm)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_LIST,       @Config\control,      2, "Тип задания",  "Напряжение|Ток|Обороты", Config\control)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_LIST,       @Config\hall,         2, "Тип холлов двигателя",  "120|60", Config\hall)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_LIST,       @Config\reverse,      2, "Направление вращения двигателя",  "По умолчанию|Инвертированое", Config\reverse)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_LIST,       @Config\inertion,     2, "Инерция двигателя",  "Отключено|Включено", Config\inertion)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_INT8T,      @Config\phase,        2, "Расположение фаз",  Str(Config\phase), 0, 0, 5)
    
    AddListItem(#GADGET_PARAM_LIST, #TYPE_BLOCK,      0,                    2, "Настройка двигателя")
    AddListItem(#GADGET_PARAM_LIST, #TYPE_FINT16T,    @Config\current,      2, "Максимальный общий ток",  Str(Config\current), 0, 0, 1000)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_FINT16T,    @Config\phaseCurrent, 2, "Максимальный фазный ток",  Str(Config\phaseCurrent), 0, 0, 1000)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_INT8T,      @Config\magnet,       2, "Кол-во магнитов",  Str(Config\magnet), 0, 0, 250)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_INT16T,     @Config\turnMax,      2, "Максимальные обороты",  Str(Config\turnMax), 0, 0, 10000)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_FINT16T,    @Config\wheelDiameter,2, "Диаметр колеса",  Str(Config\wheelDiameter), 0, 0, 1000)
    
    
    AddListItem(#GADGET_PARAM_LIST, #TYPE_BLOCK,      0,                    2, "Управление курком газа")
    AddListItem(#GADGET_PARAM_LIST, #TYPE_INT16T,     @Config\throttleMin,  2, "Минимальное положение курка газа",  Str(Config\throttleMin), 0, 0, 4096)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_INT16T,     @Config\throttleMax,  2, "Максимальное положение курка газа",  Str(Config\throttleMax), 0, 0, 4096)
    
    AddListItem(#GADGET_PARAM_LIST, #TYPE_BLOCK,      0,                    2, "Управление курком тормоза")
    AddListItem(#GADGET_PARAM_LIST, #TYPE_INT16T,     @Config\brakeMin,     2, "Минимальное положение курка тормоза",  Str(Config\brakeMin), 0, 0, 4096)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_INT16T,     @Config\brakeMax,     2, "Максимальное положение курка тормоза",  Str(Config\brakeMax), 0, 0, 4096)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_FINT16T,    @Config\brakeCurrent, 2, "Максимальный ток рекуперации",  Str(Config\brakeCurrent), 0, 0, 1000)
    
    AddListItem(#GADGET_PARAM_LIST, #TYPE_BLOCK,      0,                    2, "Ослабление поля")
    AddListItem(#GADGET_PARAM_LIST, #TYPE_LIST,       @Config\fw,           2, "Ослабление поля",  "Отключено|Включено", Config\fw)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_INT8T,      @Config\fwcorner,     2, "Максимальный угол опережения",  Str(Config\fwcorner), 0, 1, 30)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_FINT16T,    @Config\fwcurrent,    2, "Максимальный ток ослабления поля",  Str(Config\fwcurrent), 0, 0, 1000)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_INT16T,     @Config\fwturn,       2, "Минимальные обороты сработки ослабления поля",  Str(Config\fwturn), 0, 0, 10000)
    
    AddListItem(#GADGET_PARAM_LIST, #TYPE_BLOCK,      0,                    2, "Настройка аккумулятора")
    AddListItem(#GADGET_PARAM_LIST, #TYPE_FINT16T,    @Config\voltageMin,   2, "Минимальное напряжение аккумулятора",  Str(Config\voltageMin), 0, 0, 1000)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_FINT16T,    @Config\voltageMid,   2, "Среднее напряжение аккумулятора",  Str(Config\voltageMid), 0, 0, 1000)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_FINT16T,    @Config\voltageMax,   2, "Максимальное напряжение аккумулятора",  Str(Config\voltageMax), 0, 0, 1000)
    
    AddListItem(#GADGET_PARAM_LIST, #TYPE_BLOCK,      0,                    2, "Управление плавностью")
    AddListItem(#GADGET_PARAM_LIST, #TYPE_INT8T,      @Config\throttlePID,  2, "Плавность ручки газа",  Str(Config\throttlePID), 0, 1, 10)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_INT8T,      @Config\breakPID,     2, "Плавность ручки тормоза",  Str(Config\breakPID), 0, 1, 10)
    
    AddListItem(#GADGET_PARAM_LIST, #TYPE_BLOCK,      0,                    2, "Настройка датчиков")
    AddListItem(#GADGET_PARAM_LIST, #TYPE_INT16T,     @Config\bitVoltage,   2, "Количество отсчетов для вычисления напряжения",  Str(Config\bitVoltage), 0, 0, 10000)
    AddListItem(#GADGET_PARAM_LIST, #TYPE_INT16T,     @Config\bitCurrent,   2, "Количество отсчетов для вычисления тока",  Str(Config\bitCurrent), 0, 0, 10000)
  EndProcedure
  
  Procedure.l openListConsoleADC()
    ClearGadgetItems(#GADGET_CONSOLE_ADC_LIST)
    
    AddListItem(#GADGET_CONSOLE_ADC_LIST, #TYPE_INT16T, dataPackage::@recvPackage\ADC\PIN_VBAT,        3, "analogRead(PIN_VBAT)|Напряжение",  Str(dataPackage::recvPackage\ADC\PIN_VBAT), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_ADC_LIST, #TYPE_INT16T, dataPackage::@recvPackage\ADC\PIN_CURRENT_DC,  3, "analogRead(PIN_CURRENT_DC)|Датчик тока",  Str(dataPackage::recvPackage\ADC\PIN_CURRENT_DC), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_ADC_LIST, #TYPE_INT16T, dataPackage::@recvPackage\ADC\PIN_THROTTLE,    3, "analogRead(PIN_THROTTLE)|Ручка газа",  Str(dataPackage::recvPackage\ADC\PIN_THROTTLE), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_ADC_LIST, #TYPE_INT16T, dataPackage::@recvPackage\ADC\PIN_BRAKE,       3, "analogRead(PIN_BRAKE)|Ручка тормоза",  Str(dataPackage::recvPackage\ADC\PIN_BRAKE), 0, 0, 0)
  EndProcedure
  
  Procedure.l openListConsoleSENS()
    ClearGadgetItems(#GADGET_CONSOLE_SENS_LIST)
    
    AddListItem(#GADGET_CONSOLE_SENS_LIST, #TYPE_INT8T, dataPackage::@recvPackage\DEV\devHall,         4, "devHall()|Холлы",  Str(dataPackage::recvPackage\DEV\devHall), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_SENS_LIST, #TYPE_FINT16T, dataPackage::@recvPackage\DEV\devVoltage,    4, "devVoltage()|Напряжение",  Str(dataPackage::recvPackage\DEV\devVoltage), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_SENS_LIST, #TYPE_FINT16T, dataPackage::@recvPackage\DEV\devCurrent,    4, "devCurrent()|Датчик тока",  Str(dataPackage::recvPackage\DEV\devCurrent), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_SENS_LIST, #TYPE_INT16T, dataPackage::@recvPackage\DEV\devThrottle,    4, "devThrottle()|Ручка газа",  Str(dataPackage::recvPackage\DEV\devThrottle), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_SENS_LIST, #TYPE_INT16T, dataPackage::@recvPackage\DEV\devBrake,       4, "devBrake()|Ручка тормоза",  Str(dataPackage::recvPackage\DEV\devBrake), 0, 0, 0)
  EndProcedure
  
  Procedure.l openListConsoleENGINE()
    ClearGadgetItems(#GADGET_CONSOLE_ENGINE_LIST)
    
    AddListItem(#GADGET_CONSOLE_ENGINE_LIST, #TYPE_INT16T, dataPackage::@recvPackage\BLDC\engineTurnover,        5, "engineTurnover()|Обороты в минуту",  Str(dataPackage::recvPackage\BLDC\engineTurnover), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_ENGINE_LIST, #TYPE_INT16T, dataPackage::@recvPackage\BLDC\engineDegree,     5, "engineDegree()|Интерполяция датчика положения двигателя",  Str(dataPackage::recvPackage\BLDC\engineDegree), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_ENGINE_LIST, #TYPE_INT8T, dataPackage::@recvPackage\BLDC\engineReverse,    5, "engineReverse()|Направление вращения",  Str(dataPackage::recvPackage\BLDC\engineReverse), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_ENGINE_LIST, #TYPE_INT8T, dataPackage::@recvPackage\BLDC\PIN_HALL_A,    5, "digitalRead(PIN_HALL_A)|Холл А",  Str(dataPackage::recvPackage\BLDC\PIN_HALL_A), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_ENGINE_LIST, #TYPE_INT8T, dataPackage::@recvPackage\BLDC\PIN_HALL_B,    5, "digitalRead(PIN_HALL_B)|Холл B",  Str(dataPackage::recvPackage\BLDC\PIN_HALL_B), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_ENGINE_LIST, #TYPE_INT8T, dataPackage::@recvPackage\BLDC\PIN_HALL_C,    5, "digitalRead(PIN_HALL_C)|Холл C",  Str(dataPackage::recvPackage\BLDC\PIN_HALL_C), 0, 0, 0)
    
    AddListItem(#GADGET_CONSOLE_ENGINE_LIST, #TYPE_BLOCK,      0,                    5, "|ШИМ двигателя")
    AddListItem(#GADGET_CONSOLE_ENGINE_LIST, #TYPE_INT16T, dataPackage::@recvPackage\BLDC\engineCurrentPWM, 5, "engineCurrentPWM()|Скважность сигнала ШИМ Общий",  Str(dataPackage::recvPackage\BLDC\engineCurrentPWM), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_ENGINE_LIST, #TYPE_INT8T, dataPackage::@recvPackage\MOSFET\TIMER_BLDC_A,    5, "engineMosfet(TIMER_BLDC_A)|Скважность сигнала ШИМ А",  Str(dataPackage::recvPackage\MOSFET\TIMER_BLDC_A), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_ENGINE_LIST, #TYPE_INT8T, dataPackage::@recvPackage\MOSFET\TIMER_BLDC_B,    5, "engineMosfet(TIMER_BLDC_B)|Скважность сигнала ШИМ B",  Str(dataPackage::recvPackage\MOSFET\TIMER_BLDC_B), 0, 0, 0)
    AddListItem(#GADGET_CONSOLE_ENGINE_LIST, #TYPE_INT8T, dataPackage::@recvPackage\MOSFET\TIMER_BLDC_C,    5, "engineMosfet(TIMER_BLDC_C)|Скважность сигнала ШИМ C",  Str(dataPackage::recvPackage\MOSFET\TIMER_BLDC_C), 0, 0, 0)
  EndProcedure
  
  Procedure.l openListControl()
    ClearGadgetItems(#GADGET_CONTROL_LIST)
    
    AddListItem(#GADGET_CONTROL_LIST, #TYPE_BLOCK, 0, 6, "Выходные параметры")
    
    AddListItem(#GADGET_CONTROL_LIST, #TYPE_FINT16T, dataPackage::@recvPackage\CONTROL\voltage,       6, "Напряжение",  Str(dataPackage::recvPackage\CONTROL\voltage), 0, 0, 0)
    AddListItem(#GADGET_CONTROL_LIST, #TYPE_FINT16T, dataPackage::@recvPackage\CONTROL\current,       6, "Датчик тока",  Str(dataPackage::recvPackage\CONTROL\current), 0, 0, 0)
    AddListItem(#GADGET_CONTROL_LIST, #TYPE_INT16T, dataPackage::@recvPackage\CONTROL\turnover,        6, "Обороты",  Str(dataPackage::recvPackage\CONTROL\turnover), 0, 0, 0)
    AddListItem(#GADGET_CONTROL_LIST, #TYPE_INT8T, dataPackage::@recvPackage\CONTROL\reverse,         6, "Направление",  Str(dataPackage::recvPackage\CONTROL\reverse), 0, 0, 0)
    
    AddListItem(#GADGET_CONTROL_LIST, #TYPE_BLOCK, 0, 6, "Входные параметры")
    
    AddListItem(#GADGET_CONTROL_LIST, #TYPE_INT16T, @conThrottle,       6, "Управление газом",  Str(conThrottle), 0, 0, 0)
    AddListItem(#GADGET_CONTROL_LIST, #TYPE_INT16T, dataPackage::@recvPackage\BLDC\engineTurnover,       6, "Управление торможением",  Str(dataPackage::recvPackage\BLDC\engineTurnover), 0, 0, 0)
  EndProcedure
  
  DataSection
    ICON_CONNECT:
    IncludeBinary "..\Image\connect.png"
    ICON_DISCONNECT:
    IncludeBinary "..\Image\disconnect.png"
    ICON_SIN:
    IncludeBinary "..\Image\sin.png"
    ICON_PARAM:
    IncludeBinary "..\Image\param.png"
    ICON_TABLE:
    IncludeBinary "..\Image\table.png"
    ICON_CONSOLE:
    IncludeBinary "..\Image\console.png"
    ICON_HOME:
    IncludeBinary "..\Image\home.png"
    ICON_CUBE:
    IncludeBinary "..\Image\cube.png"
    ICON_SAVE:
    IncludeBinary "..\Image\save.png"
    ICON_FOLDER:
    IncludeBinary "..\Image\folder.png"
    ICON_CHECK:
    IncludeBinary "..\Image\check.png"
  EndDataSection
  
EndModule
; IDE Options = PureBasic 5.62 (Windows - x86)
; CursorPosition = 986
; FirstLine = 796
; Folding = -XgM-
; EnableXP
; Executable = ..\..\bin\KickScooter.exe
; CompileSourceDirectory