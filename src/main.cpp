///////////////////////////////////////////////////////////////////////////////
//VUE + CONTROLLER ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include <GL/glut.h>
#include <thread>
#include "Matrices.h"
#include "Vectors.h"
#include "Engine.h"
#include "Tools.h"
float *xParticle = new float[NUMBER_OF_PARTICLES];
float *yParticle = new float[NUMBER_OF_PARTICLES];
float *zParticle = new float[NUMBER_OF_PARTICLES];
float *mParticle = new float[NUMBER_OF_PARTICLES];
float *fXParticle = new float[NUMBER_OF_PARTICLES];
float *fYParticle = new float[NUMBER_OF_PARTICLES];
float *fZParticle = new float[NUMBER_OF_PARTICLES];
namespace Model{
    void particleUpdate(int index){
        for(int a = index; a < index+STRIDE; a++){
            for(int b = 0; b < NUMBER_OF_PARTICLES; b++){
                if(a != b){
                    float dx = xParticle[b] - xParticle[a];
                    float dy = yParticle[b] - yParticle[a];
                    float dz = zParticle[b] - zParticle[a];

                    float squaredDist = (dx*dx)+(dy*dy)+(dz*dz);

                    if(squaredDist > MIN_SQUARED_DIST){
                        float force = G*((mParticle[b]*mParticle[a])/squaredDist);
                        //Norme vecteur = distance au carre entre les deux points
                        //RENDRE VECTEUR UNITAIRE

                        //Ajouter la force dans la "file d'attente"
                        fXParticle[a] += (dx / squaredDist)*force*FAKE_SPEED;
                        fYParticle[a] += (dy / squaredDist)*force*FAKE_SPEED;
                        fZParticle[a] += (dz / squaredDist)*force*FAKE_SPEED;
                    }
                    else{
                        std::cout << squaredDist << "\n";
                        mParticle[a] += mParticle[b];
                        mParticle[b] = 0.0f;
                    }
                }
            }

        }
    }

    void teleportSingleParticle(int i){
        for(int a = i; a < i+STRIDE_2; a++){
            xParticle[a] += fXParticle[a];
            //fXParticle[a] = 0.0f;
            yParticle[a] += fYParticle[a];
            //fYParticle[a] = 0.0f;
            zParticle[a] += fZParticle[a];
            //fZParticle[a] = 0.0f;
        }

    }
    //Teleport particles at new location
    void teleportParticles(){
        int numberOfThreads = NUMBER_OF_PARTICLES / STRIDE_2;
        std::thread threads[numberOfThreads];
        for (int t = 0; t < numberOfThreads; t ++){
            threads[t] = std::thread(teleportSingleParticle, t*STRIDE_2);
        }
        for (int i = 0; i < numberOfThreads; i++){
            threads[i].join();
        }
    }
    //Called every frame
    void update(){
        int numberOfThreads = NUMBER_OF_PARTICLES / STRIDE;
        std::thread threads[numberOfThreads];
        for (int i = 0; i < numberOfThreads; i++) {
            threads[i] = std::thread(particleUpdate, i*STRIDE);
        }
        for (int i = 0; i < numberOfThreads; i++){
            threads[i].join();
        }
        teleportParticles();
    }

    void setParticlesValues(){
        for(int i = 0; i < NUMBER_OF_PARTICLES; i++){
            xParticle[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * RANDOM_RANGE_XYZ;
            yParticle[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * RANDOM_RANGE_XYZ;
            zParticle[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * RANDOM_RANGE_XYZ;

            mParticle[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * MAX_MASS;
        }
    }

}

void mainLoop()
{
    Frame::start();

    //All the logic / Model stuff is executed here, every frame
    Model::update();

    Frame::render();
    Frame::end();

}

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
        //glColor3f(0.5f, 0.0f, 0.5f);
        glBegin(GL_POINTS);
        for (int i = 0; i < NUMBER_OF_PARTICLES; i++) {
            glColor3f(0.01f*mParticle[i], 0.0f, 0.02f*mParticle[i]);
            glVertex3f(xParticle[i], yParticle[i], zParticle[i]);
        }
        glEnd();
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

    Model::setParticlesValues();

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

        case ' ':
            break;
        default:
            ;
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
