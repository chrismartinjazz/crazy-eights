#ifndef CARD_H
#define CARD_H

#include "card_values.h"

struct CardEffect
{
    bool wild { false };
    bool skip { false };
    bool reverse { false };
    bool draw_two { false };
};

class Card
{
  private:
    card_values::Rank m_rank {};
    card_values::Suit m_suit {};
    CardEffect m_effect {};

  public:
    Card(card_values::Rank rank, card_values::Suit suit);
    card_values::Rank rank() const;
    card_values::Suit suit() const;
    int points() const;
    bool is_wild() const;
    bool is_skip() const;
    bool is_reverse() const;
    bool is_draw_two() const;
    std::string display() const;
    friend std::ostream& operator<<(std::ostream& out, const Card& card);
};

#endif