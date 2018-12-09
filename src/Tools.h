#ifndef TOOLS_H
#define TOOLS_H
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <chrono>
#include "Values.h"

/*
General-purpose tools for logging, measuring time, ...

*/

namespace Timer{
    using namespace std::chrono;
    bool clean = true;
    system_clock::time_point start;
    system_clock::time_point end;
    // record start time, you can only start  one timer at a time for now
    void sta(){
        if(clean){
            clean = false;
            start = system_clock::now();
        }
    }
    //Returns the duration of the timer
    double endAndGetDuration(){
        // record end time
        end = system_clock::now();
        //Get the duration
        duration<double> diff = end-start;

        //Clean up, so we can start another timer
        clean = true;

        return diff.count();
    }
}

namespace Logger{

    void print(std::string msg){
        std::cout << msg << "\n";
    }

    //For all the fps logging tools
    namespace Fps{
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
            Logger::print("Log file written !");
        }

        //Just a wrapper around Timer::start
        void startMeasuring(){
            Timer::sta();
        }
        /*
            - Assuming we did ONE frame in N time
            - "(double) 1 / endAndGetDuration()" is the FPS
            - We push that data to the fps list
        */
        void endMeasuring(){
            fpsList.push_back((double) 1 / Timer::endAndGetDuration());
        }
    }
}


#endif
