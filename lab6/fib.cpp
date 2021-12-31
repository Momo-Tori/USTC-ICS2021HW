#include <iostream>
int main()
{
    int16_t r0, r1, r2, r3, r4, r5, r6, r7;
    r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;
    r0 = 4200;
    r7=1;
    r2=1;
    r3=2;
    r6=0x03ff;
    do
    {
        r1=r7+r7;
        r7=r2&r6;
        r2=r3;
        r3=r1+r2;
        r0--;
    } while (r0 > 0);
    std::cout << r7 << std::endl;
    return 0;
}