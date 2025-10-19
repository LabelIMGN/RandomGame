#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"


void set_available_points(character_t *player);
void set_max_stat(character_t *player);
void distribute_points(character_t *player);
void level_up(character_t *player, int room_count);
int choose_highest_stat(int player_stats[]);

#endif
