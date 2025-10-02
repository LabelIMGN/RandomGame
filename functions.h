#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//Common includes -----
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

// Struct definitions -----
typedef struct {
  char name[69];
  int level;
  int stat_max;
  int available_points;
  int max_hp;
  int max_mp;
  int current_hp;
  int current_mp;
  int strength;
  int dexterity;
  int intelligence;
  int faith;
}character_t; //character_struct

typedef struct {
  char *description;
  char *success[4]; // 0: str | 1: dex | 2: int | 3: fth
  char *failure[4]; // 0: str | 1: dex | 2: int | 3: fth
  int difficulty[4]; //Difficulty per stats from 0 to 100, same order 
  int max_damage; // As a percent of player's max health
}event_t;

typedef struct{
  character_t enemy;
  char *type; // Minion or Boss
  int count;
}encounter_t;

typedef struct{
  char type[7];
  char size[6];
}potion_t;

// Function declarations -----
int set_available_points(int level);
int set_max_stat(int level, int available_points);
int choose_highest_stat(int player_stats[]);
void display_player(character_t player);
character_t distribute_points(character_t player);
character_t loot_room(character_t player);
character_t start_encounter(character_t player, int room_count);
character_t start_event(character_t player, int room_count);

#endif
