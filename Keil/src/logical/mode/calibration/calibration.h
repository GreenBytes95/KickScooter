// #INDEX# ======================================================================================================
// Title .........: calibration - Логика режима калибрации.
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================


#ifndef CALIBRATION_H

    #define CALIBRATION_H

    #include "gd32f1x0.h"
    #include "../../../framework/board/board.h"
    #include "../../../framework/framework.h"
    #include "../../../device/device.h"
    #include "../../../config/config.h"
    #include "../../../BLDC/bldc.h"
    #include "../../../logical/logical.h"

    # define CALIBRATION_STAGE_OPEN                 0x00
    # define CALIBRATION_STAGE_THROTTLE             0x01
    # define CALIBRATION_STAGE_PHASE                0x02

    # define CALIBRATION_STAGE_END                  0x03

    /* Структура */
	typedef struct {
		uint8_t     stage;
        uint8_t     control;
        uint8_t     hall;
        uint16_t    tick[2];
        uint16_t    brake;
        uint32_t    time;
	} calibration_t;

    void calibration_init(void);
    void calibration_loop(void);
    void calibrationNext(void);

#endif
