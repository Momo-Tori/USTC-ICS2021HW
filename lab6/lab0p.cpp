#include <iostream>
int main()
{
    int16_t r0, r1, r2, r3, r4, r5, r6, r7;
    r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;
    r0 = -114;
    r1 = -233;
    r6 = 1;
    do
    {
        if ((r1 & r6) != 0)
            r7 += r0;
        r0 <<= 1;
        r6 <<= 1;
    } while (r6 != 0);
    std::cout << r7 << std::endl;
    return 0;
}