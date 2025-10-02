#include "functions.h"
#include "encounters.c"
#include "events.c"

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
  int highest_stats[4];

  // Check highest stat number
  for(i = 0; i < 4; i++){
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
  int stat_table[6] = {1,1,1,1,1,1}; //Everything at 1 so no stats are at 0
  // 0: Health
  // 1: Mana
  // 2: Strength
  // 3: Dexterity
  // 4: Intelligence
  // 5: Faith

  int debug_loop_count;
  int current_stat_number;
  int table_index;

  // Randomly assign points to stat table for assigning later
    
  // Find a random number between 1 and stat_max and that won't exceed available_points
  // and assign it to the stat_table
  debug_loop_count = 1;
  do{
    current_stat_number = arc4random_uniform(character.stat_max + 1);
    if(character.available_points - current_stat_number >= 0){
      table_index = arc4random_uniform(6);
      if(current_stat_number > 0 && current_stat_number + stat_table[table_index] <= character.stat_max){
        stat_table[table_index] += current_stat_number;
        character.available_points -= current_stat_number;
      }
    }
    debug_loop_count ++;
  }while(character.available_points > 0);
  
  // Assign the values to the character's stats
  character.max_hp = stat_table[0] + 10;
  character.max_mp = stat_table[1] + 10;
  character.strength = stat_table[2];
  character.dexterity = stat_table[3];
  character.intelligence = stat_table[4];
  character.faith = stat_table[5];

  printf("----- Stat table values -----\n0: %d\n1: %d\n2: %d\n3: %d\n4: %d\n5: %d\n"
         "", stat_table[0], stat_table[1], stat_table[2], stat_table[3], stat_table[4], stat_table[5]);
  printf("Loop count: %d\n", debug_loop_count);

  return character;
}

void display_player(character_t player){
  // Character display
  system("clear");
  printf("                      ======= %s (Level %d) =======\nHP: %d/%d | MP: %d/%d | Strength: %d | Dexterity: %d |"
         " Intelligence: %d | Faith: %d\n"
         "------------------------------------------------------------------------------------------\n",
         player.name, player.level, player.current_hp, player.max_hp, player.current_mp,
         player.max_mp,player.strength, player.dexterity, player.intelligence
         ,player.faith);
}

character_t start_encounter(character_t player, int room_count){
  printf("A battle happened. You lost 1 HP\n"); 
  player.current_hp -= 1;

  //Make a enemy that is between player level and player level + room count. 
  //Distribute its points using the distribute_points function

  return player;
}

character_t start_event(character_t player, int room_count){

  int event_index;
  int min_value;
  int dice_roll; // Between difficulty level and 100 (arc4random(max - min + 1) + min)
  int outcome; // 0: Success | 1: Failure
  int damages;
  float damage_percentage;

  // Get the highest stat
  int player_stats[4] = {player.strength, player.dexterity, player.intelligence, player.faith};
  int chosen_stat_index = choose_highest_stat(player_stats);

  // Get a random event
  event_index = arc4random_uniform(4);
  
  //Initialize min value for dice roll
  min_value = 100 - events[event_index].difficulty[chosen_stat_index] + 1;
  // DEBUG
  printf("DEBUG---\nChosen stat index: %d\nMin value = %d\nDifficulty: %d\n", 
        chosen_stat_index, min_value, events[event_index].difficulty[chosen_stat_index]);

  dice_roll = arc4random_uniform(100) + 1;

  printf("DEBUG---\nDice roll before calculations = %d\n", dice_roll);
  //Augment this number by ln(chosen stat)
  dice_roll += 5*log(player_stats[chosen_stat_index]);
  //Reduce by ln(room_count)
  dice_roll -= 10*log(room_count + 1);
  // DEBUG
  printf("DEBUG---\nDice roll after calculations = %d\n", dice_roll);
  // 4 - Compare this value to difficulty, if equal or greater success, else fail
  if(dice_roll >= events[event_index].difficulty[chosen_stat_index]){
    outcome = 0; // Success
  }else{
    outcome = 1; //Failure, compute damage_percentage
    damage_percentage = (arc4random_uniform(events[event_index].max_damage) + 1)/100.0;
    printf("DEBUG---\nDamage percentage: %f\n", damage_percentage);
  }

  // Display the event description
  printf("%s\n", events[event_index].description);
  sleep(1);
  switch(outcome){
    case 0: // Success
      printf("%s\n", events[event_index].success[chosen_stat_index]);
      break;
    case 1: // Failure
      damages = player.max_hp * damage_percentage;
      player.current_hp -= damages;
      printf("%s. You lost %d HP.\n", events[event_index].failure[chosen_stat_index], damages);
      break;
    default:
      printf("Error occured. Skipping event");
  }

  return player;

}

character_t loot_room(character_t player){
  potion_t potion;
  int potion_type; //0: Health | 1: Mana
  int potion_size; //0: Small | 1: Large
  int amount_recovered = 0;
  
  potion_type = arc4random_uniform(2);
  potion_size = arc4random_uniform(2);

  switch(potion_size){ // Get the potion size
    case 0: strcpy(potion.size, "small");
    break;
    case 1: strcpy(potion.size,"large");
    break;
    default:
    printf("That was no potion...");
    break;
  }
  switch(potion_type){ // Get the potion type and recover the amount
    case 0: 
      strcpy(potion.type, "health");
      if(potion_size == 0){
        player.current_hp += 1;
        if(player.current_hp > player.max_hp){
          player.current_hp = player.max_hp;
        }
        amount_recovered = 1;
      }else if(potion_size == 1){
        player.current_hp += 3;
        amount_recovered = 3;
        if(player.current_hp > player.max_hp){
          player.current_hp = player.max_hp;
        }
      }
    break;
    case 1: 
      strcpy(potion.type, "mana");
      if(potion_size == 0){
        player.current_mp += 1;
        if(player.current_mp > player.max_mp){
          player.current_mp = player.max_mp;
        }
        amount_recovered = 1;
      }else if(potion_size == 1){
        player.current_mp += 3;
        player.current_mp += 1;
        if(player.current_mp > player.max_mp){
          player.current_mp = player.max_mp;
        }
        amount_recovered = 3;
      }
    break;
    default:
    printf("No potion was found...");
    break;
  }

  printf("You found a %s %s potion\n", potion.size, potion.type);

  return player;

}

