#pragma once

#include "Graphics/DrawableObject.h"

class DrawableObject;

class Hud : public DrawableObject
{
    public:
        Hud();
        virtual ~Hud();
        // Function
        void paint();
        void update();

    protected:
    private:

};
