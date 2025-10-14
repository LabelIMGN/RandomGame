#include "common.h"
#include "types.h"
#include "character.h"
#include "display.h"
#include "rooms.h"

#define NUM_CHOICE 3
#define GAME_SPEED 1 // Delay in seconds between loops

int main(void){

  character_t player = {0};
  int room_choice;//0: Loot room | 1: Event room | 2: Battle room
  int room_count;

  player.level = 1;
  #ifdef DEBUG
  printf("Enter starting level\n");
  scanf("%d", &player.level);
  getchar();
  #endif

  do{
    printf("Enter a name for your character: ");
    scanf("%31s", player.name); //CHANGE THIS IF YOU CHANGE MAX_NAME_LENGTH
    if(strlen(player.name) > MAX_NAME_LENGTH){
      printf("Name is too long. Try again\n");
    }
  }while(strlen(player.name) > MAX_NAME_LENGTH);
  getchar();
  
  //Game loop
  while(1){
    room_count = 0;
    player.available_points = set_available_points(player.level);
    player.stat_max = set_max_stat(player.level, player.available_points);

    #ifdef DEBUG
    printf("Level: %d\nStat maximum: %d\nDistributable points: %d\n",
      player.level, player.stat_max, player.available_points);
    #endif
    player = distribute_points(player); 
    int player_battle_stats[NUM_BATTLE_STATS] = {player.str, player.dex, player.mag, player.fth};

    player.cur_hp = player.max_hp;
    player.cur_mp = player.max_mp;

    do{
      room_count ++;
      display_player(player, room_count);
      room_choice = arc4random_uniform(NUM_CHOICE);
      switch(room_choice){
        case 0: // Loot room
          player = loot_room(player);
          sleep(GAME_SPEED);
          break;
        case 1: // Event room
          player = process_event(player, room_count);
          sleep(GAME_SPEED);
          break;  
        case 2: // Battle room
          player = process_encounters(player, player_battle_stats, room_count);
          sleep(GAME_SPEED);
          break;
        default:
          break;
      }
    }while(player.cur_hp > 0);

    //Player is dead. Ready for a new run
    printf("You died after visiting %d rooms\n", room_count);
    // Leveling part and waiting for the user to press Enter
    player = level_up(player, room_count);
    printf("You will get reborn at level %d\nPress Enter when ready...\n", player.level);
    getchar();
  }

  return 0;
}
