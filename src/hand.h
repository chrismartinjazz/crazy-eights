#ifndef HAND_H
#define HAND_H

#include "card.h"
#include "state.h"
#include <vector>

class Hand
{
  private:
    std::vector<Card> m_cards {};

  public:
    const Card& card(const int index) const;
    void add(std::vector<Card> cards);
    void add(Card card);
    void reset();
    int points() const;
    int cards_remaining() const;
    std::vector<int> find_playable_indices(const State& state) const;
    Card play(int index);
    friend std::ostream& operator<<(std::ostream& out, const Hand& hand);
};

#endif