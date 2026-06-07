#include "hand.h"
#include "card_values.h"
#include <iostream>
#include <vector>

void Hand::add(std::vector<Card> cards)
{
    m_cards.reserve(m_cards.size() + cards.size());
    std::move(cards.begin(), cards.end(), std::back_inserter(m_cards));
    cards.clear();
}

void Hand::add(Card card)
{
    m_cards.emplace_back(std::move(card));
}

void Hand::reset()
{
    m_cards.clear();
}

int Hand::points() const
{
    int points {};
    for (const auto& card : m_cards)
        points += card.points();
    return points;
}

int Hand::cards_remaining() const
{
    return static_cast<int>(m_cards.size());
}

std::vector<int> Hand::find_playable_indices(const State& state) const
{
    std::vector<int> playable_indices {};
    for (std::size_t i { 0 }; i < m_cards.size(); ++i)
    {
        if (state.rules.is_playable(
                m_cards[i], state.discards.back(), state.current_suit
            ))
            playable_indices.emplace_back(i);
    }
    return playable_indices;
}

Card Hand::play(std::size_t index)
{
    Card chosen_card { std::move(m_cards[index]) };
    m_cards.erase(
        m_cards.begin() + static_cast<std::vector<Card>::difference_type>(index)
    );
    return chosen_card;
}

std::ostream& operator<<(std::ostream& out, const Hand& hand)
{
    out << "[ ";
    for (auto& card : hand.m_cards)
        out << card << " ] ";
    return out;
}