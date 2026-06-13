#include "card.h"
#include "card_values.h"
#include <iostream>
using namespace card_values;

struct CardEffect;

Card::Card(Rank rank, Suit suit)
    : m_rank(rank)
    , m_suit(suit)
    , m_effect {
        rank == Rank::Eight, // wild
        rank == Rank::Queen, // skip
        rank == Rank::Ace,   // reverse
        rank == Rank::Two    // draw_two
    }
{
}

Rank Card::rank() const
{
    return m_rank;
}

Suit Card::suit() const
{
    return m_suit;
}

int Card::points() const
{
    return rank_points(m_rank);
}

bool Card::is_wild() const
{
    return m_effect.wild;
}

bool Card::is_skip() const
{
    return m_effect.skip;
}

bool Card::is_reverse() const
{
    return m_effect.reverse;
}

bool Card::is_draw_two() const
{
    return m_effect.draw_two;
}

std::string Card::display() const
{
    std::string out {};
    out += rank_letter(m_rank);
    out += suit_glyph(m_suit);
    return out;
}

std::ostream& operator<<(std::ostream& out, const Card& card)
{
    out << rank_letter(card.m_rank) << suit_glyph(card.m_suit);
    return out;
}