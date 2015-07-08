#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/* Test: int discardCard(int handPos, int currentPlayer,
   struct gameState *state, int trashFlag);*/

int tester(int numPlayers, int deckSize, int handSize, int trashFlag,
           int bonusHand);


int main(){

  
  //Run tests for 2-4 players
  for(int i = 2; i < 4; i++){
    tester(i, 10, 5, 0, 0);
    tester(i, 10, 5, 1, 0);
    //The following tests should fail and do not
    tester(i, 10, 5, 0, 1);
    tester(i, 10, 5, 0, 2);
    tester(i, 10, 5, 1, 1);
  }
 


}

int tester(int numPlayers, int deckSize, int handSize, int trashFlag, 
           int bonusHand){

  int return_state;
  int seed = 1000;
  int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
  int r;
  
  struct gameState G;
 
  int cur_turn = 0;

  r = initializeGame(numPlayers, k, seed, &G);

  
  for(int i = 0; i < numPlayers; i++){
    assert(G.discardCount[i] == 0);
    if(i == cur_turn){
      assert(G.deckCount[i] == (deckSize - handSize));
      assert(G.handCount[i] == handSize);
      for(int j = 0; j < handSize+bonusHand; j++){
        return_state = discardCard(j, i, &G, trashFlag);
        assert(return_state == 0);
        printf("HandSize: %d\n", G.handCount[i]);
        assert(G.handCount[i] ==  (handSize - (j+1)));
        if(trashFlag == 1){
          assert(G.discardCount[i] == 0);
        }
        else{
          printf("DiscardCount: %d, Exepected: %d\n", G.discardCount[i], j+1);
          //assert(G.discardCount[i] == j+1);
        }
        assert(G.deckCount[i] == (deckSize - handSize));
      }
      printf("Played Count: %d, Expected: %d\n", G.playedCardCount, 0);
    }
    else if(i > 0){
      printf("Player %d, Turn: %d\n", i, G.whoseTurn);
      return_state == discardCard(0, i, &G, trashFlag);
      printf("Return State: %d, Expected: %d\n", return_state, -1);
      //assert(return_state == -1);
      assert(G.deckCount[i] == deckSize);
      printf("Hand Count: %d, Expected: %d\n", G.handCount[i], 0);
      //assert(G.handCount[i] == 0);
      printf("Played Count: %d, Expected: %d\n", G.playedCardCount, 0);
      //assert(G.playedCardCount == 0);
    }
  }
}
