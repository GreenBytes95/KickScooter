// #INDEX# ======================================================================================================
// Title .........: Конфигурация процессора
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

/* # Конфигурация управляющих пинов # */

/* Пин'ы интерфейса UART0 */
/* Пин интерфейса UART RX */
# define PIN_UART0_RX													PB7
/* Пин интерфейса UART TX */
# define PIN_UART0_TX													PB6

/* Пин'ы интерфейса UART1 */
/* Пин интерфейса UART RX */
# define PIN_UART1_RX													PA3
/* Пин интерфейса UART TX */
# define PIN_UART1_TX													PA2

/* Пин'ы интерфейса программирования */
# define PIN_SWDIO                                                      PA13
# define PIN_SWCLK                                                      PA14

/* Управление двигателем (BLDC) */
/* Канал [A] */
# define PIN_BLDC_AH													PA10
# define PIN_BLDC_AL													PB15

/* Канал [B] */
# define PIN_BLDC_BH													PA9
# define PIN_BLDC_BL													PB14

/* Канал [C] */
# define PIN_BLDC_CH													PA8
# define PIN_BLDC_CL													PB13

/* # Конфигурация системы # */

/* Количество Flash памяти */
# define FLASH_MAX                                                      0xFA00
/* Адрес начала Flash памяти */
# define FLASH_ADRESS                                                   0x08000000
/* Адрес начала UART0 */
# define UART0_ADDRESS								                    ((uint32_t)0x40013824)
/* Адрес начала UART1 */
# define UART1_ADDRESS								                    ((uint32_t)0x40004424)

/* Список используемых таймеров */
/* Управление бесщеточным двигателем (BLDC) */
# define TIMER_BLDC 													TIMER0
/* Канал [A] */
# define TIMER_BLDC_A 													TIMER_CH_0
/* Канал [B] */
# define TIMER_BLDC_B 													TIMER_CH_1
/* Канал [C] */
# define TIMER_BLDC_C 													TIMER_CH_2
