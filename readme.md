# Compilating and running

## Requirements
OpenGL libraries (Mesa) and freeglut
```bash
sudo apt install mesa-utils
sudo apt install libgl1-mesa-dev
sudo apt install freeglut3-dev
``` 
## Execution
1. Compile
```bash
g++ fractal.cpp -o fractal -lGL -lGLU -lglut
```
2. Visualize
```bash
./fractal
```
### Using shaders

1. Install further dependencies
```bash
sudo apt install libglew-dev
```
2. Compile and visualize
```bash
g++ fractal-shader.cpp -o fractal -lGL -lGLU -lglut -lGLEW
./fractal
```
