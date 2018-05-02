/**
 * 
*/

#ifndef _UART_H_
#define _UART_H_

enum PARITY {
    NONE, ODD, EVEN
};

class UART {
public:
    /**
     * create a UART object. Do not init this port at this moment.
     * @param port the uart port. Port starts from 1.
    */ 
    UART(int port);

    /**	init port with default parameters: 8n1.
     * @param baud baudrate.
     * @param bits
     * @param parity
     * @param stop
     * @return 0 if fails, otherwise 1.
    */
    int init(int baud = 9600, int bits=8, int parity = NONE, int stop = 1);

    /**
    Prints the content to the uart port.
    The content is to be copied into the buffer of the port.
    The uart will start to transmit when:
        1. The buffer is full, or;
        2. There is either \r or \n in the content.
    Calling to this function will be stalled when:
        1. The DMA mechenism is sending the buffer, then this calling will wait until the sending finished;
        2. The buffer is full and there are more characters left in the content. In this case, a sending
        process will start when the buffer is full and the function waits until the sending finished. After
        that, the rest of the content will be copied into the buffer. If there are either \r or \n in the
        rest part, a second sending process will begin. Whatever there is \r or \n in, the calling will 
        return at that point.
    *DO NOT CALL THIS FUNCTION IN ANY INTERRUPT ROUTINE.*
    @param content the string to be printed.
    @return 0 if fails, otherwise 1.
    */
    void print(const char* line);

    void println(const char* line);

    void printInt(int i);

    /**
    Forces to start the sending process.
    Calling to this function returns immediately without waiting the sending finished.
    */
    void flush();

    /**
     * beings the receive process in DMA mode. 
    */ 
    void rxBegin();
    
    /**
     * returns the byte in receive buffer, but not to cosume it. 
    */ 
    int peek();
    int getchar();
    int hasLine();
    char* readLine(char* line, int len);
};

#endif 