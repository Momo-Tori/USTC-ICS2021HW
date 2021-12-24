/*
 * @Author       : Chivier Humber
 * @Date         : 2021-09-15 21:15:24
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-11-23 16:08:51
 * @Description  : file content
 */
#include "common.h"
#include "memory.h"

namespace virtual_machine_nsp
{
    void memory_tp::ReadMemoryFromFile(std::string filename, int beginning_address)
    {
        // Read from the file
        std::ifstream input_file;
        input_file.open(filename);
        std::string line;
        int i = beginning_address;
        getline(input_file, line);
        while (!input_file.eof())
            if (line.length() == 16 || line.length() == 0)
            {
                memory[i] = TranslateInstruction(line);
                getline(input_file, line);
                i++;
            }
            else
                exit(-1); //输入文件错误
        while (i < kVirtualMachineMemorySize)
        {
            memory[i] = 0;
            i++;
        }
    }

    int16_t memory_tp::GetContent(int address) const
    {
        // get the content
        return memory[address];
    }

    int16_t &memory_tp::operator[](int address)
    {
        // get the content
        return memory[address];
    }
}; // virtual machine namespace
