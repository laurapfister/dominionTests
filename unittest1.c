#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"


/*Test int buyCard(int supplyPos, struct gameState *state);*/

#define CARD_TOTAL 27

int tester(int numPlayer, int start_hand, int deck_total,int supply);

int main() {
  
  
  for(int i = 2; i < 5; i++){
    tester(i, 5, 10, 500);
    tester(i, 5, 10, 0);
    tester(i, 5, 10, 1);
    tester(i, 500, 10, 10);
  }


}

int tester(int numPlayer, int start_hand, int deck_total, int supply){
  
  int cards[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
  int seed = 1000;
  int r;
  int return_state;

  int turns = 50;
  int low_turns = 5;
  int cur_coins;
  int cur_turns;
  int cur_count;
  int successful_turns;
  
  struct gameState G;
  
  r = initializeGame(numPlayer, cards, seed, &G);
  for(int i = 0; i < CARD_TOTAL; i++){
    G.supplyCount[i] = supply;
  }
  
  for(int player = 0; player < numPlayer; player ++){
    G.coins = 100;
    G.numBuys = turns;
    cur_coins = 100;
    G.whoseTurn = player;
    successful_turns = 0;
    for(int i = 0; i < turns; i++){
      cur_count = G.supplyCount[i];
      return_state = buyCard(i, &G);
      if((cur_coins >  getCost(i)) && cur_count > 0){
        assert(return_state == 0);   
        assert(G.numBuys == (turns - successful_turns - 1));
        cur_coins -= getCost(i);
        assert(G.coins == cur_coins);
        successful_turns++;
      }
      else{
        assert(return_state == -1);
        assert(G.numBuys == turns - successful_turns);
        assert(G.coins == cur_coins);
      }
    }
    
    G.coins = 100;
    G.numBuys = low_turns;
    cur_coins = 100;
    successful_turns = 0;
    
    /*Test you can't buy more than turns*/
    for(int i = 0; i < 25; i++){
      cur_count = G.supplyCount[i];
      return_state = buyCard(i, &G);
      if(successful_turns  < low_turns && cur_count > 0){
        assert(return_state == 0);
        successful_turns++;
        assert(G.numBuys == (low_turns - successful_turns));
        cur_coins -= getCost(i);
        assert(G.coins == cur_coins);
        assert(G.supplyCount[i] == (cur_count -1));
      }
      else{
        printf("%d", G.numBuys);
        assert(return_state == -1);
        assert(G.numBuys == low_turns - successful_turns);
        assert(G.coins == cur_coins);
        assert(G.supplyCount[i] == cur_count);
      }
    }
    
    G.coins = 10;
    G.numBuys = turns;
    cur_coins = 10;
    cur_turns = turns;
    successful_turns = 0;
    
    for(int i = 0; i < 25; i++){
      cur_count = G.supplyCount[i];
      return_state = buyCard(i, &G);
      if((cur_coins - getCost(i)) >= 0 && cur_count > 0){
        printf("Card %d, Return State: %d, Buys: %d, Coins: %d\n", 
               i, return_state, G.numBuys, G.coins);
        assert(return_state == 0);
        successful_turns++;
        cur_turns--;
        assert(G.numBuys == (turns - successful_turns));
        cur_coins -= getCost(i);
        assert(G.coins == cur_coins);
        printf("Card Supply: %d, Exepected %d\n", G.supplyCount[i], cur_count);
        assert(G.supplyCount[i] == (cur_count - 1));
        
      }
      else{
        assert(return_state == -1);
        assert(G.numBuys == turns - successful_turns);
        assert(G.coins == cur_coins);
        assert(G.supplyCount[i] == cur_count);
      }
    }
  }
}

/*Add support for supply count check?*/
/*Add support for multiple card types*/
  
