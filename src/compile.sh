#make clean
rm ../out/logFiles/fpsLog.data
#rm obj/algo.cu
#nvcc -std=c++11 -c -o obj/algo.o algo.cu
#make


g++ -o ../bin/babouche main.cpp Matrices.cpp -std=c++11 -lglut -lGLU -lGL -lm -lpthread -Ofast
