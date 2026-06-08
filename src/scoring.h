#ifndef SCORING_H
#define SCORING_H

#include <memory>
#include <vector>

class Player;

namespace scoring
{
    std::vector<std::size_t>
    identify_round_winners(const std::vector<std::unique_ptr<Player>>& players);
    int calculate_round_score(
        const std::vector<std::unique_ptr<Player>>& players,
        const std::vector<std::size_t>& winner_indices
    );
    void award_round_scores(
        std::vector<std::unique_ptr<Player>>& players,
        const std::vector<std::size_t>& winner_indices,
        int score
    );
    std::vector<std::size_t>
    identify_winners(const std::vector<std::unique_ptr<Player>>& players);
    void announce_final_scores(
        const std::vector<std::unique_ptr<Player>>& players,
        std::vector<std::size_t>& winner_indices
    );
    void display_scores(const std::vector<std::unique_ptr<Player>>& players);
}; // namespace scoring

#endif