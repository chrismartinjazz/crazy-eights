#include "player_human.h"
#include "card.h"
#include "card_values.h"
#include "state.h"
#include "ui_helpers.h"
#include <algorithm> // for std::find
#include <cassert>
#include <iostream>
#include <optional> // for std::optional, std::nullopt
#include <sstream>
#include <string>
#include <string_view>
#include <utility> // for std::move
#include <vector>

PlayerHuman::PlayerHuman(std::string_view name)
    : Player { name }
{
}

std::optional<Card> PlayerHuman::play_card_or_draw(State& state)
{
    assert(
        m_hand.cards_remaining() > 0 &&
        "PlayerHuman::play_card(): A player with no cards "
        "was asked to play a card."
    );

    std::vector<int> playable_indices { m_hand.find_playable_indices(state) };

    if (!playable_indices.empty())
        return play_or_voluntary_draw(state, playable_indices);

    return forced_draw(state);
}

card_values::Suit PlayerHuman::ask_choose_suit() const
{
    while (true)
    {
        std::string input { ui::ask_input(
            "Choose a suit: (s)pades, (h)earts, (d)iamonds or (c)lubs >> "
        ) };

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

void PlayerHuman::sort_hand()
{
    m_hand.sort_by_suit_and_rank();
}

void PlayerHuman::draw_two(State& state)
{
    std::cout << "Draw two...\n";
    for (int i { 0 }; i < 2; ++i)
    {
        if (state.deck.cards_remaining() > 0)
        {
            Card drawn_card { state.deck.draw_one() };
            std::cout << "Drew " << drawn_card << '\n';
            m_hand.add(state.deck.draw_one());
        }
        else
        {
            std::cout << "Draw pile is empty.\n";
            break;
        }
    }
    ui::ask_press_enter("Press enter to end your turn...");
}

// private:

std::optional<Card> PlayerHuman::play_or_voluntary_draw(
    State& state, const std::vector<int>& playable_indices
)
{
    bool draw_is_possible { state.deck.cards_remaining() > 0 };
    std::optional<int> choice {
        ask_card_index_or_draw(playable_indices, draw_is_possible)
    };
    if (choice)
    {
        return m_hand.play(*choice);
    }
    else
    {
        Card drawn_card { draw_one_and_announce(state) };
        m_hand.add(drawn_card);

        return std::nullopt;
    }
}

std::optional<Card> PlayerHuman::forced_draw(State& state)
{
    std::cout << "You have no playable cards.\n";
    while (state.deck.cards_remaining() > 0)
    {
        ui::ask_press_enter("Press Enter to draw >> ");
        Card drawn_card { draw_one_and_announce(state) };

        if (state.rules.is_playable(
                drawn_card, state.discards.back(), state.current_suit
            ))
        {
            ui::ask_press_enter("Press Enter to play the card >> ");
            return drawn_card;
        }
        m_hand.add(drawn_card);
    }
    // The drawpile is empty - pass the turn.
    ui::ask_press_enter("The draw pile is empty - press enter to pass >> ");
    return std::nullopt;
}

std::optional<int> PlayerHuman::ask_card_index_or_draw(
    const std::vector<int>& valid_indices, const bool draw_is_possible
) const
{
    std::string prompt { build_prompt(draw_is_possible) };
    while (true)
    {
        std::string input { ui::ask_input(prompt) };

        if (input == "d" && draw_is_possible)
            return std::nullopt;

        if (auto choice {
                parse_card_index(input, valid_indices, draw_is_possible) })
            return choice;
    }
}

std::string PlayerHuman::build_prompt(bool draw_is_possible) const
{
    std::string prompt {};
    prompt += "Choose a card (1 - ";
    prompt += std::to_string(m_hand.cards_remaining());
    prompt += ")";
    if (draw_is_possible)
        prompt += " or 'd' to draw";
    prompt += " >> ";
    return prompt;
}

Card PlayerHuman::draw_one_and_announce(State& state)
{
    std::vector<Card> drawn_card { state.deck.draw_one() };
    std::cout << "You drew " << drawn_card[0] << '\n';
    return drawn_card[0];
}

std::optional<int> PlayerHuman::parse_card_index(
    std::string_view input,
    const std::vector<int>& valid_indices,
    bool draw_is_possible
) const
{
    std::stringstream ss { std::string(input) };
    int choice {};
    if (!(ss >> choice))
    {
        std::cout << "Type a number";
        if (draw_is_possible)
            std::cout << "or 'd' to draw";
        std::cout << ".\n ";
        return std::nullopt;
    }
    if (std::find(valid_indices.begin(), valid_indices.end(), choice - 1) ==
        valid_indices.end())
    {
        std::cout << "You must follow rank and suit, or play an 8.\n";
        return std::nullopt;
    }
    return choice - 1;
}