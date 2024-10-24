#pragma once
#include <string>
#include <vector>
#include <string_view>
#include <algorithm>
#include <ostream>
#include <iomanip>
#include <istream>
#include <sstream>
#include <fstream>
#include <filesystem>

class ExpenseSheet
{
public:
    struct Entry
    {

        std::string label;
        double value;

        void Serialize(std::ostream& out) const;
        void Deserialize(std::istream& in);


        inline bool operator==(const Entry &rhs) const
        {
            return label == rhs.label;
        }
    };

public:
    ExpenseSheet() = default;
    ExpenseSheet(const ExpenseSheet &) = default;

    ExpenseSheet &operator=(const ExpenseSheet &) = default;

    bool Open(const std::__fs::filesystem::path& dataFile);
    bool Save(const std::__fs::filesystem::path& dataFile = "") const;
    void New();
    
    bool Add(std::string_view label, double value);
    bool Del(std::string_view label);
    void List(std::ostream &os) const;
    double Eval() const;

    bool ExportCsv(const std::__fs::filesystem::path& file) const;
    bool ExportHtml(const std::__fs::filesystem::path& file) const;

private:
    static inline std::string TextHTMLClass(double value)
    {
        return value >= 0. ? "text-success-emphasis" : "text-danger-emphasis";
    }

private:
    std::__fs::filesystem::path m_path;
    std::vector<Entry> entrys;

};
