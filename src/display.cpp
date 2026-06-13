#include "display.h"
#include "config.h"
#include "player.h"
#include "state.h"
#include "ui_helpers.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>

Display::Display(
    const State& state, const std::vector<std::unique_ptr<Player>>& players
)
    : m_state { state }
    , m_players { players }
{
}

void Display::render()
{
    std::string output {};

    output += ui::clear_screen;
    output += render_header();
    output += render_computer_players();
    output += render_deck();
    output += render_player();

    std::cout << output;
}

std::string Display::render_header() const
{
    int target { config::winning_score_per_player *
                 static_cast<int>(m_players.size()) };

    std::stringstream ss {};
    ss << "                ~~ CrAzY 8s!!! ~~\n";
    ss << "                ** Target: " << target << " **\n\n";
    ss << "8 - Wild   A - Reverse   Q - Skip   2 - Draw two\n\n";
    return ss.str();
}

std::string Display::render_computer_players() const
{
    std::stringstream ss {};
    // For each player
    for (std::size_t i { 0 }; i < m_players.size() - 1; ++i)
    {
        // Line 1: name with turn indicator and top of cards
        // "*Joe*          ┌──┐─┐─┐─┐─┐"
        char turn_indicator { i == m_state.current_player_index ? '*' : ' ' };
        std::size_t name_width { m_players[i]->name().length() + 2 };
        std::size_t indent_size1 { name_width < config::player_indent
                                       ? config::player_indent - name_width
                                       : 0 };
        std::string deck1 {
            build_deck(m_players[i]->cards_remaining(), "┌──┐", "─┐")
        };

        ss << turn_indicator << m_players[i]->name() << turn_indicator
           << std::string(indent_size1, ' ') << deck1 << '\n';

        // Line 2: score and back of cards
        // "0              │%%│%│%│%│%│"
        std::size_t indent_size2 { config::player_indent -
                                   ui::count_digits(m_players[i]->score()) };
        std::string deck2 {
            build_deck(m_players[i]->cards_remaining(), "│%%│", "%│")
        };
        ss << m_players[i]->score() << std::string(indent_size2, ' ') << deck2
           << '\n';

        // Line 3: bottom of cards
        // "               └──┘─┘─┘─┘─┘"
        std::string deck3 {
            build_deck(m_players[i]->cards_remaining(), "└──┘", "─┘")
        };
        ss << std::string(config::player_indent, ' ') << deck3 << '\n';
    }
    ss << '\n';
    return ss.str();
}

std::string Display::render_deck()
{
    m_discard_size =
        static_cast<int>(m_state.discards.size()) / config::deck_cards_per_line;
    m_draw_size = m_state.deck.cards_remaining() / config::deck_cards_per_line;
    std::string indent { std::string(config::player_indent, ' ') };
    std::stringstream ss {};

    // "               ┌────┐┐┐ ┌────┐┐┐┐┐┐┐"
    ss << build_draw_and_discard(indent, "┌────┐", "┌────┐", "┐") << '\n';

    // "           *H* │ TH │││ │ %  │││││││"
    std::string indent2 { std::string(config::player_indent - 4, ' ') };
    char suit_star { m_state.discards.back().is_wild() ? '*' : ' ' };
    std::string suit_glyph { m_state.discards.back().is_wild()
                                 ? card_values::suit_glyph(m_state.current_suit)
                                 : " " };
    indent2 += suit_star + suit_glyph + suit_star + ' ';

    std::string discard_first { "│ " + m_state.discards.back().display() +
                                " │" };
    ss << build_draw_and_discard(indent2, discard_first, "│ %  │", "│") << '\n';

    // "               │    │││ │  % │││││││"
    ss << build_draw_and_discard(indent, "│    │", "│  % │", "│") << '\n';

    // "               └────┘┘┘ └────┘┘┘┘┘┘┘"
    ss << build_draw_and_discard(indent, "└────┘", "└────┘", "┘") << '\n';

    return ss.str();
}

std::string Display::build_deck(
    int cards, std::string_view first, std::string_view repeater
) const
{
    std::stringstream ss {};
    ss << first;
    for (int i { 0 }; i < cards - 1; ++i)
        ss << repeater;
    return ss.str();
}

std::string Display::build_draw_and_discard(
    std::string_view indent,
    std::string_view discard,
    std::string_view draw,
    std::string_view repeater
)
{
    std::stringstream ss {};
    ss << indent << build_deck(m_discard_size, discard, repeater) << ' '
       << build_deck(m_draw_size, draw, repeater);
    return ss.str();
}

std::string Display::render_player() const
{
    std::stringstream ss {};
    char turn_indicator { m_state.current_player_index == m_players.size() - 1
                              ? '*'
                              : ' ' };
    ss << turn_indicator << m_players.back()->name() << turn_indicator << '\n';
    ss << m_players.back()->score() << '\n';
    ss << m_players.back()->display() << '\n';

    return ss.str();
}