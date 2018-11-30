///////////////////////////////////////////////////////////////////////////////
//VUE + CONTROLLER ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Matrices.h"
#include "Vectors.h"

#include "Engine.h"

#include "Model.h"

#include "vendors/influxdb-cpp/influxdb.hpp"
using std::stringstream;
using std::ends;
//MAIN LOOP
void displayCB()
{
    displayCBStart();
    //All the logic / Model stuff is executed here, every frame
    updateModel();
    displayCBEnd();
}
void displayCBStart(){
    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // save the initial ModelView matrix before modifying ModelView matrix
    glPushMatrix();
    // tramsform camera
    matrixView.identity();
    matrixView.rotate(cameraAngleY, 0, 1, 0);
    matrixView.rotate(cameraAngleX, 1, 0, 0);
    matrixView.translate(0, 0, -cameraDistance);
    glLoadMatrixf(matrixView.get());
    drawGrid();
    // compute model matrix
    matrixModel.identity();
    matrixModel.rotateY(45);
    // compute modelview matrix
    matrixModelView = matrixView * matrixModel;
    // copy modelview matrix to OpenGL
    glLoadMatrixf(matrixModelView.get());
}
void displayCBEnd(){
    //showInfo();
    glPopMatrix();
    glutSwapBuffers();
}
int main(int argc, char **argv)
{
    std::cout << "BABOUCHE ENGINE" <<std::endl;
/*
    //INFLUX DB STUFF
    influxdb_cpp::server_info si("192.168.1.37", 8086, "sensor_data", "pi", "lousachir");
    influxdb_cpp::builder()
        .meas("Babouche_performance")
        .field("FPS", 350)
        .timestamp(1543600706)
        .post_http(si);
*/


    //INFLUX DB STUFF
    influxdb_cpp::server_info si(DB_ADDRESS, DB_PORT, DB_TABLE_NAME, DB_USER_NAME, DB_USER_PASS);
    influxdb_cpp::builder()
        .meas(MEASUREMENT_NAME)
        .field(MEASUREMENT_FPS, 350)
        .timestamp(1543600706)
        .post_http(si);

    // init global vars
    initSharedMem();
    // register exit callback
    atexit(exitCB);
    // init GLUT and GL
    initGLUT(argc, argv);
    initGL();
    //Currently, startModel() is just used to setup particles values
    startModel();
    // the last GLUT call (LOOP)
    // window will be shown and display callback is triggered by events
    // NOTE: this call never return main().
    glutMainLoop(); /* Start GLUT event-processing loop */
    return 0;
}
// draw a grid on XZ-plane
void drawGrid(float size, float step)
{
    // 20x20 grid
    glBegin(GL_LINES);

    glColor3f(0.5f, 0.5f, 0.5f);
    for(float i=step; i <= size; i+= step)
    {
        glVertex3f(-size, 0,  i);   // lines parallel to X-axis
        glVertex3f( size, 0,  i);
        glVertex3f(-size, 0, -i);   // lines parallel to X-axis
        glVertex3f( size, 0, -i);

        glVertex3f( i, 0, -size);   // lines parallel to Z-axis
        glVertex3f( i, 0,  size);
        glVertex3f(-i, 0, -size);   // lines parallel to Z-axis
        glVertex3f(-i, 0,  size);
    }

    // x-axis
    glColor3f(1, 0, 0);
    glVertex3f(-size, 0, 0);
    glVertex3f( size, 0, 0);

    // z-axis
    glColor3f(0,0,1);
    glVertex3f(0, 0, -size);
    glVertex3f(0, 0,  size);

    glEnd();
}
// initialize GLUT for windowing
int initGLUT(int argc, char **argv)
{
    // GLUT stuff for windowing
    // initialization openGL window.
    // it is called before any other GLUT routine
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);   // display mode

    glutInitWindowSize(screenWidth, screenHeight);  // window size

    glutInitWindowPosition(100, 100);           // window location

    // finally, create a window with openGL context
    // Window will not displayed until glutMainLoop() is called
    // it returns a unique ID
    int handle = glutCreateWindow(argv[0]);     // param is the title of window

    // register GLUT callback functions
    glutDisplayFunc(displayCB);
    glutTimerFunc(33, timerCB, 33);             // redraw only every given millisec
    //glutIdleFunc(idleCB);                       // redraw whenever system is idle
    glutReshapeFunc(reshapeCB);
    glutKeyboardFunc(keyboardCB);
    glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);

    return handle;
}

// initialize global variables
bool initSharedMem()
{
    screenWidth = SCREEN_WIDTH;
    screenHeight = SCREEN_HEIGHT;

    mouseLeftDown = mouseRightDown = false;
    mouseX = mouseY = 0;

    cameraAngleX = cameraAngleY = 0;
    cameraDistance = CAMERA_DISTANCE;

    drawMode = 0; // 0:fill, 1: wireframe, 2:points

    return true;
}
// clean up shared memory
void clearSharedMem()
{
}
// set camera position and lookat direction
void setCamera(float eyeX, float eyeY, float eyeZ, float targetX, float targetY, float targetZ)
{
    Vector3 eye(eyeX, eyeY, eyeZ);
    Vector3 target(targetX, targetY, targetZ);
    Vector3 up(0, 1, 0); // assume the camera is always straight up (roll=0)

    // compute the forward, left  and up vectors
    // NOTE: the direction is reversed (target to camera pos) because of camera transform
    Vector3 forward = eye - target;
    float distance = forward.length();  // remember the distance
    forward /= distance;    // normalize
    Vector3 left = up.cross(forward).normalize();
    up = forward.cross(left); // up vector is unit length here

    // set inverse of rotation matrix: M^-1 = M^T if it is Euclidean transform
    matrixView.identity();
    matrixView.setRow(0, left);
    matrixView.setRow(1, up);
    matrixView.setRow(2, forward);

    // set translation column
    Vector3 trans(0, 0, -distance);
    matrixView.setColumn(3, trans);
}
// set projection matrix as orthogonal
void toOrtho()
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    // set orthographic viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// set the projection matrix as perspective
void toPerspective()
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)(screenWidth)/screenHeight, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// initialize OpenGL
// disable unused features
void initGL()
{
    glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glDisable(GL_LIGHTING);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0, 0, 0, 0);                   // background color
    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);
}
//=============================================================================
// CALLBACKS
//=============================================================================
void reshapeCB(int w, int h)
{
    screenWidth = w;
    screenHeight = h;

    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    //matrixProjection = setFrustum(45, (float)w/h, 1.0f, 100.0f);
    //glLoadMatrixf(matrixProjection.get());
    //@@ the equivalent OpenGL call
    gluPerspective(45.0f, (float)(w)/h, 1.0f, 100.0f); // FOV, AspectRatio, NearClip, FarClip

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
}

void timerCB(int millisec)
{
    glutTimerFunc(millisec, timerCB, millisec);
    glutPostRedisplay();
}

void idleCB()
{
    glutPostRedisplay();
}

void keyboardCB(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27: // ESCAPE
        exit(0);
        break;

    case ' ':
        break;
    default:
        ;
    }
}

void mouseCB(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }
}

void mouseMotionCB(int x, int y)
{
    if(mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX += (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
    if(mouseRightDown)
    {
        cameraDistance -= (y - mouseY) * 0.2f;
        mouseY = y;
    }
}

void exitCB()
{
    clearSharedMem();
}
