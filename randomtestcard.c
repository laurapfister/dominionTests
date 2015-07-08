#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>


/*Random test plan
  
  randomize handsize of all players, randomize deck size of all players
  tester will take max handsize and max decksize.
  we will manually create both hand size and deck size randomly.
  we can use parts of adventurer code to fill deck and hand.

  randomly pick player.
  keep track of how many cards each player should have. 
  randomly pick how many times test should run?
*/

int tester(int numPlayers, int maxHand, int maxDeck, int turns);
int fillHand(int handSize, struct gameState *G, int handTracker[][MAX_HAND],
	     int player);
int fillDeck(int deckSize, struct gameState *G, int deckTracker[][MAX_HAND],
	     int player);
int updateTrackers(int drawnCards, int player, int handCountTrack[MAX_PLAYERS],
		   int deckCountTrack[MAX_PLAYERS], int handTracker[][MAX_HAND],
		   int deckTracker[][MAX_DECK]);
 

int main(){

  srand(time(NULL));
  tester(2, 10, 10, 2);
  tester(2, 100, 100, 25);
  tester(3, 100, 100, 25);
  tester(4, 100, 100, 25);
  tester(2, 50, 50, 25);
  tester(2, 25, 25, 10);

}


int tester(int numPlayers, int maxHand, int maxDeck, int turns){

  int seed = 1000;

  int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
  int r;
  int return_state;
  //int player = 0;
  int bonus = 0;
  int player;
  int handTracker[MAX_PLAYERS][MAX_HAND];
  int deckTracker[MAX_PLAYERS][MAX_DECK];
  int handCountTrack[MAX_PLAYERS];
  int deckCountTrack[MAX_PLAYERS];
  int tempCard;

  struct gameState G;

  r = initializeGame(numPlayers, k, seed, &G);
  
  for(int p = 0; p < numPlayers; p++){
    handCountTrack[p] = rand() % maxHand;
    deckCountTrack[p] = rand() % maxDeck;
    fillHand(handCountTrack[p], &G, handTracker, p);
    fillDeck(deckCountTrack[p], &G, deckTracker, p);
    G.handCount[p] = handCountTrack[p];
    G.deckCount[p] = deckCountTrack[p];

  }
  for(int p = 0; p < numPlayers; p++){
    printf("Player %d, starting hand %d, starting deck %d\n", p, handCountTrack[p], deckCountTrack[p]);
   }
  
  for(int i = 0; i < turns; i++){
    player = rand()%2;
    G.whoseTurn = player;
    printf("Turn %d, player %d\n", i, player);
    return_state = cardEffect(council_room, 0, 0, 0, &G, 0, &bonus);
    //Change our trackers to match what cards should be
    for(int p = 0; p < numPlayers; p++){
      if(p == player){
	updateTrackers(4, p, handCountTrack, deckCountTrack,
		       handTracker, deckTracker);
      }
      else{
	updateTrackers(1, p, handCountTrack, deckCountTrack,
		       handTracker, deckTracker);
      }
    }
    for(int p = 0; p < numPlayers; p++){
      printf("player %d, handCount: %d, Expected %d\n", p, G.handCount[p], handCountTrack[p]);
      assert(handCountTrack[p] == G.handCount[p]);
      assert(deckCountTrack[p] == G.deckCount[p]);
      for(int d = 0; d < G.deckCount[p]; d++){
	assert(deckTracker[p][d] == G.deck[p][d]);
      }
    }
    
  }
}
    

int fillHand(int handSize, struct gameState *G, int handTracker[][MAX_HAND],
	     int player){

  int cardType;
  for(int i = 0; i < handSize; i++){
    cardType = rand() % 3;
    if(cardType == 0){
      G->hand[player][i] = (rand() % 3) + 4;
    }
    else if(cardType == 1){
      G->hand[player][i] = (rand() % 19) + 8;
    }
    else if(cardType == 2){
      G->hand[player][i] = (rand() % 4);
    }
    handTracker[player][i] = G->hand[player][i];
  }
  return 0;
}

int fillDeck(int deckSize, struct gameState *G, int deckTracker[][MAX_HAND],
	     int player){

  int cardType;
  for(int i = 0; i < deckSize; i++){
    cardType = rand() % 3;
    if(cardType == 0){
      G->deck[player][i] = (rand() % 3) + 4;
    }
    else if(cardType == 1){
      G->deck[player][i] = (rand() % 19) + 8;
    }
    else if(cardType == 2){
      G->deck[player][i] = (rand() % 4);
    }
    deckTracker[player][i] = G->deck[player][i];
  }
  return 0;
}
	       
int updateTrackers(int drawnCards, int player, int handCountTrack[MAX_PLAYERS],
		   int deckCountTrack[MAX_PLAYERS], int handTracker[][MAX_HAND],
		   int deckTracker[][MAX_DECK])
{
  
  int tempCard;
  int tempCard2;
  if(drawnCards == 4 && handCountTrack[player] > 0){
    for(int j = 0; j < handCountTrack[player]-1; j++){
      handTracker[player][j] = handTracker[player][j+1];
    }
    handCountTrack[player]--;
  }
  for(int j = 0; j < drawnCards; j++){
    if(deckCountTrack[player] <= 0){
      break;
    }
    tempCard = deckTracker[player][deckCountTrack[player]-1];
    deckCountTrack[player]--;
    handCountTrack[player]++;
    handTracker[player][handCountTrack[player]-1] = tempCard;
  }
}
