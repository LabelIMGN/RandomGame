#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "types.h"

#define NUM_DISPLAY_STATS (NUM_BATTLE_STATS + NUM_RESOURCE_STATS)
#define STAT_V_SPACING_MULT 3
#define BOX_EXT_PADDING 1
#define BOX_INT_H_PADDING 2
#define BOX_INT_V_PADDING 1
#define TEXT_SPACING 2
#define GAME_SPEED 1000 // Delay in milliseconds
#define INTRO_TYPE 0 
#define LOOT_TYPE 1
#define EVENT_TYPE 2
#define BATTLE_TYPE 3

static WINDOW *text_box = NULL;
static WINDOW *word_wrap_window = NULL;
static int msg_count = 0;

void update_player_display(character_t *player){
  // BOX SIZING
  // stat box
  int stat_box_height, stat_box_width, start_y, start_x, y_beg, y_max, x_beg, x_max; 
  getbegyx(stdscr, y_beg, x_beg);
  getmaxyx(stdscr, y_max, x_max);
  stat_box_height = y_max/2 + NUM_BATTLE_STATS - BOX_INT_V_PADDING*4;
  stat_box_width = x_max/4 - BOX_EXT_PADDING;
  start_y = start_x = BOX_EXT_PADDING;
  // resource box
  int res_box_height, res_box_width;
  char *res_box_level_display = " Level %d ";
  res_box_height = 4;
  res_box_width = stat_box_width - BOX_INT_H_PADDING*2; // To account for both sides
  
  #ifdef DEBUG
  mvprintw(y_max - 10 , 0, "y_beg: %d\ny_max: %d\nx_beg: %d\nx_max: %d\n", y_beg, y_max, x_beg, x_max);
  printw("Display stat: %d\nStat spacing: %d\n", NUM_DISPLAY_STATS, STAT_V_SPACING_MULT);
  getch();
  #endif

  clear();
  refresh();

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
  char *player_stat_names[NUM_BATTLE_STATS] = {"Strength", "Dexterity", "Magic", "Faith"};

  // stat names display
  for(i = 0; i < NUM_BATTLE_STATS; i++){
  wattron(stat_box, COLOR_PAIR(i+1));
  mvwprintw(stat_box, res_box_height + 3 + STAT_V_SPACING_MULT*i, BOX_INT_H_PADDING + 1,
            "%s: %d", player_stat_names[i], player->battle_stat[i]);
  wattroff(stat_box, COLOR_PAIR(i+1));
  }
  wrefresh(stat_box);
  wrefresh(resources_box);
  // getch();

  delwin(resources_box);
  delwin(stat_box);
}

void update_text_box(int room_type, char *event, char *outcome, int damages){

  int text_box_height, text_box_width, start_y, start_x, y_beg, y_max, x_beg, x_max; 
  // SIZING
  getbegyx(stdscr, y_beg, x_beg);
  getmaxyx(stdscr, y_max, x_max);
  text_box_height = y_max/2 + NUM_BATTLE_STATS - BOX_INT_V_PADDING*4;
  text_box_width = x_max - (x_max/4 - BOX_EXT_PADDING) - 2*BOX_EXT_PADDING; // Confusing. Change this when you have time
  start_y = BOX_EXT_PADDING;
  start_x = (x_max/4 - BOX_EXT_PADDING) + BOX_EXT_PADDING;

  // BOX LOOK
  chtype left, right, top, bottom, tlc, trc, blc, brc; 
  left = right = ACS_VLINE;
  top = bottom = ACS_HLINE;
  tlc = ACS_ULCORNER;
  trc = ACS_URCORNER;
  blc = ACS_LLCORNER;
  brc = ACS_LRCORNER;

  // DRAWING
  if(text_box == NULL){
    text_box = newwin(text_box_height, text_box_width, start_y, start_x);
    word_wrap_window = derwin(text_box, text_box_height - 2, text_box_width - 4, 1, 2);
  }

  switch(room_type){
    case 0: // Status message(intro, game over, level up)
      wclear(word_wrap_window);
      msg_count = 0;
      mvwprintw(word_wrap_window, msg_count*TEXT_SPACING, 0, event);
      msg_count ++;
      wrefresh(word_wrap_window);
      wrefresh(text_box);
      break;
    case 1: // Loot room
      mvwprintw(word_wrap_window, msg_count*TEXT_SPACING, 0, event);
      msg_count ++;
      wrefresh(word_wrap_window);
      wrefresh(text_box);
      break;
    case 2: // Event room
      mvwprintw(word_wrap_window, msg_count*TEXT_SPACING, 0, event);
      msg_count ++;
      wrefresh(word_wrap_window);
      wrefresh(text_box);
      if(damages == -1 && outcome != NULL){ // Positive outcome
        mvwprintw(word_wrap_window, msg_count*TEXT_SPACING, 0, outcome);
      }
      else{
        if(outcome != NULL){
          mvwprintw(word_wrap_window, msg_count*TEXT_SPACING, 0, outcome);
        }
        msg_count ++;
        wrefresh(word_wrap_window);
        wrefresh(text_box);
        mvwprintw(word_wrap_window, msg_count*TEXT_SPACING , 0, "You loose %d HP", damages);
      }
      break;
    case 3: // Battle room
      mvwprintw(word_wrap_window, msg_count*TEXT_SPACING, 0, event);
      msg_count ++;
      wrefresh(word_wrap_window);
      wrefresh(text_box);
      napms(GAME_SPEED);
      mvwprintw(word_wrap_window, msg_count*TEXT_SPACING, 0, outcome);
      wrefresh(word_wrap_window);
      wrefresh(text_box);
      break;
  }

  wborder(text_box, left, right, top, bottom, tlc, trc, blc, brc);
  wrefresh(word_wrap_window);
  wrefresh(text_box);
  // getch();
  napms(GAME_SPEED);
}

void cleanup_text_box(void){
  if(word_wrap_window) delwin(word_wrap_window);
  if(text_box) delwin(text_box);
}
