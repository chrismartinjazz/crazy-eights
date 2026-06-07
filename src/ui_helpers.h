#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace ui
{
    inline std::string ask_input(std::string_view prompt)
    {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input))
            throw std::runtime_error("Unexpected end of input");
        return input;
    }

    inline void ask_press_enter(std::string prompt)
    {
        std::cout << prompt;
        std::cin.ignore();
    }
} // namespace ui

#endif