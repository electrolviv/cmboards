#ifndef VHIENCODER_HPP
#define VHIENCODER_HPP

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

    uint8_t     readpins();

    int32_t     _value;
    bool        _dir;
    uint8_t     _encPosOld;
    void *      _ptrconfig;
    bool        _changed;
};

#endif // VHIENCODER_HPP
