##compilation example 
´´´
g++ fractal.cpp -o fractal -lGL -lGLU -lglut

./fractal
´´

##with shaders (need sudo apt install libglew-dev)
´´´
 g++ fractal-shader.cpp -o fractal -lGL -lGLU -lglut -lGLEW
´´´
##For Python Wrapper for OpenGL with Molecular Dynamics example

Create venv and import requirements.txt

´´´python3 mol-dim.py´´´
