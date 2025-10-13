#include "types.h"
#include "rooms.h"

// Events text and difficulty
const event_t events[NUM_EVENTS] = {
  {"You enter a room and see a chest locked with a rune.",
  {"You bash it open", "You successfully reach the lock through the rune", "You dispel the rune", "The gods accept to help you"},
  {"You fail to bash it open", "The rune prevents you from reaching the lock", "You fail to dispel it", "Your prayers go unanswered"},
  {75, 70, 20, 40},
  10.0},

  {"A treasure room is in front of you but the door is locked.",
  {"You kick it down", "You lockpick it", "You blast it open with a spell", "You burn it with holy light"},
  {"You fail to kick it open", "You fail to lockpick it", "Your spell bounces back", "Gods don't open doors"},
  {50, 25, 50, 60},
  10.0},

  {"An unfamiliar pentagram is drawn in blood on the wall in front of you.",
  {"You forceefully break the wall", "You scrape it off with your dagger", "You activate it", "You banish it with a prayer"},
  {"You can't break it", "The blood won't scrape off", "You activate it and get blasted", "It reacts violently"},
  {85, 75, 15, 10},
  30.0},

  {"You see a heavy looking golden cobra shaped statue in the center of the room, apparently linked to the door",
  {"You manage to move it", "You carefully inspect the mouth and find a button", "You successfuly activate it with magic", "You pray to it and the door opens"},
  {"It bites you while you lift it", "It bites your hand while you inspect it", "It animates from your magic and attacks you", "It answers your prayer with a bite"},
  {100, 90, 85, 40}, 
  60.0}
};

_Static_assert(sizeof(events) / sizeof(events[0]) == NUM_EVENTS,
               "NUM_EVENTS doesn't match the event count!");
