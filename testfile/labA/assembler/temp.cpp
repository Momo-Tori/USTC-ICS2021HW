/*
 * @Author       : Chivier Humber
 * @Date         : 2021-08-30 14:29:14
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-09-15 09:25:21
 * @Description  : A small assembler for LC-3
 */

#include "assembler.cpp"

using namespace std;

bool gIsDebugMode = false;
bool gIsErrorLogMode = false;
bool gIsHexMode = false;
// A simple arguments parser
std::pair<bool, string> getCmdOption(char **begin, char **end, const std::string &option) {
    char **itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return std::make_pair(true, *itr);
    }
    return std::make_pair(false, "");
}

bool cmdOptionExists(char **begin, char **end, const std::string &option) {
    return std::find(begin, end, option) != end;
}

int main(int argc, char **argv) {
    // Print out Basic information about the assembler
    if (cmdOptionExists(argv, argv + argc, "-h")) {
        std::cout << "This is a simple assembler for LC-3." << std::endl << std::endl;
        std::cout << "\e[1mUsage\e[0m" << std::endl;
        std::cout << "./assembler \e[1m[OPTION]\e[0m ... \e[1m[FILE]\e[0m ..." << std::endl << std::endl;
        std::cout << "\e[1mOptions\e[0m" << std::endl;
        std::cout << "-h : print out help information" << std::endl;
        std::cout << "-f : the path for the input file" << std::endl;
        std::cout << "-d : print out debug information" << std::endl;
        std::cout << "-e : print out error information" << std::endl;
        std::cout << "-o : the path for the output file" << std::endl;
        std::cout << "-s : hex mode" << std::endl;
        return 0;
    }

    string input_filename;

    string output_filename;

    // Check the input file name
        input_filename = "input.txt";
        output_filename = "";

        SetDebugMode(false);
        SetErrorLogMode(true);
        SetHexMode(false);
    auto ass = assembler();
    auto status = ass.assemble(input_filename, output_filename);

    if (gIsErrorLogMode) {
        cout << std::dec << status << endl;
    }
    return 0;
}
