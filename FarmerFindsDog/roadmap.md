# Basic
* _farmer_ is controlled by player
* _dog_ controlled by AI. DO
* _doghouse_ is the place where dog should be moved

Dog appears somewhere on the map. Farmer (player) appear somewhere else. Farmer moves to the dog and gives command; after dog hears command it starts to follow farmer. Then farmer moves to crate and gives another command. Dog hears second command enters _doghouse_; game successfully finished.

## Farmer
1. Farmer walks over field

 1. Farmer moved by keyboard arrows
 1. Arrow moves farmer by one map tile
 1. If no other arrow was pressed since farmer started to move, he continues to move in that direction
 1. If another arrow was pressed since farmer started to move, he continues his move. After reaching destination, he moves in second requested direction.
 1. If few another arrows were pressed, only last one will be processed
 1. If "space" was pressed, farmed stops after reaching goal (next tile)
 1. Farmer always stops face to player

1. Farmer figure is animated

1. If farmer reaches end of screen, screen moves (refocuses) so that farmer appears in the center.
 1. If farmer reaches end of map, no refocusing will be made

1. There are obstacles on the map (another layer on tiled map).
 1. Farmer does not even try to move to tile if there is obstacle There
 1. ???: additional processing if player tries to move player over obstacle; like animate but stay on same tile.

## Dog

Same as farmer (same base class???), but controlled by AI.

Dog can receive two commands:
* "follow" tries to follow farmer
* "go home" if is near _dog house_ goes inside

So there are three modes for dog:
* freely (randomly) go over map
* follow farmer
* go to house

## Dog house

Like any other obstacle on the map

## Fog of war

Farmer should not see the full map, just few squares around.

## Enemies and traps
Idea to be developed.
Anyway farmer is not expected to fight; he just should avoid enemies. So enemies also should appear only in some "dangerous" zones.

#Menu and Gui

# Additional character should appear over game map sometimes; like give advices and show hints.

##Linux and common
Menu with options:
* New Game (starts new game. Later: offers to select level)
* Options
* Exit

Options are:
* (checkbox) Fog of war enabled
* Key bingings (PC only): options to change key bindings
* TODO: one more option required here?



