#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

/*Test Adventurer Random
  Cost: 6
  Action: Reveal 2 cards from your deck until 2 Treasure Cards.
   Put those cards into your hand, discard other cards.
*/

/*Test plan: 

randomize cards that are put into the desk.
randomize size of deck.

27 different cards in deck. 
4-6 are treasure cards. 7-26 are action

To Do: 
Then add testers with really low treasure, and another with really high action.

*/

#define NUM_TESTS 2000

int tester(int numPlayers, int vic_max, int treas_max, int act_max, int hand_max);

int main(){
  
  srand(time(NULL));
  for(int i = 0; i < NUM_TESTS; i++){
    tester(2, 10, 10, 10, 25);
    tester(2, 25, 25, 25, 10);
    tester(3, 100, 100, 100, 50);
    tester(4, 100, 100, 100, 50);
  }
}
  


int tester(int numPlayers, int vic_max, int treas_max, int act_max, int hand_max){

  int seed = 1000;
  int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
  int r;
  int return_state;
  int bonus;

  int handSize = rand() % hand_max;
  int deckSize;

  int treasureCards = rand() % treas_max;
  int actionCards = rand() % act_max;
  int victoryCards = rand() % vic_max;

  int tempTreasure;
  int tempAction;
  int tempVictory;
  int cardType;
  int cardAdded;
  int tempCard;
  int expectedPostDeck;
  int expectedPostHand;
  int twoCards;
  int foundTreasures[2]; //Keep track of treasure cards we find.
  int failure = 0;
  

  deckSize = treasureCards + actionCards + victoryCards;
  
  struct gameState G;

  r = initializeGame(numPlayers, k, seed, &G);
  
 
  for(int player = 0; player < numPlayers; player++){
    foundTreasures[0] = 0;
    foundTreasures[1] = 0;
    tempTreasure = treasureCards;
    tempAction = actionCards;
    tempVictory = victoryCards;

    G.whoseTurn = player;    

    G.deckCount[player] = deckSize;

    for(int i = 0; i < handSize; i++){
      G.deck[player][i] = (rand() % 26+1);
    }
    G.handCount[player] = handSize;
    
    for(int i = 0; i < deckSize; i++){
      cardAdded = 0;
      while(cardAdded == 0){
	cardType = rand() % 3;
	if(cardType == 0 && tempTreasure > 0){
	  cardAdded = 1;
	  tempTreasure--;
	  G.deck[player][i] = (rand() % 3) + 4;
	  
	}
	else if(cardType == 1 && tempAction > 0){
	  cardAdded = 1;
	  tempAction--;
	  G.deck[player][i] = (rand() % 19) + 8;
	}
	else if(cardType == 2 && tempVictory > 0){
	  cardAdded = 1;
	  tempVictory--;
	  G.deck[player][i] = (rand() % 4);
	}
      }
    }
    
    //Control Actions, numBuys and coins avaliable
    G.numBuys = 1;
    G.numActions = 1;
    G.coins = 0;

 
    expectedPostHand = handSize;
    expectedPostDeck = deckSize;
    twoCards = 0;
    for(int i = (deckSize-1); i >= 0; i--){
      tempCard = G.deck[player][i];
      if(tempCard >= 4 && tempCard <= 6){
	foundTreasures[twoCards] = tempCard;
	twoCards++;
	expectedPostDeck--;
	expectedPostHand++;
      }
      if(twoCards == 2){
	break;
      }
    }

    //Control for segfault imminent
    if(handSize > 0 && failure != 1){
      return_state = cardEffect(adventurer, 0, 0, 0, &G, 0, &bonus);
    }
    else{
      printf("skipped a test that would have failed\n");
    }

    //Predict segfault imminent
    if(G.handCount[player] < 0){
      failure = 1;
    }
    else{
      failure = 0;
    }
    printf("handSize: %d Expected: %d\n", G.handCount[player], expectedPostHand);
    printf("Actions: %d, Expected: 0\n", G.numActions);
    
    assert(G.numBuys == 1);
    assert(G.coins == 0);
    assert(bonus == 0);
    printf("DeckSize: %d, Expected: %d\n", G.deckCount[player], expectedPostDeck);
    if(foundTreasures[0] != 0)
      printf("Expected first Treasure: %d, Actual %d\n", foundTreasures[0], G.hand[player][G.handCount[player] - 1]);
      continue;
    if(G.handCount[player] > 1 && foundTreasures[1] != 0)
      printf("Expected second Treasure: %d, Actual %d\n", foundTreasures[1], G.hand[player][G.handCount[player] - 2]);
      continue;
  }
}
    
    
    


    
  
  
