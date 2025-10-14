#include "common.h"
#include "character.h"
#include "rooms.h"

character_t process_encounters(character_t player, int player_battle_stats[], int room_count){
  // NOTE: player_battle_stats is passed as a copy to allow future modifications without
  // affecting the original player struct. 

  character_t enemy = {0};
  int enemy_score = 0;
  int enemy_index = arc4random_uniform(NUM_ENEMIES);

  strncpy(enemy.name, enemies[enemy_index].type, MAX_NAME_LENGTH - 1);
  enemy.name[MAX_NAME_LENGTH - 1] = '\0';
  // Enemy level is the room count until I find something cooler
  enemy.level = room_count;
  enemy.available_points = set_available_points(enemy.level);
  enemy.stat_max = set_max_stat(enemy.level, enemy.available_points);
  enemy = distribute_points(enemy);
  int enemy_battle_stats[NUM_BATTLE_STATS] = {enemy.str, enemy.dex, enemy.mag, enemy.fth};

  printf("You encounter a level %d %s\n", enemy.level, enemy.name);
  sleep(ROOM_TEXT_SPEED);

  #ifdef DEBUG
  printf("Enemy stats:\nLevel: %d\nstr: %d\ndex: %d\nmag: %d\nfth: %d\n", enemy.level, enemy.str, enemy.dex, enemy.mag, enemy.fth);
  getchar();
  #endif

  for(int i = 0; i < NUM_BATTLE_STATS; i++){
    if(player_battle_stats[i] - enemy_battle_stats[i] < 0){
      enemy_score += 1 + enemies[enemy_index].resistances[i];
    }
  }

  #ifdef DEBUG
  printf("Enemy score: %d\n", enemy_score);
  #endif

  if(enemy_score > 0){
    player.cur_hp -= enemy_score + enemy.level / 2;
    printf("You fight it and take %d damage\n", enemy_score + enemy.level / 2);
  }else{
    printf("You easily beat it\n");
  }

  return player;
}

character_t process_event(character_t player, int room_count){

  int event_index;
  int dice_roll; // Between difficulty level and 100 (arc4random(max - min + 1) + min)
  int outcome; // 0: Success | 1: Failure
  int damages;
  float damage_percentage;

  // Get the highest stat
  int player_stats[NUM_BATTLE_STATS] = {player.str, player.dex, player.mag, player.fth};
  int chosen_stat_index = choose_highest_stat(player_stats);

  // Get a random event
  event_index = arc4random_uniform(NUM_EVENTS);
  
  
  #ifdef DEBUG
  printf("DEBUG---\nChosen stat index: %d\nDifficulty: %d\n", 
        chosen_stat_index , events[event_index].difficulty[chosen_stat_index]);
  #endif

  dice_roll = arc4random_uniform(100) + 1;

  #ifdef DEBUG
  printf("Dice roll before calculations = %d\n", dice_roll);
  #endif

  // After the dice roll, give a bonus and a malus that scales with the chosen stat and 
  // difficulty but that accounts for extremely high values using logarithmic scaling.
  // Modifiers used here are from trial and error and can be adjusted freely
  dice_roll += DICE_ROLL_BONUS_MODIFIER*log(player_stats[chosen_stat_index]);
  dice_roll -= DICE_ROLL_MALUS_MODIFIER*log(room_count + 1);

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
  sleep(ROOM_TEXT_SPEED);
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
