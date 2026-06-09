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
        while (true)
        {
            std::cout << prompt;
            std::string input;
            if (!std::getline(std::cin, input))
                throw std::runtime_error("Unexpected end of input");

            if (input.length() == 0)
                continue;

            return input;
        }
    }

    inline void ask_press_enter(std::string prompt)
    {
        std::cout << prompt;
        std::cin.ignore();
    }

    inline void clear_terminal()
    {
#if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
#else
        std::cout << "\033[2J\033[1;1H";
#endif
    }
} // namespace ui

#endif