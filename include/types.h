#ifndef TYPES_H
#define TYPES_H

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

#endif
