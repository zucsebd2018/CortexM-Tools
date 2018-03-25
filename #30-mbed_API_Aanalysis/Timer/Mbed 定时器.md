# Mbed 定时器
Mbed一共提供了三种定时器，各有各的用途，并且每一种计时器都没有个数限制，在性能允许的情况下可以同时创建的计时器不限数量。
## 计时器
### Include
`#include <Timer.h>`

实际上只需包含mbed.h即可使用
### Declaration
`Timer timer;`
### Public member functions
```
void start()
	开始计时
void stop()
	停止计时
void reset()
	重置计时器
float read()
	获得计时器经过的时间
int read_ms()
	以毫秒为单位获得计时器经过的时间
int read_us()
	以微秒为单位获得计时器经过的时间
operator float()
	操作符重载，与read()返回值相同
us_timestamp_t read_high_resolution_us()
	以高分辨率类型获得计时器经过的时间
```
### 用途
这个计时器只有轮询功能，适合用于纯粹的计时。

使用样例：

```
Timer timer;
timer.start();
while(1) {
    wait(1);
    pc.printf("Time readed:%f.\n",timer.read());
}

```

## 延时器
### Include
`#include <Timeout.h`

同样的，实际上只需包含mbed.h即可使用
### Declaration
`Timeout timeout;`
### Public member functions
```
void attach(Callback< void()> func, float t)
	将一个函数绑定到延时器上并指定时间（单位：秒），该函数将在该指定时间后被调用一次（注意只有一次）
```

### 用途
延时器可以用于延时执行一个任务。

使用样例：

```
void L2()
{
    led2 = !led2;
}
int main()
{
    Timeout timeout;
    timeout.attach(L2,5);
    while(1)；
}

```

## 定时器
### Include 
`#include <Ticker.h>`

同样的，实际上只需包含mbed.h即可使用
### Declaration
`Ticker ticker;`
### Public member functions
```
void attach(Callback< void()> func, float t)
	将一个函数绑定到延时器上并指定时间（单位：秒），该函数每隔该指定时间段会被执行一次
```
### 用途
延时器可以用于延时执行一个任务。

使用样例：

```
void L3()
{
    led3 = !led3;
}
int main()
{
    Ticker ticker;
    ticker.attach(L3,1);
    while(1)；
}

```
>以上这三个定时器都是中断安全的。