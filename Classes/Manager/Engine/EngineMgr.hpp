#pragma once

#include "Manager/Manager.h"

class EngineMgr : public Manager
{
    public:
        EngineMgr();
        virtual ~EngineMgr();

		void Init();
		void Process(const float dt);
		void End();

        // Function
        bool collisionCircle(sf::FloatRect box1, sf::CircleShape circle);
        bool collisionAABB(sf::FloatRect box1, sf::FloatRect box2);

        // Inline
    protected:
	private:
};
