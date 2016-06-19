#pragma once

#include "Graphics/DrawableObject.h"

class Resources : public DrawableObject
{
    public:
        Resources();
        virtual ~Resources();
        void draw(sf::RenderWindow* window);
        void update(sf::RenderWindow* window);
    protected:

    private:
};
