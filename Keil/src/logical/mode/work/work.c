// #INDEX# ======================================================================================================
// Title .........: work - Логика рабочего режима.
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

#include "gd32f1x0.h"
#include "work.h"
#include "../../../framework/board/board.h"
#include "../../../framework/framework.h"
#include "../../../device/device.h"
#include "../../../config/config.h"
#include "../../../BLDC/bldc.h"
#include "../../../dataPackage/dataPackage.h"

extern config_t config;

void work_init(void) {
    enginePhase (config.phase);
    engineHall  (config.hall);
    engineTable (config.pwm);
    engineEnable(TRUE);
}

void work_loop(void) {
    uint16_t throttle;
    uint16_t brake;
    /* Автономный режим */
    throttle = devThrottle();
    brake    = devBrake();
    /* Режим автономного устройства */
    if (logicRemoteDevice() == FALSE) {
        /* Установка задания */
        engineWrite(throttle, TRUE);
        engineBrake(brake, TRUE);
    }
	/* Режим мастера */
	#ifdef UART1_ENA
		dpMaster(UART1);
	#endif
    /* Проверка АКБ */
    logicVoltageControl();
	/* Выключение устройства */
    logicTurnigOff();
}
