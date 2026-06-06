#ifndef RULES_H
#define RULES_H

#include "card.h"
#include "card_values.h"

class Rules
{
  public:
    Rules() {};
    bool is_playable(
        const Card& card,
        const Card& top_discard,
        card_values::Suit current_suit
    ) const
    {
        return (
            card.rank() == top_discard.rank() || card.suit() == current_suit ||
            card.is_wild()
        );
    }
};

#endif