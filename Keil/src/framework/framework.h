// #INDEX# ======================================================================================================
// Title .........: framework - Набор команд для облегчения работы с микроконтроллером.
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

#ifndef FRAMEWORK_H

    #define FRAMEWORK_H

    #include "gd32f1x0.h"
	#include "board/board.h"
    #include <stdarg.h>

	/* # Константы # */

    # define INPUT												GPIO_MODE_INPUT
	# define OUTPUT												GPIO_MODE_OUTPUT
	# define ANALOG												GPIO_MODE_ANALOG


	# define PULL_NULL											GPIO_PUPD_NONE
	# define PULL_NONE											GPIO_PUPD_NONE
	# define PULL_UP											GPIO_PUPD_PULLUP
	# define PULL_DOWN											GPIO_PUPD_PULLDOWN
	
	# define LOW												RESET
	# define HIGH												SET
	# define OFF												RESET
	# define ON													SET
	
	# define TRUE												0x01
	# define FALSE												0x00
	
	# define NULL												0x00

	# define UART_COUNT											2
	# define UART_BUFFERSIZE									32
	# define UART_SPEED											115200

	# define UART0												0
	# define UART1												1

	# define PIN_NULL											0xFF
	
	/* # Математические функции # */

	# define ABS(a) (((a) < 0.0) ? -(a) : (a))
	# define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
	# define MAX(x, high) (((x) > (high)) ? (high) : (x))
	# define MAP(x, xMin, xMax, yMin, yMax) ((x - xMin) * (yMax - yMin) / (xMax - xMin) + yMin)

    enum PIN {
		PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
		PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
		PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
		PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
		PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15
	};

	/* # Структуры # */

	/* Структура UART */
	typedef struct {
		/* Адрес памяти для отправки */
		uint32_t address;
		/* Буфер */
		uint8_t  buffer[UART_BUFFERSIZE + 1];
		/* Прием одного байта, вызывается событием */
		uint8_t	 RX;
		/* Количество байт в буфере */
		uint8_t  count;
	} uartData_t;

	/* Структура звукового генератора */
	typedef struct {
		/* Частота сигнала */
		uint16_t 	tone;
		/* Время */
		uint16_t 	time;
		/* Статус вывода */
		FlagStatus 	toggle;
	} buzzer_t;

	/* Структура АЦП */
	typedef struct {
		uint16_t CHANNEL_0;
		uint16_t CHANNEL_1;
		uint16_t CHANNEL_2;
		uint16_t CHANNEL_3;
		uint16_t CHANNEL_4;
		uint16_t CHANNEL_5;
		uint16_t CHANNEL_6;
		uint16_t CHANNEL_7;
		uint16_t CHANNEL_8;
		uint16_t CHANNEL_9;
	} adc_buf_t;

	typedef struct {
		uint64_t	micstime;
		uint32_t	milstime;
		uint32_t	tick;
	} msticks_t;

	/* # Описание процедур # */

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Определяет port вывода
	// Argument.......: 
	//		uint8_t PIN		= Вываод из таблицы enum PIN;
	// ------------------------------------------------------------
	uint32_t getPort(uint8_t PIN);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Определяет pin вывода
	// Argument.......: 
	//		uint8_t PIN		= Вываод из таблицы enum PIN;
	// ------------------------------------------------------------
	uint32_t getPin(uint8_t PIN);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Определяет канал АЦП вывода, если имеется
	// Argument.......: 
	//		uint8_t PIN		= Вывод из таблицы enum PIN;
	// ------------------------------------------------------------
	uint8_t getChannel(uint8_t PIN);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Устанавливает тип вывода
	// Argument.......: 
	//		uint8_t PIN			= Вывод из таблицы enum PIN;
	//		uint32_t TYPE		= Тип вывода _
	//							INPUT 		- Цифровой вход
	//							OUTPUT 		- Цифровой выход
	//							ANALOG		- Аналоговый вход (АЦП)
	//		uint32_t PUPD_PULL 	= Подтяжка _
	//							PULL_NULL 	- Нет подтяжки
	//							PULL_NONE	- Нет подтяжки
	//							PULL_UP		- Подтяжка к питанию
	//							PULL_DOWN	- Подтяжка к земле
	// Update.........: 20.01.2022 
	// ------------------------------------------------------------
	void pinMode(uint8_t PIN, uint32_t TYPE, uint32_t MODE, uint32_t PUPD_PULL);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Устанавливает бит цифрового вывода
	// Argument.......: 
	//		uint8_t PIN			= Вывод из таблицы enum PIN;
	//		FlagStatus fSTATUS	= Тип вывода _
	//							LOW 	- Минимальный уровень
	//							HIGH 	- Максимальный уровень
	// ------------------------------------------------------------
	void digitalWrite(uint8_t PIN, FlagStatus fSTATUS);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Считывает бит цифрового вывода
	// Argument.......: 
	//		uint8_t PIN			= Вывод из таблицы enum PIN;
	// ------------------------------------------------------------
	FlagStatus digitalRead(uint8_t PIN);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Считывает уровень сиганла с аналогово вывода
	// Argument.......: 
	//		uint8_t PIN			= Вывод из таблицы enum PIN;
	// ------------------------------------------------------------
	uint16_t analogRead(uint8_t PIN);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Время в миллисекундах с момента включения процессора
	// Argument.......: Нет
	// ------------------------------------------------------------
	uint32_t millis(void);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Время в микросекундах с момента включения процессора (отсчет по таймеру PWM)
	// Argument.......: Нет
	// ------------------------------------------------------------
	uint64_t micros(void);
	
	// #FRAMEWORK# ------------------------------------------------
	// Description....: Задержка
	// Argument.......: 
	//		uint32_t dlyTicks	= Значение в миллисекундах
	// ------------------------------------------------------------
	void delay(uint32_t dlyTicks);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Отчистка страницы памяти микропроцессора
	// Argument.......: 
	//		uint32_t address	= Адрес страницы во Flash памяти
	// ------------------------------------------------------------
	void flashErase(uint32_t address);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Считывает 4 байта с памяти микропроцессора
	// Argument.......: 
	//		uint32_t address	= Адрес ячейки;
	// ------------------------------------------------------------
	uint32_t flashRead(uint32_t address);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Записывает 4 байта в память микропроцессора
	// Argument.......: 
	//		uint32_t address	= Адрес ячейки;
	//		uint32_t data		= Значение для записи (4 байта)
	// ------------------------------------------------------------	
	uint8_t flashWrite(uint32_t address, uint32_t data);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Запись буфера памяти в память микропроцессора
	// Argument.......: 
	//		uint32_t address	= Адрес ячейки;
	//		uint32_t pbuffer	= Адрес начала памяти буфера
	//		uint8_t len			= Количество байт буфера
	// ------------------------------------------------------------	
	void flashWriteBuffer(uint32_t address, uint32_t pbuffer, uint8_t len);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Чтение в буфер памяти микропроцессора
	// Argument.......: 
	//		uint32_t address	= Адрес ячейки;
	//		uint32_t pbuffer	= Адрес начала памяти буфера
	//		uint8_t len			= Количество байт буфера
	// ------------------------------------------------------------
	void flashReadBuffer(uint32_t address, uint32_t pbuffer, uint8_t len);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Обнуление данных и установка адреса
	// Argument.......: 
	//		uint8_t type		= Номер UART порта _
	//							UART0, UART1
	//		uint32_t address	= Адрес
	// ------------------------------------------------------------
	void uartClear(uint8_t type, uint32_t address);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Запрос адреса UART
	// Argument.......: 
	//		uint8_t type		= Номер UART порта _
	//							UART0, UART1
	// ------------------------------------------------------------
	uint32_t uartAddress(uint8_t type);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Запись принявшего байта (вызывается событием)
	// Argument.......: 
	//		uint8_t type		= Номер UART порта _
	//							UART0, UART1
	// ------------------------------------------------------------
	void uartEventData(uint8_t type);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Запрос поступивших байт с UART порта
	// Argument.......: 
	//		uint8_t type		= Номер UART порта _
	//							UART0, UART1
	// ------------------------------------------------------------
	uint8_t uartAvaliable(uint8_t type);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Чтение байта с UART порта
	// Argument.......: 
	//		uint8_t type		= Номер UART порта _
	//							UART0, UART1
	// ------------------------------------------------------------
	uint8_t uartRead(uint8_t type);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Передача байта по UART каналу
	// Argument.......: 
	//		uint8_t type		= Номер UART порта _
	//							UART0, UART1
	//		uint8_t data		= Значение байта
	// ------------------------------------------------------------
	void uartWrite(uint8_t type, uint8_t data);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Передача буфера данных UART каналу
	// Argument.......: 
	//		uint8_t type		= Номер UART порта _
	//							UART0, UART1
	//		uint8_t data		= Адрес буфера
	//		uint8_t len			= Количество байт буфера
	// ------------------------------------------------------------
	void uartWriteBuffer(uint8_t type, uint32_t pdata,  uint8_t len);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Передача строки по UART каналу
	// Argument.......: 
	//		uint8_t type		= Номер UART порта _
	//							UART0, UART1
	//		const char* str		= Строка
	// ------------------------------------------------------------
	void uartPrint(uint8_t type, const char* str);

	/* Включение режима отладки */
	#ifdef DEBUG_UART

		// #FRAMEWORK# ------------------------------------------------
		// Description....: Передача строки int значения по UART каналу
		// Argument.......: 
		//		uint8_t type		= Номер UART порта _
		//							UART0, UART1
		//		int uint   			= целое значение для вывода
		// ------------------------------------------------------------
		void uartPrintInt(uint8_t type, int uint);

		// #FRAMEWORK# ------------------------------------------------
		// Description....: Передача строки float значения по UART каналу
		// Argument.......: 
		//		uint8_t type		= Номер UART порта _
		//							UART0, UART1
		//		float ufloat		= значение с плавающей точкой
		// ------------------------------------------------------------
		void uartPrintFloat(uint8_t type, float ufloat);

	#endif

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Генерация воспроизведения звукового тона
	// Argument.......: 
	//		uint32_t tone		= Частота воспроизведения
	// ------------------------------------------------------------
	void buzzerTone(uint32_t tone);

	void deathTime(uint16_t u_deathTime);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Инициализация холлов
	// Argument.......: 
	//		uint8_t pin			= Номер вывода микроконтроллера
	// ------------------------------------------------------------
	void initHall(uint8_t pin);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Инициализация фаз
	// Argument.......: 
	//		uint8_t pin			= Номер вывода микроконтроллера
	// ------------------------------------------------------------
	void initPhase(uint8_t pin);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Инициализация таймера двигателя
	// Argument.......: 
	//		uint32_t times		= Номер таймера
	// ------------------------------------------------------------
	void initTimerBLDC(uint32_t times);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Включение таймера фаз
	// Argument.......: 
	//		uint32_t pin		= Номер таймера
	//		uint8_t ccx			= Включение / Отклюение таймера_
	//			FALSE			= Отключить
	//			TRUE			= Включить
	// ------------------------------------------------------------
	void timerBLDC(uint32_t pin, uint8_t ccx);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Инициализация портов UART
	// Argument.......: 
	//		uint32_t pin		= Номер вывода микроконтроллера
	//		uint32_t gpaf		= Принадлежность AF
	// ------------------------------------------------------------
	void initUartPin(uint8_t pin, uint32_t gpaf);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Инициализация конфигурации UART
	// Argument.......: 
	//		uint32_t pin		= Номер USART0/1
	// ------------------------------------------------------------
	void initUartConfig(uint32_t pin);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Инициализация конфигурации DMA UART
	// Argument.......: 
	//		dma_channel_enum dma= DMA
	//		uint32_t pin		= Номер USART0/1
	// ------------------------------------------------------------
	void initUartDMA(dma_channel_enum dma, uint32_t pin);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Инициализация Framework
	// Argument.......: Нет
	// ------------------------------------------------------------
	void fw_init(void);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Инициализация сторжевого таймера
	// Argument.......: Нет
	// ------------------------------------------------------------
	ErrStatus wd_init(void);

	/* # Процедуры таймеров # */

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Системные прерывания
	// ------------------------------------------------------------
	void SysTick_Handler(void);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Прерывание считывания АЦП
	// ------------------------------------------------------------
	void DMA_Channel0_IRQHandler(void);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Прерывание получение байта по UART0
	// ------------------------------------------------------------
	void DMA_Channel1_2_IRQHandler(void);
	
	// #FRAMEWORK# ------------------------------------------------
	// Description....: Прерывание получение байта по UART1
	// ------------------------------------------------------------
	void DMA_Channel3_4_IRQHandler(void);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Прерывание аппаратного таймера-0
	// ------------------------------------------------------------
	void TIMER0_BRK_UP_TRG_COM_IRQHandler(void);

	// #FRAMEWORK# ------------------------------------------------
	// Description....: Прерывание аппаратного таймера-13
	// ------------------------------------------------------------
	void TIMER13_IRQHandler(void);

#endif
