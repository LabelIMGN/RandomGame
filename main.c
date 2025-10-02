#include "functions.c"

int main(void){

  // Declarations
  character_t player;
  int room_choice;//0: Loot room | 1: Event room | 2: Battle room
  int delay = 2;
  int room_count = 0;

  // Character initial setup
  printf("What level are you?: ");
  scanf("%d", &player.level); // Temporary
  getchar();
  player.available_points = set_available_points(player.level);
  player.stat_max = set_max_stat(player.level, player.available_points);
  printf("Level: %d\nStat maximum: %d\nDistributable points: %d\n",
    player.level, player.stat_max, player.available_points);
  player = distribute_points(player);
  printf("Enter a name for your character: ");
  scanf("%s", player.name);
  getchar();

  //Game loop
  while(1){
    player.current_hp = player.max_hp;
    player.current_mp = player.max_mp;
    do{
      display_player(player);
      room_choice = arc4random_uniform(3);
      switch(room_choice){
        case 0: // Loot room
          player = loot_room(player);
          room_count ++;
          sleep(delay);
          break;
        case 1: // Event room
          player = start_event(player, room_count);
          room_count ++;
          sleep(delay + 2);
          // getchar();
          break;  
        case 2: // Battle room
          player = start_encounter(player, room_count);
          room_count ++;
          sleep(delay);
          break;
        default:
          break;
      }
    }while(player.current_hp > 0);
    //Player is dead. Ready for a new run
    printf("You died after visiting %d rooms\n", room_count);
    break;
  }

  return 0;
}
