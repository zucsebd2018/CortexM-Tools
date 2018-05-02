Class MySystick
====

`
Jiang's version
`

This is an interface for you to manipulate SysTick unit in STM32. Following is a series of sample code.

* Instantiate an object of the class

```C
MySystick ms;
```
* Enable the unit

```C
ms.enable();
```

* If you want to delay 500 ms
```C
ms.delay(500);
```

* If you want to get the ms from start-up
```C
int current = ms.getmtime();
```

Here provide a complete test program for you. Simply replace your `main.c`with it. Remember to include `systick.h` if you need it.
```C
#include "gpio.h"
#include "systick.h"

int main(){
	uint16_t i=0;

	MySystick tick;
	tick.enable();
	IOPin a2(PORT_A,2,OUTPUT,PUSH_PULL);
	a2.init();
	//UART u1(1);
	//u1.init();
	int flag=0;
	// char buffer[100];

	while(1){
		
		tick.delay(500);

		if(flag){
			a2.write(LOW);
			flag=0;
		}
		else{
			a2.write(HIGH);
			flag=1;
		}
		// u1.rxBegin();
		// if(u1.hasLine()){
		// 	u1.readLine(buffer,100);
		// 	u1.print(buffer);
		// 	u1.flush();
		// }
	}
	return 0;
}
```

*p.s. Test code will be updated to test `getmtime()` if uart is ready.*
