# Tetris

This repository contains a clone of the famous Tetris game.  

> CMake is used as software build system (https://cmake.org/)

> SFML is used as graphics library (https://en.wikipedia.org/wiki/Simple_and_Fast_Multimedia_Library)

## Overview

Main.cpp
========
Contains the 'game-loop'.
Retrieve input from UI, update the 'Game' and render it to the screen

Game.cpp
========
The 'Game' object controls the game.
putting on 'pause'
'gameover'
contains a reference to the player-profile
contains a reference to the field

Player_profile.cpp
==================
Keeps the highscore

load/save profile to file

Field.cpp
=========
Represents the playing field.
contains array of 'Tile' objects:
holds a reference to the 'current_block' and 'next_block'

checks if gameover
checks for complete lines
Updates the tiles based on the position of the 'current_block'

Tile.cpp
========
Represents 1 part of the playing field
holds a reference to the 'Block' object which overlaps the tile.

Block.cpp
=========
Knows its (four) layout's (for each rotation)
Contains its 'position' (row/col) in the field and its current 'layout' (based on rotation)

Contains the logic to move itself and does boundary check

Level.cpp
=========
Contains some level-specific logic.
for example: speed 

## Technical 

- class diagrams
- sequence diagrams
- schema's and figures

## Features/ideas to be implemented

- music
- [done] soundeffects
- better visuals (background, ...)
- start menu : choose playfield, choose block-set, choose playerprofile, choose gametype
- [done] more levels (goal is 9 levels) -> 6 levels
- [done] user defined playfields
- other set of blocks
- other game-type: instead of full lines, combine 'truchet tiles' (all connected same color, or 4 in a vierkant, ...)
 

## Improvements

- 2D-array -> 1D-array

## Release Notes