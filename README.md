# COMP-371 Team 9 Project

<hr />

We chose to do a simple tennis game simulation. To achieve this, we created a simple animation, using keyframes,
of 2 players playing tennis against each other in front of a crowd.

The animation is non-interactive, except for camera controls.

The basic animation is as follows: The blue-side player scores on the red-side player. Then the red player scores on the blue player.
Finally, both players volley the ball back and forth for a while until the blue player finally scores.

Each time a player scores, their respective scoreboard increments an appropriate amount according to the rules of tennis.
In addition, a crowd sound plays, indicating cheering.

There is a constant murmur of crowd noise throughout the scene to make it more realistic.

The crowd sounds are composed of 2 male crowds and 2 female crowds. One side has the male crowd sounds and vice-versa.
When a player scores, a random crowd sound from their appropriate sound bank plays.

In addition, when a racket hits a ball, a random ball sound plays. There are 3 possible ball sounds.

## Controls

 - arrow left - Rotate the world about the positive X axis
 - arrow right - Rotate the world about the negative X axis
 - arrow up - Rotate the world about the positive Y axis
 - arrow down - Rotate the world about the negative Y axis
 - P - Render model as points
 - L - Render model as lines
 - T - Render model as triangles
 - HOME - Resets projection, view, world matrices to initial position
 - Mouse button left - Movement with left mouse button up/ down will tilt the camera
 - Mouse button right - Movement with right mouse button left/ right will pan the camera
 - Mouse Button Middle - Movement up/ down will zoom in and out the camera

## TODO for Project

- [x] Create simple tennis game animation
- [x] Add ball sounds when ball is hit
- [x] Add crowd sounds to cheer when player scores
- [x] Implement working scoreboard that increments appropriately when a player scores
- [x] Draw 2 rackets with arms representing the players
- [ ] Create video for project
- [ ] Add screenshots to README 


## Possible Add-ons
- [ ] Add animated crowd
- [ ] Add foliage
- [ ] Add a clock to show animation duration
- [ ] Add walking people
- [ ] Animate a camera

## Build Instructions

### Command Line

1. Download and Install CMake
2. Open a terminal
3. Run CMake:

```shell
cd <source_folder>
cmake -S . -B cmake
cmake --build cmake --target install
```

To generate an XCode project from the command line use:

```shell
cd <source_folder>
cmake -S . -B cmake -G Xcode
cmake --build cmake --target install
```
