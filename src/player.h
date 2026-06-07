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
  protected:
    std::string_view m_name {};
    Hand m_hand {};
    int m_score {};

  public:
    Player(std::string_view name);
    virtual std::optional<Card> play_card_or_draw(State& state) = 0;
    virtual card_values::Suit ask_choose_suit() = 0;
    virtual ~Player() = default;

    std::string_view name() const;
    int score() const;
    void add_to_score(int points);
    void reset_score();

    void display_hand() const;
    void reset_hand();
    void add_to_hand(std::vector<Card>& cards);
    int cards_remaining() const;
    int points_in_hand() const;
    bool has_valid_card_in_hand(State& state);
};

#endif