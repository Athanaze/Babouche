// GLUT CALLBACK functions
void displayCBStart();
void displayCBEnd();
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void idleCB();
void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);

// CALLBACK function when exit() called ///////////////////////////////////////
void exitCB();

void initGL();
int  initGLUT(int argc, char **argv);
bool initSharedMem();
void clearSharedMem();
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
void drawString(const char *str, int x, int y, float color[4], void *font);
void drawString3D(const char *str, float pos[3], float color[4], void *font);
void showInfo();
void toOrtho();
void toPerspective();
void drawGrid(float size=10.0f, float step=1.0f);
void drawModel(float x, float y, float z, float r, float g, float b);
Matrix4 setFrustum(float l, float r, float b, float t, float n, float f);
Matrix4 setFrustum(float fovY, float aspectRatio, float front, float back);
Matrix4 setOrthoFrustum(float l, float r, float b, float t, float n, float f);

// constants
const int   SCREEN_WIDTH    = 1920;
const int   SCREEN_HEIGHT   = 1080;
const float CAMERA_DISTANCE = 50.0f;
const int   TEXT_WIDTH      = 8;
const int   TEXT_HEIGHT     = 13;
const float DEG2RAD         = 3.141593f / 180;

// global variables
void *font = GLUT_BITMAP_8_BY_13;
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
