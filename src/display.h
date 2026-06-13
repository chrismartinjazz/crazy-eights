#ifndef DISPLAY_H
#define DISPLAY_H

#include "player.h"
#include "state.h"
#include <memory>
#include <string>
#include <string_view>

class Display
{
  private:
    const State& m_state {};
    const std::vector<std::unique_ptr<Player>>& m_players {};
    int m_draw_size {};
    int m_discard_size {};

  public:
    Display(
        const State& state, const std::vector<std::unique_ptr<Player>>& players
    );
    void render();

  private:
    std::string render_header() const;
    std::string render_computer_players() const;
    std::string render_deck();
    std::string build_deck(
        int cards, std::string_view first, std::string_view repeater
    ) const;
    std::string build_draw_and_discard(
        std::string_view indent,
        std::string_view discard,
        std::string_view draw,
        std::string_view repeater
    );
    std::string render_player() const;
};

#endif