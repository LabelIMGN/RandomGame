#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#include "types.h"
#include "character.h"
#include "display.h"
#include "rooms.h"

#define NUM_CHOICE 3

int main(void){

  initscr(); // ncurses starts
  echo();
  cbreak();
  curs_set(0);
  start_color();

  // COLORS
  init_pair(10, COLOR_BLACK, COLOR_WHITE); // Name of the player
  init_pair(11, COLOR_WHITE, COLOR_BLACK); // Player level
  init_pair(12, COLOR_RED, COLOR_BLACK); // HP
  init_pair(13, COLOR_CYAN, COLOR_BLACK); // MP
  init_pair(1, COLOR_MAGENTA, COLOR_BLACK); // str
  init_pair(2, COLOR_GREEN, COLOR_BLACK); // dex
  init_pair(3, COLOR_BLUE, COLOR_BLACK); // mag
  init_pair(4, COLOR_YELLOW, COLOR_BLACK); // fth
  
  character_t player = {0};
  int room_choice;//0: Loot room | 1: Event room | 2: Battle room
  int room_count;

  player.level = 1;
  #ifdef DEBUG
  printw("DEBUG Enter starting level: ");
  // scanf("%d", &player.level);
  char input[10];
  getnstr(input, sizeof(input) - 1);
  player.level = atoi(input);
  #endif

  printw("Enter a name for your character: ");
  getnstr(player.name, MAX_NAME_LENGTH);
  refresh();
  
  //Game loop
  while(1){

    room_count = 0;
    set_available_points(&player);
    set_max_stat(&player);

    #ifdef DEBUG
    printw("Level: %d\nStat maximum: %d\nDistributable points: %d\n",
      player.level, player.stat_max, player.available_points);
    getch();
    #endif
    distribute_points(&player); 

    player.cur_hp = player.max_hp;
    player.cur_mp = player.max_mp;
    player.battle_stat[0] = player.str;
    player.battle_stat[1] = player.dex;
    player.battle_stat[2] = player.mag;
    player.battle_stat[3] = player.fth;

    do{
      room_count ++;
      update_player_display(&player);
      update_text_box(0, "You enter the next room", NULL, 0);
      room_choice = arc4random_uniform(NUM_CHOICE);
      switch(room_choice){
        case 0: // Loot room
          loot_room(&player);
          break;
        case 1: // Event room
          process_event(&player, room_count);
          break;
        case 2: // Battle room
          process_encounters(&player, room_count);
          break;
        default:
          break;
      }
    }while(player.cur_hp > 0);

    //Player is dead. Ready for a new run
    printf("You died after visiting %d rooms\n", room_count);
    // Leveling part and waiting for the user to press Enter
    level_up(&player, room_count);
    printf("You will get reborn at level %d\nPress Enter when ready...\n", player.level);
    getchar();
  }

  endwin(); // ncurses ends
  return 0;
}
