#include "systick.h"
#include "gpio.h"

static unsigned int current_ms = 0;
static unsigned int ticks = 0;

void MySystick::delay(int ms) {
    unsigned int begin = SysTick->VAL;
    unsigned int delta = SystemCoreClock / 1000 * ms;

    while (1) {
        unsigned int now = SysTick->VAL;
        if ( now > begin ) {
            delta -= begin;
            begin = SysTick_LOAD_RELOAD;
        }
        if ( begin - now >= delta ) {
            break;
        }
    }
}

void MySystick::enable() {
    current_ms = 0;
    SysTick->CTRL = 0;
    SysTick->VAL = 0;
    SysTick->LOAD = SysTick_LOAD_RELOAD;
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}

unsigned int MySystick::getmtime(){
    return current_ms;
}

extern "C" void SysTick_Handler(void) {
    if (ticks % (SystemCoreClock / 1000) == 0) {
        ticks = 0;
        ++current_ms;
    }
    else
        ++ticks;
}