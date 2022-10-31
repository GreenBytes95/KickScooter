// #INDEX# ======================================================================================================
// Title .........: calibration - Логика режима калибрации.
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

#include "gd32f1x0.h"
#include "calibration.h"
#include "../../../framework/board/board.h"
#include "../../../framework/framework.h"
#include "../../../device/device.h"
#include "../../../config/config.h"
#include "../../../BLDC/bldc.h"
#include "../../../logical/logical.h"

extern config_t config;
/* Структура управления двигателя */
extern engine_t engine;

calibration_t calibration;

void calibration_init(void) {
    /* Устанавливаем переменные в стандартное значение */
    confDefine      ();

    /* Запоминаем режим работы */
    calibration.control = config.control;

    /* Конфигурация двигателя */
    engineHall      (config.hall);
    engineTable     (config.pwm);
    engineControl   (ENGINE_CONTROL_CURRENT);
    engineEnable    (FALSE);

    /* Задаем пределы курка газа и тормоза */
    config.throttleMin 	= PWM_MAX;
    config.throttleMax 	= 0;

    config.brakeMin 	= PWM_MAX;
    config.brakeMax 	= 0;

    calibration.brake = devBrake();
}

void calibration_loop(void) {
    /* Объявление переменных */
    uint8_t     button              = devPress();
    uint16_t    throttle            = devThrottle();
    uint16_t    brake               = devBrake();
    uint8_t     calibrationsNext    = FALSE;
    uint16_t    degreeHall;

    /* Этап первый, подтверждение входа в режим калибровки и установка пределов напряжения */
    if (calibration.stage == CALIBRATION_STAGE_OPEN) {
        if (button == BUTTON_OK) {
            logicSound(2);
            devTurn(OFF);
        }
        if (button == BUTTON_HOLD) {
            calibrationNext();
            button = BUTTON_NONE;
        }
    }

    /* Этап второй, установка пределов курка газа и тормоза */
    if (calibration.stage == CALIBRATION_STAGE_THROTTLE) {

        if (config.throttleMin > throttle)  config.throttleMin  = throttle;
        if (config.throttleMax < throttle)  config.throttleMax  = throttle;

        if (config.brakeMin > brake)        config.brakeMin     = brake;
        if (config.brakeMax < brake)        config.brakeMax     = brake;

        if (button == BUTTON_OK) {
			
			config.throttleMin													= (config.throttleMin 	+ BIT_CALIBRATION);
			if (config.throttleMax > BIT_CALIBRATION) config.throttleMax		= (config.throttleMax 	- BIT_CALIBRATION);
			
			config.brakeMin														= (config.brakeMin 		+ BIT_CALIBRATION);
			if (config.brakeMax > BIT_CALIBRATION) config.brakeMax				= (config.brakeMax 		- BIT_CALIBRATION);
			
            if (config.throttleMin > config.throttleMax) {
                config.throttleMin  = 0;
                config.throttleMax  = 0;
            }
            if (config.brakeMin > config.brakeMax) {
                config.brakeMin     = 0;
                config.brakeMax     = 0;
            }
            if (calibration.brake == brake) {
                config.brakeMin     = 0;
                config.brakeMax     = 0;
            }
            calibrationsNext   = TRUE;
        }
        if (calibrationsNext == TRUE) {
            calibrationNext();
            button = BUTTON_NONE;
        }
    }

    if (calibration.stage == CALIBRATION_STAGE_PHASE) {
        if ( (millis() - calibration.time) > 5000) {
            config.phase ++;
            if (config.phase > 5) config.phase = 0;
            engineEnable(FALSE);
            engineWrite(0, FALSE);
            while (engineTurnover() != 0) delay(100);
            enginePhase(config.phase);
            engineEnable(TRUE);
            engineBrake(0, FALSE);
            engineWrite(20, FALSE);
            calibration.time = millis();
        }

        if ( (millis() - calibration.time) > 1000) {
            if (engineTurnover() == 0) {
                calibration.time = calibration.time - 10000;
            }else{
                degreeHall = engine.hall.degree;
                if (degreeHall != calibration.hall) {
                    if (calibration.tick[0] < 5) {
                        if (degreeHall > calibration.hall) {
                            calibration.tick[0] ++;
                        }
                        if (degreeHall < calibration.hall) {
                            calibration.tick[0] = 0;
                        }
                    }
                    if (calibration.tick[1] < 5) {
                        if (degreeHall < calibration.hall) {
                            calibration.tick[1] ++;
                        }
                        if (degreeHall > calibration.hall) {
                            calibration.tick[1] = 0;
                        }
                    }
                    calibration.hall = degreeHall;
                }
            }
        }

        if ( (millis() - calibration.time) > 1500) {
            if (calibration.tick[0] < 5 && calibration.tick[1] < 5) {
                calibration.time = calibration.time - 10000;
            }
        }
        
        if (button == BUTTON_OK) {
            engineEnable(FALSE);
            engineControl(calibration.control);
            logicSound(1);
            while (engineTurnover() != 0) delay(100);
            confWrite();
            logicSound(4);
            devTurn(OFF);
        }
    }
}

void calibrationNext(void) {
    calibration.stage ++;
    logicSound(calibration.stage);
    if (calibration.stage > CALIBRATION_STAGE_END) calibration.stage = CALIBRATION_STAGE_END;
}
