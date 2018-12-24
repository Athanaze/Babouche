#pragma once
#include <string>
// N Particles related values
const int N_PARTICLES = 10000;
//const int N_PARTICLES = 100;

//DONT CHANGE THIS VALUE
const int NUMBER_OF_PARTICLES = 10 * N_PARTICLES;
//Physics stuff
const float G = 5.0f;
const float FAKE_SPEED = 0.01f;
const float MIN_SQUARED_DIST = 0.5f;
//Used for collision detection
const float MIN_PROX = 0.0001f;
//Particles setup
const float RANDOM_RANGE_XYZ = 200.0f;
const float MAX_MASS = 99.0f;
const float FORCE_RANGE = 10.0f;
////////////////////////////////////////////////
// VIEW VALUES
////////////////////////////////////////////////
const float FOV = 50.0f;
const float NEAR_CLIP = 0.000001f;
const float FAR_CLIP = 999999999.0f;


//Size of elements
const float POINT_SIZE = 4.0f;
const float VECTOR_WIDTH = 5.0f;
const float VECTOR_BACK_LENGTH = 0.2f;
const int   SCREEN_WIDTH    = 2560;
const int   SCREEN_HEIGHT   = 1440;
//const float CAMERA_DISTANCE = 5000.0f;
const float CAMERA_DISTANCE = 50.0f;
const float DEG2RAD         = 3.141593f / 180;

const char WINDOW_NAME[] = "Babouche Engine";

//Colors
//vector
const float VECTOR_COLOR_R = 0.2f;
const float VECTOR_COLOR_G = 0.8f;
const float VECTOR_COLOR_B = 1.0f;

////////////////////////////////////////////////
// LOGGING VALUES
////////////////////////////////////////////////
const std::string START_MSG = "Babouche Engine";
//Relative to the binary file
const char FPS_LOG_PATH[] = "../out/logFiles/fpsLog.data";
const int STRIDE_VALUE = 1;
// DONT CHANGE THIS VALUE
const int STRIDE = NUMBER_OF_PARTICLES / STRIDE_VALUE;


//Other stride stuck
const int STRIDE_2 = NUMBER_OF_PARTICLES / 1;
