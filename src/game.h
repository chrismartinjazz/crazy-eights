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
    void reset_round();
    void reset_game();
    bool round_is_over();
    void display_turn_info(const Player& current_player) const;
    void handle_discarded_card(const Card& card, Player& player);
    bool ask_keep_playing();
};

#endif