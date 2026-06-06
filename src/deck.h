#ifndef DECK_H
#define DECK_H

#include "card.h"
#include "card_values.h"
#include <vector>

class Deck
{
  private:
    std::vector<Card> m_cards {};
    std::vector<Card> m_original {};

  public:
    template <typename T, typename V>
    Deck(const T& ranks, const V& suits)
    {
        for (auto& rank : ranks)
            for (auto& suit : suits)
                m_cards.emplace_back(rank, suit);

        m_original = m_cards;
        shuffle();
    }

    int size() const;
    std::vector<Card> deal(int count);
    Card draw_one();
    void shuffle_in(Card card);
    void reset();

  private:
    void shuffle();
};

#endif