# Accepted input:

 - W - positive y movement  
 - Shift + A - Positive x movement
 - S - Negative y movement
 - Shift + D - Negative x movement
 - U - Scale up
 - J - Scale down
 - a / d - pressing a or d allows for model rotation about y axis
 - SPACE - will randomly reposition model inside viewing area
 - arrow left - rotate  world about positive x axis          
 - arrow right - rotate world about negative x axis               
 - arrow up - rotate    world about positive y axis          
 - arrow down - rotate  world  about negative y axis         
 - P - render model as points                                
 - L - render model as lines                                 
 - T - Render model as triangles                              
 - HOME - resets projection, view, world matrix to initial position
 - Mouse button left- Movement with left mouse button up/down will tilt the camera
 - Mouse button right - Movement with right mouse button left/right will pan the camera
 - Mouse Button Middle - Movement up/down will zoom in and out the camera
 - TAB- Changes model selection
 - SHIFT+TAB - Change Joint selection for enabled models

# TODO Assignment2:
- [ ] individual scaling
- [x] create spherical tennis ball using triangle mesh representations
- [x] illuminate scene with point light (white) 30 units above the mesh using Phong Model
- [x] render scene with clay texture on ground mesh, green texture on ball, and glossy finish for racket
- [x] Render scene with shadows using two pass shadow algorithm
- [x] Make the racket movements possible by rotating arm at various joints -> translation forward and backward, as well as clockwise anticlockwise  
- [x] windows should handle resize events and correctly adjust aspect ratio
- [x] mesh turn right of left by a small amount NOTE: here mesh means the model	 
- [x] application must use OpenGL 4.1 
- [x] more elaborate model of the arm with finger movements and rotating light etc
- [x] put shaders in .glsl files
- [x] convert to CMake

# user inputs:
- [x] X toggles textures on and off
- [x] B toggles Shadows

# Checklist of Functionality:
- [x] 1: model needs upper and lower arm, ideally independent of each other
- [x] 2: grid needs to be 100 x 100
- [x] 3: coordinate axes need to be 5 grids long in each direction
- [x] 4: GLFW 1024x 768, using perspective view and back face culling [-> res is set, should be set in perspective, and back face culling enabled]
- [x] 5: pressing space bar should move model  to random grid location
- [x] 6: U scales up hand ; J scales down -> small increments
- [x] 7: Move model positions WASD for direction ;
- [x] 7.1 : a d for rotation about y axis (5 degrees)
- [x] 8: World orientation using (left arrow -> Rx) ; (right arrow -> R -x ) ; (up arrow -> Ry) ; (down arrow -> R -y) 
  			Rx denotes a small anti-clockwise
  			rotation about positive x axis, R-x about negative x axis, etc
- [x] 8.1: home reset to initial position and orientation  
- [x] 9: User can change rendering mode for the model i.e.: points, line, triangles based on keyboard input (P L T)
- [x] 10: Pan and tilt camera -> tilt being move the camera up and down
			mouse right button is pressed → use mouse movement in x direction to pan;
			mouse middle button is pressed → use mouse movement in y direction to tilt.
- [x] 11: Camera zoom in and out of the scene - while  mouse left button is pressed → mouse movement to move into/out of the scene

# Possible Add-ons:
- [ ] 1: rotate forearm independently
- [ ] 2: E and W for z axis
- [ ] 3:  Mouse scroll zoom and mouse movement pan/tilt

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
