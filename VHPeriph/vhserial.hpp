#ifndef SERIAL_HPP
#define SERIAL_HPP

#include "vhperiph.hpp"

class VHHardwareSerial
{
public:

    VHHardwareSerial();

    bool                begin();
    bool                end();
    void                strout(const char *pstr, bool nl = true);
    void                strout(const uint8_t *pstr, bool nl = true);
    bool                isInitialized();

private:
    xSemaphoreHandle    _semaphore;
    bool                _initialized;
    uint16_t            slen(const char *pstr);
    void                reset();
};

extern VHHardwareSerial Serial;

#endif // SERIAL_HPP
