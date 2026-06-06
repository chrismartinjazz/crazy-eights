#ifndef CARD_H
#define CARD_H

#include "card.h"
#include "card_values.h"

using namespace card_values;

class Card
{
  private:
    Rank m_rank {};
    Suit m_suit {};

  public:
    Card(Rank rank, Suit suit);
    Rank rank() const;
    Suit suit() const;
    int points();
    friend std::ostream& operator<<(std::ostream& out, const Card& card);
};

#endif