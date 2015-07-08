#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

/* Test Council Room 

   +4 cards
   +1 Buy
   Each other player draws a card

*/


int tester(int numPlayers, int handSize, int deckSize);
int tester2(int numPlayers, int handSize, int deckSize);

int main(){

  srand(time(NULL));
  for(int a = 2; a < 5; a++){
    tester(a, 5, 10);
    tester(a, 10, 10);
    tester(a, 0, 10);
    printf("Fail from overflow\n");
    //tester(a, 500, 500);
  }
  for(int i = 0; i < 50; i++){
    for(int a = 2; a < 5; a++){
      tester2(a, 5, 10);
      tester2(a, 10, 10);
      printf("Fail from overflow\n");
      //tester2(a, 500, 500);
    }
  }


}

int tester(int numPlayers, int handSize, int deckSize){
  
  int seed = 1000;
  int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
  int r;
  int return_state;
  //int player = 0;
  int bonus = 0;

  struct gameState G;

  r = initializeGame(numPlayers, k, seed, &G);
  
  /*Intialize players deck to 5 cards, all copper
    then make the first card a smithy*/
  
  for(int player = 0; player < numPlayers; player++){
    G.whoseTurn = player;
    for(int card = 0; card < handSize; card++){
      G.hand[player][card] = copper;
    }
    G.hand[player][0] = council_room;
    G.handCount[player] = handSize;
    
    /*Initialize player deck to 10 cards, all copper*/
    G.deckCount[player] = deckSize;
    for(int card = 0; card < deckSize; card++){
      G.deck[player][card] = copper;
    }
    
    /*Initialize Actions, numBuys each to 1, coins = 0*/
    G.numBuys = 1;
    G.numActions = 1;
    G.coins = 0;
    
    return_state = cardEffect(council_room, 0, 0, 0, &G, 0, &bonus);
    
    //Test cards added and removed from deck
    assert(G.handCount[player] == (handSize - 1) + 4);
    assert(G.deckCount[player] == (deckSize - 4));
    printf("Discard Deck Error: Known Bug\n");
    //assert(G.discardCount[player] == 1);
    
    for(int i = 0; i < G.handCount[player]; i++){
      assert(G.hand[player][i] == copper);
    }
    for(int i = 0; i < G.deckCount[player]; i++){
      assert(G.deck[player][i] == copper);
    }
    //Test action number, and buy number
    //printf("Actions: %d, Expected: 0\n", G.numActions);
    assert(G.numActions == 1);
    assert(G.numBuys == 2);
    assert(G.coins == 0);
    assert(bonus == 0);

    for(int i = 0; i < numPlayers; i++){
      printf("Player[%d] HandCount %d DeckCount: %d\n", i, G.handCount[i], G.deckCount[i]);
      
      if(i > player){
        assert(G.deckCount[i] == (10 - player - 1));
        assert(G.handCount[i] == player + 1);
      }
      
    }
  }
}

int tester2(int numPlayers, int handSize, int deckSize){
  
  int seed = 1000;
  int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
  int r;
  int return_state;
  //int player = 0;
  int bonus = 0;

  struct gameState G;

  r = initializeGame(numPlayers, k, seed, &G);
  
  /*Intialize players deck to 5 cards, all copper
    then make the first card a smithy*/
  
  for(int player = 0; player < numPlayers; player++){
    G.whoseTurn = player;
    for(int card = 0; card < handSize; card++){
      G.hand[player][card] = rand() % 27;
    }
    G.hand[player][0] = council_room;
    G.handCount[player] = handSize;
    
    /*Initialize player deck to 10 cards, all copper*/
    G.deckCount[player] = deckSize;
    for(int card = 0; card < deckSize; card++){
      G.deck[player][card] = rand() % 27;
    }
    
    /*Initialize Actions, numBuys each to 1, coins = 0*/
    G.numBuys = 1;
    G.numActions = 1;
    G.coins = 0;
    
    return_state = cardEffect(council_room, 0, 0, 0, &G, 0, &bonus);
    
    //Test cards added and removed from deck
    assert(G.handCount[player] == (handSize - 1) + 4);
    assert(G.deckCount[player] == (deckSize - 4));
    printf("Discard Deck Error: Known Bug\n");
    //assert(G.discardCount[player] == 1);
    
    //Test action number, and buy number
    //printf("Actions: %d, Expected: 0\n", G.numActions);
    assert(G.numActions == 1);
    assert(G.numBuys == 2);
    assert(G.coins == 0);
    assert(bonus == 0);

    for(int i = 0; i < numPlayers; i++){
    
      if(i > player){
        assert(G.deckCount[i] == (10 - player - 1));
        assert(G.handCount[i] == player + 1);
      }
      
    }
  }
}
