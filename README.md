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
