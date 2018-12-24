/*
    Babouche Engine scene.
    Must be in namespace Scene.
    functions starting with b_ are mandatory
*/

namespace Scene{

    namespace Model{
        void b_setup(){
            for(int i = 0; i < NUMBER_OF_PARTICLES; i++){
                mParticle[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * MAX_MASS;

                fXParticle[i] = 0.0f;
                fYParticle[i] = 0.0f;
                fZParticle[i] = 0.0f;
            }

            std::vector<float> objData = Resource::loadOBJ();
            if(objData.size() <= NUMBER_OF_PARTICLES){
                for(int i = 0; i< objData.size();i+=3){
                    xParticle[i] = objData[i];
                    yParticle[i] = objData[i+1];
                    zParticle[i] = objData[i+2];
                }
                Logger::print("Particles set to OBJ Model vertices");
            }
            else{
                Logger::print("Not enough particles for the OBJ model");
            }
        }

        //Called every frame
        void b_update(){
            //Logger::print("Suzi Scene Update doing nothing :D");
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
