# Terminal Dungeon Crawler

A roguelike dungeon crawler game built in C for the terminal. Progress through randomly-generated rooms, face skill checks, find loot, and battle enemies until you meet your demise. Each death reveals how far you made it, and you can start over with a reborn character that accumulates experience with eah run.

## Features

- **Dynamic stat distribution** - Characters are randomly generated based on level with 6 core stats
- **Three room types**:
  - Loot rooms with health/mana potions
  - Event rooms with skill-based challenges
  - Battle encounters
- **Scaling difficulty** - Events and battles become harder as you progress deeper into the dungeon
- **Stat-based skill checks** - Your highest stat is automatically used to attempt events

## Building the Project

### Prerequisites

- GCC or Clang compiler
- Make
- Math library (libm)

### Compilation

From the project root directory:

```bash
make
```

This will create the executable at `bin/game`.

### Running

```bash
make run
```

Or directly:

```bash
./bin/game
```

### Cleaning Build Artifacts

```bash
make clean
```

### Rebuild Everything

```bash
make rebuild
```

## Project Structure

```
.
├── Makefile              # Build configuration
├── include/              # Header files
│   ├── character.h       # Character stat management
│   ├── common.h          # Common includes
│   ├── display.h         # Display functions
│   ├── rooms.h           # Room type declarations
│   └── types.h           # Type definitions
├── src/                  # Source files
│   ├── main.c            # Main game loop
│   ├── character.c       # Character creation and stats
│   ├── display.c         # UI rendering
│   └── rooms.c           # Room logic implementation
├── data/                 # Game data
│   ├── events.c          # Event definitions
│   └── encounters.c      # Encounter definitions (WIP)
├── obj/                  # Compiled object files (generated)
└── bin/                  # Executable (generated)
```

## Contributing

This is a personal learning project to get back into C. Nothing real will come of this

## License

This project is for educational purposes.
