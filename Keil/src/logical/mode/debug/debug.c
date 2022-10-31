// #INDEX# ======================================================================================================
// Title .........: debug - отладка программы.
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

#include "gd32f1x0.h"
#include "debug.h"
#include "../../../framework/board/board.h"
#include "../../../framework/framework.h"
#include "../../../device/device.h"
#include "../../../config/config.h"
#include "../../../BLDC/bldc.h"
#include "../../../logical/logical.h"
#include "../../../dataPackage/dataPackage.h"

extern config_t config;
extern current_t current;
extern engine_t engine;

#ifdef DEBUG_MODE

    void debug_mode_init(void) {
    }

    void debug_mode_loop(void) {

        //uartPrintInt(UART0, uartAvaliable(UART0));
        //uartPrint(UART0, "\r\n");
        #ifdef DEBUG_METHOD_SENSOR
            /*
                Проверка напряжения на контуре и датчика тока
            */
            uartPrintInt      (UART0, devCurrent());
            uartPrint         (UART0, ",");
            uartPrintInt      (UART0, devVoltage());
            uartPrint         (UART0, "\r\n");
        #endif
        #ifdef DEBUG_METHOD_THROTTLE
            /*
                Проверка ручки газа и тормоза
            */
            uartPrintInt      (UART0, devThrottle());
            uartPrint         (UART0, ",");
            uartPrintInt      (UART0, devBrake());
            uartPrint         (UART0, "\r\n");
        #endif
        #ifdef DEBUG_METHOD_VOLTAGE
            /*
                Методика расчета попугаев на 1 V
                Выводим данные попугаев, установить 35 вольт, запомнить значение, поднять до 36 и высчитать разницу
            */
            uartPrintInt    (UART0, analogRead(PIN_VBAT));
            uartPrint       (UART0, "\r\n");
        #endif
        #ifdef DEBUG_METHOD_CURRENT
            /*
                Методика расчета попугаев на 1 А
                Выводим данные попугаев, установить 37.5 Вольт на ЛБП и двумя резисторами (75/2R) замкнуть через шунт.
            */
            uartPrintInt    (UART0, analogRead(PIN_CURRENT_DC));
            uartPrint       (UART0, "\r\n");
        #endif
    }

#endif
