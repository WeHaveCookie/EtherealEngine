#pragma once

#include "Graphics/DrawableObject.h"

class Character : public DrawableObject
{
public:
    Character(std::string path);
    virtual ~Character();

    // Function
    void draw(sf::RenderWindow* window);
    void update(sf::RenderWindow* window);
   
    // Inline

protected:
private:
    // Function
};

