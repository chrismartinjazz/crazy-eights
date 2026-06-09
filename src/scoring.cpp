#include "scoring.h"
#include "config.h"
#include "player.h"
#include <iostream>
#include <vector>

class Player;

std::vector<std::size_t> scoring::identify_round_winners(
    const std::vector<std::unique_ptr<Player>>& players
)
{
    std::vector<std::size_t> winner_indices {};
    // Any player with no cards remaining is a winner.
    for (std::size_t i { 0 }; i < players.size(); ++i)
        if (players[i]->cards_remaining() == 0)
            winner_indices.push_back(i);

    // If all players have cards remaining, identify the player/s with the
    // lowest score/s as the winner/s
    if (winner_indices.empty())
    {
        int min_score { players[0]->score() };
        for (auto& player : players)
            min_score = std::min(min_score, player->points_in_hand());

        for (std::size_t i { 0 }; i < players.size(); ++i)
            if (players[i]->points_in_hand() == min_score)
                winner_indices.push_back(i);
    }
    return winner_indices;
}

int scoring::calculate_round_score(
    const std::vector<std::unique_ptr<Player>>& players,
    const std::vector<std::size_t>& winner_indices
)
{
    // Winners by definition all have the same score in their hands, so just
    // retrieve the first winners score.
    int winning_score { players[winner_indices[0]]->points_in_hand() };

    // Add the difference between the players score and the winning score to the
    // round score. For the winner/s, this means adding 0 points.
    int round_score { 0 };
    for (auto& player : players)
        round_score += player->points_in_hand() - winning_score;

    return round_score;
}

void scoring::award_round_scores(
    std::vector<std::unique_ptr<Player>>& players,
    const std::vector<std::size_t>& winner_indices,
    int score
)
{
    // Distribute the points evenly to the winners.
    int score_per_winner { score / static_cast<int>(winner_indices.size()) };
    for (std::size_t i : winner_indices)
        players[i]->add_to_score(score_per_winner);
}

std::vector<std::size_t>
scoring::identify_winners(const std::vector<std::unique_ptr<Player>>& players)
{
    int highest_score { players[0]->score() };
    for (auto& player : players)
        highest_score = std::max(highest_score, player->score());

    std::vector<std::size_t> winner_indices {};
    for (std::size_t i { 0 }; i < players.size(); ++i)
        if (players[i]->score() == highest_score)
            winner_indices.push_back(i);

    return winner_indices;
}

void scoring::announce_final_scores(
    const std::vector<std::unique_ptr<Player>>& players,
    std::vector<std::size_t>& winner_indices
)
{
    if (winner_indices.size() == 1)
        std::cout
            << "The winner is "
            << players[static_cast<std::size_t>(winner_indices[0])]->name()
            << "!\n";
    else
    {
        std::cout << "The winners are ";
        std::cout << players[winner_indices[0]]->name();
        for (std::size_t i { 1 }; i < winner_indices.size(); ++i)
            std::cout << ", " << players[winner_indices[i]]->name();
        std::cout << "!\n";
    }

    std::cout << "Final scores: \n";
    for (auto& player : players)
    {
        std::cout << player->name() << ": " << player->score() << '\n';
    }
    std::cout << '\n';
}

void scoring::display_scores(
    const std::vector<std::unique_ptr<Player>>& players
)
{
    std::cout << "-- Current scores (" << config::winning_score
              << " to win) --\n";
    for (auto& player : players)
        std::cout << player->name() << ": " << player->score() << '\n';
}