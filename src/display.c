#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define NUM_DISPLAY_STATS (NUM_BATTLE_STATS + NUM_RESOURCE_STATS)
#define STAT_V_SPACING_MULT 3
#define BOX_EXT_PADDING 1
#define BOX_INT_H_PADDING 2
#define BOX_INT_V_PADDING 1

void update_player_display(character_t *player){
  // Ncurses starts here
  initscr();
  clear();
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

  
  // BOX SIZING
  int height, width, start_y, start_x, y_beg, y_max, x_beg, x_max; 
  getbegyx(stdscr, y_beg, x_beg);
  getmaxyx(stdscr, y_max, x_max);
  height = y_max/2 + NUM_DISPLAY_STATS - BOX_INT_V_PADDING*3;
  width = x_max/4 - BOX_EXT_PADDING;
  start_y = start_x = BOX_EXT_PADDING;
  #ifdef DEBUG
  printw("y_beg: %d\ny_max: %d\nx_beg: %d\nx_max: %d\n", y_beg, y_max, x_beg, x_max);
  printw("Display stat: %d\nStat spacing: %d\n", NUM_DISPLAY_STATS, STAT_V_SPACING_MULT);
  getch();
  #endif

  //  BOX LOOK
  char left, right, top, bottom, tlc, trc, blc, brc; 
  left = right = addch(ACS_VLINE);
  top = bottom = addch(ACS_HLINE);
  tlc = trc = blc = brc = addch(ACS_ULCORNER);
  
  // BOXES FRAME DISPLAY
  // Main box
  WINDOW *stat_box = newwin(height, width, start_y, start_x);
  wborder(stat_box, left, right, top, bottom, tlc, trc, blc, brc);
  wattron(stat_box, COLOR_PAIR(10));
  mvwprintw(stat_box, 0, (width - strlen(player->name)) / 2, " %s ", player->name);
  wattroff(stat_box, COLOR_PAIR(10));
  wattron(stat_box, COLOR_PAIR(11));
  mvwprintw(stat_box, BOX_INT_V_PADDING, (width - strlen(player->name)) / 2, "Level %d", player->level);
  wattroff(stat_box, COLOR_PAIR(11));
  // Hp and MP box
  WINDOW *resources_box = derwin(stat_box, 4, width - BOX_INT_H_PADDING*2, BOX_INT_V_PADDING, BOX_INT_H_PADDING);
  wborder(resources_box, left, right, top, bottom, tlc, trc, blc, brc);
  mvwprintw(resources_box, BOX_INT_V_PADDING, BOX_INT_H_PADDING, "Test stat");
  mvwprintw(resources_box, BOX_INT_V_PADDING + 1, BOX_INT_H_PADDING, "Test stat");

  // STATS
  int i;
  char *player_stat_names[NUM_BATTLE_STATS] = {"Strength:", "Dexterity:", "Magic:", "Faith:"};
  printw("%d", STAT_V_SPACING_MULT);

  // stat names display
  // for(i = 0; i < NUM_DISPLAY_STATS; i ++){
  // wattron(stat_box, COLOR_PAIR(i+1));
  // mvwprintw(stat_box, BOX_INT_H_PADDING + 1 + STAT_V_SPACING_MULT*i, BOX_INT_H_PADDING, "%s", player_stat_names[i]);
  // wattroff(stat_box, COLOR_PAIR(i+1));
  // }
  wrefresh(stat_box);
  wrefresh(resources_box);
  refresh();

  getch();
  endwin();
  // Ncurses ends here
}

