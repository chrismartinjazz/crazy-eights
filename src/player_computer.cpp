#include "player_computer.h"
#include "card_values.h"
#include "config.h"
#include "player.h"
#include "random_mt.h"
#include "state.h"
#include "ui_helpers.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <string_view>
#include <unordered_map>

PlayerComputer::PlayerComputer(std::string_view name)
    : Player { name }
{
}

std::optional<Card> PlayerComputer::play_card_or_draw(State& state)
{
    assert(
        m_hand.cards_remaining() > 0 &&
        "PlayerComputer::play_card(): A player with no cards "
        "was asked to play a card."
    );

    ui::sleep(config::ai_sleep_milliseconds);
    update_hand_indices(state);

    if (m_hand_indices.playable.size() > 0)
        return select_card(state);

    return forced_draw(state);
}

card_values::Suit PlayerComputer::ask_choose_suit() const
{
    std::unordered_map<card_values::Suit, int> suit_counts { count_suits() };

    // Find the suit with the most cards in hand.
    auto iterator { std::ranges::max_element(
        suit_counts,
        [](const auto& a, const auto& b) { return a.second < b.second; }
    ) };

    if (iterator != suit_counts.end())
        return iterator->first;

    // Reaching this line indicates the player holds only wild cards in
    // hand. Choose a random suit.
    return static_cast<card_values::Suit>(
        random_mt::get(0, static_cast<int>(card_values::Suit::maxSuit) - 1)
    );
}

void PlayerComputer::sort_hand()
{
    m_hand.sort_by_points();
}

void PlayerComputer::draw_two(State& state)
{
    for (int i { 0 }; i < 2; ++i)
        if (state.deck.cards_remaining() > 0)
            m_hand.add(state.deck.draw_one());
    ui::sleep(config::ai_sleep_milliseconds);
}

Card PlayerComputer::select_card(State& state)
{
    if (m_hand_indices.suits_map.contains(state.current_suit))
        return follow_suit(state);

    Card& top_card { state.discards.back() };
    if (m_hand_indices.ranks_map.contains(top_card.rank()))
        return follow_rank(top_card);

    if (m_hand_indices.wilds.size() > 0)
        return m_hand.play(m_hand_indices.wilds[0]);

    std::cerr << "A player indicated they had valid card indices, but "
                 "could not select a card. Playing the first playable "
                 "card.\n";
    return m_hand.play(m_hand_indices.wilds[0]);
}

void PlayerComputer::update_hand_indices(State& state)
{
    m_hand_indices.ranks_map.clear();
    m_hand_indices.suits_map.clear();
    m_hand_indices.wilds.clear();
    m_hand_indices.playable.clear();

    // Determine the ranks, suits and wilds in hand
    for (int i { 0 }; i < m_hand.cards_remaining(); ++i)
    {
        if (m_hand.card(i).is_wild())
        {
            m_hand_indices.wilds.push_back(i);
            continue;
        }
        m_hand_indices.ranks_map[m_hand.card(i).rank()].push_back(i);
        m_hand_indices.suits_map[m_hand.card(i).suit()].push_back(i);
    }
    m_hand_indices.playable = m_hand.find_playable_indices(state);
}

Card PlayerComputer::follow_suit(State& state)
{
    std::vector<int> indices { m_hand_indices.suits_map[state.current_suit] };
    auto iterator { std::ranges::min_element(
        indices,
        [&](const int a, const int b)
        {
            return m_hand_indices.ranks_map[m_hand.card(a).rank()].size() <
                   m_hand_indices.ranks_map[m_hand.card(b).rank()].size();
        }
    ) };
    if (iterator != indices.end())
        return m_hand.play(*iterator);

    throw std::runtime_error("Unable to follow suit.");
}

Card PlayerComputer::follow_rank(Card& top_card)
{
    std::vector<int> indices { m_hand_indices.ranks_map[top_card.rank()] };
    auto iterator { std::ranges::max_element(
        indices,
        [&](const int a, const int b)
        {
            return m_hand_indices.suits_map[m_hand.card(a).suit()].size() <
                   m_hand_indices.suits_map[m_hand.card(b).suit()].size();
        }
    ) };
    if (iterator != indices.end())
        return m_hand.play(*iterator);

    throw std::runtime_error("Unable to follow rank.");
}

std::optional<Card> PlayerComputer::forced_draw(State& state)
{
    while (state.deck.cards_remaining() > 0)
    {
        Card drawn_card { state.deck.draw_one() };
        std::cout << m_name << " drew a card.\n";
        if (state.rules.is_playable(
                drawn_card, state.discards.back(), state.current_suit
            ))
            return drawn_card;

        m_hand.add(drawn_card);
    }

    // The drawpile is empty - pass the turn.
    std::cout << m_name << " passes.\n";
    return std::nullopt;
}

std::unordered_map<card_values::Suit, int> PlayerComputer::count_suits() const
{
    std::unordered_map<card_values::Suit, int> suits_count {};
    for (int i { 0 }; i < m_hand.cards_remaining(); ++i)
    {
        if (m_hand.card(i).is_wild())
            continue;
        if (!suits_count.contains(m_hand.card(i).suit()))
            suits_count[m_hand.card(i).suit()] = 0;
        else
            suits_count[m_hand.card(i).suit()] += 1;
    }
    return suits_count;
};