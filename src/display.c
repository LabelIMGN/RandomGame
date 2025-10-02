#include "common.h"
#include "types.h"

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
