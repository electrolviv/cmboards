/* (C) V01G04A81 2021 */

#pragma once

#include "vhperiph.hpp"

class VHIncEncoder
{
public:

    struct cfg {

        uint16_t pina;
        GPIO_TypeDef * porta;

        uint16_t pinb;
        GPIO_TypeDef * portb;

        uint16_t pinz;
        GPIO_TypeDef * portz;

    };

    VHIncEncoder(const cfg *pcfg);

    void        begin();
    void        tick();
    void        end();

    bool        isChanged();
    void        resetevt();
    int32_t     readValue();
    bool        readDirection();

private:

    static const char STATE_INC      = 0;   // 00
    static const char STATE_DEC      = 1;   // 01
    static const char STATE_STABLE   = 2;   // 10
    static const char STATE_INVALID  = 3;   // 11

    static const uint8_t enctbl[4];

    uint8_t     readpins();

    int32_t     _value;
    bool        _dir;
    uint8_t     _encPosOld;
    void *      _ptrconfig;
    bool        _changed;
};
