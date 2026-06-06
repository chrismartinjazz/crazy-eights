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
- [ ] Cards and the top discard are shown graphically 
- [x] HumanPlayer can play a card following the rules or draw
- [ ] Game Loop in place

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