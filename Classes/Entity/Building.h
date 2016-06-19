#pragma once

#include "Graphics/DrawableObject.h"

class Pnj;

class Building : public DrawableObject
{
    public:
        Building(sf::Vector2f pos);
        virtual ~Building();
        void draw(sf::RenderWindow* window);
        void update(sf::RenderWindow* window);
      
    protected:
      
    private:

};
