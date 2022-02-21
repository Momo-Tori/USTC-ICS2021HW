#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
int main()
{
    fstream f;
    f.open("outfile.txt",ios::out | ios::trunc);
    vector<int> s;
    s.push_back(1);
    s.push_back(1);
    s.push_back(2);
    int i;
    for (i = 3; i <= 16384; i++)
    {
        s.push_back((s[i - 1] + 2 * s[i - 3]) % 1024);
    }
    for (i = 0; i <= 16384; i++)
    f<<",0x"<<hex<<s[i];
    return 0;
}