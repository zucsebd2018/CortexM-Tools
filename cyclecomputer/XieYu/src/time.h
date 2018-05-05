#ifndef _TIME_H_
#define _TIME_H_

int timeInit(void);

void delay(unsigned int ms);
int getmtime(void);
void setgpstime(const char* stime);
const char* getgpstime(void);

#endif