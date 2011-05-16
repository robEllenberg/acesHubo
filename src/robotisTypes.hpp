#ifndef ACES_ROBOTISTYPES_HPP
#define ACES_ROBOTISTYPES_HPP

namespace Robotis {

    enum COMP_TYPE { JOINT=3 };
    enum DIRECTION { UP=1, DOWN };

    enum INST { PING=1, READ, WRITE, REG_WRITE, ACTION, RESET,
                SYNC_WRITE=0x83 };

    enum PARAM_TABLE { 
            MODEL_NUMBER = 0x0,                VERSION_OF_FIRMWARE = 0x2,
            ID = 0x3,                          BAUD_RATE = 0x4,
            RETURN_DELAY_TIME = 0x5,           CW_ANGLE_LIMIT = 0x6,
            CCW_ANGLE_LIMIT = 0x8,             HIGHEST_LIMIT_TEMPERATURE = 0xB,
            LOWEST_LIMIT_VOLTAGE = 0xC,        HIGHEST_LIMIT_VOLTAGE = 0xD,
            MAX_TORQUE = 0xE,                  STATUS_RETURN_LEVEL = 0x10,
            ALARM_LED = 0x11,                  ALARM_SHUTDOWN = 0x12,

            TORQUE_ENABLE = 0x18,              LED = 0x19,
            CW_COMPLIANCE_MARGIN = 0x1A,       CCW_COMPLIANCE_MARGIN = 0x1B,
            CW_COMPLIANCE_SLOPE = 0x1C,        CCW_COMPLIANCE_SLOPE = 0x1D,
            GOAL_POSITION = 0x1E,              MOVING_SPEED = 0x20,
            TORQUE_LIMIT = 0x22,               PRESENT_POSITION = 0x24,
            PRESENT_SPEED = 0x26,              PRESENT_LOAD = 0x28,
            PRESENT_VOLTAGE = 0x2A,            PRESENT_TEMPERATURE = 0x2B,
            REGISTERED_INSTRUCTION = 0x2C,     MOVING = 0x2E,
            LOCK = 0x2F,                       PUNCH = 0x30
    };
};

#endif
