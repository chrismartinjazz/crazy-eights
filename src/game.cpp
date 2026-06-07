#include "card_values.h"
#include "config.h"
#include "player_human.h"
#include "state.h"
#include "ui_helpers.h"
#include <algorithm> // for std::rotate, std::any_of
#include <vector>

class Game
{
  private:
    State m_state {};
    std::vector<PlayerHuman> m_players {};
    std::vector<std::reference_wrapper<PlayerHuman>> m_winners {};

  public:
    Game()
        : m_state {
            {},                                         // rules
            card_values::Suit::Club,                    // current_suit
            { card_values::ranks, card_values::suits }, // deck
            {}                                          // discard_pile
        }
        , m_players { { "P1" }, { "P2" } }
    {
    }

    void game_loop()
    {
        while (true)
        {
            while (!a_player_has_won())
            {
                deal();
                play_round();
                update_scores();
                display_scores();
                reset_round();
            }

            identify_winners();
            announce_final_scores();
            if (!ask_keep_playing())
                return;

            reset_game();
        }
    }

  private:
    bool a_player_has_won()
    {
        for (auto& player : m_players)
            if (player.score() >= config::winning_score)
                return true;
        return false;
    }

    void deal()
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

            std::cout << "We drew "
                      << card_values::rank_letter(drawn_card.rank())
                      << "! Shuffling it in...\n";
            m_state.deck.shuffle_in(drawn_card);
        }
    }

    void play_round()
    {
        while (!round_is_over())
        {
            Player& current_player { m_players[0] };
            std::cout << "\nTop card: " << m_state.discards.back() << " ("
                      << card_values::suit_glyph(m_state.current_suit) << ")\n";
            std::cout << current_player.name() << ": ";
            current_player.display_hand();
            std::cout << '\n';

            std::optional<Card> discarded_card {
                current_player.play_card_or_draw(m_state)
            };
            if (discarded_card)
            {
                if (discarded_card->is_wild())
                    m_state.current_suit = current_player.ask_choose_suit();
                else
                    m_state.current_suit = discarded_card->suit();

                m_state.discards.emplace_back(*discarded_card);
            }
            std::rotate(
                m_players.begin(), m_players.begin() + 1, m_players.end()
            );
        }
    }

    void update_scores()
    {
        // Calculate and update the scores
        std::vector<std::reference_wrapper<PlayerHuman>> winners {};
        int total_score { 0 };

        for (auto& player : m_players)
            if (player.cards_remaining() == 0)
                winners.emplace_back(std::ref(player));

        if (winners.empty())
        {
            int min_score { 0 };
            for (auto& player : m_players)
                min_score = std::min(min_score, player.points_in_hand());

            for (auto& player : m_players)
                if (player.points_in_hand() == min_score)
                    winners.emplace_back(std::ref(player));
        }

        for (auto& player : m_players)
        {
            bool is_winner { std::any_of(
                winners.begin(),
                winners.end(),
                [&](std::reference_wrapper<PlayerHuman> winner)
                { return &winner.get() == &player; }
            ) };
            if (!is_winner)
                total_score += player.points_in_hand();
        }

        int score_per_winner = total_score / static_cast<int>(winners.size());
        for (auto& winner : winners)
        {
            winner.get().add_to_score(score_per_winner);
        }
    }

    void display_scores()
    {
        // Display the scores
        std::cout << "-- Current scores (" << config::winning_score
                  << " to win) --\n";
        for (auto& player : m_players)
            std::cout << player.name() << ": " << player.score() << '\n';
        ui::ask_press_enter(
            "Dealing...\nPress enter to play the next hand...\n"
        );
    }

    void reset_round()
    {
        for (auto& player : m_players)
            player.reset_hand();
        m_state.discards.clear();
        m_state.deck.reset();
    }

    void identify_winners()
    {
        int highest_score { 0 };
        for (auto& player : m_players)
            highest_score = std::max(highest_score, player.score());

        m_winners.clear();
        for (auto& player : m_players)
            if (player.score() == highest_score)
                m_winners.emplace_back(std::ref(player));
    }

    void announce_final_scores()
    {
        if (m_winners.size() == 1)
            std::cout << "The winner is " << m_winners[0].get().name() << "!\n";
        else
        {
            std::cout << "The winners are";
            for (auto& winner : m_winners)
                std::cout << ", " << winner.get().name();
            std::cout << ".\n";
        }

        std::cout << "Final scores: ";
        for (auto& player : m_players)
        {
            std::cout << player.name() << ": " << player.score() << '\n';
        }
        std::cout << '\n';
    }

    void reset_game()
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
    bool round_is_over()
    {
        for (auto& player : m_players)
            if (player.cards_remaining() == 0)
                return true;

        if (m_state.deck.cards_remaining() == 0)
        {
            for (auto& player : m_players)
                if (player.has_valid_card_in_hand(m_state))
                    return false;

            return true;
        }

        return false;
    }

    bool ask_keep_playing()
    {
        while (true)
        {
            std::string input {
                ui::ask_input("Play again? (y)es or (n)o >> ")
            };
            switch (static_cast<char>(input[0]))
            {
            case 'y': return true;
            case 'n': return false;
            default:
            {
                std::cout << "Enter y or n.\n";
                continue;
            }
            }
        }
    }
};