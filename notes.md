# Crazy Eights

## Rules

### Players and Deck
- 52-card deck, no jokers
- 2 - 7 players
  - 2-5 players, 52 card deck. 6-7 players, double (104) deck.

### Goal of the game

- Goal is to discard all cards. Game ends when:
  - Deck runs out
  - One player discards all their cards

### Play

- 5 cards dealt to each player - 7 in 2 player game
- Remaining deck in center
- Top card turned face up as first card in discard pile
- Players take turns to take one action
  - Match rank
  - Match suit
  - 8
  - Draw a card
- If can't take an action must draw until an action can be made.
- Play proceeds clockwise

### Scoring

- Player who discards all cards collects points from those remaining cards in opponents hands:
  - 8 - 50
  - J Q K - 10
  - A - 1
  - Others face value
- If deck runs out:
  - Who has lowest points in hand
  - What is the point score of their hand
  - They score difference between their hand and all other hands
  - If multiple lowest scores, share the points
- Points to win = players * 50

### Variations

- Queens skip next players turn
- Ace reverses direction of play
- 2 forces draw 2 - multiple 2's "stack"

## Planning the code

### Objects

- Game to hold everything together and the gamestate
- ScoreBoard
- Display
- Player
  - HumanPlayer
  - ComputerPlayer
- Deck
- Card
  - EightCard

### Implementation steps

- [x] Cards in a Deck
- [x] Deck can be shuffled
- [x] HumanPlayer dealt a hand of 7 cards
- [x] Cards and the top discard are shown graphically 
- [x] HumanPlayer can play a card following the rules or draw
- [x] Game Loop in place

### Game Loop

For 2 players

- While playing
  - Initialize a deck, 2 players, and the play order
  - While no player's score is greater than or equal to 100
    - Deal each player 7 cards
    - Draw one card (top_card)
    - While the draw pile is not empty and no player has 0 cards
      - Display top card of discard pile
      - player_order[0] plays a card, draws or passes
      - If plays a card, add that card to the discard pile
      - Rotate player_order
    - Calculate and display the round scores
  - Congratulate the winner
  - Ask if play again

  ## Refactoring Player class

  Currently this class manages its own score:
    add_to_score, reset_score
  Manages its own hand, which is a vector:
    add_to_hand, reset_hand, cards_remaining, has_valid_card_in_hand
  Chooses which card to play:
    play_card_or_draw, valid
  Reports the points in its hand:
    points_in_hand
  Handles input from the player
    ask_press_enter
    ask_card_index_or_draw
  
  Candidates for refactoring:
  - Extract a Hand object
    - add_to_hand, reset_hand, cards_remaining, has_valid_card_in_hand, points_in_hand
  - Extract an 'input' helper class (there is stuff for this in main too)
    - ask_press_enter
    - ask_card_index_or_draw
  
  In main we also have:
  - display_hand (this should be moved to a hand object)
  - game_over (State::Rules)
  - ask_keep_playing (should probably move to a user interaction namespace) (AskManager)
  - ask_press_enter (duplication)

  ## Refactoring Main

  Plausible methods to extract:
  - initialize (constructor for Game)
  - play_round (loop until a player has higher than max_score) (Game)
    - deal (Game::DealManager)
      - deal_hands_to_players
      - draw_top_card
    - play_hand (loop until a player has no cards or draw pile is empty) (Game::HandManager)
      - update_display
      - handle_play_card_or_draw
      - rotate_players
    - calculate_scores (Game)
      - determine_hand_winners
      - determine_total_points
      - allocate_points_to_winners
      - display_scores
    - reset_hand (Game::HandManager)
  - determine_round_winners 
  - display_final_scores 
  - reset 

  ## Potential Improvements

- [x] Add AI Players
- [x] Add sorting for hands
- [x] Fix scoring mistake
  - Should score difference between each losers hand and the winning score, not the total score,  when the deck runs out
- [x] Improve display - clear screen each action, improve game state information
- [] Fix bug where seg fault occurs when draw pile is empty
- [] Add support for different numbers of players
  - 2-7 players
  - One human player, others are AI players with randomised names
    - deck
      - 2-5 players, 52 card deck
      - 6-7 players, 104 card deck
    - hand size
      - 2 players, 7 cards
      - 3-7 players, 5 cards
- [] Add colored text for hearts and diamonds
- [] Add special card functions
  - [] Queens skip turn
  - [] Aces reverse direction
  - [] Two is draw two (unless game is over)

  ## Display

`std::cout << "\033[31mThis text is Red\033[0m" << std::endl;`

```
~~ CRAZY 8S!!! ~~
** Target: 150 **

*John* ┌──┐─┐─┐                
0      │%%|%|%|                
       └──┘─┘─┘                
Jane   ┌──┐─┐
22     │%%|%|
       └──┘─┘

       ┌────┐┐ ┌────┐┐┐
   *♠* │ 8♣ || │ %  |||
       │    || │  % ||| 
       └────┘┘ └────┘┘┘

Chris
0
┌────┐────┐────┐────┐────┐────┐────┐ 
│ 5♣ | 3♣ | 9♠ | 2♦ | J♠ | J♦ | K♠ |
|    |    |    |    |    |    |    |
└────┘────┘────└────┘────┘────┘────┘
   1    2    3    4    5    6    7

Choose a card or (d) to draw >> 
```

Only display the *♠* current suit indicator if the top card is wild.
Display one card back per 6 cards for the deck and discard pile.