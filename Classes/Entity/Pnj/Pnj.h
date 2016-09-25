#pragma once
#include "Entity/Entity.h"

class Pnj : public Entity
{


    public:

        Pnj(std::string path, Vector2 pos);
        virtual ~Pnj();

        // Function
        void paint();
        void update();

    protected:
    private:

};

