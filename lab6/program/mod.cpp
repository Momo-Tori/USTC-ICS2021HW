#include <iostream>
int16_t r0, r1, r2, r3, r4, r5, r6, r7;
void foo()
{
    r4 = 0;
    r3 = 8;
    r2 = 1;
    do
    {
        if ((r3 & r1) != 0)
            r4 += r2;
        r2 <<= 1;
        r3 <<= 1;
    } while (r3 != 0);
}
int main()
{
    r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;
    r1 = 0x120;
    do
    {
        foo();
        r2 = r1 & 0b111;
        r1 = r2 + r4;
    } while (r1 - 7 > 0);
    r1 = (r1 == 7) ? 0 : r1;
    std::cout << r1 << std::endl;
    return 0;
}