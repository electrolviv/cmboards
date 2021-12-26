#include "vhiencoder.hpp"

#define BYTE4B2(A,B,C,D) ( (A<<6)|(B<<4)|(C<<2)|(D<<0) )


const uint8_t VHIncEncoder::enctbl[] = {

    BYTE4B2( STATE_STABLE  , STATE_DEC     , STATE_INC     , STATE_INVALID   ),
    BYTE4B2( STATE_INC     , STATE_STABLE  , STATE_INVALID , STATE_DEC       ),
    BYTE4B2( STATE_DEC     , STATE_INVALID , STATE_STABLE  , STATE_INC       ),
    BYTE4B2( STATE_INVALID , STATE_INC     , STATE_DEC     , STATE_STABLE    ),

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

    // Read PIN A/B levels
    uint8_t encPosCur = readpins();

    // Return on same pos
    if(encPosCur == _encPosOld)
        return;

    // Walt through table
    uint8_t tblbyte = enctbl[_encPosOld];
    uint8_t tblval  = ( tblbyte >> ( encPosCur << 1 ) );

    if(!(tblval & 2)) {
        if(tblval & 1)  { _value--; _dir=false; }
        else            { _value++; _dir=true;  }
        _changed = true;
    }

    _encPosOld = encPosCur;

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
