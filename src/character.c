#include "common.h"
#include "types.h"

#define MAX_DISTRIBUTE_ITERATIONS 1000000

int set_available_points(int level){
  /*Sets the number of points that will be distributed to the character
   * The assigning function will substract from this pool until empty*/

  int available_points;

  if(level < 10){
  available_points = level;
  }else if(level < 60){
    available_points = level + 1;
  }else{
    available_points = level + 2;
  }

  return available_points;
}

int set_max_stat(int level, int available_points){
  if(level <= 10){
    return 15;
  }else if(level <= 60){
    return (available_points / 3);
  }else{
    return available_points / 2;
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
  for(i = 0; i < 4; i++){
    if(player_stats[i] == highest_stat_number){
      highest_stats[stat_added_count] = i;
      stat_added_count ++;
    }
  }
  
  // Randomly choose a highest stat from the array
  random_pick = arc4random_uniform(stat_added_count);
  return highest_stats[random_pick];

}

character_t distribute_points(character_t character){
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
    current_stat_number = arc4random_uniform(character.stat_max + 1);
    if(character.available_points - current_stat_number >= 0){
      table_index = arc4random_uniform(NUM_BATTLE_STATS);
      if(current_stat_number > 0 && current_stat_number + stat_table[table_index] <= character.stat_max){
        stat_table[table_index] += current_stat_number;
        character.available_points -= current_stat_number;
      }
    }
  }while(character.available_points > 0 && loop_count < MAX_DISTRIBUTE_ITERATIONS);
  
  // Assign the values to the character's stats
  character.max_hp = 10 + character.level;
  character.max_mp = 1 + (character.level % 2);
  character.str = stat_table[0];
  character.dex = stat_table[1];
  character.mag = stat_table[2];
  character.fth = stat_table[3];

  #ifdef DEBUG
  printf("----- Stat table values -----\n0: %d\n1: %d\n2: %d\n3: %d\n4:"
         "", stat_table[0], stat_table[1], stat_table[2], stat_table[3]);
  printf("Loop count: %d\n", loop_count);
  #endif

  return character;
}
