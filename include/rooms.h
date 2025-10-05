#ifndef ROOMS_H
#define ROOMS_H

#include "types.h"

#define NUM_EVENTS 4

extern const event_t events[NUM_EVENTS];

character_t loot_room(character_t player);
character_t start_encounter(character_t player, int room_count);
character_t start_event(character_t player, int room_count);

#endif
