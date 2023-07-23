# Accepted input:
 <p>W - positive y movement  <br>
 Shift+ A - Positve x movement<br>
 S - Negative y movement<br>
 Shift + D - Negative x movement<br>
 U - Scale up<br>
 J - Scale down<br>
 a / d - pressing a or d allows for model rotation about y axis<br>
 SPACE - will randomly reposition model inside viewing area<br>
 arrow left - rotate  world about positive x axis          <br>
 arrow right - rotate world about -ve x axis               <br>
 arrow up - rotate    world about positive y axis          <br>
 arrow down - rotate  world  about negative y axis         <br>
 P - render model as points                                <br>
 L - render model as lines                                 <br>
 T- Render model as triangles                              <br>
 HOME - resets projection, view, world matrix to initial position<br>
 Mouse button left- Movement with left mouse button up/down will tilt the camera<br>
 Mouse button right - Movement with right mouse button left/right will pan the camera<br>
 Mouse Button Middle - Movement up/down will zoom in and out the camera</p>

# toDo Assignment2:
- [ ] create spherical tennis ball using triangle mesh representations
- [ ] illuminate scene with point light (white) 30 units above the mesh using Phong Model
- [ ] render scene with clay texture on ground mesh, green texture on ball, and glossy finish for racket
- 	-> for this we need UV coordinates
- [ ] Render scene with shadows using two pass shadow algorithm
- [x] Make the racket movements possible by rotating arm at various joints
-     -> translation forward and backward, aswell as clockwise anticlockwise
- [x] windows should handle resize events and correctly adjust aspect ratio
- [ ] mesh turn right of left by a small amount
- 	 NOTE: here mesh means the model
- [x] application must use opengl 4.1 
- [ ] more elaborate model of the arm with finger movements and rotating light etc
- [ ] put shaders in glsl files
- [ ] convert to cmake
- user inputs:
- [ ] X toggles textures on and off
- [ ] B toggles Shadows
# Checklist of Functionality Assignment1:
- [x] 1 model needs upper and lower arm, ideally independent of eachother
- [x] 2: grid needs to be 100 x 100
- [x] 3: coord needs to be 5 grids long in each direction
- [x] 4: GLFW 1024x 768, using perspective view and backface culling [-> res is set, should be set in perspective, and backface cullin enabled]
- [x] 5: pressing spacebar should move model  to random grid location
- [x] 6: U scales up hand ; J scales down -> small increments
- [x] 7: Move model positions WASD for direction ;
- [x] 7.1 : a d for rotation about y axis (5 degrees)
- [x] 8: World orientation using (left arrow -> Rx) ; (right arrow -> R -x ) ; (up arrow -> Ry) ; (down arrow -> R -y) 
  			Rx denotes a small anti-clockwise
  			rotation about positive x axis, R-x about negative x axis, etc
- [x] 8.1: home reset to initial position and orientation  
- [x] 9: User can change rendering mode for the model ie: points, line, triangles based on keyboard input (P L T)
- [x] 10: Pan and tilt camerea -> tilt being move the camera up and down
			mouse right button is pressed → use mouse movement in x direction to pan;
			mouse middle button is pressed → use mouse movement in y direction to tilt.
- [x] 11: Camera zoom in and out of the scene - while  mouse left button is pressed → mouse movement to move into/out of the scene

# Possible Addons:
 <p>ADDON: rotate forearm independently<br>
 ADDON: E and W for z axis<br>
 ADDON: Mouse scroll zoom and mouse movement pan/tilt</p>

 ## Build Instructions
### Command Line

1. Download and Install CMake
2. Open a terminal
3. Run cmake:

```
cd <source_folder>
cmake -S . -B <build_folder>
cmake --build <build_folder> --target install
```

To generate an Xcode project from the command line use:

```
cd <source_folder>
cmake -S . -B <build_folder> -G Xcode
cmake --build <build_folder> --target install
```