// #INDEX# ======================================================================================================
// Title .........: framework - Набор команд для работы с микроконтроллером.
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

/* Подключаемые библиотеки */
#include "gd32f1x0.h"
#include "board/board.h"
#include "framework.h"
#include "../device/device.h"
#include "../dataPackage/dataPackage.h"
#include <stdarg.h>
#include <stdio.h>

/* Буфер для работы с UART портами */
uartData_t 		uartData[UART_COUNT];
/* Структура пищалки */
buzzer_t		buzzer;
/* Структура для работы с АЦП */
adc_buf_t 		fw_adc;
/* Таймер */
msticks_t 		msTicks;
/* */
timer_break_parameter_struct 		timerBldc_break_parameter_struct;

/* Определяет port вывода */
uint32_t getPort(uint8_t PIN) {
	if (PIN <= PA15) return GPIOA; //  PIN >= PA0 && 
	if (PIN >= PB0 && PIN <= PB15) return GPIOB;
	if (PIN >= PC0 && PIN <= PC15) return GPIOC; 
	if (PIN >= PD0 && PIN <= PD15) return GPIOD;
	if (PIN >= PF0 && PIN <= PF15) return GPIOF;
	return 0;
}

/* Определяет pin вывода */
uint32_t getPin(uint8_t PIN) {
	if (PIN >= PB0 && PIN <= PB15) PIN = PIN - PB0;
	if (PIN >= PC0 && PIN <= PC15) PIN = PIN - PC0;
	if (PIN >= PD0 && PIN <= PD15) PIN = PIN - PD0;
	if (PIN >= PF0 && PIN <= PF15) PIN = PIN - PF0;
	return BIT(PIN);
}

/* Определяет канал АЦП вывода, если имеется */
uint8_t getChannel(uint8_t PIN) {
	switch (PIN) {
		case PA0:
			return ADC_CHANNEL_0;
		case PA1:
			return ADC_CHANNEL_1;
		case PA2:
			return ADC_CHANNEL_2;
		case PA3:
			return ADC_CHANNEL_3;
		case PA4:
			return ADC_CHANNEL_4;
		case PA5:
			return ADC_CHANNEL_5;
		case PA6:
			return ADC_CHANNEL_6;
		case PA7:
			return ADC_CHANNEL_7;
		case PB0:
			return ADC_CHANNEL_8;
		case PB1:
			return ADC_CHANNEL_9;
	}
	return 0;
}

/* Устанавливает тип вывода */
void pinMode(uint8_t PIN, uint32_t TYPE, uint32_t MODE, uint32_t PUPD_PULL) {
	if (TYPE == OUTPUT) {
		gpio_mode_set(getPort(PIN) , TYPE, PUPD_PULL, getPin(PIN));	
		gpio_output_options_set(getPort(PIN), GPIO_OTYPE_PP, MODE, getPin(PIN));
		return;
	}
	gpio_mode_set(getPort(PIN) , TYPE, PUPD_PULL, getPin(PIN));
}

/* Устанавливает бит цифрового вывода */
void digitalWrite(uint8_t PIN, FlagStatus fSTATUS) {
	gpio_bit_write(getPort(PIN), getPin(PIN), fSTATUS);
}

/* Считывает бит цифрового вывода */
FlagStatus digitalRead(uint8_t PIN) {
	return gpio_input_bit_get(getPort(PIN), getPin(PIN));
}

/* Считывает уровень сиганла с аналогово вывода */
uint16_t analogRead(uint8_t PIN) {
	switch (PIN) {
		case PA0:
			return fw_adc.CHANNEL_0;
		case PA1:
			return fw_adc.CHANNEL_1;
		case PA2:
			return fw_adc.CHANNEL_2;
		case PA3:
			return fw_adc.CHANNEL_3;
		case PA4:
			return fw_adc.CHANNEL_4;
		case PA5:
			return fw_adc.CHANNEL_5;
		case PA6:
			return fw_adc.CHANNEL_6;
		case PA7:
			return fw_adc.CHANNEL_7;
		case PB0:
			return fw_adc.CHANNEL_8;
		case PB1:
			return fw_adc.CHANNEL_9;
	}
	return 0;  
}

/* Время в миллисекундах с момента включения процессора */
uint32_t millis(void) {
	return msTicks.milstime;
}

/* Время в микросекундах с момента включения процессора */
uint64_t micros(void) {
	return msTicks.micstime;
}

/* Задержка */
void delay(uint32_t dlyTicks) {
	uint32_t curTicks = millis();
	fwdgt_counter_reload();
	while ((millis() - curTicks) < dlyTicks) {
		fwdgt_counter_reload();
		__NOP();
	}
}

/* Отчистка страницы памяти микропроцессора */
void flashErase(uint32_t address) {
	fmc_unlock();
	fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR);
	fmc_page_erase(address);
	fmc_lock();
}

/* Считывает 4 байта с памяти микропроцессора */
uint32_t flashRead(uint32_t address) {
	return *(uint32_t*)address;
}

/* Записывает 4 байта в память микропроцессора */
uint8_t flashWrite(uint32_t address, uint32_t data) {
	uint8_t fflash = FALSE;
	fmc_unlock();
	fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR);
	if (fmc_word_program(address, data) == FMC_READY) fflash = TRUE;
	fmc_lock();
	return fflash;
}

/* Запись буфера памяти в память микропроцессора */
void flashWriteBuffer(uint32_t address, uint32_t pbuffer, uint8_t len) {
	int16_t icount = 0;
	while (1) {
		if (flashWrite(address+icount, *(uint32_t*)(pbuffer + icount)) == TRUE) {
			if (icount > len) break;
			icount = icount + 4;
		}
	}
}

/* Чтение в буфер памяти микропроцессора */
void flashReadBuffer(uint32_t address, uint32_t pbuffer, uint8_t len) {
	int16_t icount = 0;
	while (1) {
		*(uint32_t*)(pbuffer + icount) = *(uint32_t*)(address+icount);
		if (icount > len) break;
		icount = icount + 4;
	}
}

/* Обнуление данных и установка адреса */
void uartClear(uint8_t type, uint32_t address) {
	uartData[type].address 	= address;
	uartData[type].count 	= 0;
}

/* Запрос адреса UART */
uint32_t uartAddress(uint8_t type) {
	return uartData[type].address;
}

/* Запись принявшего байта */
void uartEventData(uint8_t type) {
	uartData[type].count ++;
	if (uartData[type].count < UART_BUFFERSIZE) {
		uartData[type].buffer[uartData[type].count - 1] = uartData[type].RX;
	}else{
		uartData[type].count --;
	}
	datapEvent(type);
}

/* Запрос поступивших байт с UART порта */
uint8_t uartAvaliable(uint8_t type) {
	return uartData[type].count;
}

/* Чтение байта с UART порта */
uint8_t uartRead(uint8_t type) {
	uint8_t ret, i;
	if (uartAvaliable(type) == 0) return 0x00;
	ret = uartData[type].buffer[0];
	uartData[type].count --;
	for (i=0; i<=(UART_BUFFERSIZE - 1); i++) {
		uartData[type].buffer[i] = uartData[type].buffer[i + 1];
	}
	return ret;
}

/* Передача байта по UART каналу */
void uartWrite(uint8_t type, uint8_t data) {
	usart_data_transmit(uartData[type].address, data);
	while (usart_flag_get(uartData[type].address, USART_FLAG_TC) == RESET) {}
}

/* Передача буфера данных */
void uartWriteBuffer(uint8_t type, uint32_t pdata,  uint8_t len) {
	uint8_t ucount = 0;
	for (ucount=0;ucount<len;ucount++) {
		usart_data_transmit(uartData[type].address, *(uint8_t*)(pdata + ucount));
		while (usart_flag_get(uartData[type].address, USART_FLAG_TC) == RESET) {}
	}
}

/* Передача строки по UART каналу */
void uartPrint(uint8_t type, const char* str) {
	while(*str != '\0') {
		uartWrite(type, (uint8_t)*str);
		str++;
	}
}

/* Включение режима отладки */
#ifdef DEBUG_UART

	/* Передача строки int значения по UART каналу */
	void uartPrintInt(uint8_t type, int uint) {
		char buffer[16];
		snprintf(buffer, 16, "%d", uint);
		uartPrint(type, buffer);
	}

	/* Передача строки float значения по UART каналу */
	void uartPrintFloat(uint8_t type, float ufloat) {
		char buffer[16];
		snprintf(buffer, 16, "%f", ufloat);
		uartPrint(type, buffer);
	}

#endif

/* Установить тон сигнала */
void buzzerTone(uint32_t tone) {
	buzzer.tone = tone;
}

void deathTime(uint16_t u_deathTime) {
	if (timerBldc_break_parameter_struct.deadtime == u_deathTime) return;
	timerBldc_break_parameter_struct.deadtime = u_deathTime;
	timer_break_config(TIMER_BLDC, &timerBldc_break_parameter_struct);
}

/* Инициализация холлов */
void initHall(uint8_t pin) {
	gpio_mode_set(getPort(pin) , GPIO_MODE_INPUT, GPIO_PUPD_NONE, getPin(pin));
}

/* Инициализация фаз */
void initPhase(uint8_t pin) {
	gpio_mode_set(getPort(pin), GPIO_MODE_AF, GPIO_PUPD_NONE, getPin(pin));
	gpio_output_options_set(getPort(pin), GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, getPin(pin));
	gpio_af_set(getPort(pin), GPIO_AF_2, getPin(pin));
}

/* Инициализация таймера двигателя */
void initTimerBLDC(uint32_t times) {
	timer_channel_output_fast_config			(TIMER_BLDC, times, TIMER_OC_FAST_DISABLE);
	timer_channel_output_shadow_config			(TIMER_BLDC, times, TIMER_OC_SHADOW_DISABLE);
	timer_channel_output_mode_config			(TIMER_BLDC, times, TIMER_OC_MODE_PWM1);
	timer_channel_output_pulse_value_config		(TIMER_BLDC, times, 0);
}

/* Включение таймера фаз */
void timerBLDC(uint32_t pin, uint8_t ccx) {
	if (ccx == TRUE) {
		timer_channel_output_state_config					(TIMER_BLDC, pin, TIMER_CCX_ENABLE);
		timer_channel_complementary_output_state_config		(TIMER_BLDC, pin, TIMER_CCXN_ENABLE);
	}else{
		timer_channel_output_state_config					(TIMER_BLDC, pin, TIMER_CCX_DISABLE);
		timer_channel_complementary_output_state_config		(TIMER_BLDC, pin, TIMER_CCXN_DISABLE);
	}
}

/* Инициализация портов UART */
void initUartPin(uint8_t pin, uint32_t gpaf) {
	gpio_mode_set(getPort(pin) , GPIO_MODE_AF, GPIO_PUPD_PULLUP, getPin(pin));
	gpio_output_options_set(getPort(pin), GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, getPin(pin));
	gpio_af_set(getPort(pin), gpaf, getPin(pin));
}

/* Инициализация конфигурации UART */
void initUartConfig(uint32_t pin) {
	usart_baudrate_set(pin, UART_SPEED);
	usart_parity_config(pin, USART_PM_NONE);
	usart_word_length_set(pin, USART_WL_8BIT);
	usart_stop_bit_set(pin, USART_STB_1BIT);
	usart_oversample_config(pin, USART_OVSMOD_16);
	
	usart_transmit_config(pin, USART_TRANSMIT_ENABLE);
	usart_receive_config(pin, USART_RECEIVE_ENABLE);
	
	usart_enable(pin);
}

/* Инициализация конфигурации DMA UART */
void initUartDMA(dma_channel_enum dma, uint32_t pin) {
	dma_circulation_enable(dma);
	dma_memory_to_memory_disable(dma);

	usart_dma_receive_config(pin, USART_DENR_ENABLE);
	dma_interrupt_enable(dma, DMA_CHXCTL_FTFIE);
	dma_transfer_number_config(dma, 1);
	dma_channel_enable(dma);
}

/* Инициализация */
void fw_init(void) {
	/* Локальные структуры для инициализации */
	dma_parameter_struct 				dma_init_struct_usart;
	dma_parameter_struct 				dma_init_struct_adc;
	timer_parameter_struct 				timerBldc_paramter_struct;
	timer_parameter_struct 				timeoutTimer_paramter_struct;
	timer_oc_parameter_struct 			timerBldc_oc_parameter_struct;

	/* Сторжевой таймер */
	if (wd_init() == ERROR) while(1);

	/* Установка системных прерываний */
	SystemCoreClockUpdate();
  	SysTick_Config(SystemCoreClock / 1000000);
	
	nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);

	/* Инициализация RCU */
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_GPIOF);

	/* Установка холлов */
	initHall(PIN_HALL_A);
	initHall(PIN_HALL_B);
	initHall(PIN_HALL_C);

	/* Инициализация аварийного отключения */
	gpio_mode_set(getPort(PIN_BLDC_EMERGENCY_SHUTDOWN) , GPIO_MODE_AF, GPIO_PUPD_NONE, getPin(PIN_BLDC_EMERGENCY_SHUTDOWN));
	gpio_af_set(getPort(PIN_BLDC_EMERGENCY_SHUTDOWN), GPIO_AF_2, getPin(PIN_BLDC_EMERGENCY_SHUTDOWN));

	/* Инициализация фаз */
	initPhase(PIN_BLDC_CH);
	initPhase(PIN_BLDC_BH);
	initPhase(PIN_BLDC_AH);
	initPhase(PIN_BLDC_CL);
	initPhase(PIN_BLDC_BL);
	initPhase(PIN_BLDC_AL);
	//pinMode(PIN_BLDC_CL,      OUTPUT,     GPIO_OSPEED_10MHZ,  PULL_NULL);
	//pinMode(PIN_BLDC_BL,      OUTPUT,     GPIO_OSPEED_10MHZ,  PULL_NULL);
	//pinMode(PIN_BLDC_AL,      OUTPUT,     GPIO_OSPEED_10MHZ,  PULL_NULL);

	/* Инициализация RCU */
	rcu_periph_clock_enable(RCU_DMA);
	rcu_periph_clock_enable(RCU_ADC);
	rcu_periph_clock_enable(RCU_DMA);

	/* Инициализация АЦП */
	rcu_adc_clock_config(RCU_ADCCK_APB2_DIV8);
	nvic_irq_enable(DMA_Channel0_IRQn, 1, 0);
	dma_deinit(DMA_CH0);
	dma_init_struct_adc.direction 							= DMA_PERIPHERAL_TO_MEMORY;
	dma_init_struct_adc.memory_addr 						= (uint32_t)&fw_adc;
	dma_init_struct_adc.memory_inc 							= DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct_adc.memory_width 						= DMA_MEMORY_WIDTH_16BIT;
	dma_init_struct_adc.number 								= 10;
	dma_init_struct_adc.periph_addr 						= (uint32_t)&ADC_RDATA;
	dma_init_struct_adc.periph_inc 							= DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct_adc.periph_width 						= DMA_PERIPHERAL_WIDTH_16BIT;
	dma_init_struct_adc.priority 							= DMA_PRIORITY_ULTRA_HIGH;
	dma_init(DMA_CH0, &dma_init_struct_adc);
	dma_circulation_enable(DMA_CH0);
	dma_memory_to_memory_disable(DMA_CH0);
	dma_interrupt_enable(DMA_CH0, DMA_CHXCTL_FTFIE);
	dma_transfer_number_config(DMA_CH0, 10);
	dma_channel_enable(DMA_CH0);
	adc_channel_length_config(ADC_REGULAR_CHANNEL, 10);
	adc_regular_channel_config(0, ADC_CHANNEL_0, ADC_SAMPLETIME_1POINT5);
	adc_regular_channel_config(1, ADC_CHANNEL_1, ADC_SAMPLETIME_1POINT5);
	adc_regular_channel_config(2, ADC_CHANNEL_2, ADC_SAMPLETIME_1POINT5);
	adc_regular_channel_config(3, ADC_CHANNEL_3, ADC_SAMPLETIME_1POINT5);
	adc_regular_channel_config(4, ADC_CHANNEL_4, ADC_SAMPLETIME_1POINT5);
	adc_regular_channel_config(5, ADC_CHANNEL_5, ADC_SAMPLETIME_1POINT5);
	adc_regular_channel_config(6, ADC_CHANNEL_6, ADC_SAMPLETIME_1POINT5);
	adc_regular_channel_config(7, ADC_CHANNEL_7, ADC_SAMPLETIME_1POINT5);
	adc_regular_channel_config(8, ADC_CHANNEL_8, ADC_SAMPLETIME_1POINT5);
	adc_regular_channel_config(9, ADC_CHANNEL_9, ADC_SAMPLETIME_1POINT5);
	adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
	adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);
	adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);// ADC_EXTTRIG_INSERTED_T0_TRGO);
	adc_tempsensor_vrefint_disable();
	adc_vbat_disable();
	adc_watchdog_disable();
	adc_enable();
	adc_calibration_enable();
	adc_dma_mode_enable();
	adc_special_function_config(ADC_SCAN_MODE, ENABLE);

	/* Инициализация таймера PWM двигателя */
	rcu_periph_clock_enable(RCU_TIMER0);
	timer_deinit(TIMER_BLDC);
	timerBldc_paramter_struct.counterdirection 				= TIMER_COUNTER_UP;
	timerBldc_paramter_struct.prescaler 					= 0;
	timerBldc_paramter_struct.alignedmode 					= TIMER_COUNTER_CENTER_DOWN;
	timerBldc_paramter_struct.period						= 72000000 / 2 / PWM_FREQ;
	timerBldc_paramter_struct.clockdivision 				= TIMER_CKDIV_DIV1;
	timerBldc_paramter_struct.repetitioncounter 			= 0;
	timer_auto_reload_shadow_disable(TIMER_BLDC);
	timer_init(TIMER_BLDC, &timerBldc_paramter_struct);

	initTimerBLDC(TIMER_BLDC_C);
	initTimerBLDC(TIMER_BLDC_B);
	initTimerBLDC(TIMER_BLDC_A);

	timerBldc_oc_parameter_struct.ocpolarity 				= TIMER_OC_POLARITY_HIGH;
	timerBldc_oc_parameter_struct.ocnpolarity 				= TIMER_OCN_POLARITY_LOW;
	timerBldc_oc_parameter_struct.ocidlestate 				= TIMER_OC_IDLE_STATE_LOW;
	timerBldc_oc_parameter_struct.ocnidlestate 				= TIMER_OCN_IDLE_STATE_HIGH;

	timer_channel_output_config(TIMER_BLDC, TIMER_BLDC_C, &timerBldc_oc_parameter_struct);
  	timer_channel_output_config(TIMER_BLDC, TIMER_BLDC_B, &timerBldc_oc_parameter_struct);
	timer_channel_output_config(TIMER_BLDC, TIMER_BLDC_A, &timerBldc_oc_parameter_struct);

	timerBldc_break_parameter_struct.runoffstate			= TIMER_ROS_STATE_ENABLE;
	timerBldc_break_parameter_struct.ideloffstate 			= TIMER_IOS_STATE_ENABLE;
	timerBldc_break_parameter_struct.protectmode			= TIMER_CCHP_PROT_OFF;
	timerBldc_break_parameter_struct.deadtime 				= TIMEOUT_DEAD;
	timerBldc_break_parameter_struct.breakstate				= TIMER_BREAK_ENABLE;
	timerBldc_break_parameter_struct.breakpolarity			= TIMER_BREAK_POLARITY_LOW;
	timerBldc_break_parameter_struct.outputautostate 		= TIMER_OUTAUTO_ENABLE;
	timer_break_config(TIMER_BLDC, &timerBldc_break_parameter_struct);
	timer_disable(TIMER_BLDC);

	timerBLDC(TIMER_BLDC_C, FALSE);
	timerBLDC(TIMER_BLDC_B, FALSE);
	timerBLDC(TIMER_BLDC_A, FALSE);

	nvic_irq_enable(TIMER0_BRK_UP_TRG_COM_IRQn, 0, 0);
	timer_interrupt_enable(TIMER_BLDC, TIMER_INT_UP);
	timer_enable(TIMER_BLDC);
	//
	pinMode(PIN_BUZZER, 		OUTPUT, 	GPIO_OSPEED_50MHZ, PULL_NONE);
	rcu_periph_clock_enable(RCU_TIMER13);
	timer_deinit(TIMER13);
	timeoutTimer_paramter_struct.counterdirection 			= TIMER_COUNTER_UP;
	timeoutTimer_paramter_struct.prescaler 					= 0;
	timeoutTimer_paramter_struct.alignedmode 				= TIMER_COUNTER_CENTER_DOWN;
	timeoutTimer_paramter_struct.period						= 72000000 / 2 / TIMEOUT_FREQ;
	timeoutTimer_paramter_struct.clockdivision 				= TIMER_CKDIV_DIV4;
	timeoutTimer_paramter_struct.repetitioncounter 			= 0;
	timer_auto_reload_shadow_disable(TIMER13);
	timer_init(TIMER13, &timeoutTimer_paramter_struct);
	nvic_irq_enable(TIMER13_IRQn, 0, 0);
	timer_interrupt_enable(TIMER13, TIMER_INT_UP);
	timer_enable(TIMER13);

	#ifdef UART0_ENA
		/* UART 0 */
		initUartPin(PIN_UART0_TX, GPIO_AF_0);
		initUartPin(PIN_UART0_RX, GPIO_AF_0);
		
		rcu_periph_clock_enable(RCU_USART0);
		rcu_periph_clock_enable(RCU_DMA);
		
		initUartConfig(USART0);
		
		nvic_irq_enable(DMA_Channel1_2_IRQn, 2, 0);
		
		dma_deinit(DMA_CH2);
		dma_init_struct_usart.direction = DMA_PERIPHERAL_TO_MEMORY;
		dma_init_struct_usart.memory_addr = (uint32_t)&uartData[UART0].RX;
		dma_init_struct_usart.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
		dma_init_struct_usart.memory_width = DMA_MEMORY_WIDTH_8BIT;
		dma_init_struct_usart.number = 1;
		dma_init_struct_usart.periph_addr = UART0_ADDRESS;
		dma_init_struct_usart.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
		dma_init_struct_usart.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
		dma_init_struct_usart.priority = DMA_PRIORITY_ULTRA_HIGH;
		dma_init(DMA_CH2, &dma_init_struct_usart);
		
		initUartDMA(DMA_CH2, USART0);
	#endif

	#ifdef UART1_ENA
		/* UART 1 */
		initUartPin(PIN_UART1_TX, GPIO_AF_1);
		initUartPin(PIN_UART1_RX, GPIO_AF_1);

		/* UART 1 */

		rcu_periph_clock_enable(RCU_USART1);
		rcu_periph_clock_enable(RCU_DMA);

		initUartConfig(USART1);

		nvic_irq_enable(DMA_Channel3_4_IRQn, 2, 0);

		dma_deinit(DMA_CH4);

		dma_init_struct_usart.memory_addr = (uint32_t)&uartData[UART1].RX;
		dma_init_struct_usart.periph_addr = UART1_ADDRESS;
		dma_init(DMA_CH4, &dma_init_struct_usart);

		initUartDMA(DMA_CH4, USART1);
				
	#endif

	/* Конец */

	uartClear(UART0, USART0);
	uartClear(UART1, USART1);
}

/* Инициализация сторжевого таймера */
ErrStatus wd_init(void) {
	if (RESET != rcu_flag_get(RCU_FLAG_FWDGTRST)) {
		rcu_all_reset_flag_clear();
	}
	if (fwdgt_config(0x0FFF, FWDGT_PSC_DIV16) != SUCCESS ||
		fwdgt_window_value_config(0x0FFF) != SUCCESS) {
		return ERROR;
	}
	fwdgt_enable();
	return SUCCESS;
}

/* Системные прерывания */
void SysTick_Handler(void) {
	if (msTicks.tick >= 1000) {
		msTicks.milstime ++;
		msTicks.tick = 0;
	}
	msTicks.tick ++;
	msTicks.micstime ++;
}

/* Прерывание считывания АЦП */
void DMA_Channel0_IRQHandler(void) {
	if (dma_interrupt_flag_get(DMA_CH0, DMA_INT_FLAG_FTF)) {
		devEventADC();
		dma_interrupt_flag_clear(DMA_CH0, DMA_INT_FLAG_FTF);        
	}
}

/* Прерывание получение байта по UART0 */
void DMA_Channel1_2_IRQHandler(void) {
	if (dma_interrupt_flag_get(DMA_CH2, DMA_INT_FLAG_FTF)) {
		uartEventData(UART0);
		dma_interrupt_flag_clear(DMA_CH2, DMA_INT_FLAG_FTF);        
	}
}

/* Прерывание получение байта по UART1 */
void DMA_Channel3_4_IRQHandler(void) {
	if (dma_interrupt_flag_get(DMA_CH4, DMA_INT_FLAG_FTF)) {
		uartEventData(UART1);
		dma_interrupt_flag_clear(DMA_CH4, DMA_INT_FLAG_FTF);        
	}
}

/* Прерывание аппаратного таймера-0 */
void TIMER0_BRK_UP_TRG_COM_IRQHandler(void) {
	adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
	engineEventPWM();
	timer_interrupt_flag_clear(TIMER_BLDC, TIMER_INT_UP);
}

/* Прерывание аппаратного таймера-13 */
void TIMER13_IRQHandler(void) {
	if (buzzer.tone != 0) {
		buzzer.time++;
		if (buzzer.time % (buzzer.tone / (TIMEOUT_FREQ / 100)) == 0) {
			buzzer.toggle = buzzer.toggle == LOW ? HIGH : LOW;
			digitalWrite(PIN_BUZZER, buzzer.toggle);
		}
	}else{
		digitalWrite(PIN_BUZZER, LOW);
	}
	engineEventTimer();
	timer_interrupt_flag_clear(TIMER13, TIMER_INT_UP);
}
