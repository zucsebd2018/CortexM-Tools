/**
*/
#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>
#include <stddef.h>
#include "stm32f10x.h"

enum GPIO_PORT {
    PORT_A, PORT_B, PORT_C, PORT_D
};

enum GPIO_DIRECTION {
    INPUT, OUTPUT
};

enum GPIO_MODE {
    PULL_OPEN, PULL_UP, PULL_DOWN, OPEN_DRAIN, PUSH_PULL
};

enum GPIO_VALUE {
    LOW, HIGH
};

#define NOT_GPIO_PORT(PORT) (((PORT)<PORT_A)||((PORT)>PORT_D))
#define NOT_GPIO_DIRECTION(DIRECTION) ((DIRECTION)!=INPUT&&(DIRECTION)!=OUTPUT)
#define NOT_GPIO_MODE(MODE) ((MODE)<PULL_OPEN||(MODE)>PUSH_PULL)
#define NOT_GPIO_VALUE(VALUE) ((VALUE)!=LOW&&(VALUE)!=HIGH)
#define NOT_GPIO_PIN(PIN) ((PIN)<0||(PIN)>15)

class IOPin {
private:
    int _port, _pin, _direction, _mode;
    static uint16_t pin_used[4];
    static GPIO_TypeDef* GPIOx[4];
public:
    // *
    //  * creates a GPIO pin object. 
    //  * This ctor saves all parameters rather than to initialize this pin at this moment.
    //  * @param port Port name, as defined in GPIO_PORT
    //  * @param pin Pin number, range from 0 to 31
    //  * @param direction Input or output, as defined in GPIO_DIRECTION
    //  * @param mode Pin mode for input/output, as defined in GPIO_MODE
    
    IOPin(int port, int pin, int direction, int mode);

    /**
     * close this pin, release all resources.
     * It may deactivate the port when there are no pins used in this port.
    */ 
    ~IOPin();

    /**
     * inits this pin.
     * It may config and start the GPIO port when this is the first pin used in this port.
     * @return 0 if 
     *  a) this pin has been used by another object; or 
     *  b) this pin is not available in this chip.
     * otherwise returns 1
    */
    int init();

    /**
     * return the value at this pin.
     * @return LOW or HIGH as defined in GPIO_VALUE.
     * Returns LOW always when it has not been inited.
    */ 
    int read();

    /**
     * set the value at this pin.
     * @return 0 when it has not been inited, otherwise 1.
    */ 
    int write(int value);
    /*
    * combine this pin with exti
    * afio will auto enable
    */
    int exti_config();

    /**
    * read current output value
    */
    int read_output();

};

#endif