#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <string_view>

class ImputArguments{
    public:
        ImputArguments() = default;
        ImputArguments(const ImputArguments&) = default;
        ImputArguments(std::string_view line);

        ImputArguments& operator = (const ImputArguments&) = default;

        ImputArguments Subset(size_t offset, size_t count = std::numeric_limits<size_t>::max());

        inline size_t Count() const { return args.size(); }
        inline const std::string& Get(size_t idx) const { return args[idx];}
        inline const std::string& operator[](size_t idx) const { return Get(idx);}
    private: 
        std::vector<std::string> args;
};