/*
    Babouche Engine scene.
    Must be in namespace Scene.
    functions starting with b_ are mandatory

    Newton Scene
*/

namespace Scene{

    namespace Model{
        void b_setup(){
            for(int i = 0; i < NUMBER_OF_PARTICLES; i++){
                xParticle[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * RANDOM_RANGE_XYZ;
                yParticle[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * RANDOM_RANGE_XYZ;
                zParticle[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * RANDOM_RANGE_XYZ;

                mParticle[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * MAX_MASS;
            }
        }

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
                            //std::cout << squaredDist << "\n";
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
        void b_update(){
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
    }

    namespace Vue{
        void b_render(){
            //ARGS: [x0, y0, z0, x1, y1, z1]
            float a[6] = {0.0f, 0.0f, -1.0f, 3.0f, 0.0f, 2.0f};
            Draw::vector(a);

            float b[6] = {0.1f, -0.4f, -1.0f, 0.1f, -0.2f, 0.3f};
            Draw::vector(b);

            float c[6] = {-0.8f, 0.4f, 1.0f, -0.1f, 0.2f, 0.3f};
            Draw::vector(c);
        }
    }


    namespace Controller{
        /*
        Called each time there is a keyboard event that is not already handled
        by the core engine
        */
        void b_kb(unsigned char key){
            Logger::print("KB EVENT");
        }
    }
}
