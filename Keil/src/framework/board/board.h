// #INDEX# ======================================================================================================
// Title .........: Выбор управляющей платы для компиляции проекта
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

#include "../../BLDC/bldc.h"

/* Выбор управляющей платы */
# define PCB_BOARD_0x1A

#ifndef BOARD_H

    #ifdef PCB_BOARD_0x1A
		# include "0x1A.h"
	#endif

	#ifdef PCB_BOARD_0x1B
		# include "0x1B.h"
	#endif

	#ifdef PCB_BOARD_0x2A
		# include "0x2A.h"
	#endif

	#ifdef PCB_BOARD_0x2B
		# include "0x2B.h"
	#endif

	#ifdef PCB_BOARD_0x3A
		# include "0x3A.h"
	#endif

	#ifdef PCB_BOARD_0x3B
		# include "0x3B.h"
	#endif
	
	#ifdef PCB_BOARD_0x4A
		# include "0x4A.h"
	#endif

	#ifdef PCB_BOARD_0x4B
		# include "0x4B.h"
	#endif

	/* # Конфигурация стандартных настроек # */

	/* Расположение фаз */
	# define CONFIG_BOARD_PHASE                                             2
	/* Тип управления */
	# define CONFIG_BOARD_CONTROL                                           ENGINE_CONTROL_PWM
	/* Тип ШИМ подаваемый на двигатель */
	# define CONFIG_BOARD_PWM                                               ENGINE_PWM_SIN
	/* Установка градусность холлов */
	# define CONFIG_BOARD_HALL                                              ENGINE_HALL_120
	/* Инерция двигателя */
	# define CONFIG_BOARD_INERTION                                          TRUE
	/* Режим реверса */
	# define CONFIG_BOARD_REVERSE                                           ENGINE_FORWARD
	/* Ослабление поля */
	# define CONFIG_BOARD_FIELDWEAKING                                      FALSE
	/* Максимальный ток ослабления */
	# define CONFIG_BOARD_FIELDWEAKING_CURRENT                              50
	/* С каких оборотов начнет работать ослабление поля */
	# define CONFIG_BOARD_FIELDWEAKING_TURN                                 500
	/* Максимальный угол опережения */
	# define CONFIG_BOARD_FIELDWEAKING_CORNER                               6
	/* Максимальный фазный ток в А */
	# define CONFIG_BOARD_PHASE_CURRENT                                     100
	/* Максимальный ток в А */
	# define CONFIG_BOARD_CURRENT                                           150
	/* Кол-во магнитов */
	# define CONFIG_BOARD_MAGNET                                            30
	/* Максимальные обороты */
	# define CONFIG_BOARD_TURN                                              4000
	/* Максимальный ток торможения */
	# define CONFIG_BOARD_BRAKE_CURRENT                                     100
	/* Диаметр колеса */
	# define CONFIG_BOARD_WHELL_DIAMETER                                    100
	/* Минимальная позиция аналогово задания в попугаях */
	# define CONFIG_BOARD_THROTTLE_MIN                                      0
	/* Максимальная позиция аналогово задания в попугаях */
	# define CONFIG_BOARD_THROTTLE_MAX                                      4069
	/* Минимальная позиция тормоза в попугаях */
	# define CONFIG_BOARD_BRAKE_MIN                                         0
	/* Максимальная позиция тормоза в попугаях */
	# define CONFIG_BOARD_BRAKE_MAX                                         4069
	/* Минимальное напряжение (отключает двигатель) */
	# define CONFIG_BOARD_VOLTAGE_MIN                                       330
	/* Минимальное напряжение для информарования о разряде АКБ */
	# define CONFIG_BOARD_VOLTAGE_MID                                       340
	/* Максимальное напряжение АКБ, информирование о перезаряде */
	# define CONFIG_BOARD_VOLTAGE_MAX                                       420
	/* Плавность задания аналогово задания */
	# define CONFIG_BOARD_PID_THROTTLE                                      5
	/* Плавность срабатывания торможения */
	# define CONFIG_BOARD_PID_BREAK                                         5

	/* # Конфигурация системы # */

	/* Несущая частота двигателя */
	# define PWM_FREQ														16000
	/* Расчет частоты таймера PWM */
	# define PWM_TIME_PHASE 												72000000 / 2 / PWM_FREQ
	/* Несущая частота таймера */
	# define TIMEOUT_FREQ													16000
	/* Отключение таймера */
	# define TIMEOUT_DEAD   											    48
	/* Время замера тока в мс */
	# define TIMEOUT_CURRENT                                                15
	/* Время замера аналогово задания */
	# define TIMEOUT_THROTTLE                                               50
	/* Время зажатия на кнопку включения */
	# define TIMEOUT_BUTTON_PRESS                                           2500
	/* Время звучания звука после удержания клавиши включения */
	# define TIMEOUT_BUTTON_PRESS_HOLD                                      100
	/* Частота тона звука после удержания клавиши включения */
	# define TONE_BUTTON_PRESS_HOLD                                         1250
	/* Максимальное значение АЦП */
	# define PWM_MAX                                                        4096
	/* Разрядость датчиков холлов в градусах */
	# define PWM_BIT_HALL                                                   60
	/* Гапертезис в попугаях для режима калибровки */
	# define BIT_CALIBRATION												150

	/* Максимальное значение передаваемое в TIMER */
	# define TIMER_BLDC_MAX                                                 (PWM_TIME_PHASE / 2)
	/* Среднее значение передаваемое в TIMER */
	# define TIMER_BLDC_MID                                                 0
	/* Минимальное значение передаваемое в TIMER */
	# define TIMER_BLDC_MIN                                                 -(PWM_TIME_PHASE / 2)

	/* Включение дополнительных UART команд для отладки */
	# define DEBUG_UART
	/* Включение режима отладчика */
	//# define DEBUG_MODE

	/* Включение отладочных режимов */
	/* Методика замера попугаев на 1 А */
	//# define DEBUG_METHOD_CURRENT
	/* Методика замера попугаев на 1 V */
	//# define DEBUG_METHOD_VOLTAGE

	/*
		*   - Методика определения попугаев на 1 V, необходимо вывести значение в попугаях и на ЛБП прибавить или убавить напряжение равное одному вольту.
		**  - Методика определения попугаев на 1 А, необходимо вывести значение в попугаях, повесить резисторную сборку на контур после шунта. На ЛБП настроить 35 Вольт и выставить значение тока больше 2 А.
	*/

#endif
