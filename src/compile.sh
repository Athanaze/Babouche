make clean
rm ../out/logFiles/fpsLog.data
rm obj/algo.cu
nvcc -std=c++11 -c -o obj/algo.o algo.cu
make
