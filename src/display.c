#include "common.h"
#include "types.h"

void display_player(character_t player, int room_count){
  
  // Character display
  system("clear");
  printf("                      ======= %s (Level %d) room %d =======\nHP: %d/%d | MP: %d/%d | Strength: %d | Dexterity: %d |"
         " Magic: %d | Faith: %d\n"
         "------------------------------------------------------------------------------------------\n",
         player.name, player.level, room_count, player.cur_hp, player.max_hp, player.cur_mp,
         player.max_mp,player.str, player.dex, player.mag
         ,player.fth);
}
