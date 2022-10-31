// #INDEX# ======================================================================================================
// Title .........: dataPackage - Управление протоколом.
// Arch ..........: GD32F130C8C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

#include "gd32f1x0.h"
#include "dataPackage.h"
#include "../framework/board/board.h"
#include "../framework/framework.h"
#include "../device/device.h"
#include "../BLDC/bldc.h"
#include "../config/config.h"
#include "../logical/logical.h"

dataPackage_t dataPackage;

extern engine_t engine;
extern config_t config;
extern logical_t logical;

void datap_init(void) {
    dataPackage.status = DATAP_STATUS_OK;
	dataPackage.time   = millis();
}

void datapEvent(uint8_t type) {
    uint8_t dataInc;
    uint8_t size8 = uartAvaliable(type);
    
    if (size8 <= 0) return;

    if (dataPackage.status == DATAP_STATUS_OK) {
        if (uartRead(type) == DATAP_HEADER_PACK) dataPackage.status = DATAP_STATUS_RECV;
        return;
    }
    
    if (dataPackage.status == DATAP_STATUS_RECV) {
        if (size8 >= DATAP_HEADER_SIZE) {
            for (dataInc = 0; dataInc <= (DATAP_HEADER_SIZE - 1); dataInc++) {
                dataPackage.pack[type].data[dataInc] = uartRead(type);
            }

            if (dpCRC8(&dataPackage.pack[type].data[0], DATAP_HEADER_SIZE - 2) == dataPackage.pack[type].data[DATAP_HEADER_SIZE - 1]) {
                dpEvent(type);
                
            }
            
            dataPackage.status = DATAP_STATUS_OK;
        }
    }
    
}

void dpEvent(uint8_t type) {
    switch (dataPackage.pack[type].data[0]) {
        case DATAP_INSTRUCTION_DEBUG:
            /* Параметр отладчика: uint16_t */
            *(uint16_t*)&dataPackage.pack[type].data[1]      = 0;
            /* Параметр отладчика: int16_t */
            *(int16_t *)&dataPackage.pack[type].data[3]      = 0;
            /* Параметр отладчика: uint8_t */
            *(uint8_t *)&dataPackage.pack[type].data[5]      = 0;
            /* Параметр отладчика: int8_t */
            *(int8_t  *)&dataPackage.pack[type].data[6]      = 0;
            /* Параметр отладчика: (float) int16_t */
            *(int16_t *)&dataPackage.pack[type].data[7]      = 0;
            dpSend(type);
            break;
        case DATAP_INSTRUCTION_ADC:
            *(uint16_t*)&dataPackage.pack[type].data[1]      = analogRead(PIN_VBAT);
            *(uint16_t*)&dataPackage.pack[type].data[3]      = analogRead(PIN_CURRENT_DC);
            *(uint16_t*)&dataPackage.pack[type].data[5]      = analogRead(PIN_THROTTLE);
            *(uint16_t*)&dataPackage.pack[type].data[7]      = analogRead(PIN_BRAKE);
            dpSend(type);
            break;
        case DATAP_INSTRUCTION_DEV:
            *(uint8_t *)&dataPackage.pack[type].data[1]      = devHall();
            *(int16_t *)&dataPackage.pack[type].data[2]      = devCurrent();
            *(uint16_t*)&dataPackage.pack[type].data[4]      = devVoltage();
            *(uint16_t*)&dataPackage.pack[type].data[6]      = devThrottle();
            *(uint16_t*)&dataPackage.pack[type].data[8]      = devBrake();
            dpSend(type);
            break;
        case DATAP_INSTRUCTION_BLDC:
            *(uint16_t*)&dataPackage.pack[type].data[1]      = engineTurnover();
            *(uint16_t*)&dataPackage.pack[type].data[3]      = engineDegree();
            *(uint16_t*)&dataPackage.pack[type].data[5]      = engineCurrentPWM();
            *(uint8_t *)&dataPackage.pack[type].data[7]      = engineReverse();
            *(uint8_t *)&dataPackage.pack[type].data[8]      = digitalRead(PIN_HALL_A);
            *(uint8_t *)&dataPackage.pack[type].data[9]      = digitalRead(PIN_HALL_B);
            *(uint8_t *)&dataPackage.pack[type].data[10]     = digitalRead(PIN_HALL_C);
            dpSend(type);
            break;
        case DATAP_INSTRUCTION_MOSFET:
            *(int16_t *)&dataPackage.pack[type].data[1]      = engineMosfet(TIMER_BLDC_A);
            *(int16_t *)&dataPackage.pack[type].data[3]      = engineMosfet(TIMER_BLDC_B);
            *(int16_t *)&dataPackage.pack[type].data[5]      = engineMosfet(TIMER_BLDC_C);
            dpSend(type);
            break;
        case DATAP_INSTRUCTION_READ_CONFIG:
            *(uint32_t*)&dataPackage.pack[type].data[5]      = flashRead(*(uint32_t*)&dataPackage.pack[type].data[1]);
            dpSend(type);
            break;
        case DATAP_INSTRUCTION_WRITE_CONFIG:
            flashWrite(*(uint32_t*)&dataPackage.pack[type].data[1], *(uint32_t*)&dataPackage.pack[type].data[5]);
            dpSend(type);
            break;
        case DATAP_INSTRUCTION_ERASE_FLASH:
            confErase();
            dpSend(type);
            break;
        case DATAP_INSTRUCTION_TURN:
			dataPackage.turn = TRUE;
            break;
        case DATAP_INSTRUCTION_CONTROL:
            /*
                *(uint16_t*)&dataPackage.pack[type].data[1]      [2 byte]    = Управление газом
                *(uint8_t *)&dataPackage.pack[type].data[7]      [1 byte]    = Отправить пакет назад ?
                *(uint16_t*)&dataPackage.pack[type].data[3]      [2 byte]    = Тормоз в А
                *(uint8_t *)&dataPackage.pack[type].data[5]      [1 byte]    = Вкл/Откл двигатель
                *(uint8_t *)&dataPackage.pack[type].data[6]      [1 byte]    = Приоритет управления
                *(uint8_t *)&dataPackage.pack[type].data[8]      [1 byte]    = Сторона движения
            */
            // Установка приортиета управления
            logical.remote = *(uint8_t*)&dataPackage.pack[type].data[6];
            /* Отправка пакета при условии приоритета */
            if (logical.remote == TRUE) {
                // Установка ручки газа
                engineWrite(*(uint16_t*)&dataPackage.pack[type].data[1], FALSE);
                // Установка тормоза
                engineBrake(*(uint16_t*)&dataPackage.pack[type].data[3], FALSE);
                // Установка активации двигателя
                engineEnable(*(uint8_t*)&dataPackage.pack[type].data[5]);
            }
            if (*(uint8_t *)&dataPackage.pack[type].data[7] == TRUE) {
                /* Отправка пакета */
                *(int16_t *)&dataPackage.pack[type].data[1]      = devCurrent();
                *(uint16_t*)&dataPackage.pack[type].data[3]      = devVoltage();
                *(uint16_t*)&dataPackage.pack[type].data[5]      = engineTurnover();
                *(uint16_t*)&dataPackage.pack[type].data[7]      = engineReverse();
                dpSend(type);
            }
            break;
        case DATAP_INSTRUCTION_VERSION:
            *(uint8_t *)&dataPackage.pack[type].data[1]      = CONFIG_BOARD_TYPE;
            *(uint32_t*)&dataPackage.pack[type].data[2]      = FLASH_ADRESS;
            *(uint32_t*)&dataPackage.pack[type].data[6]      = FLASH_MAX;
            dpSend(type);
            break;
        case DATAP_INSTRUCTION_UPDATE_CONFIG:
            confRead();
            logicSoundTime(700, 150);
            dpSend(type);
            break;
		case DATAP_INSTRUCTION_PWM:
			*(int16_t *)&dataPackage.pack[type].data[1]      	 = engine.input.pwm;
			*(int16_t *)&dataPackage.pack[type].data[3]			 = engine.input.input;
			*(uint16_t*)&dataPackage.pack[type].data[5]		 	 = engine.driver.brake;
			dpSend(type);
			break;
    }
}

void dpMaster(uint8_t type) {
	if ((millis() - dataPackage.time) < 100) return;
    *(uint16_t*)&dataPackage.pack[type].data[0] = DATAP_INSTRUCTION_CONTROL;
    *(uint16_t*)&dataPackage.pack[type].data[1] = engine.input.input;
    *(uint16_t*)&dataPackage.pack[type].data[3] = engine.driver.brake;
    *(uint8_t *)&dataPackage.pack[type].data[5] = engine.driver.enable;
    *(uint8_t *)&dataPackage.pack[type].data[6] = TRUE;
    *(uint8_t *)&dataPackage.pack[type].data[7] = FALSE;
    *(uint8_t *)&dataPackage.pack[type].data[8] = ENGINE_FORWARD;
    dpSend(type);
	dataPackage.time = millis();
}

void dpTurn(uint8_t type) {
	*(uint16_t*)&dataPackage.pack[type].data[0] = DATAP_INSTRUCTION_TURN;
	dpSend(type);
}

uint8_t dpGetTurn(void) {
	return dataPackage.turn;
}

void dpSend(uint8_t type) {
    dataPackage.pack[type].data[DATAP_HEADER_SIZE - 1] = dpCRC8(&dataPackage.pack[type].data[0], DATAP_HEADER_SIZE - 2);
    uartWrite(type, DATAP_HEADER_PACK);
    uartWriteBuffer(type, (uint32_t)&dataPackage.pack[type].data[0], sizeof(dPack_t));
}

uint8_t dpCRC8(const uint8_t *addr, uint8_t len) {
    uint8_t i;
    uint8_t mix;
    uint8_t crc = 0;
    uint8_t inbyte;
    while (len--) {
        inbyte = *addr++;
        for (i = 8; i; i--) {
            mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}
