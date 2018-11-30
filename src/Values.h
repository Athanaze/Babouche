#pragma once
#include <string>
#include "HiddenValues.h"
// N Particles related values
const int NUMBER_OF_PARTICLES = 3000;
const int N1 = NUMBER_OF_PARTICLES -1;
const int N_THREAD = NUMBER_OF_PARTICLES * N1;
const float CUBE_SIZE = 0.2f;

//Physics stuff
const float G = 5.0f;
const float FAKE_SPEED = 0.001f;
const float MIN_SQUARED_DIST = 10.0f;

//Particles setup
const float RANDOM_RANGE_XYZ = 15.0f;
const float MAX_MASS = 100.0f;

//Cuda calculations
const int CUDA_BLOCK_SIZE = 1024;
const int CUDA_N_BLOCKS = ((NUMBER_OF_PARTICLES*N1) /CUDA_BLOCK_SIZE) +1;

////////////////////////////////////////////////
// LOGGING VALUES
////////////////////////////////////////////////
/*

FROM HiddenValues.h :

//Logging into influxdb
const char DB_ADDRESS[] = "XXX.XXX.X.XX";
const char DB_TABLE_NAME[] = "XXXXXXXX";
const char DB_USER_NAME[] = "XXXXXXX";
const char DB_USER_PASS[] = "XXXXXXX";

*/

//Just leave it like that
const int DB_PORT = 8086;

//Logging words
const char MEASUREMENT_NAME[] = "Babouche_performance";
const char MEASUREMENT_FPS[] = "FPS";
