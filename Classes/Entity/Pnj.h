#pragma once
#include "Graphics/DrawableObject.h"

class Pnj : public DrawableObject
{


    public:

        Pnj(std::string path, sf::Vector2f pos);
        virtual ~Pnj();

        // Function
        void draw(sf::RenderWindow* window);
        void update(sf::RenderWindow* window);

    protected:
    private:

};

