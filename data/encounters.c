#include "types.h"
#include "rooms.h"

// REMINDER: 0: str | 1: dex | 2: int | 3: fth

const encounter_t encounters[NUM_ENCOUNTERS] = { 
  {"slime", {1, 2, -2, 0}},
  {"bat", {0, -2, -2, 0}},
  {"zombie", {1, 2, -1, -2}},
  {"ghost", {3, 2, -1, -3}}
};

_Static_assert(sizeof(encounters) / sizeof(encounters[0]) == NUM_ENCOUNTERS,
               "NUM_ENCOUNTERS doesn't match the encounters count!");

