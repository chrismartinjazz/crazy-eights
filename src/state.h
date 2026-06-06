#ifndef STATE_H
#define STATE_H

#include "card.h"
#include "card_values.h"
#include "deck.h"
#include "rules.h"
#include <vector>

class Player;

struct State
{
    const Rules rules {};
    card_values::Suit current_suit {};
    Deck deck { card_values::ranks, card_values::suits };
    std::vector<Card> discards {};
};

#endif