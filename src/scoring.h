#ifndef SCORING_H
#define SCORING_H

#include <vector>

class PlayerHuman;

namespace scoring
{
    std::vector<std::size_t>
    identify_round_winners(const std::vector<PlayerHuman>& players);
    int calculate_round_score(
        const std::vector<PlayerHuman>& players,
        const std::vector<std::size_t>& winner_indices
    );
    void award_round_scores(
        std::vector<PlayerHuman>& players,
        const std::vector<std::size_t>& winner_indices,
        int score
    );
    std::vector<std::size_t>
    identify_winners(const std::vector<PlayerHuman>& players);
    void announce_final_scores(
        const std::vector<PlayerHuman>& players,
        std::vector<std::size_t>& winner_indices
    );
    void display_scores(const std::vector<PlayerHuman>& players);
}; // namespace scoring

#endif