#include "card.h"
#include "card_values.h"
#include <iostream>
using namespace card_values;

Card::Card(Rank rank, Suit suit)
    : m_rank(rank)
    , m_suit(suit)
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

int Card::points()
{
    return rank_points(m_rank);
}

std::ostream& operator<<(std::ostream& out, const Card& card)
{
    out << rank_letter(card.m_rank) << suit_glyph(card.m_suit);
    return out;
}