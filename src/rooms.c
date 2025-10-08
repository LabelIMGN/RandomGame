#include "common.h"
#include "character.h"
#include "rooms.h"

#define POTION_SIZE_SMALL 0
#define POTION_SIZE_BIG 1
#define SMALL_POTION_RECOVERY 1
#define BIG_POTION_RECOVERY 3

character_t process_encounters(character_t player, int room_count){

  character_t enemy = {0};

  return player;
}

character_t process_event(character_t player, int room_count){

  int event_index;
  int min_value;
  int dice_roll; // Between difficulty level and 100 (arc4random(max - min + 1) + min)
  int outcome; // 0: Success | 1: Failure
  int damages;
  float damage_percentage;

  // Get the highest stat
  int player_stats[4] = {player.str, player.dex, player.mag, player.fth};
  int chosen_stat_index = choose_highest_stat(player_stats);

  // Get a random event
  event_index = arc4random_uniform(NUM_EVENTS);
  
  //Initialize min value for dice roll
  min_value = 100 - events[event_index].difficulty[chosen_stat_index] + 1;
  
  #ifdef DEBUG
  printf("DEBUG---\nChosen stat index: %d\nMin value = %d\nDifficulty: %d\n", 
        chosen_stat_index, min_value, events[event_index].difficulty[chosen_stat_index]);
  #endif

  dice_roll = arc4random_uniform(100) + 1;

  #ifdef DEBUG
  printf("Dice roll before calculations = %d\n", dice_roll);
  #endif

  dice_roll += 5*log(player_stats[chosen_stat_index]);
  dice_roll -= 10*log(room_count + 1);

  #ifdef DEBUG
  printf("Dice roll after calculations = %d\n", dice_roll);
  #endif

  // 4 - Compare this value to difficulty, if equal or greater success, else fail
  if(dice_roll >= events[event_index].difficulty[chosen_stat_index]){
    outcome = 0; // Success
  }else{
    outcome = 1; //Failure, compute damage_percentage
    damage_percentage = (arc4random_uniform(events[event_index].max_damage) + 1)/100.0;
    #ifdef DEBUG
    printf("Damage percentage: %f\n", damage_percentage);
    #endif
  }

  // Display the event description
  printf("%s\n", events[event_index].description);
  sleep(1);
  switch(outcome){
    case 0: // Success
      printf("%s\n", events[event_index].success[chosen_stat_index]);
      break;
    case 1: // Failure
      damages = player.max_hp * damage_percentage + room_count * damage_percentage;
      player.cur_hp -= damages;
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
  
  potion_type = arc4random_uniform(NUM_POTION_TYPE);
  potion_size = arc4random_uniform(NUM_POTION_SIZE);

  // Get the potion size
  switch(potion_size){
    case 0: strcpy(potion.size, "small");
    break;
    case 1: strcpy(potion.size,"large");
    break;
    default:
    printf("That was no potion...");
    break;
  }
  
  // Get the potion type and recover the amount
  switch(potion_type){ 
    case 0: 
      strcpy(potion.type, "health");
      if(potion_size == POTION_SIZE_SMALL){
        player.cur_hp += SMALL_POTION_RECOVERY;
        if(player.cur_hp > player.max_hp){
          player.cur_hp = player.max_hp;
        }
      }else if(potion_size == POTION_SIZE_BIG){
        player.cur_hp += BIG_POTION_RECOVERY;
        if(player.cur_hp > player.max_hp){
          player.cur_hp = player.max_hp;
        }
      }
    break;
    case 1: 
      strcpy(potion.type, "mana");
      if(potion_size == POTION_SIZE_SMALL){
        player.cur_mp += 1;
        if(player.cur_mp > player.max_mp){
          player.cur_mp = player.max_mp;
        }
      }else if(potion_size == POTION_SIZE_BIG){
        player.cur_mp += 3;
        if(player.cur_mp > player.max_mp){
          player.cur_mp = player.max_mp;
        }
      }
    break;
    default:
    printf("No potion was found...");
    break;
  }

  printf("You found a %s %s potion\n", potion.size, potion.type);

  return player;

}
