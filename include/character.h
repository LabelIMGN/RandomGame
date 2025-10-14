#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"


int set_available_points(int level);
int set_max_stat(int level, int available_points);
int choose_highest_stat(int player_stats[]);
character_t distribute_points(character_t player);
character_t level_up(character_t player, int room_count);

#endif
