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

  // Character initial setup
  printf("What level are you?: ");
  scanf("%d", &player.level); // Temporary
  getchar();

  //Game loop
  while(1){
    room_count = 1;
    player.available_points = set_available_points(player.level);
    player.stat_max = set_max_stat(player.level, player.available_points);

    #ifdef DEBUG
    printf("Level: %d\nStat maximum: %d\nDistributable points: %d\n",
      player.level, player.stat_max, player.available_points);

    #endif
    player = distribute_points(player); 
    int player_battle_stats[NUM_BATTLE_STATS] = {player.str, player.dex, player.mag, player.fth};

    do{
      printf("Enter a name for your character: ");
      scanf("%s", player.name);
      if(strlen(player.name) > MAX_NAME_LENGTH){
        printf("Name is too long. Try again\n");
      }
    }while(strlen(player.name) > MAX_NAME_LENGTH);
    // system("clear");

    player.cur_hp = player.max_hp;
    player.cur_mp = player.max_mp;
    do{
      display_player(player, room_count);
      room_choice = arc4random_uniform(NUM_CHOICE);
      switch(room_choice){
        case 0: // Loot room
          player = loot_room(player);
          room_count ++;
          sleep(GAME_SPEED);
          break;
        case 1: // Event room
          player = process_event(player, room_count);
          room_count ++;
          sleep(GAME_SPEED + 1);
          break;  
        case 2: // Battle room
          player = process_encounters(player, player_battle_stats, room_count);
          room_count ++;
          sleep(GAME_SPEED);
          break;
        default:
          break;
      }
    }while(player.cur_hp > 0);

    //Player is dead. Ready for a new run
    printf("You died after visiting %d rooms\n", room_count);
    getchar();
  }

  return 0;
}
