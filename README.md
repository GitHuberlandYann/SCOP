# SCOP
Spatial Computing and Object Pendering

Project done during my formation at 19. https://campus19.be/

## Controls (Qwerty)
```
WSADER      - turn object around
arrows      - move object around
+-          - size object

C           - switch color mode (DEFAULT-MATERIAL-TEXTURE-GRAYS-GRADIENT)
X           - switch shading when color mode is GRAYS (WHITE-RED-GREEN-BLUE)
V           - Hide/Show faces with normals facing away from viewpoint
B           - Draw faces from furthest to closest (slower, but highly recommended)
F           - Fill faces with color
N           - Display vertices' normal vector
U           - Display box surrounding object
O           - Reset all settings to original values
P           - Enable/Disable perspective

* (pad)     - Enable/Disable plane {0, 0, z} cutting object
+- (pad)    - change z coordinate of plane from point above
enter (pad) - switch side of plane to hide
```

## Run
After cloning the project, cd into repo and run:

```
/SCOP $> make
/SCOP $> ./scop					# to run default file
/SCOP $> ./scop Ressources/file_you_want.obj
```

My personnal recommendation: 
* Launch ./scop
* Press the following keys in order: F C B SSSSSSSSSSSSSSSSSSSSS

If you have never run a project using mlxlib .. best of luck with compiling

## Sample
GIFs incomming, supposedly