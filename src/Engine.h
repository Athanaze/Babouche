namespace Frame{
    void start();
    void render();
    void end();
}

namespace Callback{
    void reshape(int w, int h);
    void timer(int millisec);
    void idle();
    void keyboard(unsigned char key, int x, int y);
    void mouse(int button, int stat, int x, int y);
    void mouseMotion(int x, int y);
    void exitBabouche();
}

void initGL();
int  initGLUT(int argc, char **argv);
bool initSharedMem();

// global variables
int screenWidth;
int screenHeight;
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance;
int drawMode = 0;
Matrix4 matrixView;
Matrix4 matrixModel;
Matrix4 matrixModelView;    // = matrixView * matrixModel
Matrix4 matrixProjection;
