#include "stm32f1xx_hal.h"

using namespace std;

class DigitalIn
{
public:
    DigitalIn(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t GPIO_Pull);
    ~DigitalIn();
    int read();
    operator int ();
private:
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
};
