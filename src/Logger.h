#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include "Values.h"
namespace Logger{
    std::list<double> fpsList;
    std::list<double>::iterator it;
    void addFPSData(double fps){
        fpsList.push_back(fps);
    }

    void writeLogFile(){
        std::ofstream logFile;
        logFile.open(FPS_LOG_PATH);

        for (it=fpsList.begin(); it!=fpsList.end(); ++it){
             logFile << *it<<"\n";
        }
        logFile.close();
    }
}
#endif
