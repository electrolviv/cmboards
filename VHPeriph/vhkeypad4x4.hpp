#pragma once

#include "vhperiph.hpp"

class Key4x4 {

public:

    void        begin();
    void        tick();
    void        dump();
    uint16_t    read();

private:

    uint16_t    _vals;
    uint8_t     _cnt;

    uint8_t     readpins();

    void        setping(int pin);
    void        setpinu(int pin);
    void        setpind(int pin);
    void        setpin(int pin, bool state);

};

extern Key4x4 keypad4x4;
