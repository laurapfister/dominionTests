#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

/* Test: drawCard(int player, struct gameState *state); 

   Test: drawcard when empty deck, drawcard when empty hand
   Test: card drawn is in k
   Test: deck decremented
   

*/

int tester(int cards[10], int drawCount, int deckSize, int numPlayers);

int main(){
  
  srand(time(NULL));

  int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};


  for(int b = 0; b < 50; b++){
    for(int a = 2; a < 5; a++){ 
      tester(k, 5, 10, a);
      tester(k, 10, 10, a);
      tester(k, 15, 10, a);
      tester(k, 5, 0, a);
      tester(k, 0, 5, a);
    }
  }
}

int tester(int cards[10], int drawCount, int deckStartSize, int numPlayers){
   int return_state;
   int seed = 1000;
   int r;
   int top_card;
   

   struct gameState G;
   
   r = initializeGame(numPlayers, cards, seed, &G);

   for(int player = 0; player < numPlayers; player++){
     int deckSize = deckStartSize;
     if(G.handCount[player] > 0){
       int count = G.handCount[player];
       for(int card = 0; card < count; card++){
         //printf("discarding");
         G.hand[player][card] = -1;
       }
     }
     G.deckCount[player] = deckSize;
     G.handCount[player] = 0;
     //Fill Deck with Random Cards to Make sure they all work
     for(int card = 0; card < deckSize; card++){
       G.deck[player][card] = rand() % 50;
     }
     
     for(int i = 0; i < drawCount; i++){ 
       top_card = G.deck[player][deckSize - 1];
       return_state = drawCard(player, &G);
       if(deckSize > 0){
         deckSize--; 
         assert(return_state == 0);
         printf("Player: %d, HandCount %d, Expected: %d\n", player, G.handCount[player], i+1);
         assert(G.handCount[player] == (i + 1));
         //printf("DeckCount: %d", G.deckCount[player]);
         assert(G.deckCount[player] == deckSize);
         //printf("DeckSize: %d", deckSize);
         printf("Top: %d, Hand: %d\n", top_card, G.hand[player][i]);
         assert(top_card ==  G.hand[player][i]);
       }
       else{
         assert(return_state == -1);
         assert(G.handCount[player] == deckStartSize);
         assert(G.deckCount[player] == 0);
       }
     }
   }
   printf("Passed\n");
   memset(&G, 23, sizeof(struct gameState));
}

