#include <iostream>
#include "imputArg.hpp"
#include <string>
#include <algorithm>
#include "ExpenseSheet.hpp"
#include <iomanip>
#include <filesystem>

int main(int argc, char** argv)
{
    ExpenseSheet expenses;
    
    if (argc == 2)
    {
        std::__fs::filesystem::path path = argv[1];
        expenses.Open(path);

    }
    
    std::string line;
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, line);
        ImputArguments imArgs(line);

        std::string cmd = imArgs[0];
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](char c)
                       { return std::tolower(c); });
        auto args = imArgs.Subset(1);

        if (cmd == "add")
        {
            if (args.Count() == 2 || args.Count() == 3)
            {
                const auto &label = args[0];
                double value;
                if (args.Count() == 2)
                {
                    value = atof(args[1].c_str()) * -1.;
                }
                else
                {
                    value = atof(args[2].c_str());
                    if (args[1] != "+")
                    {
                        value *= -1.;
                    }
                }
                if (!expenses.Add(label, value))
                {
                    std::cout << "Failed to add!" << std::endl;
                }
            }
            else
            {
                std::cout << "Usage: add <label> (+/-) <values>" << std::endl;
            }
        }
        else if (cmd == "del")
        {
            if (args.Count() == 1)
            {
                if (!expenses.Del(args[0]))
                {
                    std::cout << "Failed to delete!" << std::endl;
                }
            }
            else
            {
                std::cout << "Usage: del <label>" << std::endl;
            }
        }
        else if (cmd == "list")
        {
            expenses.List(std::cout);
        }
        else if (cmd == "eval")
        {
            auto value = expenses.Eval();
            std::cout << "TOTAL:" << value << std::endl;
        }
        else if (cmd == "exit")
        {
            break;
        }
        else if (cmd == "open")
        {
            if (args.Count() == 1)
            {
                std::__fs::filesystem::path p = args[0];
                
                if (!expenses.Open(p))
                {
                    std::cout << "failed to open file!" << std::endl;
                }
            }
            else
            {
                std::cout << "Usage: open <path>" << std::endl;
            }
        }
        else if (cmd == "save")
        {
            if (args.Count() == 1)
            {
                std::__fs::filesystem::path p = args[0];
                
                if (!expenses.Save(p))
                {
                    std::cout << "failed to save file!" << std::endl;
                }
            }
            else if (args.Count() == 0)
            {
                if (!expenses.Save())
                {
                    std::cout << "failed to save file! Try save <path>" << std::endl;
                }
            }
            else
            {
                std::cout << "Usage: save <path>" << std::endl;
            }
        }
        else if (cmd == "new")
        {
            expenses.New();
        }
        else if (cmd == "export")
        {
            if (args.Count() == 2)
            {
                if(args[0] == "csv")
                {
                    expenses.ExportCsv(args[1]);
                }
                else if (args[0] == "html")
                {
                    expenses.ExportHtml(args[1]);
                }
                else
                {
                    std::cout << "Usage: export <csv/html> <file>" << std::endl;
                }
            }
            else
            {
                std::cout << "Usage: export <csv/html> <file>" << std::endl;
            }
        }
        else
        {
            std::cout << "command" << " " << cmd << " is unknown " << std::endl;
        }
    }
}