#include "stm32f1xx_hal.h"

using namespace std;

class DigitalOut
{
public:
    DigitalOut(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
    DigitalOut(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t GPIO_Mode, uint32_t GPIO_Speed);
    ~DigitalOut();
    void write(int value);
    int read();
    DigitalOut& operator= (int value);
    operator int ();
private:
    int state = 0;
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
};
