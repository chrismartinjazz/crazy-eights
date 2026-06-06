#include "deck.h"
#include "card.h"
#include "card_values.h"
#include "random_mt.h"
#include <cassert>
#include <iterator>
#include <vector>

std::vector<Card> Deck::deal(int count)
{
    assert(
        static_cast<int>(m_cards.size()) >= count &&
        "Deck::deal(count): count cannot exceed m_size.size() - the deck "
        "has run out of cards."
    );

    std::vector<Card> cards(
        std::make_move_iterator(m_cards.begin()),
        std::make_move_iterator(m_cards.begin() + count)
    );
    m_cards.erase(m_cards.begin(), m_cards.begin() + count);
    return cards;
}

Card Deck::draw_one()
{
    assert(
        m_cards.size() > 0 &&
        "Deck::draw_one(): m_cards must be > 0 - the deck has run out of cards."
    );
    Card card = std::move(m_cards.front());
    m_cards.erase(m_cards.begin());
    return card;
}

void Deck::reset()
{
    m_cards = m_original;
    shuffle();
}

int Deck::size()
{
    return static_cast<int>(m_cards.size());
}

void Deck::shuffle_in(Card& card)
{
    int idx { random_mt::get(1, size()) };
    m_cards.insert(m_cards.begin() + idx, card);
}

void Deck::create_cards(std::vector<Rank>& ranks, std::vector<Suit>& suits)
{
    for (const auto& rank : ranks)
    {
        for (const auto& suit : suits)
        {
            m_cards.push_back(Card(rank, suit));
        }
    }
    return;
}

void Deck::shuffle()
{
    std::shuffle(m_cards.begin(), m_cards.end(), random_mt::mt);
}