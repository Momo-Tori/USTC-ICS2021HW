/*
 * @Author       : Chivier Humber
 * @Date         : 2021-08-30 15:10:31
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-11-23 15:34:30
 * @Description  : file content
 */

#include "assembler.h"

void label_map_tp::AddLabel(const std::string &str, const value_tp &val)
{
    labels_.insert(std::make_pair(str, val));
}

value_tp label_map_tp::GetValue(const std::string &str) const
{
    // User (vAddress, -1) to represent the error case
    if (labels_.find(str) == labels_.end())
    {
        // not found
        return value_tp(vAddress, -1);
    }
    else
    {
        return labels_.at(str);
    }
}

std::ostream &operator<<(std::ostream &os, const StringType &item)
{
    switch (item)
    {
    case sComment:
        os << "Comment ";
        break;
    case sLabel:
        os << "Label";
        break;
    case sValue:
        os << "Value";
        break;
    case sOpcode:
        os << "Opcode";
        break;
    case sOprand:
        os << "Oprand";
        break;
    default:
        os << "Error";
        break;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const ValueType &val)
{
    switch (val)
    {
    case vAddress:
        os << "Address";
        break;
    case vValue:
        os << "Value";
        break;
    default:
        os << "Error";
        break;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const value_tp &value)
{
    if (value.type_ == vValue)
    {
        os << "[ " << value.type_ << " -- " << value.val_ << " ]";
    }
    else
    {
        os << "[ " << value.type_ << " -- " << std::hex << "0x" << value.val_ << " ]";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const label_map_tp &label_map)
{
    for (auto item : label_map.labels_)
    {
        os << "Name: " << item.first << " " << item.second << std::endl;
    }
    return os;
}

int RecognizeNumberValue(std::string s)
{
    // Convert string s into a number
    int sum = 0;
    if (s[0] == 'x')
    {
        s.erase(0);
        int i = s.length() - 1;
        while (i >= 0)
        {
            if (s[i] >= '0' && s[i] <= '9')
            {
                sum *= 16;
                sum += s[i] - '0';
            }
            else if (s[i] >= 'A' && s[i] <= 'F')
            {
                sum *= 16;
                sum += s[i] - 'A' + 10;
            }
            else if (s[i] == '-' && i == 0)
                sum *= (-1);
            else
                return std::numeric_limits<int>::max();
            i--;
        }
    }
    else
    {
        if (s[0] == '#')
            s.erase(0);
        int i = s.length() - 1;
        while (i >= 0)
        {
            if (s[i] >= '0' && s[i] <= '9')
            {
                sum *= 10;
                sum += s[i] - '0';
            }
            else if (s[i] == '-' && i == 0)
                sum *= (-1);
            else
                return std::numeric_limits<int>::max();
            i--;
        }
    }
    return sum;
}

std::string NumberToAssemble(const int &number)
{
    // Convert the number into a 16 bit binary string
    int i = 1 << 15;
    std::string s;
    while (i != 0)
    {
        if (i & number)
            s.push_back('1');
        else
            s.push_back('0');
        i >>= 1;
    }
    return s;
}

std::string NumberToAssemble(const std::string &number)
{
    // Convert the number into a 16 bit binary string
    // You might use `RecognizeNumberValue` in this function
    return NumberToAssemble(RecognizeNumberValue(number));
}

std::string ConvertBin2Hex(std::string bin)
{
    // Convert the binary string into a hex string
    static const char *table = "0123456789ABCDEF";
    std::string s;
    int temp;
    for (int i = 0; i < 4; i++)
    {
        temp = 0;
        for (int j = 3; j >= 0; j--)
        {
            temp <<= 1;
            temp += (bin[4 * i + j] - '0');
        }
        s.push_back(table[temp]);
    }
    return s;
}

std::string assembler::TranslateOprand(int current_address, std::string str, int opcode_length)
{
    // Translate the oprand
    str = Trim(str);
    auto item = label_map.GetValue(str);
    if (!(item.getType() == vAddress && item.getVal() == -1))
    {
        // str is a label
        int offset = item.getVal() - current_address - 1;
        return NumberToAssemble(offset).substr(str.length() - opcode_length);
    }
    if (str[0] == 'R')
    {
        // str is a register
        int temp = str[1] - '0';
        int mask = 1 << 2;
        std::string s;
        while (mask != 0)
        {
            if (temp & mask)
                s.push_back('1');
            else
                s.push_back('0');
            mask >>= 1;
        }
        return s;
    }
    else
    {
        // str is an immediate number
        return NumberToAssemble(str).substr(str.length() - opcode_length);
    }
}

// TODO: add error line index
int assembler::assemble(std::string input_filename, std::string output_filename)
{
    // assemble main function
    // parse program

    // store the original string
    std::vector<std::string> file_content;
    std::vector<std::string> origin_file;
    // store the tag for line
    std::vector<LineStatusType> file_tag;
    std::vector<std::string> file_comment;
    std::vector<int> file_address;
    int orig_address = -1;
    std::string line;

    std::ifstream input_file(input_filename);

    if (input_file.is_open())
    {
        // Scan #0:
        // Read file
        // Store comments
        while (std::getline(input_file, line))
        {
            // Remove the leading and trailing whitespace
            line = Trim(line);
            if (line.size() == 0)
            {
                // Empty line
                continue;
            }
            std::string origin_line = line;
            // Convert `line` into upper case
            for (int i = 0, length = line.length(); i < length; i++)
            {
                if ('a' <= line[i] && line[i] <= 'z')
                    line[i] += 'A' - 'a';
            }

            // Store comments
            auto comment_position = line.find(";");
            if (comment_position == std::string::npos)
            {
                // No comments here
                file_content.push_back(line);
                origin_file.push_back(origin_line);
                file_tag.push_back(lPending);
                file_comment.push_back("");
                file_address.push_back(-1);
                continue;
            }
            else
            {
                // Split content and comment
                auto comment_str = line.substr(0, comment_position);
                auto content_str = line.substr(comment_position);
                // Delete the leading whitespace and the trailing whitespace
                comment_str = Trim(comment_str);
                content_str = Trim(content_str);
                // Store content and comment separately
                file_content.push_back(content_str);
                origin_file.push_back(origin_line);
                file_comment.push_back(comment_str);
                if (content_str.size() == 0)
                {
                    // The whole line is a comment
                    file_tag.push_back(lComment);
                }
                else
                {
                    file_tag.push_back(lPending);
                }
                file_address.push_back(-1);
            }
        }
    }
    else
    {
        std::cout << "Unable to open file" << std::endl;
        // @ Input file read error
        return -1;
    }

    // Scan #1:
    // Scan for the .ORIG & .END pseudo code
    // Scan for jump label, value label, line comments
    int line_address = -1;
    for (int line_index = 0; line_index < file_content.size(); ++line_index)
    {
        if (file_tag[line_index] == lComment)
        {
            // This line is comment
            continue;
        }

        auto line = file_content[line_index];

        // * Pseudo Command
        if (line[0] == '.')
        {
            file_tag[line_index] = lPseudo;
            // This line is a pseudo instruction
            // Only .ORIG & .END are line-pseudo-command
            auto line_stringstream = std::istringstream(line);
            std::string pseudo_command;
            line_stringstream >> pseudo_command;

            if (pseudo_command == ".ORIG")
            {
                // .ORIG
                std::string orig_value;
                line_stringstream >> orig_value;
                orig_address = RecognizeNumberValue(orig_value);
                if (orig_address == std::numeric_limits<int>::max())
                {
                    // @ Error address
                    return -2;
                }
                file_address[line_index] = -1;
                line_address = orig_address;
            }
            else if (pseudo_command == ".END")
            {
                // .END
                file_address[line_index] = -1;
                // If set line_address as -1, we can also check if there are programs after .END
                // line_address = -1;
            }
            else if (pseudo_command == ".STRINGZ")
            {
                file_address[line_index] = line_address;
                std::string word;
                line_stringstream >> word;
                if (word[0] != '\"' || word[word.size() - 1] != '\"')
                {
                    // @ Error String format error
                    return -6;
                }
                auto num_temp = word.size() - 1;
                line_address += num_temp;
            }
            else if (pseudo_command == ".FILL")
            {
                std::string word;
                line_stringstream >> word;
                auto num_temp = RecognizeNumberValue(word);
                if (num_temp == std::numeric_limits<int>::max())
                {
                    // @ Error Invalid Number input @ FILL
                    return -4;
                }
                if (num_temp > 65535 || num_temp < -65536)
                {
                    // @ Error Too large or too small value  @ FILL
                    return -5;
                }
                file_address[line_index] = line_address;
                line_address++;
            }
            else if (pseudo_command == ".BLKW")
            {
                file_address[line_index] = line_address;
                std::string number;
                line_stringstream >> number;
                auto num = RecognizeNumberValue(number);
                if (num == std::numeric_limits<int>::max())
                {
                    // @ Error Invalid Number input @ BLKW
                    return -7;
                }
                if (num <= 0)
                    return -7;
                //@error
                line_address += num;
            }
            else
            {
                // @ Error Unknown Pseudo command
                return -100;
            }

            continue;
        }

        if (line_address == -1)
        {
            // @ Error Program begins before .ORIG
            return -3;
        }

        file_address[line_index] = line_address;
        line_address++;

        // Split the first word in the line
        auto line_stringstream = std::stringstream(line);
        std::string word;
        line_stringstream >> word;
        if (IsLC3Command(word) != -1 || IsLC3TrapRoutine(word) != -1)
        {
            // * This is an operation line
            file_tag[line_index] = lOperation;
            continue;
        }

        // * Label
        // Store the name of the label
        auto label_name = word;
        // Split the second word in the line
        line_stringstream >> word;
        if (IsLC3Command(word) != -1 || IsLC3TrapRoutine(word) != -1 || word == "")
        {
            // a label used for jump/branch
            file_tag[line_index] = lOperation;
            label_map.AddLabel(label_name, value_tp(vAddress, line_address - 1));
            if (word == "")
            {
                file_tag[line_index] = lComment;
                line_address--;
            }
        }
        else
        {
            file_tag[line_index] = lPseudo;
            if (word == ".FILL")
            {
                line_stringstream >> word;
                auto num_temp = RecognizeNumberValue(word);
                if (num_temp == std::numeric_limits<int>::max())
                {
                    // @ Error Invalid Number input @ FILL
                    return -4;
                }
                if (num_temp > 65535 || num_temp < -65536)
                {
                    // @ Error Too large or too small value  @ FILL
                    return -5;
                }
                label_map.AddLabel(label_name, value_tp(vAddress, line_address - 1));
            }
            if (word == ".BLKW")
            {
                // modify label map
                // modify line address
                label_map.AddLabel(label_name, value_tp(vAddress, line_address - 1));
                std::string number;
                line_stringstream >> number;
                auto num = RecognizeNumberValue(number);
                if (num == std::numeric_limits<int>::max())
                {
                    // @ Error Invalid Number input @ BLKW
                    return -7;
                }
                if (num <= 0)
                    return -7;
                //@error
                line_address += num - 1;
            }
            if (word == ".STRINGZ")
            {
                // modify label map
                // modify line address
                label_map.AddLabel(label_name, value_tp(vAddress, line_address - 1));
                std::string word;
                line_stringstream >> word;
                if (word[0] != '\"' || word[word.size() - 1] != '\"')
                {
                    // @ Error String format error
                    return -6;
                }
                auto num_temp = word.size() - 1;
                line_address += num_temp - 1;
            }
        }
    }

    if (gIsDebugMode)
    {
        // Some debug information
        std::cout << std::endl;
        std::cout << "Label Map: " << std::endl;
        std::cout << label_map << std::endl;

        for (auto index = 0; index < file_content.size(); ++index)
        {
            std::cout << std::hex << file_address[index] << " ";
            std::cout << file_content[index] << std::endl;
        }
    }

    // Scan #2:
    // Translate

    // Check output file
    if (output_filename == "")
    {
        output_filename = input_filename;
        if (output_filename.find(".") == std::string::npos)
        {
            output_filename = output_filename + ".asm";
        }
        else
        {
            output_filename = output_filename.substr(0, output_filename.rfind("."));
            output_filename = output_filename + ".asm";
        }
    }

    std::ofstream output_file;
    // Create the output file
    output_file.open(output_filename);
    if (!output_file)
    {
        // @ Error at output file
        return -20;
    }

    for (int line_index = 0; line_index < file_content.size(); ++line_index)
    {
        if (file_address[line_index] == -1 || file_tag[line_index] == lComment)
        {
            // * This line is not necessary to be translated
            continue;
        }

        auto line = file_content[line_index];
        auto line_stringstream = std::stringstream(line);

        if (gIsDebugMode)
            output_file << std::hex << file_address[line_index] << ": ";
        if (file_tag[line_index] == lPseudo)
        {
            // Translate pseudo command
            std::string word;
            line_stringstream >> word;
            if (word[0] != '.')
            {
                // Fetch the second word
                // Eliminate the label
                line_stringstream >> word;
            }

            if (word == ".FILL")
            {
                std::string number_str;
                line_stringstream >> number_str;
                auto output_line = NumberToAssemble(number_str);
                if (gIsHexMode)
                    output_line = ConvertBin2Hex(output_line);
                output_file << output_line << std::endl;
            }
            else if (word == ".BLKW")
            {
                // Fill 0 here
                std::string number_str;
                line_stringstream >> number_str;
                auto num = RecognizeNumberValue(number_str);
                std::string output_line;
                if (gIsHexMode)
                    output_line = "0000";
                else
                    output_line = "0000000000000000";
                output_file << output_line << std::endl;
                for (int i = 1; i < num; i++)
                {
                    output_file << std::hex << file_address[line_index] + i << ": ";
                    output_file << output_line << std::endl;
                }
            }
            else if (word == ".STRINGZ")
            {
                // Fill string here
                std::string str;
                line_stringstream >> str;
                int num = str.length();
                if (gIsHexMode)
                {
                    output_file << ConvertBin2Hex(NumberToAssemble((int)str[1])) << std::endl;
                    for (int i = 2; i < num - 1; i++)
                    {
                        output_file << std::hex << file_address[line_index] + i << ": ";
                        output_file << ConvertBin2Hex(NumberToAssemble((int)str[i])) << std::endl;
                    }
                }
                else
                {
                    output_file << NumberToAssemble((int)str[1]) << std::endl;
                    for (int i = 2; i < num - 1; i++)
                    {
                        output_file << std::hex << file_address[line_index] + i << ": ";
                        output_file << NumberToAssemble((int)str[i]) << std::endl;
                    }
                }
            }
            continue;
        }

        if (file_tag[line_index] == lOperation)
        {
            std::string word;
            line_stringstream >> word;
            if (IsLC3Command(word) == -1 && IsLC3TrapRoutine(word) == -1)
            {
                // Eliminate the label
                line_stringstream >> word;
            }

            std::string result_line = "";
            auto command_tag = IsLC3Command(word);
            auto parameter_str = line.substr(line.find(word) + word.size());
            parameter_str = Trim(parameter_str);

            // Convert comma into space for splitting
            replace(parameter_str.begin(), parameter_str.end(), ',', ' ');

            auto current_address = file_address[line_index];

            std::vector<std::string> parameter_list;
            auto parameter_stream = std::stringstream(parameter_str);
            while (parameter_stream >> word)
            {
                parameter_list.push_back(word);
            }
            auto parameter_list_size = parameter_list.size();
            if (command_tag != -1)
            {
                // This is a LC3 command
                switch (command_tag)
                {
                case 0:
                    // "ADD"
                    result_line += "0001";
                    if (parameter_list_size != 3)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1]);
                    if (parameter_list[2][0] == 'R')
                    {
                        // The third parameter is a register
                        result_line += "000";
                        result_line += TranslateOprand(current_address, parameter_list[2]);
                    }
                    else
                    {
                        // The third parameter is an immediate number
                        result_line += "1";
                        // std::cout << "hi " << parameter_list[2] << std::endl;
                        result_line += TranslateOprand(current_address, parameter_list[2], 5);
                    }
                    break;
                case 1:
                    // "AND"
                    result_line += "0101";
                    if (parameter_list_size != 3)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1]);
                    if (parameter_list[2][0] == 'R')
                    {
                        // The third parameter is a register
                        result_line += "000";
                        result_line += TranslateOprand(current_address, parameter_list[2]);
                    }
                    else
                    {
                        // The third parameter is an immediate number
                        result_line += "1";
                        // std::cout << "hi " << parameter_list[2] << std::endl;
                        result_line += TranslateOprand(current_address, parameter_list[2], 5);
                    }
                    break;
                case 2:
                    // "BR"
                    result_line += "0000000";
                    if (parameter_list_size != 1)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0], 9);
                    break;
                case 3:
                    // "BRN"
                    result_line += "0000100";
                    if (parameter_list_size != 1)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0], 9);
                    break;
                case 4:
                    // "BRZ"
                    result_line += "0000010";
                    if (parameter_list_size != 1)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0], 9);
                    break;
                case 5:
                    // "BRP"
                    result_line += "0000001";
                    if (parameter_list_size != 1)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0], 9);
                    break;
                case 6:
                    // "BRNZ"
                    result_line += "0000110";
                    if (parameter_list_size != 1)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0], 9);
                    break;
                case 7:
                    // "BRNP"
                    result_line += "0000101";
                    if (parameter_list_size != 1)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0], 9);
                    break;
                case 8:
                    // "BRZP"
                    result_line += "0000011";
                    if (parameter_list_size != 1)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0], 9);
                    break;
                case 9:
                    // "BRNZP"
                    result_line += "0000111";
                    if (parameter_list_size != 1)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0], 9);
                    break;
                case 10:
                    // "JMP"
                    result_line += "1100000";
                    if (parameter_list_size != 1)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += "000000";
                    break;
                case 11:
                    // "JSR"
                    result_line += "01001";
                    if (parameter_list_size != 1)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0],11);
                    break;
                case 12:
                    // "JSRR"
                    result_line += "0100000";
                    if (parameter_list_size != 1)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += "000000";
                    break;
                case 13:
                    // "LD"
                    result_line += "0010";
                    if (parameter_list_size != 2)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1],9);
                    break;
                case 14:
                    // "LDI"
                    result_line += "1010";
                    if (parameter_list_size != 2)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1],9);
                    break;
                case 15:
                    // "LDR"
                    result_line += "0110";
                    if (parameter_list_size != 3)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1]);
                    result_line += TranslateOprand(current_address, parameter_list[2],6);
                    break;
                case 16:
                    // "LEA"
                    result_line += "1110";
                    if (parameter_list_size != 2)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1],9);
                    break;
                case 17:
                    // "NOT"
                    result_line += "1001";
                    if (parameter_list_size != 2)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1]);result_line += "111111";
                    break;
                case 18:
                    // RET
                    result_line += "1100000111000000";
                    if (parameter_list_size != 0)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    break;
                case 19:
                    // RTI
                    result_line += "1000000000000000";
                    if (parameter_list_size != 0)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    break;
                case 20:
                    // ST
                    result_line += "0011";
                    if (parameter_list_size != 2)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1], 9);
                    break;
                case 21:
                    // STI
                    result_line += "1011";
                    if (parameter_list_size != 2)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1], 9);
                    break;
                case 22:
                    // STR
                    result_line += "0111";
                    if (parameter_list_size != 3)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += TranslateOprand(current_address, parameter_list[0]);
                    result_line += TranslateOprand(current_address, parameter_list[1]);
                    result_line += TranslateOprand(current_address, parameter_list[2],6);
                    break;
                case 23:
                    // TRAP
                    result_line += "11110000";
                    if (parameter_list_size != 1)
                    {
                        // @ Error parameter numbers
                        return -30;
                    }
                    result_line += NumberToAssemble(parameter_list[0]).substr(8);
                    break;
                default:
                    // Unknown opcode
                    // @ Error
                    break;
                }
            }
            else
            {
                // This is a trap routine
                command_tag = IsLC3TrapRoutine(word);
                switch (command_tag)
                {
                case 0:
                    // x20
                    result_line += "1111000000100000";
                    break;
                case 1:
                    // x21
                    result_line += "1111000000100001";
                    break;
                case 2:
                    // x22
                    result_line += "1111000000100010";
                    break;
                case 3:
                    // x23
                    result_line += "1111000000100011";
                    break;
                case 4:
                    // x24
                    result_line += "1111000000100100";
                    break;
                case 5:
                    // x25
                    result_line += "1111000000100101";
                    break;
                default:
                    // @ Error Unknown command
                    return -50;
                }
            }

            if (gIsHexMode)
                result_line = ConvertBin2Hex(result_line);
            output_file << result_line << std::endl;
        }
    }

    // Close the output file
    output_file.close();
    // OK flag
    return 0;
}
