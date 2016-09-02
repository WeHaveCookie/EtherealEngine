#pragma once

#include "Entity/Entity.h"

class Character : public Entity
{
public:
    Character(const char* path);
    virtual ~Character();

    // Function
    void paint();
    void update();
   
    // Inline

protected:
private:
    // Function
};

