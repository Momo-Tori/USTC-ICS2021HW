#include <iostream>
#include <stack>
int16_t r0, r1, r2, r3, r4, r5, r6, r7;
std::stack<int16_t> s;
void times()
{
    s.push(r3);

    r3 = r1;
    r1 = 0;
    r4 = 1;
    do
    {
        if ((r2 & r4) != 0)
            r1 += r3;
        r3 <<= 1;
        r4 <<= 1;
    } while (r4 != 0);

    r3 = s.top();
    s.pop();
}
void mod()
{
    r2 = -r1;
    r1 = r0;
    s.push(0);
    do
    {
        if (r2 + r0 < 0)
            break;
        s.push(r2);
        r2 <<= 1;
    } while (1);
    do
    {
        r2 = s.top();
        s.pop();
        if(r2==0) break;
        if(r2+r1>=0)
            r1+=r2;
    } while (1);
}
void foo()
{
    s.push(r2);
    s.push(r3);
    s.push(r4);
    s.push(r5);
    s.push(r7);

    r3 = 2;
    do
    {
        r1 = r2 = r3;
        times();
        if (r0 - r1 < 0)
        {
            r1 = 1;
            break;
        }
        r1 = r3;
        mod();
        if (r1 == 0)
            break;
        r3++;
    } while (1);

    r7 = s.top();
    s.pop();
    r5 = s.top();
    s.pop();
    r4 = s.top();
    s.pop();
    r3 = s.top();
    s.pop();
    r2 = s.top();
    s.pop();
}
int main()
{
    r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;
    r0 = 981;
    foo();
    std::cout << r1 << std::endl;
    return 0;
}