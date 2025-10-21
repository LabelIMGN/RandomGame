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
  // BOX SIZING
  // stat box
  int stat_box_height, stat_box_width, start_y, start_x, y_beg, y_max, x_beg, x_max; 
  getbegyx(stdscr, y_beg, x_beg);
  getmaxyx(stdscr, y_max, x_max);
  stat_box_height = y_max/2 + NUM_BATTLE_STATS - BOX_INT_V_PADDING*3;
  stat_box_width = x_max/4 - BOX_EXT_PADDING;
  start_y = start_x = BOX_EXT_PADDING;
  // resource box
  int res_box_height, res_box_width;
  char *res_box_level_display = " Level %d ";
  res_box_height = 4;
  res_box_width = stat_box_width - BOX_INT_H_PADDING*2; // To account for both sides
  
  #ifdef DEBUG
  printw("y_beg: %d\ny_max: %d\nx_beg: %d\nx_max: %d\n", y_beg, y_max, x_beg, x_max);
  printw("Display stat: %d\nStat spacing: %d\n", NUM_DISPLAY_STATS, STAT_V_SPACING_MULT);
  getch();
  #endif

  //  BOX LOOK
  chtype left, right, top, bottom, tlc, trc, blc, brc; 
  left = right = ACS_VLINE;
  top = bottom = ACS_HLINE;
  tlc = ACS_ULCORNER;
  trc = ACS_URCORNER;
  blc = ACS_LLCORNER;
  brc = ACS_LRCORNER;
  
  // BOXES FRAME DISPLAY
  // Main box
  WINDOW *stat_box = newwin(stat_box_height, stat_box_width, start_y, start_x);
  wborder(stat_box, left, right, top, bottom, tlc, trc, blc, brc);
  wattron(stat_box, COLOR_PAIR(10));
  mvwprintw(stat_box, 0, (stat_box_width - strlen(player->name)) / 2, " %s ", player->name);
  wattroff(stat_box, COLOR_PAIR(10));
  // Hp and MP box
  WINDOW *resources_box = derwin(stat_box, res_box_height, res_box_width, BOX_INT_V_PADDING + 1, BOX_INT_H_PADDING);
  wborder(resources_box, left, right, top, bottom, tlc, trc, blc, brc);
  wattron(resources_box, COLOR_PAIR(11));
  mvwprintw(resources_box, 0, (res_box_width - strlen(res_box_level_display))/2, res_box_level_display, player->level);
  wattroff(resources_box, COLOR_PAIR(11));
  wattron(resources_box, COLOR_PAIR(12));
  mvwprintw(resources_box, BOX_INT_V_PADDING, BOX_INT_H_PADDING, "HP %d/%d", player->cur_hp, player->max_hp);
  wattroff(resources_box, COLOR_PAIR(12));
  wattron(resources_box, COLOR_PAIR(13));
  mvwprintw(resources_box, BOX_INT_V_PADDING + 1, BOX_INT_H_PADDING, "MP %d/%d", player->cur_mp, player->max_mp);
  wattroff(resources_box, COLOR_PAIR(13));

  // STATS
  int i;
  char *player_stat_names[NUM_BATTLE_STATS] = {"Strength:", "Dexterity:", "Magic:", "Faith:"};

  // stat names display
  for(i = 0; i < NUM_BATTLE_STATS; i++){
  wattron(stat_box, COLOR_PAIR(i+1));
  mvwprintw(stat_box, res_box_height + 3 + STAT_V_SPACING_MULT*i, BOX_INT_H_PADDING, "%s", player_stat_names[i]);
  wattroff(stat_box, COLOR_PAIR(i+1));
  }
  wrefresh(stat_box);
  wrefresh(resources_box);
  getch();

  delwin(resources_box);
  delwin(stat_box);
}
