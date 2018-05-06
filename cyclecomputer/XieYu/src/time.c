#include "time.h"
#include <stm32f10x.h>

#define TURN_OVEN_TICKS (SystemCoreClock/8)

static int __tick = 0;

static char __gpstime[7] = {0};

static void __delay(int ms);

int timeInit(void)
{
	int ret = SysTick_Config(TURN_OVEN_TICKS);   // turn at one second
	if ( ret == 0 ) {
		/* Select AHB clock(HCLK) divided by 8 as SysTick clock source */
	    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	}
	return ret;
}

void delay(unsigned int ms)
{
    while ( ms > 0 ) {
        unsigned int slice = ms>1000?1000:ms;
        unsigned int begin = SysTick->VAL;
        unsigned int delta = SystemCoreClock/8/1000*slice;
        ms -= slice;
        while ( 1 ) {
            unsigned int now = SysTick->VAL;
            if ( now > begin ) {
            	if ( delta < begin ) {
            		break;
            	}
                delta = delta - begin - (TURN_OVEN_TICKS-now);
                begin = now;
            } 
            if ( begin - now >= delta ) {
                break;
            }
        }
    }
}

int getmtime(void)
{
    unsigned int begin = SysTick->VAL;
    unsigned int ticks = __tick;
    unsigned int now = SysTick->VAL;
    if ( now > begin ) {
        ticks = __tick;
    }
    unsigned int ret = ticks*1000 +
        (TURN_OVEN_TICKS - now)/(SystemCoreClock/8/1000);
    return ret;
}

void setgpstime(const char* stime)
{
    for ( int i=0; i<6; i++ ) {
        __gpstime[i] = stime[i];
    }
}

const char* getgpstime(void)
{
    return __gpstime;
}

void SysTick_Handler(void)
{
    __tick++;
}
