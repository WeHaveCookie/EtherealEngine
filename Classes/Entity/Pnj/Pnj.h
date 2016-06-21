#pragma once
#include "Graphics/DrawableObject.h"

class Pnj : public DrawableObject
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

