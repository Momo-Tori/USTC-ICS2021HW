# Lab6 Learn from the past
姓名：杨涛
学号：PB20020599

## 高级语言还原

可以将汇编语言根据BR跳转命令划分为多个模块，每个模块的BR可以与高级语言例如cpp中的if,do...while,break等关键词所对应，则易于将其转化为高级语言

当跳转的Label在BR前面时一般是do...while，否则可能是if或break

如下面的例子

```
ADD R6,R6,x1;            R6置1用于AND判断
LOOP2 AND R2,R1,R6;      用于判断R6的对应位是否为1，只用于设置状态码，存储无用
BRz LOOP1;               为0的时候跳过相加
ADD R7,R7,R0;            对应位置为1，加上R0*2^i
LOOP1 ADD R0,R0,R0;      R0*2,左移一位
ADD R6,R6,R6;            R6左移一位，掩码位置向左移动
BRnp LOOP2;              R6非零，继续计算相加
```

第一个BRz LOOP1是if，第二个BRnp LOOP2是do...while,翻译后变成

```cpp
r6 = 1;
do
    {
        if ((r1 & r6) != 0)
            r7 += r0;
        r0 <<= 1;
        r6 <<= 1;
    } while (r6 != 0);
```

七个汇编程序的翻译皆遵循这个规则

## 问题
1. how to evaluate the performance of your own high-level language programs

通过对应系统的时间接口来获取程序开始的时间与结束的时间

| cpp      | windows                     | linux          |
| -------- | --------------------------- | -------------- |
| 头文件   | windows.h                   | sys/time.h     |
| 结构体   | LARGE_INTEGER               | timeval        |
| 时间函数 | QueryPerformanceFrequency() | gettimeofday() |

但一般来说时间精度都比较低，这时可以使用for循环循环程序$10^n$倍，放大时间计量之后再除去，这样就可以较为准确地计量小时间

2. why is a high-level language easier to write than LC3 assembly

首先高级语言将程序设计概括为顺序结构，选择结构与循环结构，之后通过编译器将其转化为汇编语言，因此高级语言是易于理解与编写的，而汇编语言则需要考虑使用BR来同时完成循环与选择。

其次高级语言可以方便地进行表达式的求值，而不是像汇编那样需要把一个表达式拆成多个汇编指令

最后高级语言很能方便地组织数据结构，而不需要考虑数据结构具体的内存分配，这是汇编语言不能做到的

3. what instructions do you think need to be added to LC3? (You can think about the previous experiments and what instructions could be added to greatly simplify the previous programming)

减法，乘法，取余，左移，算数右移，逻辑右移，除法

4. is there anything you need to learn from LC3 for the high-level language you use?

内存的理论模型，数据结构的存储方式