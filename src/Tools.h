#ifndef TOOLS_H
#define TOOLS_H
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "Values.h"

/*
Tools for logging, measuring time, ...

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

namespace Resource{
    std::vector<float> loadOBJ(){
        using namespace std;
        vector<float> result;
        string line;
        ifstream myfile("../res/suzi.obj");
        if (myfile.is_open()){
          while (getline (myfile,line)){
              if(line.at(0) == 'v' && line.at(1) == ' '){
                  istringstream iss(line);
                  vector<string> tokens;
                  copy(
                      istream_iterator<string>(iss),
                      istream_iterator<string>(),
                      back_inserter(tokens)
                  );

                  //Convert string to float, and push to vector
                  result.push_back(stof(tokens[1]));
                  result.push_back(stof(tokens[2]));
                  result.push_back(stof(tokens[3]));
              }
          }
          myfile.close();
        }
        else{
            cout << "Unable to open file";
        }
        // [x,y,z, x,y,z, x,y,z, ...]
        return result;
    }
}

#endif
