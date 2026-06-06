#include "card.h"
#include "card_values.h"
#include "config.h"
#include "deck.h"
#include "player.h"
#include "state.h"
#include <algorithm>  // for std::all_of
#include <functional> // for std::reference_wrapper
#include <iostream>
#include <optional> // for std::optional
#include <vector>

void display_hand(Player& player)
{
    std::cout << player.name() << ": [ ";
    for (auto& card : player.hand())
        std::cout << card << " ] ";
    std::cout << '\n';
}

bool game_over(State& state)
{
    // The game is over when any player has played all their cards, or,
    // when the draw pile is exhausted and no player can play any of their
    // cards.

    for (auto& player : state.players)
        if (player.cards_remaining() == 0)
        {
            return true;
        }
    if (state.deck.size() == 0)
    {
        for (auto& player : state.players)
            if (player.has_valid_card_in_hand(state))
            {
                return false;
            }
        return true;
    }

    return false;
}

bool ask_keep_playing()
{
    while (true)
    {
        std::cout << "Play again? (y)es or (n)o >> ";
        std::string input;
        if (!std::getline(std::cin, input))
        {
            throw std::runtime_error("Unexpected end of input");
        }

        if (input.length() == 0)
            continue;

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

void ask_press_enter(std::string prompt)
{
    std::cout << prompt;
    std::string temp {};
    std::getline(std::cin, temp);
}

int main()
{
    // Initialize the game
    State state {
        card_values::Suit::Club,                    // current_suit
        { { "P1" }, { "P2" } },                     // players
        { card_values::ranks, card_values::suits }, // deck
    }; // discard_pile is initialized empty;

    bool keep_playing { true };
    while (keep_playing)
    {
        // Main game loop
        // While no player has scored 100 or more
        while (std::all_of(
            state.players.begin(),
            state.players.end(),
            [](Player& player)
            { return player.score() < config::winning_score; }
        ))
        {
            // Deal cards to all players and start the discard pile
            for (Player& player : state.players)
            {
                std::vector<Card> hand { state.deck.deal(config::hand_size) };
                player.add_to_hand(hand);
            }

            Card drawn_card { state.deck.draw_one() };
            bool keep_drawing { true };
            while (keep_drawing)
            {
                if (drawn_card.rank() == card_values::Rank::Eight)
                {
                    std::cout << "It's an 8! Shuffling it in...\n";
                    state.deck.shuffle_in(drawn_card);
                    drawn_card = state.deck.draw_one();
                }
                else
                {
                    keep_drawing = false;
                }
            }
            state.current_suit = drawn_card.suit();
            state.discards.emplace_back(drawn_card);

            // Turn game loop
            while (!game_over(state))
            {
                std::cout << "\nTop card: " << state.discards.back() << " ("
                          << card_values::suit_glyph(state.current_suit)
                          << ")\n";
                display_hand(state.players[0]);
                std::optional<Card> discarded_card {
                    state.players[0].play_card_or_draw(state)
                };
                if (discarded_card)
                {
                    if (discarded_card->rank() == card_values::Rank::Eight)
                        state.current_suit = state.players[0].ask_choose_suit();
                    else
                        state.current_suit = discarded_card->suit();

                    state.discards.emplace_back(*discarded_card);
                }
                std::rotate(
                    state.players.begin(),
                    state.players.begin() + 1,
                    state.players.end()
                );
            } // End turn game loop

            // Calculate and update the scores
            std::vector<std::reference_wrapper<Player>> winners {};
            int total_score { 0 };

            for (auto& player : state.players)
                if (player.cards_remaining() == 0)
                    winners.emplace_back(std::ref(player));

            if (winners.empty())
            {
                int min_score { 0 };
                for (auto& player : state.players)
                    min_score = std::min(min_score, player.points_in_hand());

                for (auto& player : state.players)
                    if (player.points_in_hand() == min_score)
                        winners.emplace_back(std::ref(player));
            }

            for (auto& player : state.players)
            {
                bool is_winner { std::any_of(
                    winners.begin(),
                    winners.end(),
                    [&](std::reference_wrapper<Player> winner)
                    { return &winner.get() == &player; }
                ) };
                if (!is_winner)
                    total_score += player.points_in_hand();
            }

            int score_per_winner =
                total_score / static_cast<int>(winners.size());
            for (auto& winner : winners)
            {
                winner.get().add_to_score(score_per_winner);
            }

            // Display the scores
            std::cout << "-- Current scores (" << config::winning_score
                      << " to win) --\n";
            for (auto& player : state.players)
                std::cout << player.name() << ": " << player.score() << '\n';
            ask_press_enter(
                "Dealing...\nPress enter to play the next hand...\n"
            );

            // Reset for next round
            for (auto& player : state.players)
                player.reset_hand();
            state.discards.clear();
            state.deck.reset();
        }

        // Identify the winner/s
        int highest_score { 0 };
        for (auto& player : state.players)
            highest_score = std::max(highest_score, player.score());

        std::vector<std::reference_wrapper<Player>> winners {};
        for (auto& player : state.players)
            if (player.score() == highest_score)
                winners.emplace_back(std::ref(player));

        // Announce final scores
        if (winners.size() == 1)
            std::cout << "The winner is " << winners[0].get().name() << "!\n";
        else
        {
            std::cout << "The winners are";
            for (auto& winner : winners)
                std::cout << ", " << winner.get().name();
            std::cout << ".\n";
        }

        std::cout << "Final scores: ";
        for (auto& player : state.players)
        {
            std::cout << player.name() << ": " << player.score() << '\n';
        }
        std::cout << '\n';

        keep_playing = ask_keep_playing();
        if (keep_playing)
        {
            // Reset for next round
            for (auto& player : state.players)
            {
                player.reset_hand();
                player.reset_score();
            }
            state.discards.clear();
            state.deck = Deck { card_values::ranks, card_values::suits };
        }
    } // End main game loop
    return 0;
}