#include "game.h"
#include "card_values.h"
#include "config.h"
#include "player_human.h"
#include "scoring.h"
#include "state.h"
#include "ui_helpers.h"
#include <algorithm> // for std::rotate, std::any_of
#include <vector>

Game::Game()
    : m_state { .rules = {},
                .current_suit = {},
                .deck = { card_values::ranks, card_values::suits },
                .discards = {} }
    , m_players { { "P1" }, { "P2" } }
{
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

bool Game::a_player_has_won()
{
    return std::any_of(
        m_players.begin(),
        m_players.end(),
        [](const PlayerHuman& player)
        { return player.score() >= config::winning_score; }
    );
}

void Game::deal()
{
    for (PlayerHuman& player : m_players)
    {
        std::vector<Card> hand { m_state.deck.deal(config::hand_size) };
        player.add_to_hand(hand);
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
        Player& current_player { m_players[0] };
        display_turn_info(current_player);

        std::optional<Card> discarded_card {
            current_player.play_card_or_draw(m_state)
        };
        if (discarded_card)
        {
            handle_discarded_card(*discarded_card, current_player);
            m_state.discards.emplace_back(*discarded_card);
        }
        std::rotate(m_players.begin(), m_players.begin() + 1, m_players.end());
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
    for (auto& player : m_players)
        player.reset_hand();
    m_state.discards.clear();
    m_state.deck.reset();
}

void Game::reset_game()
{
    for (auto& player : m_players)
    {
        player.reset_hand();
        player.reset_score();
    }
    m_state.discards.clear();
    m_state.deck.reset();
}

// The game is over when any player has played all their cards, or,
// when the draw pile is exhausted and no player can play any of their
// cards.
bool Game::round_is_over()
{
    if (std::any_of(
            m_players.begin(),
            m_players.end(),
            [](const PlayerHuman& player)
            { return player.cards_remaining() == 0; }
        ))
        return true;

    if (m_state.deck.cards_remaining() == 0)
        return (std::none_of(
            m_players.begin(),
            m_players.end(),
            [&](PlayerHuman& player)
            { return player.has_valid_card_in_hand(m_state); }
        ));

    return false;
}

void Game::display_turn_info(const Player& current_player) const
{
    std::cout << "\n\nTop card: " << m_state.discards.back() << " ("
              << card_values::suit_glyph(m_state.current_suit) << ")\n\n";
    std::cout << current_player.name() << ": ";
    current_player.display_hand();
    std::cout << "\n\n";
}

void Game::handle_discarded_card(const Card& card, Player& player)
{
    if (card.is_wild())
        m_state.current_suit = player.ask_choose_suit();
    else
        m_state.current_suit = card.suit();
}

bool Game::ask_keep_playing()
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