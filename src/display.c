#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define NUM_DISPLAY_STAT NUM_BATTLE_STATS + 2
#define BOX_EXT_PADDING 1
#define BOX_INT_PADDING 1

void update_player_display(character_t *player){
  // Ncurses starts here
  initscr();
  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);

  // Box size
  int height, width, start_y, start_x, y_beg, y_max, x_beg, x_max;
  getbegyx(stdscr, y_beg, x_beg);
  getmaxyx(stdscr, y_max, x_max);
  height = y_max/2;
  width = x_max/4 - BOX_EXT_PADDING;
  start_y = start_x = BOX_EXT_PADDING;
  #ifdef DEBUG
  printw("y_beg: %d\ny_max: %d\nx_beg: %d\nx_max: %d\n", y_beg, y_max, x_beg, x_max);
  refresh();
  getch();
  #endif

  //  Box look
  char left, right, top, bottom, tlc, trc, blc, brc; 
  left = right = addch(ACS_VLINE);
  top = bottom = addch(ACS_HLINE);
  tlc = trc = blc = brc = addch(ACS_ULCORNER);
  
  // Box display
  clear();
  WINDOW *stat_box = newwin(height, width, start_y, start_x);
  refresh();

  wborder(stat_box, left, right, top, bottom, tlc, trc, blc, brc);
  wattron(stat_box, COLOR_PAIR(1));
  mvwprintw(stat_box, 0, (width - strlen(player->name)) / 2, " %s ", player->name);
  wattroff(stat_box, COLOR_PAIR(1));
  mvwprintw(stat_box, BOX_INT_PADDING, (width - strlen(player->name)) / 2, "Level %d", player->level);
  wrefresh(stat_box);

  // STATS
  for(int i = 0; i < NUM_DISPLAY_STAT; i++){
    mvwprintw(stat_box, BOX_INT_PADDING + 1 + 2*i, BOX_INT_PADDING, "Test stat");
  }
  wrefresh(stat_box);

  getch();
  endwin();
  // Ncurses ends here
}

void update_stat_display(character_t *player, int room_count){
  // Ncurses starts here
  initscr();
  start_color();

}
