#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>


int tester(int numPlayers, int handCount, int DeckCount);

/*Test int endTurn(struct gameState *state);*/

int main(){
  
  /*test handcount of previous player
    test player number
    test coins = 0
    test buy number = 1
    test played card count
    
    test current player handcount = 5
  */  
  srand(time(NULL));
  
  for(int a = 2; a < 5; a++){
    tester(a, 5, 10);
    tester(a, 10, 10);
    tester(a, 10, 0);
    tester(a, 10, 500);
  }
  //tester(6, 5, 10);

}

int tester(int numPlayers, int handSize, int deckSize){
  
  int cur_turn = 0;
  int prev_player;
  int return_state;
  int seed = 1000;
  int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
  int r;
  
  
  struct gameState G;

  r = initializeGame(numPlayers, k, seed, &G);

  for(int i = 0; i < 50; i++){
    

    //printf("Players: %d, Turn: %d\n", numPlayers, cur_turn);
    
    G.handCount[cur_turn] = handSize;
    G.deckCount[cur_turn] = deckSize;
    for(int card = 0; card < handSize; card++){
      G.hand[cur_turn][card] = rand() % 27;
    }
    for(int card = 0; card < deckSize; card++){
      G.deck[cur_turn][card] = rand() % 27;
    }
    
    prev_player = cur_turn;
    return_state = endTurn(&G);
    
    if(cur_turn < numPlayers-1){
      cur_turn++;
    }
    else{
      cur_turn = 0;
    }

    assert(return_state == 0);
    //printf("%d, Turn: %d, %d\n",i, G.whoseTurn, cur_turn); 
    assert(G.whoseTurn == cur_turn);
    assert(G.handCount[cur_turn] == 5);
    assert(G.handCount[prev_player] == 0);
    assert(G.playedCardCount == 0);
    assert(G.numBuys == 1);
    assert(G.numActions == 1);
    printf("Starting Size: %d, Current Size: %d\n", deckSize, G.deckCount[prev_player]);
    printf("Starting Size: %d, Current Size: %d\n", deckSize, G.deckCount[cur_turn]);
    //assert(G.deckCount[prev_player] == (deckSize - 5));
    printf("Deck Count of Previous: %d\n", G.deckCount[prev_player]);
    //assert(G.deckCount[prev_player] == ((deckSize - 5) || deckSize));

  }
  
  return 0;
}
