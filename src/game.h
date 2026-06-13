#ifndef GAME_H
#define GAME_H

#include "display.h"
#include "player.h"
#include <memory>
#include <string>
#include <vector>

class Game
{
  private:
    State m_state {};
    std::vector<std::unique_ptr<Player>> m_players {};
    Display m_display;

  public:
    static Game create();
    void game_loop();

  private:
    Game(std::string_view player_name, int num_players);
    bool a_player_has_won() const;
    void deal();
    void play_round();
    void update_scores();
    void reset_round();
    void reset_game();
    bool round_is_over() const;
    void handle_discarded_card(const Card& card, Player& player);
    bool ask_keep_playing() const;
    std::vector<std::string> random_names(int count) const;
    std::vector<std::unique_ptr<Player>>
    create_players(std::string_view player_name, int number_of_players);
};

#endif