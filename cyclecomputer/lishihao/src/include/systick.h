#ifndef _SYSTICK_H
#define _SYSTICK_H

#include "stm32f10x.h"

class IOPin;

class MySystick {

public:
    void enable();
    unsigned int getmtime();
    void delay(int);
};

#endif