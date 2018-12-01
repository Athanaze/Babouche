#pragma once
#include <string>
// N Particles related values
const int NUMBER_OF_PARTICLES = 8000;
const int N1 = NUMBER_OF_PARTICLES -1;
const int N_THREAD = NUMBER_OF_PARTICLES * N1;
const float CUBE_SIZE = 0.2f;

//Physics stuff
const float G = 5.0f;
const float FAKE_SPEED = 0.001f;
const float MIN_SQUARED_DIST = 10.0f;

//Particles setup
const float RANDOM_RANGE_XYZ = 25.0f;
const float MAX_MASS = 200.0f;

//Cuda calculations
const int CUDA_BLOCK_SIZE = 1024;
const int CUDA_N_BLOCKS = ((NUMBER_OF_PARTICLES*N1) /CUDA_BLOCK_SIZE) +1;

////////////////////////////////////////////////
// LOGGING VALUES
////////////////////////////////////////////////

//Relative to the binary file
const char FPS_LOG_PATH[] = "../out/logFiles/fpsLog.data";
