#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"


void update_player_display(character_t *player);
void update_text_box(int room_type, char *event, char *outcome, int damages);
char *resource_bar_display(character_t *player);

#endif
