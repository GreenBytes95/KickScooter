// #INDEX# ======================================================================================================
// Title .........: logical - Общая логика программы.
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

#ifndef LOGICAL_H

    #define LOGICAL_H

    #include "gd32f1x0.h"
    #include "../framework/board/board.h"
    #include "../framework/framework.h"
    #include "../device/device.h"
    #include "../config/config.h"


    /* # Константы # */

    # define LOGICAL_MODE_WORK                          BUTTON_OK
    # define LOGICAL_MODE_SLAVE                         BUTTON_NONE
    # define LOGICAL_MODE_CALIBRATION                   BUTTON_HOLD

    /* # Структуры # */

    /* Структура управления двигателем */
	typedef struct {
        uint32_t    mstime;
        uint8_t     enable;
        uint8_t     busy;
        uint32_t    time;
	} sound_t;

    /* Структура управления двигателем */
	typedef struct {
        uint8_t     mode;
        uint8_t     remote;
		uint32_t	time;
        sound_t     sound;
	} logical_t;

    /* # Описание процедур # */

    void logic_init(void);
    void logic_loop(void);
    void logicVoltageControl(void);
    void logicTurnigOff(void);
    void logicSound(uint8_t nSound);
    void logicSoundTime(uint32_t tone, uint32_t mstime);
    void logicSoundDevice(void);
    uint8_t logicRemoteDevice(void);

#endif
