
#include "user.h"
#include "mcu_driver.h"
#include "motor_driver.h"

uint16_t hUartSum1 = 0;
uint8_t hUartScopeArr1[60] = {0};

void Printf(unsigned char *str)
{
    static uint8_t bUartScopeCount1 = 0;
    uint8_t strLength = strlen((const char *)str);

    if (bUartScopeCount1 < strLength)
        hUartSum1 += hUartScopeArr1[bUartScopeCount1];
    else if (bUartScopeCount1 == strLength)
        hUartScopeArr1[strLength] = (uint8_t)hUartSum1;

    UART0->THR = hUartScopeArr1[bUartScopeCount1];
    UART0->END = 0x0;

    if (++bUartScopeCount1 >= strLength)
    {
        bUartScopeCount1 = 0;
        hUartSum1 = 0;

        for (uint8_t i = 0; i < strLength; i++)
        {
            hUartScopeArr1[i] = (uint8_t)(str[i]);
        }
    }
}


