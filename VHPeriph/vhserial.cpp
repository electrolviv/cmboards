#include "vhserial.hpp"

#include "usbd_cdc_if.h"
#include "usb_device.h"


#define USB_DP_PIN          GPIO_PIN_12
#define USB_DP_PORT         GPIOA

#define UART_MUTEX_LOCK()    do {} while (xSemaphoreTake(_semaphore, portMAX_DELAY) != pdPASS)
#define UART_MUTEX_UNLOCK()  xSemaphoreGive(_semaphore)


// global object
VHHardwareSerial Serial;


static const char *pstrnl = "\n";




VHHardwareSerial::VHHardwareSerial() :
    _semaphore(nullptr),
    _initialized(false)
{

}

bool
VHHardwareSerial::begin() {

    _semaphore = xSemaphoreCreateMutex();

    // Reset : DP to GND for 100 ms
    reset();

    /* init code for USB_DEVICE */
    MX_USB_DEVICE_Init();

    _initialized = true;
    return true;
}

bool
VHHardwareSerial::end() {
    return true;
}

// USBD_CDC_ReceivePacket(&hUsbDeviceFS);

void
VHHardwareSerial::strout(const char *pstr, bool nl) {
    UART_MUTEX_LOCK();
    {
        uint16_t ll = slen(pstr);
        while(USBD_BUSY == CDC_Transmit_FS( (uint8_t *)pstr, ll)) {
            osDelay(1);
            // if(result == USBD_OK)   { }
            // else                    { }
        }

        if(nl)  {
            while(USBD_BUSY == CDC_Transmit_FS( (uint8_t *)pstrnl, slen(pstrnl)) ) {
                osDelay(1);
                //        if(result == USBD_OK)   { }
                //        else                    { }
            }
        }

    }
    UART_MUTEX_UNLOCK();
}

void
VHHardwareSerial::strout(const uint8_t *pstr, bool nl) {
    return strout((char *)pstr, nl);
}


bool VHHardwareSerial::isInitialized() {
    return _initialized;
}

uint16_t VHHardwareSerial::slen(const char *pstr)
{
    uint8_t *pptr = (uint8_t *)pstr;
    uint16_t buflen = 0;
    while(*pptr) { buflen++; pptr++; }
    return buflen;
}

void
VHHardwareSerial::reset() {

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure GPIO pin : USB_DP_PORT:USB_DP_PIN */
    GPIO_InitStruct.Pin     = USB_DP_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(USB_DP_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(USB_DP_PORT, USB_DP_PIN, GPIO_PIN_RESET);
    osDelay(100);
}
