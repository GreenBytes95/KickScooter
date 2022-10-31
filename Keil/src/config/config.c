// #INDEX# ======================================================================================================
// Title .........: setting - Управление настройками.
// Arch ..........: GD32F130C8C6, GD32F130C6C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// ==============================================================================================================

#include "gd32f1x0.h"
#include "config.h"
#include "../framework/board/board.h"
#include "../framework/framework.h"
#include "../device/device.h"
#include "../BLDC/bldc.h"

config_t config;

/* Инициализация */
void conf_init(void) {
    confRead();
}

/* Задание настроек по умолчанию */
void confDefine(void) {
    config.phase            = CONFIG_BOARD_PHASE;
    config.control          = CONFIG_BOARD_CONTROL;
    config.pwm              = CONFIG_BOARD_PWM;
    config.hall             = CONFIG_BOARD_HALL;
    config.inertion         = CONFIG_BOARD_INERTION;
    config.reverse          = CONFIG_BOARD_REVERSE;
    config.fw               = CONFIG_BOARD_FIELDWEAKING;
    config.fwcurrent        = CONFIG_BOARD_FIELDWEAKING_CURRENT;
    config.fwturn           = CONFIG_BOARD_FIELDWEAKING_TURN;
    config.fwcorner         = CONFIG_BOARD_FIELDWEAKING_CORNER;
    config.phaseCurrent     = CONFIG_BOARD_PHASE_CURRENT;
    config.current          = CONFIG_BOARD_CURRENT;
    config.magnet           = CONFIG_BOARD_MAGNET;
    config.turnMax          = CONFIG_BOARD_TURN;
    config.brakeCurrent     = CONFIG_BOARD_BRAKE_CURRENT;
    config.wheelDiameter    = CONFIG_BOARD_WHELL_DIAMETER;
    config.throttleMin      = CONFIG_BOARD_THROTTLE_MIN;
    config.throttleMax      = CONFIG_BOARD_THROTTLE_MAX;
    config.brakeMin         = CONFIG_BOARD_BRAKE_MIN;
    config.brakeMax         = CONFIG_BOARD_BRAKE_MAX;
    config.voltageMin       = CONFIG_BOARD_VOLTAGE_MIN;
    config.voltageMid       = CONFIG_BOARD_VOLTAGE_MID;
    config.voltageMax       = CONFIG_BOARD_VOLTAGE_MAX;
    config.throttlePID      = CONFIG_BOARD_PID_THROTTLE;
    config.breakPID         = CONFIG_BOARD_PID_BREAK;
    config.bitVoltage       = CONFIG_BOARD_BIT_VBAT;
    config.bitCurrent       = CONFIG_BOARD_BIT_CURRENT;
}

/* Чтение настроек из памяти процессора */
void confRead(void) {
    if (flashRead((FLASH_ADRESS + FLASH_MAX) - (sizeof(config) + 4)) == 0xFFFFFFFF) {
        confDefine();
        confWrite();
    }
    flashReadBuffer((FLASH_ADRESS + FLASH_MAX) - (sizeof(config) + 4), (uint32_t)&config, sizeof(config) - 4);
}

/* Запись настроек в память процесора */
void confWrite(void) {
    /* Отчищаем блок памяти */
    flashErase((FLASH_ADRESS + FLASH_MAX) - (sizeof(config) + 4));
    /* Записываем структуру */
    flashWriteBuffer((FLASH_ADRESS + FLASH_MAX) - (sizeof(config) + 4), (uint32_t)&config, sizeof(config) - 4);
}

void confErase(void) {
    flashErase((FLASH_ADRESS + FLASH_MAX) - (sizeof(config) + 4));
}


