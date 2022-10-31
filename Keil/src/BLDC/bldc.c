// #INDEX# ======================================================================================================
// Title .........: BLDC - Управление двигателем.
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

#include "gd32f1x0.h"
#include "bldc.h"
#include "../framework/board/board.h"
#include "../framework/framework.h"
#include "../device/device.h"
#include "../config/config.h"

#include "table/hall.h"
#include "table/sin.h"
#include "table/meander.h"

/* Структура управления двигателя */
engine_t engine;

/* Структура конфигурации */
extern config_t config;

uint16_t hallTempL;

/* Инициализация */
void engine_init(void) {
	
}

/* Установка таймеров номерам фаз */
void engineMosfetPhase(uint32_t timeA, uint32_t timeB, uint32_t timeC) {
    engine.mosfet.TIMER_A = timeA;
    engine.mosfet.TIMER_B = timeB;
    engine.mosfet.TIMER_C = timeC;
}

/* Установка варианта фазировки */
void enginePhase(uint8_t setTable) {
    switch (setTable) {
        case 0:
            engineMosfetPhase(TIMER_BLDC_A, TIMER_BLDC_B, TIMER_BLDC_C);
            break;
        case 1:
            engineMosfetPhase(TIMER_BLDC_A, TIMER_BLDC_C, TIMER_BLDC_B);
            break;
        case 2:
            engineMosfetPhase(TIMER_BLDC_B, TIMER_BLDC_A, TIMER_BLDC_C);
            break;
        case 3:
            engineMosfetPhase(TIMER_BLDC_B, TIMER_BLDC_C, TIMER_BLDC_A);
            break;
        case 4:
            engineMosfetPhase(TIMER_BLDC_C, TIMER_BLDC_A, TIMER_BLDC_B);
            break;
        case 5:
            engineMosfetPhase(TIMER_BLDC_C, TIMER_BLDC_B, TIMER_BLDC_A);
            break;
    }
}

/* Установка таблицы холлов */
void engineHall(uint8_t setHall) {
    config.hall = setHall;
}

/* Установка таблицы работы фаз */
void engineTable(uint8_t setTable) {
    config.pwm = setTable;
    switch (config.pwm) {
        case ENGINE_PWM_MEANDER:
            engine.driver.tablePtr = tableMeander;
            break;
        case ENGINE_PWM_SIN:
            engine.driver.tablePtr = tableSin; 
            break;
    }
}

/* Установка типом управления двигателем */
void engineControl(uint8_t setControl) {
    config.control = setControl;
}
    
/* Включение / Отключение двигателя */
void engineEnable(uint8_t setEnable) {
    engine.driver.enable = setEnable;
    timerBLDC(TIMER_BLDC_C, engine.driver.enable);
    timerBLDC(TIMER_BLDC_B, engine.driver.enable);
    timerBLDC(TIMER_BLDC_A, engine.driver.enable);
}

/* */
void engineDirection(uint8_t setDirection) {
    config.reverse = setDirection;
}

/* Установка задания (учитывается выбранный тип управления двигателя) */
void engineWrite(uint16_t ewPWM, uint8_t target) {
    if (target == TRUE) ewPWM   = engineTarget(ewPWM);
    engine.input.input          = ewPWM;
}

/* Управление силой торможения (в попугаях от 0 - 100) */
void engineBrake(uint16_t ewPWM, uint8_t target) {
    if (target == TRUE) {
        if (ewPWM > config.brakeMax)    ewPWM = config.brakeMax;
        if (ewPWM < config.brakeMin)    ewPWM = config.brakeMin;
        ewPWM = MAP(ewPWM, config.brakeMin, config.brakeMax, 0, config.brakeCurrent);
    }
    engine.driver.brake     = ewPWM; // <-!
}

/* Получить кол-во оборотов двигателя в мин */
uint16_t engineTurnover(void) {
    return engine.speed.turnover;
}

/* Получить текущее направление двигателя */
uint8_t engineReverse(void) {
    return engine.degree.reverse;
}

/* Получить интеполяцию эл. угла двигателя */
uint16_t engineDegree(void) {
    return engine.degree.degree;
}

/* Получить сигнал с фазы */
int32_t engineMosfet(uint32_t id) {
    if (id == TIMER_BLDC_A) return engine.mosfet.A;
    if (id == TIMER_BLDC_B) return engine.mosfet.B;
    return engine.mosfet.C;
}

/* Получить текущий уровень подачи PWM */
int16_t engineCurrentPWM(void) {
    return engine.input.output;
}

uint8_t engineRun(void) {
	if (engine.speed.run == 0) return 0x00;
	return 0x01;
}

uint8_t engineTick(void) {
	return engine.speed.runs;
}

/* Расчет сигналов с датчиков холла */
void engineEncoder(void) {
    uint8_t hall = devHall();
    if (config.hall == ENGINE_HALL_60)   engine.hall.degree =  tableHall60 [hall];
    if (config.hall == ENGINE_HALL_120)  engine.hall.degree =  tableHall120[hall];
}

/* Расчет входных данных с учетом установленных параметров */
uint16_t engineTarget(uint16_t pwmAnalog)  {
    uint16_t eTarget = 0;
    if (pwmAnalog < config.throttleMin) pwmAnalog = config.throttleMin;
    if (pwmAnalog > config.throttleMax) pwmAnalog = config.throttleMax;
    switch (config.control) {
        case ENGINE_CONTROL_PWM:
            eTarget = MAP(pwmAnalog, config.throttleMin, config.throttleMax, TIMER_BLDC_MID, TIMER_BLDC_MAX);
            break;
        case ENGINE_CONTROL_CURRENT:
            eTarget = MAP(pwmAnalog, config.throttleMin, config.throttleMax, 0, config.current);
            break;
        case ENGINE_CONTROL_TURN:
            eTarget = MAP(pwmAnalog, config.throttleMin, config.throttleMax, 0, config.turnMax);
            break;
    }
    return eTarget;
}

/* Расчет оборотов в секунду */
void engineTurn(void) {
    uint64_t time           = micros();
    engine.speed.limit      = (((((1000 * 1000) / (time - engine.speed.time)) / 6) / config.magnet) * 60);
    engine.speed.time       = time;
}

/* Расчет интерполяции */
void engineInterpolation(void) {
    /* Устанавливаем временную переменную для хранения информации о положение ротора */
    uint32_t iDegree;
    /* Проверяем изменение эл. градусов датчика холлов */
    if (engine.degree.hall.degree != engine.hall.degree) {
        /* Устанавливаем кол-во отсчетов за период */
        engine.degree.count         = engine.degree.tick;
        /* Расчитываем шаг периода с точностью .000f */
        engine.degree.step          = (PWM_BIT_HALL * 1000) / engine.degree.count;
        /* Обнуляем значение линейной интерполяции */
        engine.degree.line          = 0;
        /* Обнуляем значение отсчетов */
        engine.degree.tick          = 0;
        /* Определяем сторону вращения ротора */
        if (engine.degree.hall.degree == 300 && engine.hall.degree == 0) {
            /* Установка стороны вращение ротора вперед */
            engine.degree.reverse = ENGINE_FORWARD;
        }else if(engine.degree.hall.degree == 0 && engine.hall.degree == 300) {
            /* Установка стороны вращение ротора назад */
            engine.degree.reverse = ENGINE_BACKWARD;
        }else if (engine.degree.hall.degree < engine.hall.degree) {
            /* Установка стороны вращение ротора вперед */
            engine.degree.reverse = ENGINE_FORWARD;
        }else{
            /* Установка стороны вращение ротора назад */
            engine.degree.reverse = ENGINE_BACKWARD;
        }
        /* Устанавливаем положение ротора во временную переменную */
        iDegree = engine.hall.degree;
        /* Вызываем процедуру отсчета оборотов */
        engineTurn();
		/* */
		if (engine.speed.run < engine.degree.count) {
			engine.speed.runs = 0x01;
		}else{
			engine.speed.runs = 0x02;
		}
		/* Записываем, мотор в движении, кол-во тиков */
		engine.speed.run = engine.degree.count;
        /* Устанавливаем текущее положение ротора во временную переменную */
        engine.degree.hall.degree = engine.hall.degree;
    }
    /* Расчет линейной интерполяции */
    engine.degree.line    = engine.degree.line + engine.degree.step;
    /* Условие для расчета линейной интерполяции вращения ротора вперед */
    if (engine.degree.reverse == ENGINE_FORWARD) {
        /* Расчет интерполяции в сторону движения ротора */
        iDegree  = engine.hall.degree + (engine.degree.line / 1000);
        /* Условие проверки превышения рассчитываемого эл. угла */
        if (iDegree >= (engine.hall.degree + 60)) {
            /* Условие проверки превышении эл. угла максимального угла в 360 */
            if (iDegree >= 360) {
                /* Устанавливает положение ротора в 0 эл. градус */
                iDegree = 0;
            }else{
                /* Устанавливаем положение ротора макимального угла */
                /* Фактически это приравнивается к сработке ошибок */
                iDegree = (engine.hall.degree + 60);
            }
            /* Блокирование дальнейшего расчета интерполяции, установив шаг интерполяции в 0 */
            engine.degree.step = 0;
			engine.speed.runs = 0;
        }
    }
    /* Условие для расчета линейной интерполяции вращения ротора назад */
    if (engine.degree.reverse == ENGINE_BACKWARD) {
        /* Из-за невозможности работать с 0 эл. углом, принудительно устанавливаем угол 360 */
        if (engine.hall.degree == 0) engine.hall.degree = 360;
        /* Расчет интерполяции в сторону движения ротора */
        iDegree  = engine.hall.degree - (engine.degree.line / 1000);
        /* Условие проверки превышения рассчитываемого эл. угла */
        if (iDegree <= (engine.hall.degree - 60)) {
            /* Устанавливаем положение ротора макимального угла */
            /* Фактически это приравнивается к сработке ошибок */
            iDegree = (engine.hall.degree - 60);
            /* Блокирование дальнейшего расчета интерполяции, установив шаг интерполяции в 0 */
            engine.degree.step = 0;
        }
        /* Условие проверки превышении эл. угла максимального угла в 0 */
        if (iDegree <= 0) {
            /* Устанавливает положение ротора в 360 эл. градус */
            iDegree = 360;
            /* Блокирование дальнейшего расчета интерполяции, установив шаг интерполяции в 0 */
            engine.degree.step = 0;
			engine.speed.runs = 0;
        }
    }
    /* */
    if ((micros() - engine.speed.time) > (100 * 1000)) {
        /* */
        engine.speed.turnover = 0;
        engine.degree.degree = engine.hall.degree;
        engine.degree.tick = 0;
		engine.speed.runs = 0;
    }else{
        /* Задаем в положение ротора в структуру для дальнейшего использования */
        engine.degree.degree = iDegree;
    }
    /* Прибавляем количество отсчетов */
    engine.degree.tick ++;
}

/* Дешифрование угла таблицы */
int8_t engineEncodeTable(int16_t idegress) {
    return (*(engine.driver.tablePtr + idegress));
}

/* Расчет движения по таблице */
void engineMosfetTable(void) {
    int16_t degree = engine.degree.degree;

    degree = degree + 90;

    if (engine.input.output > 0) {
        degree = degree - 30 + engine.driver.fw;
    }else{
        degree = degree + 30 - engine.driver.fw;
    }

    if (degree > 360) degree = (degree - 360);
    engine.mosfet.A = (engineEncodeTable(degree) * engine.input.output / 127); // -

    degree = degree + 120;
    if (degree > 360) degree = (degree - 360);
    engine.mosfet.C = (engineEncodeTable(degree) * engine.input.output / 127); // 0

    degree = degree + 120;
    if (degree > 360) degree = (degree - 360);
    engine.mosfet.B = (engineEncodeTable(degree) * engine.input.output / 127); // +
}

/* Основная процедура работы с двигателем */
void enginePWM(void) {

    engineEncoder();
    engineInterpolation();
    engineMosfetTable();
	
    timer_channel_output_pulse_value_config(TIMER_BLDC, engine.mosfet.TIMER_A, engine.mosfet.A + PWM_TIME_PHASE / 2);
    timer_channel_output_pulse_value_config(TIMER_BLDC, engine.mosfet.TIMER_B, engine.mosfet.B + PWM_TIME_PHASE / 2);
    timer_channel_output_pulse_value_config(TIMER_BLDC, engine.mosfet.TIMER_C, engine.mosfet.C + PWM_TIME_PHASE / 2);
}

/*  */
void engineWriteTimer(void) {
    int16_t eCurrent = devCurrent();
	uint8_t hallLock = FALSE;
	
	if (hallTempL != engine.hall.degree) {
		hallLock = TRUE;
		hallTempL = engine.hall.degree;
	}
	
    /* Стандартное прямое управление */
    /* engine.input.input = от 0 до 1000 */
    /* engine.input.input = PWM */
    if (config.control == ENGINE_CONTROL_PWM) {
        /* Отключен датчик тока */
        if (config.current == 0) {
            /* Вывод напрямую т.к используется управление по PWM */
            engine.input.pwm = engine.input.input;
        }else{
            /* Конттроль торможения */
            if (engine.driver.brake > 0) {
                /* Условие контроля по току */
                if (config.brakeCurrent > 0) {
                    /* Переводим колесо в режим рекуперации */
                    if (eCurrent > -(engine.driver.brake)) {
                        /* Понижаем напряжение */
                        if (engine.input.pwm > 0) engine.input.pwm --;
                    }else{
                        /* Повышаем напряжение */
                        if (engine.input.pwm < TIMER_BLDC_MAX) engine.input.pwm ++;
                    }
                }else{
                    /* Понижаем напряжение */
                    if (engine.input.pwm > engine.input.input) engine.input.pwm --;
                }
            }
            /* Контроль по току и напряжению */
            if (engine.input.input > 0 && engine.driver.brake == 0) {
                /* Контроль по максимальному току */
                if (eCurrent < config.current) {
                    /* Контроль задания */
                    if (engine.input.input > engine.input.pwm) {
                        /* Контроль по максимальному значению PWM */
                        if (engine.input.pwm < TIMER_BLDC_MAX) {
                            /* Контроль по максимально заданому значению */
                            if (engine.input.pwm < engine.input.input) engine.input.pwm ++;
                        }
                    }else{
                        /* Не даем колесу перейти в режим рекуперации */
                        if (eCurrent < 0) {
                            /* Понижаем напряжение */
                            if (engine.input.pwm > engine.input.input) engine.input.pwm --;
                        }
                    }
                }
                /* Контроль по срабатыванию защиты максимального тока */
                if (eCurrent > config.current) {
                    if (engine.input.pwm > TIMER_BLDC_MID) engine.input.pwm --;
                }
            }
            if (engine.driver.brake == 0) {
                /* Условие на инерцию (накат) */
                if (config.inertion == TRUE) {
                    /* Когда на нем катятся (имитация наката) */
                    if (engine.input.input == 0 && eCurrent > 0) {
                        /* Задания нету, ручка отпущена, снижаем PWM */
                        if (engine.input.pwm > TIMER_BLDC_MID) engine.input.pwm --;
                    }
					/* Когда толкают колесо */
					if (engine.input.input == 0 && eCurrent <= 0 && hallLock == TRUE) {
						if (engine.input.pwm < TIMER_BLDC_MAX) engine.input.pwm ++;
					}
                }else{
                    if (engine.input.input < engine.input.pwm) {
                        if (engine.input.pwm > TIMER_BLDC_MID) engine.input.pwm --;
                    }
                }
            }
        }
    }
    /* Управление током */
    /* engine.input.input = от 0 до config.current */
    /* engine.input.input = CURRENT 12 это 1.2 А */
    if (config.control == ENGINE_CONTROL_CURRENT) {
        /* */
        if (config.current == 0) {
            engine.input.pwm = 0;
        }else{
            /* Конттроль торможения */
            if (engine.driver.brake > 0 && engineTurnover() > 0) {
                /* Условие контроля по току */
                if (config.brakeCurrent > 0) {
                    /* Переводим колесо в режим рекуперации */
                    if (eCurrent > -(engine.driver.brake)) {
                        /* Понижаем напряжение */
                        if (engine.input.pwm > TIMER_BLDC_MID) engine.input.pwm --;
                    }
                }else{
                    /* Понижаем напряжение */
                    if (engine.input.pwm > TIMER_BLDC_MID) engine.input.pwm --;
                }
            }else{
                if (engine.input.pwm < 0) {
                    engine.input.pwm = 0;
                }
            }
            if (engine.input.input > 0 && engine.driver.brake == 0) {
                if (eCurrent < engine.input.input) {
                    if (engine.input.pwm < TIMER_BLDC_MAX)  engine.input.pwm ++;
                }
                if (eCurrent > engine.input.input) {
                    if (engine.input.pwm > TIMER_BLDC_MID)  engine.input.pwm --;
                }
            }
            if (engine.driver.brake == 0) {
                if (config.inertion == TRUE) {
                    /* Когда на нем катятся (имитация наката) */
                    if (engine.input.input == 0 && eCurrent > 0) {
                        /* Задания нету, ручка отпущена, снижаем PWM */
                        if (engine.input.pwm > TIMER_BLDC_MID)  engine.input.pwm --;
                    }
                    /* Когда толкают колесо */
                    if (engine.input.input == 0 && eCurrent <= 0 && hallLock == TRUE) {
                        if (engine.input.pwm < TIMER_BLDC_MAX)  engine.input.pwm ++;
                    }
                }else{
                    if (engine.input.pwm > TIMER_BLDC_MID)      engine.input.pwm --;
                }
            }
        }
    }
    /* Управление оборотами */
    /* engine.input.input = от 0 до config.turnMax */
    /* engine.input.input = TURN обороты в минуты, 1000 об/мин */
	/* engineTurnover()   = Обороты двигателя */
    if (config.control == ENGINE_CONTROL_TURN) {
        /* Отключен датчик тока */
        if (config.current == 0) {
            engine.input.pwm = 0;
        }else{
            /* Конттроль торможения */
            if (engine.driver.brake > 0) {
                /* Условие контроля по току */
                if (config.brakeCurrent > 0) {
                    /* Переводим колесо в режим рекуперации */
                    if (eCurrent > -(engine.driver.brake)) {
                        /* Понижаем напряжение */
                        if (engine.input.pwm > 0) engine.input.pwm --;
                    }else{
                        /* Повышаем напряжение */
                        if (engine.input.pwm < TIMER_BLDC_MAX) engine.input.pwm ++;
                    }
                }else{
                    /* Понижаем напряжение */
                    if (engineTurnover() > engine.input.input) {
						if (engine.input.pwm > TIMER_BLDC_MID) engine.input.pwm --;
					}
                }
            }
            /* Контроль по току и напряжению */
            if (engine.input.input > 0 && engine.driver.brake == 0) {
                /* Контроль по максимальному току */
                if (eCurrent < config.current) {
                    /* Контроль задания */
                    if (engine.input.input > engineTurnover()) {
                        /* Контроль по максимальному значению PWM */
                        if (engine.input.pwm < TIMER_BLDC_MAX) engine.input.pwm ++;
                    }else{
                        /* Не даем колесу перейти в режим рекуперации */
                        if (eCurrent < 0) {
                            /* Понижаем напряжение */
                            if (engine.input.pwm > TIMER_BLDC_MID) engine.input.pwm --;
                        }
                    }
                }
                /* Контроль по срабатыванию защиты максимального тока */
                if (eCurrent > config.current) {
                    if (engine.input.pwm > TIMER_BLDC_MID) engine.input.pwm --;
                }
            }
            if (engine.driver.brake == 0) {
                /* Условие на инерцию (накат) */
                if (config.inertion == TRUE) {
                    /* Когда на нем катятся (имитация наката) */
                    if (engine.input.input == 0 && eCurrent > 0) {
                        /* Задания нету, ручка отпущена, снижаем PWM */
                        if (engine.input.pwm > TIMER_BLDC_MID) engine.input.pwm --;
                    }
                    /* Когда толкают колесо */
                    if (engine.input.input == 0 && eCurrent <= 0 && hallLock == TRUE) {
                        if (engine.input.pwm < TIMER_BLDC_MAX) engine.input.pwm ++;
                    }
                }else{
                    if (engine.input.input < engine.input.pwm) {
                        if (engine.input.pwm > TIMER_BLDC_MID) engine.input.pwm --;
                    }
                }
            }
        }
    }
	
	/* Ослабление поля */
	/* engine.driver.fw = Ослабление */
	if (config.fw == TRUE && config.current > 0 && (millis() - engine.input.time) > 50) {
		/* Включаем ослабление поля */
		if (engineTurnover() >= config.fwturn && engine.input.input > 0 && engine.driver.brake == 0) {
			/* Смотрим датчик тока */
			if (eCurrent < config.fwcurrent) {
				if (engine.driver.fw < config.fwcorner) engine.driver.fw ++; 
			}else{
				if (engine.driver.fw > 0) engine.driver.fw --;
			}
		}else{
			if (engine.driver.fw > 0) engine.driver.fw --;
		}
		
		engine.input.time = millis();
	}
    
    engine.input.output = engine.input.pwm;
}

/* */
void engineLimitTimer(void) {
    if (engine.speed.limit < engine.speed.turnover) engine.speed.turnover --;
    if (engine.speed.limit > engine.speed.turnover) engine.speed.turnover ++;
}

/* Событие управления PWM таймером */
void engineEventPWM(void) {
    enginePWM();
}

/* Событие управления TIMER13 */
void engineEventTimer(void) {
    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!! - Отдельное событие */
    engine.input.tick ++;
    if (engine.input.tick < 10) return;
    engine.input.tick = 0;

    engineWriteTimer();
    engineLimitTimer();
    /* !!!!!!!!!!!!!!!!!!!!!!!!! */
    devEventTimer();
}
