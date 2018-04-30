/**
 * GPIO 
*/ 
#include "gpio.h"
#define NOT_GPIO_PORT(PORT) (((PORT)<PORT_A)||((PORT)>PORT_D))
#define NOT_GPIO_DIRECTION(DIRECTION) ((DIRECTION)!=INPUT&&(DIRECTION)!=OUTPUT)
#define NOT_GPIO_MODE(MODE) ((MODE)<PULL_OPEN||(MODE)>PUSH_PULL)
#define NOT_GPIO_VALUE(VALUE) ((VALUE)!=LOW&&(VALUE)!=HIGH)
#define NOT_GPIO_PIN(PIN) ((PIN)<0||(PIN)>15)


uint16_t IOPin::pin_used[4]={0,0,0,0};
GPIO_TypeDef* IOPin::GPIOx[4]={NULL,NULL,NULL,NULL};
IOPin::IOPin(int port, int pin, int direction, int mode):_port(port),_pin(pin),_direction(direction),_mode(mode){}
IOPin::~IOPin(){
	pin_used[_port] &= ~((uint16_t)0x01<<_pin);
	if(pin_used[_port]==0){
		RCC->APB2ENR &= ~((uint32_t)(1<<(_port+2)));
		GPIOx[_port]=NULL;
	}
}

int IOPin::init(){
	const uint32_t gpio_speed=1;
	uint32_t tmpreg = 0x00;
	uint32_t tmpmode = 0x00;
	uint32_t pinmask = 0x00;
	uint32_t tmppin = 0x00;
	if(NOT_GPIO_PORT(_port)
		||NOT_GPIO_DIRECTION(_direction)
		||NOT_GPIO_MODE(_mode)
		||NOT_GPIO_PIN(_pin)) return 0;
	if(pin_used[_port]&((uint16_t)0x01<<_pin)) return 0;
	if(GPIOx[_port]==NULL){
		if(_port==PORT_A){
			GPIOx[_port]=GPIOA;
			RCC->APB2ENR|=1<<2;
		}
		else if(_port==PORT_B){
			GPIOx[_port]=GPIOB;
			RCC->APB2ENR|=1<<3;
		}
		else if(_port==PORT_C){
			GPIOx[_port]=GPIOC;
			RCC->APB2ENR|=1<<4;
		}
		else if(_port==PORT_D){
			GPIOx[_port]==GPIOD;
			RCC->APB2ENR|=1<<5;
		}
		else;
	}
	//input mode config
	if(_direction==INPUT){
		if(_mode==PULL_UP){
			tmpmode = 0x48;
		}
		else if(_mode==PULL_DOWN){
			tmpmode = 0x28;
		}
		else if(_mode==PULL_OPEN){
			tmpmode = 0x00;
		}
		else return 0;
	}
	//output mode config
	else{
		if(_mode==OPEN_DRAIN){
			tmpmode = 0x14;
		}
		else if(_mode==PUSH_PULL){
			tmpmode = 0x10;
		}
		else return 0;
		tmpmode+=gpio_speed;
	}
	//config low 8 pin
	if(_pin<8){
		tmpreg = GPIOx[_port]->CRL;
		tmppin = _pin;
	}
	//config high 8 pin
	else{
		tmpreg = GPIOx[_port]->CRH;
		tmppin = _pin-8;
	}
	pinmask = ((uint32_t)0x0F)<<(tmppin<<2);
	tmpreg &= ~pinmask;
	tmpreg |= (tmpmode<<(tmppin<<2));
	if(_mode==PULL_DOWN){
		//write 1 == low value write 0 == no change
		GPIOx[_port]->BRR = (((uint32_t)0x01) << _pin);
	}
	else if(_mode==PULL_UP){
		//write 1 == high value write 0 == no change
		GPIOx[_port]->BSRR = (((uint32_t)0x01) << _pin);
	}
	else;
	if(_pin<8) GPIOx[_port]->CRL = tmpreg;
	else GPIOx[_port]->CRH = tmpreg;
	pin_used[_port] |= ((uint16_t)0x01<<_pin);
}

int IOPin::read(){
	if(pin_used[_port]&((uint16_t)0x01<<_pin)){
		if((GPIOx[_port]->IDR & ((uint32_t)0x01<<_pin))!=(uint32_t)0){
			return HIGH;
		}
	}
	return LOW;
}

int IOPin::write(int value){
	if(NOT_GPIO_VALUE(value)) return 0;
	if(pin_used[_port]&((uint16_t)0x01<<_pin)){
		if(value==HIGH){
			GPIOx[_port]->BSRR = (((uint32_t)0x01) << _pin);
		}
		else{
			GPIOx[_port]->BRR = (((uint32_t)0x01) << _pin);
		}
		return 1;
	}
	return 0;
}

int IOPin::exti_config(){
	RCC->APB2ENR|=0x01;
	uint32_t tmp = 0x00;
  	tmp = ((uint32_t)0x0F) << (0x04 * (_pin & (uint8_t)0x03));
  	AFIO->EXTICR[_pin >> 0x02] &= ~tmp;
  	AFIO->EXTICR[_pin >> 0x02] |= (((uint32_t)_port) << (0x04 * (_pin & (uint8_t)0x03)));
  	return 1;
}

int IOPin::read_output(){
	if(pin_used[_port]&((uint16_t)0x01<<_pin)){
		if((GPIOx[_port]->ODR & ((uint32_t)0x01<<_pin))!=(uint32_t)0){
			return HIGH;
		}
	}
	return LOW;
}
