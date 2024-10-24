#include "imputArg.hpp"

ImputArguments::ImputArguments(std::string_view line)
{
    enum class State
    {
        ReadyForText,
        TextBegin,
        TextBeginIgnoreSpaces,

    } state = State::ReadyForText;

    std::stringstream ss;
    for (char c : line)
    {
        switch (state)
        {
        case State::ReadyForText:
            switch (c)
            {
            case ' ':
                break;
            case '"':
                state = State::TextBeginIgnoreSpaces;
                break;
            default:
                ss << c;
                state = State::TextBegin;
                break;
            }
            break;
        case State::TextBegin:
            switch (c)
            {
            case ' ':
                args.push_back(ss.str());
                ss.str("");
                state = State::ReadyForText;
                break;
            default:
                ss << c;
                break;
            }
            break;
        case State::TextBeginIgnoreSpaces:
            switch (c)
            {
                case '"':
                    args.push_back(ss.str());
                    ss.str("");
                    state = State::ReadyForText;
                    break;
                default:
                    ss << c;
                    break;
            }
            break;
        }
    }

    if (state == State::TextBegin){
        args.push_back(ss.str());
    }
}

ImputArguments ImputArguments::Subset(size_t offset, size_t count)
{
    ImputArguments subset;
    auto it = args.begin() + offset;
    while(count-- && it != args.end()){
        subset.args.push_back(*(it++));
    }
    return subset;
}
