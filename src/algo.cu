#include <iostream>
#include <math.h>
#include <stdio.h>
#include "Values.h"
#include <GL/glut.h>
#include <thread>
#include <chrono>
extern "C" {
	float *x, *y, *z, *m, *r, *g, *b, *fX, *fY, *fZ;

	//Newton calculations
	__global__
	void newton(float *x,float *y,float *z,float *m,float *r,float *g,float *b,float *fX,float *fY,float *fZ) {
		int index = blockIdx.x * blockDim.x + threadIdx.x;
		int stride = blockDim.x * gridDim.x;
		for (int i = index; i < N_THREAD; i += stride){
			int a = i / N1;
			int b;
			int c = i % N1;
			if(c < a){
				b = c;
			}
			else{
				b = c+1;
			}
			float dx = x[b] - x[a];
			float dy = y[b] - y[a];
			float dz = z[b] - z[a];
			float squaredDist = 0.0f;
			squaredDist = (dx * dx)+(dy * dy)+(dz*dz);
			if(squaredDist > MIN_SQUARED_DIST){
				float force = G*((m[b]*m[a])/squaredDist);
				//Norme vecteur = distance au carre entre les deux points
				//RENDRE VECTEUR UNITAIRE
				float v0 = dx / squaredDist;
				float v1 = dy / squaredDist;
				float v2 = dz / squaredDist;

				float force_x = v0*force*FAKE_SPEED;
				float force_y = v1*force*FAKE_SPEED;
				float force_z = v2*force*FAKE_SPEED;

				//Ajouter la force dans la "file d'attente"
				fX[a] += force_x;
				fY[a] += force_y;
				fZ[a] += force_z;
			}
		}
	}

	//Called once, at the begining
	__host__ void setupCuda(){

		// Allocate Unified Memory – accessible from CPU or GPU
		cudaMallocManaged(&x, NUMBER_OF_PARTICLES*sizeof(float));
		cudaMallocManaged(&y, NUMBER_OF_PARTICLES*sizeof(float));
		cudaMallocManaged(&z, NUMBER_OF_PARTICLES*sizeof(float));
		cudaMallocManaged(&m, NUMBER_OF_PARTICLES*sizeof(float));
		cudaMallocManaged(&r, NUMBER_OF_PARTICLES*sizeof(float));
		cudaMallocManaged(&g, NUMBER_OF_PARTICLES*sizeof(float));
		cudaMallocManaged(&b, NUMBER_OF_PARTICLES*sizeof(float));
		cudaMallocManaged(&fX, NUMBER_OF_PARTICLES*sizeof(float));
		cudaMallocManaged(&fY, NUMBER_OF_PARTICLES*sizeof(float));
		cudaMallocManaged(&fZ, NUMBER_OF_PARTICLES*sizeof(float));

		// initialize x, y, et... arrays on the host
		for (int i = 0; i < NUMBER_OF_PARTICLES; i++) {

		  //RANDOM X, Y, Z  [0, 100]
		  x[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * RANDOM_RANGE_XYZ;
		  y[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * RANDOM_RANGE_XYZ;
		  z[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * RANDOM_RANGE_XYZ;
		  //RANDOM MASS [0, 10]
		  m[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * MAX_MASS;

		  //RANDOM RGB
		  r[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		  g[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		  b[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		  fX[i] = 0.0f;
		  fY[i] = 0.0f;
		  fZ[i] = 0.0f;
	  }
	  std::cout << "Setup on the shared memory done." << '\n';
	  std::cout << "CUDA_N_BLOCKS = " << CUDA_N_BLOCKS<< '\n';
	  std::cout << "NUMBER_OF_PARTICLES = " <<NUMBER_OF_PARTICLES << '\n';
	}

	__host__ int newtonHost() {
		newton<<<CUDA_N_BLOCKS, CUDA_BLOCK_SIZE>>>(x,y,z,m,r,g,b,fX,fY,fZ);
		// Wait for GPU to finish before accessing on host
		cudaDeviceSynchronize();
		//Teleport particles at new location
		for (int t = 0; t < NUMBER_OF_PARTICLES; t ++){
			x[t] += fX[t];
			fX[t] = 0.0f;
			y[t] += fY[t];
			fY[t] = 0.0f;
			z[t] += fZ[t];
			fZ[t] = 0.0f;
		}
		//Render the particles as a cube
		for (int i = 0; i < NUMBER_OF_PARTICLES; i++) {
			float x0 = 0.0f+x[i];
		    float x1 = CUBE_SIZE+x[i];
		    float x2 = CUBE_SIZE+x[i];
		    float x3 = 0.0f+x[i];
		    float x4 = 0.0f+x[i];
		    float x5 = 0.0f+x[i];
		    float x6 = CUBE_SIZE+x[i];
		    float x7 = CUBE_SIZE+x[i];

		    float y0 = 0.0f+y[i];
		    float y1 = 0.0f+y[i];
		    float y2 = CUBE_SIZE+y[i];
		    float y3 = CUBE_SIZE+y[i];
		    float y4 = CUBE_SIZE+y[i];
		    float y5 = 0.0f+y[i];
		    float y6 = 0.0f+y[i];
		    float y7 = CUBE_SIZE+y[i];

		    float z0 = 0.0f+z[i];
		    float z1 = 0.0f+z[i];
		    float z2 = 0.0f+z[i];
		    float z3 = 0.0f+z[i];
		    float z4 = CUBE_SIZE+z[i];
		    float z5 = CUBE_SIZE+z[i];
		    float z6 = CUBE_SIZE+z[i];
		    float z7 = CUBE_SIZE+z[i];
		    glColor3f(r[i], g[i], b[i]);
		    glBegin(GL_TRIANGLES);

		    glVertex3f(x0,y0,z0);
		    glVertex3f(x5,y5,z5);
		    glVertex3f(x4,y4,z4);

		    glVertex3f(x0,y0,z0);
		    glVertex3f(x3,y3,z3);
		    glVertex3f(x4,y4,z4);
		    /////////////////////
		    glVertex3f(x3,y3,z3);
		    glVertex3f(x4,y4,z4);
		    glVertex3f(x7,y7,z7);

		    glVertex3f(x3,y3,z3);
		    glVertex3f(x2,y2,z2);
		    glVertex3f(x7,y7,z7);
		    /////////////////////
		    glVertex3f(x5,y5,z5);
		    glVertex3f(x4,y4,z4);
		    glVertex3f(x7,y7,z7);

		    glVertex3f(x5,y5,z5);
		    glVertex3f(x6,y6,z6);
		    glVertex3f(x7,y7,z7);
		    /////////////////////
		    glVertex3f(x1,y1,z1);
		    glVertex3f(x2,y2,z2);
		    glVertex3f(x7,y7,z7);

		    glVertex3f(x1,y1,z1);
		    glVertex3f(x6,y6,z6);
		    glVertex3f(x7,y7,z7);
		    /////////////////////
		    glVertex3f(x0,y0,z0);
		    glVertex3f(x5,y5,z5);
		    glVertex3f(x1,y1,z1);

		    glVertex3f(x6,y6,z6);
		    glVertex3f(x5,y5,z5);
		    glVertex3f(x1,y1,z1);
		    /////////////////////
		    glVertex3f(x0,y0,z0);
		    glVertex3f(x3,y3,z3);
		    glVertex3f(x1,y1,z1);

		    glVertex3f(x3,y3,z3);
		    glVertex3f(x2,y2,z2);
		    glVertex3f(x1,y1,z1);
		    glEnd();
		}

		/*cudaFree(x);
		cudaFree(y);
		cudaFree(z);
		cudaFree(m);
		cudaFree(r);
		cudaFree(g);
		cudaFree(b);
		cudaFree(fX);
		cudaFree(fY);
		cudaFree(fZ);*/
		return 0;
	}
}
