#ifndef TYPES_H
#define TYPES_H

#define MAX_NAME_LENGTH 32 // DONT FORGET TO ADJUST THE NAME INPUT IN MAIN IF YOU CHANGE THIS!
#define NUM_BATTLE_STATS 4 // str | dex | int | fth
#define RESISTANCES_MIN -3
#define RESISTANCES_MAX 3
#define XP_GAIN_MODIFIER 10
#define LEVEL_REQ_MODIFIER 8

typedef struct {
  char name[MAX_NAME_LENGTH]; 
  int level;
  int cur_xp_pool;
  int next_level_threshold;
  int stat_max;
  int available_points;
  int max_hp;
  int max_mp;
  int cur_hp;
  int cur_mp;
  int str;
  int dex;
  int mag;
  int fth;
  int battle_stat[NUM_BATTLE_STATS];
}character_t;

typedef struct {
  char *description;
  char *success[NUM_BATTLE_STATS]; // 0: str | 1: dex | 2: int | 3: fth
  char *failure[NUM_BATTLE_STATS]; // 0: str | 1: dex | 2: int | 3: fth
  int difficulty[NUM_BATTLE_STATS]; //Difficulty per stats from 0 to 100, same order 
  int max_damage;
}event_t;

typedef struct{
  char *type;
  int resistances[NUM_BATTLE_STATS]; // Assigned to each stats from RESISTANCES_MIN to RESISTANCES_MAX
}encounter_t;

typedef struct{
  // 16 characters should be enough to describe potion sizes and types
  char type[16];
  char size[16];
}potion_t;

#endif
