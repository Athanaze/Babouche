#include <GL/glut.h>
#include <thread>
#include "Matrices.h"
#include "Vectors.h"
#include "Engine.h"
#include "Tools.h"
#include <cstdlib>
#include <math.h>
namespace Draw{
    //        0   1   2   3   4   5
    //ARGS: [x0, y0, z0, x1, y1, z1]
    void vector(float *coord){
        glLineWidth(VECTOR_WIDTH);
        glBegin(GL_LINES);
        glColor3f(VECTOR_COLOR_R, VECTOR_COLOR_G, VECTOR_COLOR_B);

        // The line
        glVertex3f(coord[0], coord[1], coord[2]);
        glVertex3f(coord[3], coord[4], coord[5]);

        // The arrow at the end of the vector
        glVertex3f(coord[3], coord[4], coord[5]);
        float eX;
        float eY;
        float eZ;

        float dX = coord[3] - coord[0];
        float dY = coord[4] - coord[1];
        float dZ = coord[5] - coord[2];
        if(dX > 0){
            eX = coord[3] - (VECTOR_BACK_LENGTH*coord[3]);
        }
        else{
            eX = coord[3] + (VECTOR_BACK_LENGTH*coord[3]);
        }
        if(dY > 0){
            eY = coord[4] - (VECTOR_BACK_LENGTH*coord[4]);
        }
        else{
            eY = coord[4] + (VECTOR_BACK_LENGTH*coord[4]);
        }
        if(dZ > 0){
            eZ = coord[5] - (VECTOR_BACK_LENGTH*coord[5]);
        }
        else{
            eZ = coord[5] + (VECTOR_BACK_LENGTH*coord[5]);
        }
        glVertex3f(eX, eY, eZ);
        glEnd();
    }
}

float *xParticle = new float[NUMBER_OF_PARTICLES];
float *yParticle = new float[NUMBER_OF_PARTICLES];
float *zParticle = new float[NUMBER_OF_PARTICLES];
float *mParticle = new float[NUMBER_OF_PARTICLES];
float *fXParticle = new float[NUMBER_OF_PARTICLES];
float *fYParticle = new float[NUMBER_OF_PARTICLES];
float *fZParticle = new float[NUMBER_OF_PARTICLES];
// SCENE HEADER MUST BE INCLUDED AT THE END
#include "SuziScene.h"
//#include "NewtonScene.h"

void mainLoop()
{
    Frame::start();

    //All the logic / Model stuff is executed here, every frame
    Scene::Model::b_update();

    Frame::render();
    Frame::end();

}

// Scene agnostic
namespace Frame{
    /*
        These functions must be called in order !
        start() -> render() -> end()
    */
    void start(){
        Logger::Fps::startMeasuring();
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
        // compute model matrix
        matrixModel.identity();
        matrixModel.rotateY(45);
        // compute modelview matrix
        matrixModelView = matrixView * matrixModel;
        // copy modelview matrix to OpenGL
        glLoadMatrixf(matrixModelView.get());
    }

    void render(){
        //Render the particles
        glBegin(GL_POINTS);
        for (int i = 0; i < NUMBER_OF_PARTICLES; i++) {
            //Change the color of each particle according to the particle's mass
            glColor3f(0.01f*mParticle[i], 0.0f, 0.02f*mParticle[i]);
            glVertex3f(xParticle[i], yParticle[i], zParticle[i]);
        }
        glEnd();

        //Call scene-specific draw calls
        Scene::Vue::b_render();
    }

    void end(){
        glPopMatrix();
        glutSwapBuffers();
        Logger::Fps::endMeasuring();
    }
}

////////////////////////////////////////////
// SETUP STUFF
////////////////////////////////////////////
int main(int argc, char **argv)
{
    Logger::print(START_MSG);

    // init global vars
    initSharedMem();

    Scene::Model::b_setup();

    // register exit callback
    atexit(Callback::exitBabouche);
    // init GLUT and GL
    initGLUT(argc, argv);
    initGL();
    /*
        the last GLUT call (LOOP)
        window will be shown and display callback is triggered by events
        NOTE: this call never return main().
        Start GLUT event-processing loop
    */
    glutMainLoop();
    return 0;
}

// initialize GLUT for windowing
int initGLUT(int argc, char **argv)
{
    /*
    GLUT stuff for windowing
    initialization openGL window.
    it is called before any other GLUT routine
    */
    glutInit(&argc, argv);
    // display mode
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

    //Setup window
    glutInitWindowSize(screenWidth, screenHeight);
    glutInitWindowPosition(100, 100);

    /*
    finally, create a window with openGL context
    Window will not displayed until glutMainLoop() is called
    it returns a unique ID
    */
    int handle = glutCreateWindow(WINDOW_NAME);

    // register GLUT callback functions
    glutDisplayFunc(mainLoop);
    // redraw only every given millisec
    //glutTimerFunc(1, Callback::timer, 1);
    // redraw whenever system is idle
    glutIdleFunc(Callback::idle);
    glutReshapeFunc(Callback::reshape);
    glutKeyboardFunc(Callback::keyboard);
    glutMouseFunc(Callback::mouse);
    glutMotionFunc(Callback::mouseMotion);
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

    drawMode = 2; // 0:fill, 1: wireframe, 2:points

    return true;
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

    glPointSize(POINT_SIZE);
}
namespace Callback{
    void reshape(int w, int h)
    {
        screenWidth = w;
        screenHeight = h;
        // set viewport to be the entire window
        glViewport(0, 0, (GLsizei)w, (GLsizei)h);
        // set perspective viewing frustum
        glMatrixMode(GL_PROJECTION);
        gluPerspective(FOV, (float)(w)/h, NEAR_CLIP, FAR_CLIP); // FOV, AspectRatio, NearClip, FarClip
        // switch to modelview matrix in order to set scene
        glMatrixMode(GL_MODELVIEW);
    }

    void timer(int millisec)
    {
        glutTimerFunc(millisec, timer, millisec);
        glutPostRedisplay();
    }

    void idle()
    {
        glutPostRedisplay();
    }

    void keyboard(unsigned char key, int x, int y)
    {
        switch(key)
        {
        case 27: // ESCAPE
            exitBabouche();
            break;

        default:
            Scene::Controller::b_kb(key);
            break;
        }
    }

    void mouse(int button, int state, int x, int y)
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

    void mouseMotion(int x, int y)
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

    void exitBabouche()
    {
        Logger::Fps::writeLogFile();
        delete [] xParticle;
        delete [] yParticle;
        delete [] zParticle;

        delete [] mParticle;

        delete [] fXParticle;
        delete [] fYParticle;
        delete [] fZParticle;

    }
}
