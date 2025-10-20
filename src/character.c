#include "types.h"

#define MAX_DISTRIBUTE_ITERATIONS 10000000

void set_available_points(character_t *character){
  /*Sets the number of points that will be distributed to the character
   * The assigning function will substract from this pool until empty*/

  if(character->level < 10){
  character->available_points = character->level;
  }else if(character->level < 60){
    character->available_points = character->level + 1;
  }else{
    character->available_points = character->level + 2;
  }

}

void set_max_stat(character_t *player){
  // All values here are from trial and error
  if(player->level <= 10){
    player->stat_max = 15;
  }else if(player->level <= 60){
    player->stat_max = player->available_points / 3;
  }else{
    player->stat_max = player->available_points / 2;
  }
}

int choose_highest_stat(int player_stats[]){

  int i;
  int random_pick;
  int highest_stat_number = 1; // Baseline to go through all stats
  int stat_added_count = 0; // Used for the random highest stat choice if more than one exists  
  int highest_stats[NUM_BATTLE_STATS];

  // Check highest stat number
  for(i = 0; i < NUM_BATTLE_STATS; i++){
    if(player_stats[i] > highest_stat_number){
      highest_stat_number = player_stats[i];
    }
  }

  // Make a table of the stats matching the highest stat number
  for(i = 0; i < NUM_BATTLE_STATS; i++){
    if(player_stats[i] == highest_stat_number){
      highest_stats[stat_added_count] = i;
      stat_added_count ++;
    }
  }
  
  // Randomly choose a highest stat from the array
  random_pick = arc4random_uniform(stat_added_count);
  return highest_stats[random_pick];

}

void distribute_points(character_t *character){
  int stat_table[NUM_BATTLE_STATS] = {1,1,1,1}; //Everything at 1 so no stats are at 0
  // 0: Strength
  // 1: Dexterity
  // 2: Magic
  // 3: Faith

  int loop_count = 0;
  int current_stat_number;
  int table_index;

  // Randomly assign points to stat table for assigning later
    
  // Find a random number between 1 and stat_max that won't exceed available_points
  // and assign it to the stat_table
  do{
    loop_count ++;
    current_stat_number = arc4random_uniform(character->stat_max + 1);
    if(character->available_points - current_stat_number >= 0){
      table_index = arc4random_uniform(NUM_BATTLE_STATS);
      if(current_stat_number > 0 && current_stat_number + stat_table[table_index] <= character->stat_max){
        stat_table[table_index] += current_stat_number;
        character->available_points -= current_stat_number;
      }
    }
  }while(character->available_points > 0 && loop_count < MAX_DISTRIBUTE_ITERATIONS);
  
  // Assign the values to the character's stats
  character->max_hp = 10 + character->level;
  character->max_mp = 1 + character->level / 2;
  character->str = stat_table[0];
  character->dex = stat_table[1];
  character->mag = stat_table[2];
  character->fth = stat_table[3];

  #ifdef DEBUG
  printf("----- Stat table values -----\n0: %d\n1: %d\n2: %d\n3: %d\n4:"
         "", stat_table[0], stat_table[1], stat_table[2], stat_table[3]);
  printf("Loop count: %d\n", loop_count);
  #endif

}

void level_up(character_t *player, int room_count){

  player->next_level_threshold = player->level * LEVEL_REQ_MODIFIER;
  player->cur_xp_pool += room_count * XP_GAIN_MODIFIER;

  while(player->cur_xp_pool >= player->next_level_threshold){

    #ifdef DEBUG
    printf("Level up threshold: %d\nCurrent xp: %d\n", player->next_level_threshold, player->cur_xp_pool);
    #endif

    player->level ++;
    player->cur_xp_pool -= player->next_level_threshold;
    player->next_level_threshold = player->level * LEVEL_REQ_MODIFIER;

  }

  #ifdef DEBUG
  printf("Current xp after level up process: %d\n", player->cur_xp_pool);
  #endif

}
