#pragma once
#include "Entity/Entity.h"

class Pnj : public Entity
{


    public:

        Pnj(std::string path, sf::Vector2f pos);
        virtual ~Pnj();

        // Function
        void paint();
        void update();

    protected:
    private:

};

