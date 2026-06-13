#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include "config.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>

namespace ui
{
    inline constexpr std::string_view clear_screen { "\033[3J\033[2J\033[H" };

    inline void sleep(int number)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(number));
    }

    inline std::size_t count_digits(int number)
    {
        if (number == 0)
            return 1;
        return static_cast<std::size_t>(
            std::floor(std::log10(std::abs(number))) + 1
        );
    }

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

    inline int ask_number_of_players()
    {
        while (true)
        {
            std::string input { ask_input("How many players? (2 - 5) >> ") };
            std::stringstream ss { input };
            int choice {};
            if (ss >> choice)
            {
                if (choice >= 2 || choice <= 5)
                    return choice;

                std::cout << "Type a number between 2 and 5\n";
                continue;
            }

            std::cout << "Type a number\n";
        }
    }

    inline std::string ask_player_name()
    {
        while (true)
        {
            std::string name { ask_input("What is your name? >> ") };
            if (name.length() > config::max_name_length)
            {
                std::cout << "That name is too long - try again";
                continue;
            }
            return name;
        }
    }
} // namespace ui

#endif