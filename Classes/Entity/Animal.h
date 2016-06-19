#pragma once

#include "Graphics/DrawableObject.h"

class Animal : public DrawableObject
{
    public:
        Animal(std::string path, sf::Vector2f pos);
        virtual ~Animal();

        // Function
        void draw(sf::RenderWindow* window);
        void update(sf::RenderWindow* window);

    protected:
   
    private:
};

