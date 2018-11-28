///////////////////////////////////////////////////////////////////////////////
//MODEL ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include "Model.h"

extern "C"{
    void setupCuda();
    int newtonHost();

}
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
//Called every frame
void updateModel(){
    newtonHost();
}
//Called just before the engine starts his GLUT event-processing loop
void startModel(){
  setupCuda();
}
