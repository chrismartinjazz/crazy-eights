#include "card.h"
#include "card_values.h"
#include "player.h"
#include "state.h"
#include <optional>
#include <string>
#include <string_view>
#include <vector>

class PlayerHuman : public Player
{
  public:
    PlayerHuman(std::string_view name);
    std::optional<Card> play_card_or_draw(State& state) override;
    card_values::Suit ask_choose_suit() override;

  private:
    std::optional<Card>
    play_or_voluntary_draw(State& state, std::vector<int>& playable_indices);
    std::optional<Card> forced_draw(State& state);
    std::optional<int> ask_card_index_or_draw(
        std::vector<int>& valid_indices, bool draw_is_possible
    );
    std::string build_prompt(bool draw_is_possible);
    Card draw_one_and_announce(State& state);
    std::optional<int> parse_card_index(
        std::string_view input,
        const std::vector<int>& valid_indices,
        bool draw_is_possible
    );
};