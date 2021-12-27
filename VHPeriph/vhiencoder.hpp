/* (C) V01G04A81 2021 */

#pragma once

#include "vhperiph.hpp"

// TODO: Reset on Z

class VHIncEncoder
{
public:
  struct cfg {

    uint16_t pina;       /// Using PIN_A
    GPIO_TypeDef *porta; /// Using PORT_A

    uint16_t pinb;       /// Using PIN_B
    GPIO_TypeDef *portb; /// Using PORT_B

    uint16_t pinz;       /// Using PIN_Z
    GPIO_TypeDef *portz; /// Using PIN_Z
  };

  VHIncEncoder(const cfg *pcfg);

  void begin();
  void tick();
  void end();

  bool isChanged();
  void resetevt();
  int32_t readValue();
  bool readDirection();

private:
  static const char STATE_INC = 0;     // 00
  static const char STATE_DEC = 1;     // 01
  static const char STATE_STABLE = 2;  // 10
  static const char STATE_INVALID = 3; // 11

  static const uint8_t enctbl[4];

  uint8_t readpins();
  void calcpos(uint8_t newpos);

  int32_t _value;
  bool _dir;
  uint8_t _encPosOld;
  void *_ptrconfig;
  bool _changed;
};
