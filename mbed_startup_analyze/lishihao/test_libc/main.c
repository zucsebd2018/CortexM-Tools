#include "gpio.h"

IOPin a2(PORT_C,13,OUTPUT,PUSH_PULL);
int main(){
	a2.init();
	while(1){
		uint32_t i=655350;
		while(--i);
		if(a2.read_output()==LOW){
			a2.write(HIGH);
		}
		else{
			a2.write(LOW);
		}
	}
}