#ifndef CARD_VALUES_H
#define CARD_VALUES_H

#include <array>
#include <string_view>

namespace card_values
{
    enum class Rank
    {
        Ace,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
        maxRank
    };

    enum class Suit
    {
        Club,
        Diamond,
        Heart,
        Spade,
        maxSuit
    };

    inline constexpr std::array<Rank, static_cast<size_t>(Rank::maxRank)>
        ranks { Rank::Ace,  Rank::Two,   Rank::Three, Rank::Four, Rank::Five,
                Rank::Six,  Rank::Seven, Rank::Eight, Rank::Nine, Rank::Ten,
                Rank::Jack, Rank::Queen, Rank::King };

    static_assert(
        ranks.size() == static_cast<size_t>(Rank::maxRank),
        "ranks is missing entries"
    );

    inline constexpr std::array<Suit, static_cast<size_t>(Suit::maxSuit)>
        suits { Suit::Club, Suit::Diamond, Suit::Heart, Suit::Spade };

    static_assert(
        suits.size() == static_cast<size_t>(Suit::maxSuit),
        "suits is missing entries"
    );

    inline constexpr std::array<int, static_cast<size_t>(Rank::maxRank)>
        rank_points_lookup {
            1,  // Ace
            2,  // Two
            3,  // Three
            4,  // Four
            5,  // Five
            6,  // Six
            7,  // Seven
            50, // Eight (wild card - penalty value)
            9,  // Nine
            10, // Ten
            10, // Jack
            10, // Queen
            10  // King
        };

    static_assert(
        rank_points_lookup.size() == static_cast<size_t>(Rank::maxRank),
        "rank_points_lookup is missing entries"
    );

    inline constexpr std::array<
        char,
        static_cast<size_t>(Rank::maxRank)>
        rank_letter_lookup {
            'A', // Ace
            '2', // Two
            '3', // Three
            '4', // Four
            '5', // Five
            '6', // Six
            '7', // Seven
            '8', // Eight
            '9', // Nine
            'T', // Ten
            'J', // Jack
            'Q', // Queen
            'K', // King
        };

    static_assert(
        rank_letter_lookup.size() == static_cast<size_t>(Rank::maxRank),
        "rank_letter_lookup is missing entries"
    );

    inline constexpr std::
        array<std::string_view, static_cast<size_t>(Suit::maxSuit)>
            suit_glyph_lookup {
                "♣",                // Club
                "\033[31m♦\033[0m", // Diamond
                "\033[31m♥\033[0m", // Heart
                "♠"                 // Spade
            };

    static_assert(
        suit_glyph_lookup.size() == static_cast<size_t>(Suit::maxSuit),
        "suit_glyph_lookup is missing entries"
    );

    inline constexpr std::
        array<std::string_view, static_cast<size_t>(Suit::maxSuit)>
            suit_text_lookup {
                "Club",    // Club
                "Diamond", // Diamond
                "Heart",   // Heart
                "Spade"    // Spade
            };

    static_assert(
        suit_text_lookup.size() == static_cast<size_t>(Suit::maxSuit),
        "suit_text_lookup is missing entries"
    );

    inline char rank_letter(Rank rank)
    {
        return rank_letter_lookup.at(static_cast<size_t>(rank));
    }

    inline int rank_points(Rank rank)
    {
        return rank_points_lookup.at(static_cast<size_t>(rank));
    }

    inline std::string_view suit_glyph(Suit suit)
    {
        return suit_glyph_lookup.at(static_cast<size_t>(suit));
    }
} // namespace card_values

#endif