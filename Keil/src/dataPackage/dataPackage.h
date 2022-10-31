// #INDEX# ======================================================================================================
// Title .........: dataPackage - Управление протоколом.
// Arch ..........: GD32F130C8C6
// Author ........: GreenBytes ( https://vk.com/greenbytes )
// Version .......: 3.5.0.0
// Description ...: Пакеты данных
// ==============================================================================================================

/*

    [Отправка ведущего для запроса или записи информации]
    [byte][start]           [0xF0]
    [byte][offset]          [0xF1]

    [byte][команда]         [0xFF]
    [uint32_t][переменная]  [0x00000000]

    [byte][crc8]            [0x45]

    [Ответ ведомого]
    [byte][start]           [0xF1]
    [byte][offset]          [0xF0]

    [byte][команда]         [0xFF]
    [uint32_t][переменная]  [0x00000000]

    [byte][crc8]            [0x45]

    [uint8_t газ/тормоз][uint16_t уровень газа / тормоза][uint8_t в какую сторону крутим мотор]

*/

#ifndef DATAPACKAGE_H

    #define DATAPACKAGE_H

    #include "gd32f1x0.h"
    #include "../framework/board/board.h"
    #include "../framework/framework.h"
    #include "../device/device.h"
    #include "../BLDC/bldc.h"
    #include "../config/config.h"
    #include "../logical/logical.h"

    # define DATAP_STATUS_OK                                0x00
    # define DATAP_STATUS_RECV                              0x01
    # define DATAP_STATUS_SEND                              0x02

    # define DATAP_HEADER_PACK                              0xF0
    # define DATAP_HEADER_SIZE                              12

    # define DATAP_INSTRUCTION_ADC                          0x01
    # define DATAP_INSTRUCTION_DEV                          0x02
    # define DATAP_INSTRUCTION_BLDC                         0x03
    # define DATAP_INSTRUCTION_MOSFET                       0x04
    # define DATAP_INSTRUCTION_DEBUG                        0x05
    # define DATAP_INSTRUCTION_READ_CONFIG                  0x06
    # define DATAP_INSTRUCTION_WRITE_CONFIG                 0x07
    # define DATAP_INSTRUCTION_ERASE_FLASH                  0x08
    # define DATAP_INSTRUCTION_TURN                         0x09
    # define DATAP_INSTRUCTION_CONTROL                      0x0A
    # define DATAP_INSTRUCTION_VERSION                      0x0B
    # define DATAP_INSTRUCTION_UPDATE_CONFIG                0x0C
	# define DATAP_INSTRUCTION_PWM							0x0D

    typedef struct {
        uint8_t data[DATAP_HEADER_SIZE + 1];
    } dPack_t;

    typedef struct {
        uint8_t status;
		uint8_t turn;
		uint32_t time;
        dPack_t pack[UART_COUNT];
	} dataPackage_t;

    void datap_init(void);
    void datapEvent(uint8_t type);
    void dpEvent(uint8_t type);
    void dpMaster(uint8_t type);
	void dpTurn(uint8_t type);
	uint8_t dpGetTurn(void);
    void dpSend(uint8_t type);
    uint8_t dpCRC8(const uint8_t *addr, uint8_t len);

#endif
