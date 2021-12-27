#include <stdio.h>

#include "vhkeypad4x4.hpp"
#include "vhserial.hpp"

// Global instance
Key4x4 keypad4x4;

#define KEYPAD4X4_PINSA GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
#define KEYPAD4X4_PINSB GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7
#define KEYPAD4X4_ALLPINS (KEYPAD4X4_PINSA | KEYPAD4X4_PINSB)

Key4x4::Key4x4() : _vals(0), _cnt(0) {}

void Key4x4::begin() {

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = KEYPAD4X4_ALLPINS;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  _cnt = 0;
}

void Key4x4::tick() {

  // Read current row
  _vals &= ~(0xf << (_cnt << 2));     // mask
  _vals |= readpins() << (_cnt << 2); // vals

  // Goto next row
  _cnt = (_cnt + 1) & 3;

  if (_cnt == 0) {
    setping(GPIO_PIN_4);
  } else {
    setpinu(GPIO_PIN_4);
  }
  if (_cnt == 1) {
    setping(GPIO_PIN_5);
  } else {
    setpinu(GPIO_PIN_5);
  }
  if (_cnt == 2) {
    setping(GPIO_PIN_6);
  } else {
    setpinu(GPIO_PIN_6);
  }
  if (_cnt == 3) {
    setping(GPIO_PIN_7);
  } else {
    setpinu(GPIO_PIN_7);
  }
}

uint8_t Key4x4::readpins() {
  uint8_t ret = 0;
  ret |= (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) ? 1 : 0;
  ret |= (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET) ? 2 : 0;
  ret |= (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET) ? 4 : 0;
  ret |= (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) ? 8 : 0;
  return ret;
}

void Key4x4::setping(int pin) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA, pin, GPIO_PIN_RESET);
}

void Key4x4::setpinu(int pin) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

#ifdef COMMENT_
void Key4x4::dump() {
  char tmp[32];
  sprintf(tmp, "Keypad: %.4X", _vals);
  Serial.strout(tmp);
}
#endif
