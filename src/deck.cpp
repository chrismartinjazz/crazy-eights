#include "deck.h"
#include "card.h"
#include "card_values.h"
#include "random_mt.h"
#include <cassert>
#include <iterator> // for std::make_move_iterator
#include <vector>

int Deck::size() const
{
    return static_cast<int>(m_cards.size());
}

std::vector<Card> Deck::deal(int count)
{
    assert(count > 0 && "Deck::deal(count): count must be positive.");
    assert(
        static_cast<int>(m_cards.size()) >= count &&
        "Deck::deal(count): count cannot exceed m_size.size() - the deck "
        "has run out of cards."
    );

    std::vector<Card> cards(
        std::make_move_iterator(m_cards.end() - count),
        std::make_move_iterator(m_cards.end())
    );
    m_cards.erase(m_cards.end() - count, m_cards.end());
    return cards;
}

Card Deck::draw_one()
{
    assert(
        !m_cards.empty() &&
        "Deck::draw_one(): m_cards must be > 0 - the deck has run out of cards."
    );
    Card card = std::move(m_cards.back());
    m_cards.pop_back();
    return card;
}

void Deck::shuffle_in(Card card)
{
    int idx { random_mt::get(1, size()) };
    m_cards.insert(m_cards.begin() + idx, card);
}

void Deck::reset()
{
    m_cards = m_original;
    shuffle();
}

void Deck::shuffle()
{
    std::shuffle(m_cards.begin(), m_cards.end(), random_mt::mt);
}