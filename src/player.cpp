#include "player.h"
#include "card.h"
#include "card_values.h"
#include "deck.h"
#include "hand.h"
#include "state.h"
#include <algorithm> // for std::find
#include <cassert>
#include <iostream>
#include <optional> // for std::optional, std::nullopt
#include <sstream>
#include <string>
#include <string_view>
#include <utility> // for std::move
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

Hand Player::hand() const
{
    return m_hand;
}

void Player::add_to_score(int points)
{
    m_score += points;
}

void Player::reset_score()
{
    m_score = 0;
}

void Player::add_to_hand(std::vector<Card>& cards)
{
    m_hand.add(cards);
}

void Player::reset_hand()
{
    m_hand.reset();
}

int Player::cards_remaining() const
{
    return m_hand.cards_remaining();
}

bool Player::has_valid_card_in_hand(State& state)
{
    return !m_hand.find_playable_indices(state).empty();
}

std::optional<Card> Player::play_card_or_draw(State& state)
{
    assert(
        m_hand.cards_remaining() > 0 &&
        "Player::play_card(): A player with no cards "
        "was asked to play a card."
    );

    std::vector<int> playable_indices { m_hand.find_playable_indices(state) };

    // If the player has a valid card in hand, they may play a card or
    // draw. They may only draw if the deck has cards available.
    if (playable_indices.size() > 0)
    {
        bool draw_is_possible { state.deck.size() > 0 };
        std::optional<int> choice {
            ask_card_index_or_draw(playable_indices, draw_is_possible)
        };
        if (choice)
        {
            return m_hand.play(static_cast<std::size_t>(*choice));
        }
        else
        {
            std::vector<Card> drawn_card { state.deck.draw_one() };
            std::cout << "You drew " << drawn_card[0] << '\n';
            m_hand.add(drawn_card);

            return std::nullopt;
        }
    }

    // Otherwise, they must draw until they draw a valid card, and play it.
    // If the deck is empty, they must pass (returning std::nullopt)
    std::cout << "You have no playable cards.\n";
    while (state.deck.size() > 0)
    {
        ask_press_enter("Press Enter to draw >> ");
        std::vector<Card> drawn_card { state.deck.draw_one() };
        std::cout << "You drew " << drawn_card[0] << '\n';

        if (state.rules.is_playable(
                drawn_card[0], state.discards.back(), state.current_suit
            ))
        {
            ask_press_enter("Press Enter to play the card >> ");
            return drawn_card[0];
        }
        m_hand.add(drawn_card);
    }
    // The drawpile is empty - pass the turn.
    ask_press_enter("The draw pile is empty - press enter to pass >> ");
    return std::nullopt;
}

card_values::Suit Player::ask_choose_suit()
{
    while (true)
    {
        std::cout << "Choose a suit: (s)pades, (h)earts, (d)iamonds or "
                     "(c)lubs >> ";
        std::string input;
        if (!std::getline(std::cin, input))
        {
            throw std::runtime_error("Unexpected end of input");
        }

        if (input.length() == 0)
            continue;

        switch (static_cast<char>(input[0]))
        {
        case 'c': return card_values::Suit::Club;
        case 'd': return card_values::Suit::Diamond;
        case 'h': return card_values::Suit::Heart;
        case 's': return card_values::Suit::Spade;
        default:
        {
            std::cout << "Enter s, h, d or c.\n";
            continue;
        }
        }
    }
}

int Player::points_in_hand() const
{
    return m_hand.points();
}

bool Player::valid(State& state, Card& card)
{
    return (
        card.rank() == state.discards.back().rank() ||
        card.suit() == state.current_suit || card.is_wild()
    );
}

void Player::ask_press_enter(std::string prompt)
{
    std::cout << prompt;
    std::string temp {};
    std::getline(std::cin, temp);
}

std::optional<int> Player::ask_card_index_or_draw(
    std::vector<int> valid_indices, bool draw_is_possible
)
{
    while (true)
    {
        std::cout << "Choose a card (1 - " << m_hand.cards_remaining() << ')';
        if (draw_is_possible)
            std::cout << " or 'd' to draw";
        std::cout << " >> ";

        std::string input;
        if (!std::getline(std::cin, input))
        {
            throw std::runtime_error("Unexpected end of input");
        }

        if (input == "d" && draw_is_possible)
        {
            return std::nullopt;
        }

        std::stringstream ss { input };
        int choice {};
        if (ss >> choice)
        {
            if (std::find(
                    valid_indices.begin(), valid_indices.end(), choice - 1
                ) != valid_indices.end())
            {
                return choice - 1;
            }
            else
            {
                std::cout << "You must follow rank and suit, or play an 8.\n";
                continue;
            }
        }
        else
        {
            std::cout << "Type a number";
            if (draw_is_possible)
                std::cout << "or 'd' to draw";
            std::cout << ".\n ";

            continue;
        }
    }
}
