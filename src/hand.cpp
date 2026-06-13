#include "hand.h"
#include "card_values.h"
#include "ui_helpers.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
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

std::string Hand::display() const
{
    std::stringstream ss {};
    ss << build_line(m_cards.size(), "┌", "────┐") << '\n';

    // Render the card values.
    ss << "│";
    for (auto& card : m_cards)
        ss << ' ' << card << " │";
    ss << '\n';

    ss << build_line(m_cards.size(), "│", "    │") << '\n';
    ss << build_line(m_cards.size(), "└", "────┘") << '\n';

    // Render the selection numbers for the cards, starting at 1.
    const std::size_t card_width { 5 };
    ss << "   ";
    for (int i { 1 }; i < static_cast<int>(m_cards.size() + 1); ++i)
    {
        std::size_t number_width { ui::count_digits(i) };
        ss << i << std::string((card_width - number_width), ' ');
    }
    ss << '\n';

    return ss.str();
}

std::string Hand::build_line(
    std::size_t count, std::string_view first, std::string_view repeater
) const
{
    std::stringstream ss {};
    ss << first;
    for (std::size_t i { 0 }; i < count; ++i)
        ss << repeater;
    return ss.str();
}

std::ostream& operator<<(std::ostream& out, const Hand& hand)
{
    out << hand.display();
    return out;
}
