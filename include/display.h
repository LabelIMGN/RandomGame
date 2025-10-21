#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

void update_player_display(character_t *player);
void update_event_display(character_t *player); // TODO
char *resource_bar_display(character_t *player);

#endif
