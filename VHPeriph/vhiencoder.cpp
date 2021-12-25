#include "vhiencoder.hpp"



static const int8_t encmap[16] = {

    0,   // 0    0000 STABLE
    1,   // 1    0001 L
    -1,  // 2    0010 R
    0,   // 3    0011 INVALID

    -1,  // 4    0100 R
    0,   // 5    0101 STABLE
    0,   // 6    0110 INVALID
    1,   // 7    0111 L

    1,   // 8    1000 L
    0,   // 9    1001 INVALID
    0,   // 10   1010 STABLE
    -1,  // 11   1011 R

    0,   // 12   1100 INVALID
    -1,  // 13   1101 R
    1,   // 14   1110 L
    0    // 15   1111 STABLE
};


VHIncEncoder::VHIncEncoder(const cfg *pcfg) :
    _value(0), _dir(false), _encPosOld(0),
    _ptrconfig((void *)pcfg), _changed(false) { }


void VHIncEncoder::begin() {

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    cfg * pcfg = (cfg *)_ptrconfig;

    GPIO_InitStruct.Pin     = pcfg->pina;
    GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init((GPIO_TypeDef *)pcfg->porta, &GPIO_InitStruct);

    GPIO_InitStruct.Pin     = pcfg->pinb;
    GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init((GPIO_TypeDef *)pcfg->portb, &GPIO_InitStruct);

    // Wait stable level
    osDelay(1);

    _encPosOld = readpins();
}

void VHIncEncoder::tick() {

    uint8_t encPosCur = readpins();

    // return on same pos
    if(encPosCur == _encPosOld)
        return;

    uint8_t encval = (_encPosOld << 2) | encPosCur;

    _value += encmap[encval];
    _dir    = encmap[encval] < 0;

    _encPosOld = encPosCur;
    _changed = true;
}

void VHIncEncoder::end() {

}

uint8_t VHIncEncoder::readpins() {
    cfg *pcfg = (cfg *)_ptrconfig;
    GPIO_PinState sa = HAL_GPIO_ReadPin((GPIO_TypeDef *)pcfg->porta, pcfg->pina);
    GPIO_PinState sb = HAL_GPIO_ReadPin((GPIO_TypeDef *)pcfg->portb, pcfg->pinb);
    return ((sa == GPIO_PIN_SET) ? 2:0) | ((sb == GPIO_PIN_SET) ? 1:0);
}

bool VHIncEncoder::isChanged() {
    return _changed;
}

void VHIncEncoder::resetevt() {
    _changed = false;
}

int32_t VHIncEncoder::readValue() {
    return _value;
}

bool VHIncEncoder::readDirection() {
    return _dir;
}
