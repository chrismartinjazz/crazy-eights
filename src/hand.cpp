#include "hand.h"
#include "card_values.h"
#include "ui_helpers.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void Hand::add(std::vector<Card> cards)
{
    m_cards.reserve(m_cards.size() + cards.size());
    std::move(cards.begin(), cards.end(), std::back_inserter(m_cards));
    cards.clear();
}

const Card& Hand::card(int index) const
{
    return m_cards[static_cast<std::size_t>(index)];
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

Card Hand::play(int index)
{
    Card chosen_card { std::move(m_cards[static_cast<std::size_t>(index)]) };
    m_cards.erase(
        m_cards.begin() + static_cast<std::vector<Card>::difference_type>(index)
    );
    return chosen_card;
}

void Hand::sort_by_points()
{
    std::sort(
        m_cards.begin(),
        m_cards.end(),
        [](Card& a, Card& b) { return a.points() > b.points(); }
    );
}

void Hand::sort_by_suit_and_rank()
{
    std::sort(
        m_cards.begin(),
        m_cards.end(),
        [](Card& a, Card& b)
        {
            if (a.suit() != b.suit())
                return a.suit() < b.suit();
            return a.rank() < b.rank();
        }
    );
}

std::ostream& operator<<(std::ostream& out, const Hand& hand)
{
    std::stringstream line1 {};
    line1 << "┌";
    for (std::size_t i { 0 }; i < hand.m_cards.size(); ++i)
        line1 << "────┐";
    line1 << '\n';

    std::stringstream line2 {};
    line2 << "│";
    for (auto& card : hand.m_cards)
        line2 << ' ' << card << " │";
    line2 << '\n';

    std::stringstream line3 {};
    line3 << "│";
    for (std::size_t i { 0 }; i < hand.m_cards.size(); ++i)
        line3 << "    │";
    line3 << '\n';

    std::stringstream line4 {};
    line4 << "└";
    for (std::size_t i { 0 }; i < hand.m_cards.size(); ++i)
        line4 << "────┘";
    line4 << '\n';

    std::stringstream line5 {};
    line5 << "   ";
    for (int i { 0 }; i < static_cast<int>(hand.m_cards.size()); ++i)
        line5 << i + 1
              << std::string(
                     static_cast<std::size_t>(5 - ui::count_digits(i + 1)), ' '
                 );
    line5 << '\n';

    out << line1.str() << line2.str() << line3.str() << line4.str()
        << line5.str();
    return out;
}
