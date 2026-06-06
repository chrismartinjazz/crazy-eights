#ifndef STATE_H
#define STATE_H

#include "card.h"
#include "card_values.h"
#include "deck.h"
#include "player.h"
#include <vector>

struct State
{
    card_values::Suit current_suit {};
    std::vector<Player> players {};
    Deck deck { card_values::ranks, card_values::suits };
    std::vector<Card> discards {};
};

#endif