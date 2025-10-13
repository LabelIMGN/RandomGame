#ifndef ROOMS_H
#define ROOMS_H

#include "types.h"

#define NUM_EVENTS 4
#define NUM_ENEMIES 4
#define NUM_POTION_TYPE 2
#define NUM_POTION_SIZE 2
#define POTION_SIZE_SMALL 0
#define POTION_SIZE_BIG 1
#define SMALL_POTION_RECOVERY 1
#define BIG_POTION_RECOVERY 3

extern const event_t events[NUM_EVENTS];
extern const encounter_t enemies[NUM_ENEMIES];

/*
 * Generate a random health or mana potion(big or small) for the player to loot 
 * and regenerate the apropriate amount to HP or MP
 *
 * Parameters:
 *   player - The current player struct
 *
 * Returns: 
 *   player - With updated HP or MP
 */
character_t loot_room(character_t player);

/*
 * Generate an enemy with randomly generated battle stats 
 * and compare each battle stats with the player's to determine
 * the damages done to the player
 *
 * Parameters:
 *   player - The current player struct
 *   player_battle_stats[] - For comparaison. Passed as an array for future fexibility
 *   room_count - The number of rooms the player cleared
 *
 * Returns: 
 *   player - With updated HP or MP
 */
character_t process_encounters(character_t player, int player_battle_stats[], int room_count);

/*
 * Chooses from a set of written event and decide the outcome based on player stats
 * and difficulty
 *
 * Parameters:
 *   player - The current player struct
 *   room_count - The number of rooms the player cleared
 *
 * Returns: 
 *   player - With updated stats
 */
character_t process_event(character_t player, int room_count);

#endif
