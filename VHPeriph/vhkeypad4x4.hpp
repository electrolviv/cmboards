/* (C) V01G04A81 2021 */

#pragma once

#include "vhperiph.hpp"

/**
 * @brief The Key4x4 class \n
 * PA7, PA6, PA5, PA4,   PA3, PA2, PA1, PA0
 */
class Key4x4 {

public:
  /**
   * @brief Default Constructor
   */
  Key4x4();

  /**
   * @brief Keypad Initialization
   */
  void begin();

  /**
   * @brief Call periodically to scan rows
   * ( using internal counter for auto-select )
   */
  void tick();

/**
 * @brief dump hexadecimal value to Serial1 (debug purposes)
 */
#ifdef COMMENT_
  void dump();
#endif

  /**
   * @brief Read actual keyboard states
   * @return bitmask : x16 keys status
   */
  uint16_t read();

private:
  uint16_t _vals;
  uint8_t _cnt;

  uint8_t readpins();

  void setping(int pin);
  void setpinu(int pin);
  void setpind(int pin);
  void setpin(int pin, bool state);
};

/// Global Keypad object
extern Key4x4 keypad4x4;
