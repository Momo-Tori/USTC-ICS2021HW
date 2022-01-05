#include <iostream>
#include <stack>
std::stack<int16_t> s;
int16_t r0, r1, r2, r3, r4, r5, r6, r7;
int16_t LEBAL = 5;
void foo()
{
    r2++;
    s.push(r7);
    r0++;
    r1 = LEBAL;
    r1--;
    LEBAL = r1;
    if (r1 != 0)
    {
        r7=0x300C;
        foo();
    }
    r2--;
    r7 = s.top();
    s.pop();
}
int main()
{
    r0 = r1 = r2 = r3 = r4 = r5 = r6 = r7 = 0;
    r2 = 0x300f;
    r7=0x3003;
    foo();
    std::cout << r0 << ' ' << r1 << ' ' << r2 << ' ' << r3 << ' ' << r4 << ' ' << r5 << ' ' << r6 << ' ' << r7 << std::endl;
    return 0;
}
