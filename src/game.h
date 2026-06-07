#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "player_human.h"
#include <vector>

class Game
{
  private:
    State m_state {};
    std::vector<PlayerHuman> m_players {};

  public:
    Game();
    void game_loop();

  private:
    bool a_player_has_won();
    void deal();
    void play_round();
    void update_scores();
    void display_scores();
    void reset_round();
    std::vector<int> identify_winners();
    void announce_final_scores(const std::vector<int>& winner_indices);
    void reset_game();
    bool round_is_over();
    void display_turn_info(const Player& current_player) const;
    void handle_discarded_card(const Card& card, Player& player);
    std::vector<int> identify_round_winners();
    int calculate_round_score(const std::vector<int> winner_indices);
    void award_scores(const std::vector<int> winner_indices, int score);
    bool ask_keep_playing();
};

#endif