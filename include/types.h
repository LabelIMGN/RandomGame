#ifndef TYPES_H
#define TYPES_H

#define MAX_NAME_LENGTH 32
#define NUM_STATS 6
#define NUM_BATTLE_STATS 4 // str | dex | int | fth

typedef struct {
  char name[MAX_NAME_LENGTH]; 
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
  char *success[NUM_BATTLE_STATS]; // 0: str | 1: dex | 2: int | 3: fth
  char *failure[NUM_BATTLE_STATS]; // 0: str | 1: dex | 2: int | 3: fth
  int difficulty[NUM_BATTLE_STATS]; //Difficulty per stats from 0 to 100, same order 
  int max_damage; // As a percent of player's max health
}event_t;

typedef struct{
  character_t enemy;
  char *type; // Minion or Boss
  int count;
}encounter_t;

typedef struct{
  // 16 characters should be enough to describe potion sizes and types
  char type[16];
  char size[16];
}potion_t;

#endif
