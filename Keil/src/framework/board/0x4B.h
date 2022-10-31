// #INDEX# ======================================================================================================
// Title .........: Конфигурация платы двухплатника №1
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

#include "CPU/GD32F130C6T6.h"

/* # Конфигурация управляющих пинов # */

/* Список использования Пин'ов микроконтроллера */
/* Пин курка газа (АЦП) */
# define PIN_THROTTLE													PA3
/* Пин курка тормоза */
# define PIN_BRAKE														PA4
/* Пин чтения напряжение питания (АЦП) */
# define PIN_VBAT														PA5
/* Пин чтения датчика тока (АЦП) */
# define PIN_CURRENT_DC													PA6
/* Пин чтения ЭДС фазы А (АЦП) */
# define PIN_CURRENT_A													PA2
/* Пин чтения ЭДС фазы B (АЦП) */
# define PIN_CURRENT_B													PA0
/* Пин чтения ЭДС фазы C (АЦП) */
# define PIN_CURRENT_C													PA1
/* Пин включения устройства */
# define PIN_SELF_HOLD													PC15
/* Пин кнопки включения устройства */
# define PIN_POWER														PC14
/* Пин пищалка */
# define PIN_BUZZER														PA11
/* Пин проверки З/У */
# define PIN_CHARGE_STATE												PA12
/* Пин аварийного выключение двигателя */
# define PIN_BLDC_EMERGENCY_SHUTDOWN									PB12

/* Датчики холла */
# define PIN_HALL_A														PA0
# define PIN_HALL_B														PA1
# define PIN_HALL_C														PA2

/* # Конфигурация индивидуальных настроек # */

/* Идентификация платы */
# define CONFIG_BOARD_TYPE                                              0x1A
/* Количество попугаев на 1 Вольт * */
# define CONFIG_BOARD_BIT_VBAT                                          40
/* Количество попугаев на 1 Ампер ** */
# define CONFIG_BOARD_BIT_CURRENT                                       50

/* Использование UART */
# define UART0_ENA
//# define UART1_ENA

/* Использовать для управления кнопку включения как I/O */
# define POWER_BUTTON_IO
