#pragma once

#include "Graphics/DrawableObject.h"

class Pnj;

class Building : public DrawableObject
{
    public:
        Building(sf::Vector2f pos);
        virtual ~Building();
        void paint();
        void update();
      
    protected:
      
    private:

};
