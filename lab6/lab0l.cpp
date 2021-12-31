#include <iostream>
int main()
{
    int16_t r0, r1, r2, r3, r4, r5, r6, r7;
    r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;
    r0 = -500;
    r1 = 433;
    do
    {
        r7 += r0;
        r1--;
    } while (r1 != 0);
    std::cout << r7 << std::endl;
    return 0;
}