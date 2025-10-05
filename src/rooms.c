#include "common.h"
#include "types.h"
#include "character.h"

extern event_t events[];

character_t start_encounter(character_t player, int room_count){

  printf("A battle happened. You lost 5 HP\n"); 
  player.current_hp -= 5;

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
