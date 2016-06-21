#pragma once

#include "Graphics/DrawableObject.h"

class Resources : public DrawableObject
{
    public:
        Resources();
        virtual ~Resources();
        void paint();
        void update();
    protected:

    private:
};
