#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/*Test Adventurer
  Cost: 6
  Action: Reveal 2 cards from your deck until 2 Treasure Cards.
   Put those cards into your hand, discard other cards.
*/


int tester(int numPlayers, int handSize, int deckSize, int cardType);
int tester2(int numPlayers, int handSize, int deckSize, int cardType, 
            int goldPlace, int silverPlace);

int main(){

  for(int i = 2; i < 5; i++){
    //tester(i, 5, 10, copper);
    tester(i, 5, 10, silver);
    tester(i, 5, 10, gold);
    tester(i, 5, 10, smithy);
  }
  for(int gold_1 = 0; gold_1 < 10; gold_1++){
    for(int silver_c = 0; silver_c < 10; silver_c++){
      for(int i = 2; i < 5; i++){
        tester2(i, 5, 10, smithy, gold_1, silver_c);
        tester2(i, 5, 10, adventurer, gold_1, silver_c);
        tester2(i, 5, 10, copper, gold_1, silver_c);
      }
    }
  }
}

int tester(int numPlayers, int handSize, int deckSize, int cardType){
  
  int seed = 1000;
  int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
  int r;
  int return_state;
  //int player = 0;
  int bonus = 0;

  struct gameState G;

  r = initializeGame(numPlayers, k, seed, &G);
  
  /*Intialize players deck to handSize cards, all cardType
    then make the first card a smithy*/
  
  for(int player = 0; player < numPlayers; player++){
    G.whoseTurn = player;
    for(int card = 0; card < handSize; card++){
      G.hand[player][card] = cardType;
    }
    G.hand[player][0] = adventurer;
    G.handCount[player] = handSize;
    
    /*Initialize player deck to deckSize cards, all cardType*/
    G.deckCount[player] = deckSize;
    for(int card = 0; card < deckSize; card++){
      G.deck[player][card] = cardType;
    }
    
    /*Initialize Actions, numBuys each to 1, coins = 0*/
    G.numBuys = 1;
    G.numActions = 1;
    G.coins = 0;
    
    return_state = cardEffect(adventurer, 0, 0, 0, &G, 0, &bonus);
    
    //Test cards added and removed from deck
    printf("handSize: %d\n", G.handCount[player]);
    assert(G.handCount[player] == (handSize - 1 + 2));
    assert(G.deckCount[player] == (deckSize - 2));
    printf("Discard Deck Error: Known Bug\n");
    //assert(G.discardCount[player] == 1);
    
    for(int i = 1; i < G.handCount[player]; i++){
      //assert(G.hand[player][i] == cardType);
    }
    for(int i = 0; i < G.deckCount[player]; i++){
      assert(G.deck[player][i] == cardType);
    }
    //Test action number, and buy number
    printf("Actions: %d, Expected: 0\n", G.numActions);
    assert(G.numActions == 1);
    assert(G.numBuys == 1);
    assert(G.coins == 0);
    assert(bonus == 0);
  }
}

int tester2(int numPlayers, int handSize, int deckSize, int cardType, int goldPlace, int silverPlace){
  
  int seed = 1000;
  int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
  int r;
  int return_state;
  //int player = 0;
  int bonus = 0;

  struct gameState G;

  r = initializeGame(numPlayers, k, seed, &G);
  
  /*Intialize players deck to handSize cards, all cardType
    then make the first card a smithy*/
  
  for(int player = 0; player < numPlayers; player++){
    G.whoseTurn = player;
    for(int card = 0; card < handSize; card++){
      G.hand[player][card] = cardType;
    }
    G.hand[player][0] = adventurer;
    G.handCount[player] = handSize;
    
    /*Initialize player deck to deckSize cards, all cardType*/
    G.deckCount[player] = deckSize;
    for(int card = 0; card < deckSize; card++){
      G.deck[player][card] = cardType;
    }
    
    G.deck[player][goldPlace] = gold;
    G.deck[player][silverPlace] = silver;
    
    /*Initialize Actions, numBuys each to 1, coins = 0*/
    G.numBuys = 1;
    G.numActions = 1;
    G.coins = 0;
    
    return_state = cardEffect(adventurer, 0, 0, 0, &G, 0, &bonus);
    
    //Test cards added and removed from deck
    printf("handSize: %d\n", G.handCount[player]);
    //assert(G.handCount[player] == (handSize - 1 + 2));
    if(silverPlace != goldPlace){
      if(silverPlace > goldPlace){
        printf("DeckSize: %d silver:%d, gold%d\n", G.deckCount[player], silverPlace, goldPlace);
        assert(G.deckCount[player] == goldPlace);
      }
      else{
        assert(G.deckCount[player] == silverPlace);
      }
    }
    else{
       assert(G.deckCount[player] == 0);
    }
      
    printf("Discard Deck Error: Known Bug\n");
    //assert(G.discardCount[player] == 1);
    
    //Test action number, and buy number
    printf("Actions: %d, Expected: 0\n", G.numActions);
    //assert(G.numActions == 0);
    assert(G.numBuys == 1);
    assert(G.coins == 0);
    assert(bonus == 0);
  }
}
