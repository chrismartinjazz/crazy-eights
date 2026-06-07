#include "player.h"
#include "card.h"
#include "card_values.h"
#include "state.h"
#include "ui_helpers.h"
#include <algorithm> // for std::find
#include <cassert>
#include <iostream>
#include <string_view>
#include <vector>

Player::Player(std::string_view name)
    : m_name { name }
    , m_score { 0 }
{
}

std::string_view Player::name() const
{
    return m_name;
}

int Player::score() const
{
    return m_score;
}

void Player::add_to_score(int points)
{
    m_score += points;
}

void Player::reset_score()
{
    m_score = 0;
}

void Player::display_hand() const
{
    std::cout << m_hand;
}

void Player::add_to_hand(std::vector<Card>& cards)
{
    m_hand.add(cards);
}

int Player::cards_remaining() const
{
    return m_hand.cards_remaining();
}

int Player::points_in_hand() const
{
    return m_hand.points();
}

bool Player::has_valid_card_in_hand(State& state)
{
    return !m_hand.find_playable_indices(state).empty();
}

void Player::reset_hand()
{
    m_hand.reset();
}
