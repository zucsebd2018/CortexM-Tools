## Different ways to implement the serial interface

#### class Serial

The class serial is not very useful actually. It just provides a more convenient interface for the class, `SerialBase`. Part of content in `Serial.cpp` is showed like this:

```c
Serial::Serial(PinName tx, PinName rx, const char *name, int baud) : SerialBase(tx, rx, baud), Stream(name) {}
 
Serial::Serial(PinName tx, PinName rx, int baud): SerialBase(tx, rx, baud), Stream(NULL) {}
```

The constructor invokes the constructor of class `SerialBase`, and transfer the parameters: `tx, rx` and `baud`, `name`.

It can build an instance which can read/write data from file or in other ways.



#### class SerialBase

We can have a look at the constructor of `class SerialBase`:

```c
SerialBase::SerialBase(PinName tx, PinName rx, int baud) :
#if DEVICE_SERIAL_ASYNCH
                                                 _thunk_irq(this), _tx_usage(DMA_USAGE_NEVER),
                                                 _rx_usage(DMA_USAGE_NEVER), _tx_callback(NULL),
                                                 _rx_callback(NULL),
#endif
                                                _serial(), _baud(baud) {
    // No lock needed in the constructor
    for (size_t i = 0; i < sizeof _irq / sizeof _irq[0]; i++) {
        _irq[i] = NULL;
    }
    serial_init(&_serial, tx, rx);
    serial_baud(&_serial, _baud);
    serial_irq_handler(&_serial, SerialBase::_irq_handler, (uint32_t)this);
}
```

It says it doesn't use the DMA now:

 `_tx_usage(DMA_USAGE_NEVER), _rx_usage(DMA_USAGE_NEVER)`

Then it registers an interrupt request handler:

`serial_irq_handler(&_serial, SerialBase::_irq_handler, (uint32_t)this);`

It is because this port may be using **interrupt** to deal with the data transmission/reception. It's just a kind of **initialization**, but NOT really ready. We even don't know the interrupt routine entrance here.  But it binds the instance `this` with the interrupt routine handler.



Actually, the functions named with the capital lowercase letter in "serial", such as `serial_init()`, `serial_baud()` and `serial_irq_handler()` are not in this file. We should have the look at the **HAL**. These functions are defined there.



### About the HAL

HAL is short for "Hardware Abstraction Layer", which represents the closest level to the hardware. We can invoke these functions if we want to finish the complicated tasks associated with the hardware.

All of these configuration functions are in the Hal directories and all of these functions are defined **dependent** on specific hardwares. All of the hardware has the same interface but with different implementation.

![1](D:\ljh\Files\C3\Embedded System\Experiment\W4\img\001.png)

We check the directory **"targets/TARGET_STM/TARGET_STM32F1"**, and go into "**serial_api.h**".

For normal transmission/receptions, we know we use the **"getc"** and **"putc"** interfaces. After tracking along "Serial.cpp", "SerialBase.cpp" and **"serial_device.c"**(in the HAL), we can easily find:

```c
int serial_getc(serial_t *obj)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
 
    while (!serial_readable(obj));
    if (obj_s->databits == UART_WORDLENGTH_8B) {
        return (int)(huart->Instance->DR & (uint8_t)0xFF);
    } else {
        return (int)(huart->Instance->DR & (uint16_t)0x1FF);
    }
}
 
void serial_putc(serial_t *obj, int c)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
 
    while (!serial_writable(obj));
    if (obj_s->databits == UART_WORDLENGTH_8B) {
        huart->Instance->DR = (uint8_t)(c & (uint8_t)0xFF);
    } else {
        huart->Instance->DR = (uint16_t)(c & (uint16_t)0x1FF);
    }
}
```

If you notice `serial_readable()` and `serial_writable()` functions in the while loop, you can say that it uses **polling** to deal with data communicatin.



### What about the interrupt?

Using your text editor or Chrome browser to search **"serial_irq_"**, you can find some functions.

The 1st is `uart_irq(UARTName uart_name)`:

```c
static void uart_irq(UARTName uart_name)
{
    int8_t id = get_uart_index(uart_name);
 
    if (id >= 0) {
        UART_HandleTypeDef * huart = &uart_handlers[id];
        if (serial_irq_ids[id] != 0) {
            if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE) != RESET) {
                if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE) != RESET) {
                    irq_handler(serial_irq_ids[id], TxIrq); // To deal with transmission ints
                }
            }
            if (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) != RESET) {
                if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE) != RESET) {
                    irq_handler(serial_irq_ids[id], RxIrq);	// To deal with reception ints
                    /* Flag has been cleared when reading the content */
                }
            }
            if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET) {
                if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR) != RESET) {
                    volatile uint32_t tmpval __attribute__((unused)) = huart->Instance->DR; // Clear ORE flag
                }
            }
        }
    }
}
```

Obviously its main task is to invoke the interrupt service routines(ISR). But we have two types: The transmission and the reception. When an interrupt occurs, the function `uart_irq()` will be executed, then come into some specific functions.

`irq_handler` is a global function pointer. It points to a specific function, and when an interrupt occurs, the pointed function will be executed.



The 2nd is `serial_irq_handler()`:

```c
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    struct serial_s *obj_s = SERIAL_S(obj);
  
    irq_handler = handler;
    serial_irq_ids[obj_s->index] = id;
}
```

It registers a handler in the *global variable* **irq_handler**: `irq_handler = handler;` and `handler` is actually `SerialBase::_irq_handler`. (In **SerialBase.cpp**)

And the instance base address in a global array serial_irq_ids: `serial_irq_ids[obj_s->index] = id;`

Both of them are useful in the last function we just mentioned.



The 3rd is `serial_irq_set()`:

```c
void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    struct serial_s *obj_s = SERIAL_S(obj);
    UART_HandleTypeDef *huart = &uart_handlers[obj_s->index];
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;
 
#if defined(USART1_BASE)
    if (obj_s->uart == UART_1) {
        irq_n = USART1_IRQn;
        vector = (uint32_t)&uart1_irq;
    }
#endif
 
#if defined(USART2_BASE)
    if (obj_s->uart == UART_2) {
        irq_n = USART2_IRQn;
        vector = (uint32_t)&uart2_irq;
    }
#endif
 
#if defined(USART3_BASE)
    if (obj_s->uart == UART_3) {
        irq_n = USART3_IRQn;
        vector = (uint32_t)&uart3_irq;
    }
#endif
 
    if (enable) {
        if (irq == RxIrq) {
            __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
        } else { // TxIrq
            __HAL_UART_ENABLE_IT(huart, UART_IT_TXE);
        }
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);
 
    } else { // disable
        int all_disabled = 0;
        if (irq == RxIrq) {
            __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
            // Check if TxIrq is disabled too
            if ((huart->Instance->CR1 & USART_CR1_TXEIE) == 0) {
                all_disabled = 1;
            }
        } else { // TxIrq
            __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
            // Check if RxIrq is disabled too
            if ((huart->Instance->CR1 & USART_CR1_RXNEIE) == 0) {
                all_disabled = 1;
            }
        }
 
        if (all_disabled) {
            NVIC_DisableIRQ(irq_n);
        }
    }
}
```

If the 3rd parameter `enable` equals 1, then the interrupt is enabled, and vice versa.



Then we look for these 3 functions back in the **SerialBase.cpp** to check if interrupts are associated with this  implementation. We found the function `attach(func, type)`:

```c
void SerialBase::attach(Callback<void()> func, IrqType type) {
    lock();
    // Disable interrupts when attaching interrupt handler
    core_util_critical_section_enter();
    if (func) {
        // lock deep sleep only the first time
        if (!_irq[type]) {
            sleep_manager_lock_deep_sleep();
        } 
        _irq[type] = func;
        serial_irq_set(&_serial, (SerialIrq)type, 1);
    } else {
        // unlock deep sleep only the first time
        if (_irq[type]) {
            sleep_manager_unlock_deep_sleep();
        } 
        _irq[type] = NULL;
        serial_irq_set(&_serial, (SerialIrq)type, 0);
    }
    core_util_critical_section_exit();
    unlock();
}
```

Therefore if we use this function, we can use the way of **interrupts** to transfer data and finish what we want to do.

