#ifndef CHARACTER_H
#define CHARACTER_H

int set_available_points(int level);
int set_max_stat(int level, int available_points);
int choose_highest_stat(int player_stats[]);
character_t distribute_points(character_t player);

#endif
