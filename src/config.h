#ifndef CONFIG_H
#define CONFIG_H

namespace config
{
    constexpr int hand_size { 7 };
    constexpr int number_of_players { 2 };
    constexpr int winning_score { number_of_players * 50 };
    constexpr int player_indent { 15 };
    constexpr int deck_cards_per_line { 6 };
    constexpr int ai_sleep_milliseconds { 700 };
    constexpr int max_name_length { 30 };
} // namespace config

#endif