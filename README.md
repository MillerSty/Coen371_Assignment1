# Controls

## Keyboard

- 1: Select racket 1.
- 2: Select racket 2.
- W/ S: Move selected racket up or down.
- A/ D: Move selected racket backward or forward.
- U/ J: Scale selected racket up or down.
- M: Toggle camera. There is 1 main camera, and 1 camera for each racket.
- R: Switch to main camera (regardless of which camera was active).
- HOME: Reset camera and racket translations. Switch to main camera.
- LEFT/ RIGHT arrow: Rotate world around Z-axis.
- UP/ DOWN arrow: Rotate world around Y-axis.
- B: Toggle shadows on or off.
- X: Toggle textures on or of.
- SPACE: Translate selected racket randomly around the scene.
- L: Toggle light on or off.

*Note:* Switching to a camera attached to a model does not automatically switch to the model itself.
This way, it's possible to switch between cameras and models independently of each other.

## Mouse

- LEFT click and mouse forward or back: Tilt camera up or down.
- RIGHT click and mouse left or right: Pan camera left or right.
- MIDDLE click and mouse forward or back: Zoom camera in or out.

# Required Features Implemented

- [x] Modeled 2 rackets, each in the middle on either side of the court. 
- [x] Modeled 4 letters, taken from my last name, 2 on each racket.
- [x] Each model contains 2 letters, an arm, and a racket, modeled as 1 entity.
- [x] 3 cubic poles for the tennis net. The poles have a textures.
- [x] Can transform all aspects of 1 model (the arm, racket, and 2 letters) together (only translation and scaling are implemented).
- [x] Racket has a metallic texture.
- [x] Arm has a tattoo texture. 
- [x] Each letter has a different texture.
- [x] Skybox has a distinct texture.
- [x] Light can be toggled with L key (did not interpret this is a secondary spot light, but the existing light from A2).
- [x] Each racket model has a camera attached to the front of it for a total of 3 cameras: main, racket 1, and racket 2.
- [x] The racket cameras move along with the rackets.
- [x] Shadows rendered with 2 pass algorithm (toggle-able with B key).
- [x] Uses OpenGL 4.1. Includes many comments

# Required Features Not implemented

- [ ] A fourth camera circling around the center of the scene.
- [ ] A spotlight attached to 4th camera.
- [ ] The spotlight was assumed to be the spotlight from A2, so there is no additional light. 


# Build Instructions

## Command Line

1. Download and Install CMake
2. Open a terminal
3. Run CMake:

```shell
cd <source_folder>
cmake -S . -B <build_folder>
cmake --build <build_folder> --target install
```

To generate an XCode project from the command line use:

```shell
cd <source_folder>
cmake -S . -B <build_folder> -G Xcode
cmake --build <build_folder> --target install
```
