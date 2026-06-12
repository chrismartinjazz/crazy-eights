#include "game.h"
#include "card_values.h"
#include "config.h"
#include "player.h"
#include "player_computer.h"
#include "player_human.h"
#include "random_mt.h"
#include "scoring.h"
#include "state.h"
#include "ui_helpers.h"
#include <algorithm> // for std::rotate, std::any_of
#include <cassert>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

Game::Game()
    : m_state { .rules = {},
                .current_suit = {},
                .deck = { card_values::ranks, card_values::suits },
                .discards = {} }
    , m_current_player_index { 0 }
{
    ui::clear_terminal();
    initialize_players();
}

void Game::game_loop()
{
    while (true)
    {
        while (!a_player_has_won())
        {
            deal();
            play_round();
            update_scores();
            scoring::display_scores(m_players);
            ui::ask_press_enter(
                "Dealing...\nPress enter to play the next hand...\n"
            );
            reset_round();
        }

        std::vector<std::size_t> winner_indices {
            scoring::identify_winners(m_players)
        };
        scoring::announce_final_scores(m_players, winner_indices);
        if (!ask_keep_playing())
            return;

        reset_game();
    }
}

// private

bool Game::a_player_has_won() const
{
    return std::any_of(
        m_players.begin(),
        m_players.end(),
        [&](const std::unique_ptr<Player>& player)
        {
            return player->score() >= config::winning_score_per_player *
                                          static_cast<int>(m_players.size());
        }
    );
}

void Game::deal()
{
    int hand_size { m_players.size() > 2 ? config::hand_size
                                         : config::hand_size_two_players };
    for (std::unique_ptr<Player>& player : m_players)
    {
        std::vector<Card> hand { m_state.deck.deal(hand_size) };
        player->add_to_hand(hand);
    }

    while (true)
    {
        Card drawn_card { m_state.deck.draw_one() };

        if (!drawn_card.is_wild())
        {
            m_state.current_suit = drawn_card.suit();
            m_state.discards.emplace_back(drawn_card);
            return;
        }

        std::cout << "We drew " << drawn_card << "! Shuffling it in...\n";
        m_state.deck.shuffle_in(drawn_card);
    }
}

void Game::play_round()
{
    while (!round_is_over())
    {
        Player& current_player { *m_players[m_current_player_index] };
        current_player.sort_hand();
        display_turn_info();

        std::optional<Card> discarded_card {
            current_player.play_card_or_draw(m_state)
        };
        if (discarded_card)
        {
            handle_discarded_card(*discarded_card, current_player);
            m_state.discards.emplace_back(*discarded_card);
        }
        m_current_player_index =
            (m_current_player_index + 1) % m_players.size();
    }
}

void Game::update_scores()
{
    std::vector<std::size_t> round_winner_indices {
        scoring::identify_round_winners(m_players)
    };
    int round_score {
        scoring::calculate_round_score(m_players, round_winner_indices)
    };
    scoring::award_round_scores(m_players, round_winner_indices, round_score);
}

void Game::reset_round()
{
    for (std::unique_ptr<Player>& player : m_players)
        player->reset_hand();
    m_state.discards.clear();
    m_state.deck.reset();
}

void Game::reset_game()
{
    for (std::unique_ptr<Player>& player : m_players)
    {
        player->reset_hand();
        player->reset_score();
    }
    m_state.discards.clear();
    m_state.deck.reset();
}

// The game is over when any player has played all their cards, or,
// when the draw pile is exhausted and no player can play any of their
// cards.
bool Game::round_is_over() const
{
    if (std::any_of(
            m_players.begin(),
            m_players.end(),
            [](const std::unique_ptr<Player>& player)
            { return player->cards_remaining() == 0; }
        ))
        return true;

    if (m_state.deck.cards_remaining() == 0)
        return (std::none_of(
            m_players.begin(),
            m_players.end(),
            [&](const std::unique_ptr<Player>& player)
            { return player->has_valid_card_in_hand(m_state); }
        ));

    return false;
}

void Game::display_turn_info() const
{
    ui::clear_terminal();
    std::cout << "~~ CRAZY 8S!!! ~~\n";
    std::cout << "** Target:"
              << config::winning_score_per_player * m_players.size() << " **";
    std::cout << '\n';

    // For each computer player (human player is last in vector)
    // Display their name, score, and the backs of their cards.
    for (std::size_t i { 0 }; i < m_players.size() - 1; ++i)
    {
        std::stringstream line1 {};
        if (i == m_current_player_index)
            line1 << '*' << m_players[i]->name() << '*'
                  << std::string(
                         config::player_indent - m_players[i]->name().length() -
                             2,
                         ' '
                     );
        else
            line1 << m_players[i]->name()
                  << std::string(
                         config::player_indent - m_players[i]->name().length(),
                         ' '
                     );
        line1 << "┌──┐";
        for (int j { 0 }; j < m_players[i]->cards_remaining() - 1; ++j)
            line1 << "─┐";
        line1 << '\n';

        std::stringstream line2 {};
        line2 << m_players[i]->score()
              << std::string(
                     static_cast<std::size_t>(
                         config::player_indent -
                         ui::count_digits(m_players[i]->score())
                     ),
                     ' '
                 );
        line2 << "│%%│";
        for (int j { 0 }; j < m_players[i]->cards_remaining() - 1; ++j)
            line2 << "%│";
        line2 << '\n';

        std::stringstream line3 {};
        line3 << std::string(config::player_indent, ' ');
        line3 << "└──┘";
        for (int j { 0 }; j < m_players[i]->cards_remaining() - 1; ++j)
            line3 << "─┘";
        line3 << '\n';

        std::cout << line1.str() << line2.str() << line3.str();
    }
    std::cout << '\n';

    // Display the top card of the discard pile, and a representation of the
    // deck. Also display the current suit, if the discard pile has a wild card.
    std::stringstream line1 {};
    line1 << std::string(config::player_indent, ' ') << "┌────┐";
    for (std::size_t i { 0 };
         i < m_state.discards.size() / config::deck_cards_per_line;
         ++i)
        line1 << "┐";
    line1 << " ┌────┐";
    for (int i { 0 };
         i < m_state.deck.cards_remaining() / config::deck_cards_per_line;
         ++i)
        line1 << "┐";
    line1 << '\n';

    std::stringstream line2 {};
    if (m_state.discards.back().is_wild())
        line2 << std::string(config::player_indent - 4, ' ') << '*'
              << card_values::suit_glyph(m_state.current_suit) << "* ";
    else
        line2 << std::string(config::player_indent, ' ');
    line2 << "│ " << m_state.discards.back() << " │";
    for (std::size_t i { 0 };
         i < m_state.discards.size() / config::deck_cards_per_line;
         ++i)
        line2 << "│";
    line2 << " │ %  │";
    for (int i { 0 };
         i < m_state.deck.cards_remaining() / config::deck_cards_per_line;
         ++i)
        line2 << "│";
    line2 << '\n';

    std::stringstream line3 {};
    line3 << std::string(config::player_indent, ' ') << "│    │";
    for (std::size_t i { 0 };
         i < m_state.discards.size() / config::deck_cards_per_line;
         ++i)
        line3 << "│";
    line3 << " │  % │";
    for (int i { 0 };
         i < m_state.deck.cards_remaining() / config::deck_cards_per_line;
         ++i)
        line3 << "│";
    line3 << '\n';

    std::stringstream line4 {};
    line4 << std::string(config::player_indent, ' ') << "└────┘";
    for (std::size_t i { 0 };
         i < m_state.discards.size() / config::deck_cards_per_line;
         ++i)
        line4 << "┘";
    line4 << " └────┘";
    for (int i { 0 };
         i < m_state.deck.cards_remaining() / config::deck_cards_per_line;
         ++i)
        line4 << "┘";
    line4 << '\n';

    std::cout << line1.str() << line2.str() << line3.str() << line4.str();

    if (m_current_player_index == m_players.size() - 1)
        std::cout << '*' << m_players.back()->name() << "*\n";
    else
        std::cout << '*' << m_players.back()->name() << "*\n";

    std::cout << m_players.back()->score() << '\n';

    m_players.back()->display_hand();
    std::cout << '\n';
}

void Game::handle_discarded_card(const Card& card, Player& player)
{
    if (card.is_wild())
        m_state.current_suit = player.ask_choose_suit();
    else
        m_state.current_suit = card.suit();
}

bool Game::ask_keep_playing() const
{
    while (true)
    {
        std::string input { ui::ask_input("Play again? (y)es or (n)o >> ") };
        switch (std::tolower(static_cast<unsigned char>(input[0])))
        {
        case 'y': return true;
        case 'n': return false;
        default:
        {
            std::cout << "Enter y or n.\n";
            break;
        }
        }
    }
}

int Game::ask_number_of_players() const
{
    while (true)
    {
        std::string input { ui::ask_input("How many players? (2 - 5) >> ") };
        std::stringstream ss { input };
        int choice {};
        if (ss >> choice)
        {
            if (choice >= 2 || choice <= 5)
                return choice;

            std::cout << "Type a number between 2 and 5\n";
            continue;
        }

        std::cout << "Type a number\n";
    }
}

std::string Game::ask_player_name() const
{
    while (true)
    {
        std::string name { ui::ask_input("What is your name? >> ") };
        if (name.length() > config::max_name_length)
        {
            std::cout << "That name is too long - try again";
            continue;
        }
        return name;
    }
}

std::vector<std::string> Game::random_names(int count) const
{
    assert(count <= 16 && "Game::random_names() maximum count of names is 16");

    std::vector<std::string> names { "Luca",   "Noah",   "Jack",   "James",
                                     "Mia",    "Olivia", "Sophie", "Michael",
                                     "John",   "David",  "Lisa",   "Mary",
                                     "Ashley", "Paul",   "Brian",  "Karen" };
    std::vector<std::string> output {};
    for (int i { 0 }; i < count; ++i)
    {
        int name_index {
            random_mt::get(0, static_cast<int>(names.size() - 1))
        };
        std::size_t name_index_size_t { static_cast<std::size_t>(name_index) };
        output.push_back(std::move(names[name_index_size_t]));
        names.erase(names.begin() + name_index);
    }
    return output;
}

void Game::initialize_players()
{
    std::string player_name { ask_player_name() };
    int number_of_players { ask_number_of_players() };
    std::vector<std::string> computer_player_names {
        random_names(number_of_players - 1)
    };

    for (std::size_t i { 0 };
         i < static_cast<std::size_t>(number_of_players - 1);
         ++i)
    {
        m_players.emplace_back(
            std::make_unique<PlayerComputer>(
                std::move(computer_player_names[i])
            )
        );
    }

    // The human player must be the last position in the vector.
    m_players.emplace_back(std::make_unique<PlayerHuman>(player_name));
}