#ifndef GAME_H
#define GAME_H

#include "player.h"
#include <memory>
#include <vector>

class Game
{
  private:
    State m_state {};
    std::vector<std::unique_ptr<Player>> m_players {};
    std::size_t m_current_player_index { 0 };

  public:
    Game();
    void game_loop();

  private:
    bool a_player_has_won() const;
    void deal();
    void play_round();
    void update_scores();
    void reset_round();
    void reset_game();
    bool round_is_over() const;
    void display_turn_info() const;
    void handle_discarded_card(const Card& card, Player& player);
    bool ask_keep_playing() const;
};

#endif