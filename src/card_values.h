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

    inline constexpr std::array<Suit, static_cast<size_t>(Suit::maxSuit)>
        suits { Suit::Club, Suit::Diamond, Suit::Heart, Suit::Spade };

    inline constexpr std::array<int, static_cast<size_t>(Rank::maxRank)>
        rank_points_lookup {
            1,  // Ace
            2,  // Two
            3,  // Three
            4,  // Four
            5,  // Five
            6,  // Siz
            7,  // Seven
            50, // Eight
            9,  // Nine
            10, // Tem
            10, // Jack
            10, // Queen
            10  // King
        };

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

    inline constexpr std::
        array<std::string_view, static_cast<size_t>(Suit::maxSuit)>
            suit_glyph_lookup {
                "♣", // Club
                "♦", // Diamond
                "♥", // Heart
                "♠"  // Spade
            };

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