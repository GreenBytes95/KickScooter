// #INDEX# ======================================================================================================
// Title .........: Конфигурация платы двухплатника №2
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

#include "CPU/GD32F130C6T6.h"

/* # Конфигурация управляющих пинов # */

/* Список использования Пин'ов микроконтроллера */
/* Пин курка газа (АЦП) */
# define PIN_THROTTLE													PA0
/* Пин курка тормоза */
# define PIN_BRAKE														PB1
/* Пин чтения напряжение питания (АЦП) */
# define PIN_VBAT														PA4
/* Пин чтения датчика тока (АЦП) */
# define PIN_CURRENT_DC													PA6
/* Пин чтения ЭДС фазы А (АЦП) */
# define PIN_CURRENT_A													PIN_NULL
/* Пин чтения ЭДС фазы B (АЦП) */
# define PIN_CURRENT_B													PIN_NULL
/* Пин чтения ЭДС фазы C (АЦП) */
# define PIN_CURRENT_C													PIN_NULL
/* Пин включения устройства */
# define PIN_SELF_HOLD													PB2
/* Пин кнопки включения устройства */
# define PIN_POWER														PC15
/* Пин пищалка */
# define PIN_BUZZER														PB10
/* Пин проверки З/У */
# define PIN_CHARGE_STATE												PF0
/* Пин аварийного выключение двигателя */
# define PIN_BLDC_EMERGENCY_SHUTDOWN									PB12

/* Датчики холла */
# define PIN_HALL_A														PB11
# define PIN_HALL_B														PF1
# define PIN_HALL_C														PC14

/* # Конфигурация индивидуальных настроек # */

/* Идентификация платы */
# define CONFIG_BOARD_TYPE                                              0x3A
/* Количество попугаев на 1 Вольт * */
# define CONFIG_BOARD_BIT_VBAT                                          40
/* Количество попугаев на 1 Ампер ** */
# define CONFIG_BOARD_BIT_CURRENT                                       3

/* Использование UART */
# define UART0_ENA
# define UART1_ENA

/* Использовать для управления кнопку включения как I/O */
# define POWER_BUTTON_IO
