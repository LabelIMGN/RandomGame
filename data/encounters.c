#include "types.h"
#include "rooms.h"

// REMINDER: 0: str | 1: dex | 2: mag | 3: fth

const encounter_t enemies[NUM_ENEMIES] = { 
  {"slime", {1, 2, -2, 0}},
  {"bat", {0, -2, -2, 0}},
  {"zombie", {1, 2, -1, -2}},
  {"ghost", {3, 2, -1, -3}},
  {"lich", {3, 3, 0, -3}},
};

_Static_assert(sizeof(enemies) / sizeof(enemies[0]) == NUM_ENEMIES,
               "NUM_ENEMIES doesn't match the encounters count!");

