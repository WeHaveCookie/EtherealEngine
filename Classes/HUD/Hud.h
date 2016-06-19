#pragma once

#include "Graphics/DrawableObject.h"

class DrawableObject;

class Hud : public DrawableObject
{
    public:
        Hud();
        virtual ~Hud();
        // Function
        void draw(sf::RenderWindow* window);
        void update(sf::RenderWindow* window);
        void updateAnimation();

    protected:
    private:

};
