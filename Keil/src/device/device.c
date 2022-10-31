// #INDEX# ======================================================================================================
// Title .........: device - Набор команд для управления платой.
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

#include "gd32f1x0.h"
#include "device.h"
#include "../framework/board/board.h"
#include "../framework/framework.h"
#include "../config/config.h"
#include "../BLDC/bldc.h"

/* Структура датчика тока  */
current_t   current;
/* Структура аналогово задания */
throttle_t  throttle;
/* Структура тормоза */
brake_t     brake;
/* Структура фаз */
phase_t     phase[3];
/* Структура управления двигателя */
extern engine_t engine;
/* Структура конфигурации */
extern config_t config;

/* Датчик напряжения ( Vbat ) */
uint16_t devVoltage(void) {
    return ((analogRead(PIN_VBAT) * 10) / config.bitVoltage);
}

/* Датчик тока */
int16_t devCurrent(void) {
    return current.current;
}

/* Датчик фазы A */
int16_t devPhaseA(void) {
    return phase[0].phase;
}

/* Датчик фазы B */
int16_t devPhaseB(void) {
    return phase[1].phase;
}

/* Датчик фазы C */
int16_t devPhaseC(void) {
    return phase[2].phase;
}

/* Датчик единиц холлов */
uint8_t devHall(void) {
    return (digitalRead(PIN_HALL_A) * 1 + digitalRead(PIN_HALL_B) * 2 + digitalRead(PIN_HALL_C) * 4);
}

/* Кнопка управления */
uint8_t devPress(void) {
    uint32_t time = millis();
    uint8_t pressButton = LOW;
    #ifdef POWER_BUTTON_IO
        if (digitalRead(PIN_POWER) == HIGH) {
            while(digitalRead(PIN_POWER) == HIGH) {
                if (pressButton == LOW) {
                    if ((millis() - time) >= TIMEOUT_BUTTON_PRESS) {
                        buzzerTone(TONE_BUTTON_PRESS_HOLD);
                        delay(TIMEOUT_BUTTON_PRESS_HOLD);
                        buzzerTone(0);
                        pressButton = HIGH;
                    }
                }
                delay(1);
            }
            if (pressButton == HIGH) return BUTTON_HOLD;
            return BUTTON_OK;
        }
    #endif
    #ifdef POWER_BUTTON_ADC
        if (analogRead(PIN_POWER) > 500) {
            while (analogRead(PIN_POWER) > 500) {
                if (pressButton == LOW) {
                    if ((millis() - time) >= TIMEOUT_BUTTON_PRESS) {
                        buzzerTone(TONE_BUTTON_PRESS_HOLD);
                        delay(TIMEOUT_BUTTON_PRESS_HOLD);
                        buzzerTone(0);
                        pressButton = HIGH;
                    }
                }
                delay(1);
            }
            if (pressButton == HIGH) return BUTTON_HOLD;
            return BUTTON_OK;
        }
    #endif
    return BUTTON_NONE;
}

/* Включение платы */
void devTurn(FlagStatus tn) {
    digitalWrite(PIN_SELF_HOLD, tn);
    if (tn == OFF) {
        while (1) {}
    }
}

/* Подача питания на плату (индикатор З/У) */
uint8_t devCharge(void) {
    return digitalRead(PIN_CHARGE_STATE);
}

/* Аналоговое задание */
uint16_t devThrottle(void) {
    return throttle.throttle;
}

/* Тормоз */
uint16_t devBrake(void) {
    return brake.brake;
}

/* PWM задание */
uint16_t devPWM(void) {
}

/* Расчет датчика тока */
void devEventCurrent(void) {
    int16_t devtCurrent;
    if ((millis() - current.time) > TIMEOUT_CURRENT) {
        if (current.balance == 0) {
            current.balance = (current.read / current.tick);
        }
        devtCurrent = (((current.read / current.tick) - current.balance) * 10);
        current.current = (devtCurrent / config.bitCurrent);
        current.read = 0;
        current.tick = 0;
        current.time = millis();
    }
    current.read = current.read + analogRead(PIN_CURRENT_DC);
    current.tick ++;
}

/* Расчет аналогово задания */
void devEventThrottle(void) {
    if ((millis() - throttle.time) > TIMEOUT_THROTTLE) {
        throttle.limit      = (throttle.read / throttle.tick);
        throttle.read       = 0;
        throttle.tick       = 0;
        throttle.time       = millis();
    }
    throttle.read = throttle.read + analogRead(PIN_THROTTLE);
    throttle.tick ++;
}

/* Расчет тормоза */
void devEventBrake(void) {
    if ((millis() - brake.time) > TIMEOUT_THROTTLE) {
        brake.limit      = (brake.read / brake.tick);
        brake.read       = 0;
        brake.tick       = 0;
        brake.time       = millis();
    }
    brake.read = brake.read + analogRead(PIN_BRAKE);
    brake.tick ++;
}

/* Расчет фаз */
void devEventPhase(void) {
}

/* Событие таймера */
void devEventADC(void) {
    devEventCurrent();
    devEventThrottle();
    devEventBrake();
    devEventPhase();
}

/* */
void devEventTimer(void) {
    int16_t throttleL   = throttle.throttle;
    int16_t brakeL      = brake.brake;
    /* Курок газа */
    if (throttle.limit > throttle.throttle) {
        throttleL = throttleL + config.throttlePID;
        if (throttleL > throttle.limit) throttleL = throttle.throttle;
    }
    if (throttle.limit < throttle.throttle) {
        throttleL = throttleL - config.throttlePID;
        if (throttleL < throttle.limit) throttleL = throttle.throttle;
    }
    throttle.throttle = throttleL;

    /* Курок тормоза */
    if (brake.limit > brake.brake) {
        brakeL = brakeL + config.breakPID;
        if (brakeL > brake.limit) brakeL = brake.brake;
    }
    if (brake.limit < brake.brake) {
        brakeL = brakeL - config.breakPID;
        if (brakeL < brake.limit) brakeL = brake.brake;
    }
    brake.brake = brakeL;
}

/* Инициализация */
void dev_init(void) {
    pinMode(PIN_THROTTLE,       ANALOG,     GPIO_OSPEED_10MHZ,  PULL_NONE);
    pinMode(PIN_BRAKE,          ANALOG,     GPIO_OSPEED_10MHZ,  PULL_NONE);
    pinMode(PIN_VBAT,           ANALOG,     GPIO_OSPEED_10MHZ,  PULL_NONE);
    pinMode(PIN_CURRENT_DC,     ANALOG,     GPIO_OSPEED_10MHZ,  PULL_NONE);

    #if (PIN_CURRENT_A != PIN_NULL)
        pinMode(PIN_CURRENT_A,     ANALOG,     GPIO_OSPEED_10MHZ,  PULL_NONE);
    #endif

    #if (PIN_CURRENT_B != PIN_NULL)
        pinMode(PIN_CURRENT_B,     ANALOG,     GPIO_OSPEED_10MHZ,  PULL_NONE);
    #endif

    #if (PIN_CURRENT_C != PIN_NULL)
        pinMode(PIN_CURRENT_C,     ANALOG,     GPIO_OSPEED_10MHZ,  PULL_NONE);
    #endif

    pinMode(PIN_SELF_HOLD,      OUTPUT,     GPIO_OSPEED_10MHZ,  PULL_NULL);

    pinMode(PIN_CHARGE_STATE,   INPUT,      GPIO_OSPEED_10MHZ,  PULL_NONE);

    #ifdef POWER_BUTTON_IO
        pinMode(PIN_POWER,      INPUT,      GPIO_OSPEED_10MHZ,  PULL_NONE);
    #endif
    #ifdef POWER_BUTTON_ADC
        pinMode(PIN_POWER,      ANALOG,      GPIO_OSPEED_10MHZ,  PULL_NONE);
    #endif

}

void dev_loop(void) {
}
