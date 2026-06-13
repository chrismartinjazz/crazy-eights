#ifndef PLAYER_COMPUTER_H
#define PLAYER_COMPUTER_H

#include "card_values.h"
#include "player.h"
#include "state.h"
#include <algorithm>
#include <string_view>
#include <unordered_map>
#include <vector>

class PlayerComputer : public Player
{
    struct HandIndices
    {
        std::unordered_map<card_values::Rank, std::vector<int>> ranks_map {};
        std::unordered_map<card_values::Suit, std::vector<int>> suits_map {};
        std::vector<int> wilds {};
        std::vector<int> playable {};
    };

  private:
    HandIndices m_hand_indices {};

  public:
    PlayerComputer(std::string_view name);
    std::optional<Card> play_card_or_draw(State& state) override;
    card_values::Suit ask_choose_suit() const override;
    void sort_hand() override;
    void draw_two(State& state) override;

  private:
    Card select_card(State& state);
    void update_hand_indices(State& state);
    Card follow_suit(State& state);
    Card follow_rank(Card& top_card);
    std::optional<Card> forced_draw(State& state);
    std::unordered_map<card_values::Suit, int> count_suits() const;
};

#endif