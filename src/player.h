#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "card_values.h"
#include "hand.h"
#include <optional> // for std::optional
#include <string>
#include <string_view>
#include <vector>

struct State;

class Player
{
  private:
    std::string_view m_name {};
    Hand m_hand {};
    int m_score {};

  public:
    Player(std::string_view name);
    std::string_view name() const;
    int score() const;
    Hand hand() const;

    void add_to_score(int points);
    void reset_score();
    void add_to_hand(std::vector<Card>& cards);
    void reset_hand();
    int cards_remaining() const;
    bool has_valid_card_in_hand(State& state);
    std::optional<Card> play_card_or_draw(State& state);
    card_values::Suit ask_choose_suit();
    int points_in_hand() const;

  private:
    bool valid(State& state, Card& card);
    void ask_press_enter(std::string prompt);
    std::optional<int> ask_card_index_or_draw(
        std::vector<int> valid_indices, bool draw_is_possible
    );
};

#endif